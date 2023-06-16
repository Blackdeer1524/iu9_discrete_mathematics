#include <cctype>
#include <cstdint>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <system_error>
#include <vector>

enum class TokenType {
    NUMBER,
    IDENTIFIER,

    L_PARENTHESIS,
    R_PARENTHESIS,

    PLUS,
    MINUS,
    MULT,
    DIV,

    ASSIGN,
    COMMA,
};

// https://craftinginterpreters.com/
/*
 * STATEMENT = IDENTIFIER ("," IDENTIFIER)* "=" EXPRESSION ("," EXPRESSION)*\n
 * EXPRESSION = TERM (("+"|"-") TERM)
 * TERM = FACTOR (("*" | "/") FACTOR)
 * FACTOR = ("+" | "-")* PRIMARY
 * PRIMARY = NUMBER | IDENTIFIER | "(" EXPRESSION ")"
 */

struct Token {
    explicit Token(TokenType                    type_,
                   std::optional<std::string> &&value_ = std::nullopt)
        : type(type_), value(value_) {
    }

    auto operator==(const Token &other) const -> bool {
        if (type != other.type) {
            return false;
        }
        if (!value.has_value() && !other.value.has_value()) {
            return true;
        }
        if (value.has_value() && other.value.has_value()) {
            const auto &left  = value.value();
            const auto &right = other.value.value();
            return left == right;
        }
        return false;
    }

    TokenType                  type;
    std::optional<std::string> value = std::nullopt;
};

class Scanner {
 public:
    explicit Scanner(const std::string &program) : program_(program) {
    }

    auto scan() noexcept(false) -> std::vector<std::vector<Token>> {
        statements_.clear();
        index_ = 0;
        while (!at_end()) {
            const auto c = peek();
            switch (c) {
                case ' ':
                    advance();
                    break;
                case '\n':
                    if (!statement_tokens_.empty()) {
                        statements_.push_back(statement_tokens_);
                        statement_tokens_.clear();
                    }
                    advance();
                    break;
                case ',':
                    statement_tokens_.emplace_back(TokenType::COMMA);
                    advance();
                    break;
                case '+':
                    statement_tokens_.emplace_back(TokenType::PLUS);
                    advance();
                    break;
                case '-':
                    statement_tokens_.emplace_back(TokenType::MINUS);
                    advance();
                    break;
                case '/':
                    statement_tokens_.emplace_back(TokenType::DIV);
                    advance();
                    break;
                case '*':
                    statement_tokens_.emplace_back(TokenType::MULT);
                    advance();
                    break;
                case '(':
                    statement_tokens_.emplace_back(TokenType::L_PARENTHESIS);
                    advance();
                    break;
                case ')':
                    statement_tokens_.emplace_back(TokenType::R_PARENTHESIS);
                    advance();
                    break;
                case '=':
                    statement_tokens_.emplace_back(TokenType::ASSIGN);
                    advance();
                    break;
                default:
                    if (isdigit(c)) {
                        digit();
                    } else if (isalpha(c)) {
                        identifier();
                    } else {
                        // Нормальная обработка ошибок - для неуверенных в себе
                        // истинные программисты дебажат 6 часов
                        throw std::runtime_error("error");
                    }
                    break;
            }
        }
        if (!statement_tokens_.empty()) {
            statements_.push_back(statement_tokens_);
            statement_tokens_.clear();
        }
        return statements_;
    }

 private:
    uint64_t                        index_{0};
    std::vector<Token>              statement_tokens_;
    std::vector<std::vector<Token>> statements_;
    const std::string              &program_;

    auto                            at_end() -> bool {
        return index_ >= program_.length();
    }

    auto advance() -> void {
        ++index_;
    }

    auto peek() -> char {
        return at_end() ? '\0' : program_.at(index_);
    }

    auto digit() -> void {
        while (isdigit(peek())) {
            advance();
        }
        statement_tokens_.emplace_back(TokenType::NUMBER);
    }

    static auto isalphanum(const char c) -> bool {
        return isdigit(c) || isalpha(c);
    }

    auto identifier() -> void {
        const auto start = index_;
        while (isalphanum(peek())) {
            advance();
        }
        const auto parsed_identifier = program_.substr(start, index_ - start);
        statement_tokens_.emplace_back(TokenType::IDENTIFIER,
                                       parsed_identifier);
    }
};

#include <gtest/gtest.h>

TEST(Scanner, Assignment) {
    const auto                           *program = "a = 1";

    const auto                            actual  = Scanner(program).scan();
    const std::vector<std::vector<Token>> expected{
        {
         Token{TokenType::IDENTIFIER, "a"},
         Token{TokenType::ASSIGN},
         Token{TokenType::NUMBER},
         }
    };

    ASSERT_EQ(expected, actual);
}

TEST(Scanner, CompoundAssignment) {
    const auto                           *program = "a, b = 1, c";

    const auto                            actual  = Scanner(program).scan();
    const std::vector<std::vector<Token>> expected{
        {
         Token{TokenType::IDENTIFIER, "a"},
         Token{TokenType::COMMA},
         Token{TokenType::IDENTIFIER, "b"},
         Token{TokenType::ASSIGN},
         Token{TokenType::NUMBER},
         Token{TokenType::COMMA},
         Token{TokenType::IDENTIFIER, "c"},
         }
    };

    ASSERT_EQ(expected, actual);
}

TEST(Scanner, CompoundAssignmentWithGrouping) {
    const auto                           *program = "a, b = (1 + c), 3";

    const auto                            actual  = Scanner(program).scan();
    const std::vector<std::vector<Token>> expected{
        {
         Token{TokenType::IDENTIFIER, "a"},
         Token{TokenType::COMMA},
         Token{TokenType::IDENTIFIER, "b"},
         Token{TokenType::ASSIGN},
         Token{TokenType::L_PARENTHESIS},
         Token{TokenType::NUMBER},
         Token{TokenType::PLUS},
         Token{TokenType::IDENTIFIER, "c"},
         Token{TokenType::R_PARENTHESIS},
         Token{TokenType::COMMA},
         Token{TokenType::NUMBER},
         }
    };

    ASSERT_EQ(expected, actual);
}

TEST(Scanner, Multiline) {
    const auto                           *program = R"(
        a = 3
        b = a
)";

    const auto                            actual  = Scanner(program).scan();
    const std::vector<std::vector<Token>> expected{
        {
         Token{TokenType::IDENTIFIER, "a"},
         Token{TokenType::ASSIGN},
         Token{TokenType::NUMBER},
         },
        {
         Token{TokenType::IDENTIFIER, "b"},
         Token{TokenType::ASSIGN},
         Token{TokenType::IDENTIFIER, "a"},

         }
    };

    ASSERT_EQ(expected, actual);
}

// auto main() -> int {
// }
