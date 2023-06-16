#include <algorithm>
#include <cctype>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>
#include <unordered_set>
#include <vector>

// Чтобы получить из enum строки
// https://stackoverflow.com/a/9150779
auto split(const std::string &text, char sep) -> std::vector<std::string> {
    std::vector<std::string> tokens;
    uint64_t                 start = 0;
    uint64_t                 end   = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
    return tokens;
}

#define ENUM(name, ...)                                                        \
    enum name {                                                                \
        __VA_ARGS__                                                            \
    };                                                                         \
    std::vector<std::string> name##Map = split(#__VA_ARGS__, ',');             \
    auto                     toString(const name v)->std::string {             \
        return name##Map.at(v);                            \
    }

ENUM(TokenType,
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
     END)

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

using std::literals::operator""s;

class Scanner {
 public:
    static auto scan(const std::string &program) noexcept(false)
        -> std::vector<std::vector<Token>> {
        Scanner    scanner(program);
        const auto res = scanner.scan();
        return res;
    }

 private:
    uint64_t                        index_{0};
    std::vector<Token>              statement_tokens_;
    std::vector<std::vector<Token>> statements_;
    const std::string              &program_;

    explicit Scanner(const std::string &program) : program_(program) {
    }

    auto scan() noexcept(false) -> std::vector<std::vector<Token>> {
        while (!at_end()) {
            const auto c = peek();
            switch (c) {
                case ' ':
                    advance();
                    break;
                case '\n':
                    if (!statement_tokens_.empty()) {
                        statement_tokens_.emplace_back(TokenType::END);
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
                        throw std::runtime_error("Unexpected character: "s + c);
                    }
                    break;
            }
        }
        if (!statement_tokens_.empty()) {
            statement_tokens_.emplace_back(TokenType::END);
            statements_.push_back(statement_tokens_);
            statement_tokens_.clear();
        }
        return statements_;
    }

    auto at_end() -> bool {
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
    const auto                            actual  = Scanner::scan(program);
    const std::vector<std::vector<Token>> expected{
        {
         Token{TokenType::IDENTIFIER, "a"},
         Token{TokenType::ASSIGN},
         Token{TokenType::NUMBER},
         Token{TokenType::END},
         }
    };

    ASSERT_EQ(expected, actual);
}

TEST(Scanner, CompoundAssignment) {
    const auto                           *program = "a, b = 1, c";
    const auto                            actual  = Scanner::scan(program);
    const std::vector<std::vector<Token>> expected{
        {
         Token{TokenType::IDENTIFIER, "a"},
         Token{TokenType::COMMA},
         Token{TokenType::IDENTIFIER, "b"},
         Token{TokenType::ASSIGN},
         Token{TokenType::NUMBER},
         Token{TokenType::COMMA},
         Token{TokenType::IDENTIFIER, "c"},
         Token{TokenType::END},
         }
    };

    ASSERT_EQ(expected, actual);
}

TEST(Scanner, CompoundAssignmentWithGrouping) {
    const auto                           *program = "a, b = (1 + c), 3";
    const auto                            actual  = Scanner::scan(program);
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
         Token{TokenType::END},
         }
    };

    ASSERT_EQ(expected, actual);
}

TEST(Scanner, Multiline) {
    const auto                           *program = R"(
        a = 3
        b = a
)";

    const auto                            actual  = Scanner::scan(program);
    const std::vector<std::vector<Token>> expected{
        {
         Token{TokenType::IDENTIFIER, "a"},
         Token{TokenType::ASSIGN},
         Token{TokenType::NUMBER},
         Token{TokenType::END},
         },
        {
         Token{TokenType::IDENTIFIER, "b"},
         Token{TokenType::ASSIGN},
         Token{TokenType::IDENTIFIER, "a"},
         Token{TokenType::END},
         }
    };

    ASSERT_EQ(expected, actual);
}

using std::string_literals::operator""s;

// https://craftinginterpreters.com/
/*
 * STATEMENT = IDENTIFIER ("," IDENTIFIER)* "=" EXPRESSION ("," EXPRESSION)*\n
 * EXPRESSION = FACTOR (("+"|"-") FACTOR)*
 * FACTOR = UNARY (("*" | "/") UNARY)*
 * UNARY = ("+" | "-") UNARY | PRIMARY
 * PRIMARY = NUMBER | IDENTIFIER | "(" EXPRESSION ")"
 */

class Parser {
 public:
    static auto parse(const std::vector<Token>                  &tokens,
                      std::unordered_map<std::string, uint64_t> &var2index,
                      uint64_t &var_count) noexcept(false)
        -> std::tuple<std::vector<uint64_t>, std::vector<uint64_t>> {
        Parser parser(tokens, var2index, var_count);
        parser.statement();
        return {parser.dependent_, parser.dependincies_};
    }

 private:
    explicit Parser(const std::vector<Token>                  &tokens,
                    std::unordered_map<std::string, uint64_t> &var2index,
                    uint64_t                                  &var_count)
        : tokens_(tokens), var2index_(var2index), var_count_(var_count) {
    }

    uint64_t                                   index_{0};
    const std::vector<Token>                  &tokens_;

    std::unordered_map<std::string, uint64_t> &var2index_;
    uint64_t                                  &var_count_;

    std::vector<uint64_t>                      dependent_;
    std::vector<uint64_t>                      dependincies_;

    auto                                       peek() -> const Token                                       &{
        return tokens_.at(index_);
    }

    auto advance() -> void {
        ++index_;
    }

    auto match(std::initializer_list<TokenType> expected_types) -> bool {
        const auto &next_token = peek();
        return std::any_of(expected_types.begin(),
                           expected_types.end(),
                           [next_token](const auto token_type) {
                               return next_token.type == token_type;
                           });
    }

    auto consume(std::initializer_list<TokenType> token_types) noexcept(false)
        -> void {
        if (match(token_types)) {
            advance();
            return;
        }
        const auto &next_token = peek();
        throw std::runtime_error("Unexpected token type: "s +
                                 std::to_string(next_token.type));
    }

    auto at_end() -> bool {
        return index_ >= tokens_.size();
    }

    auto statement() -> void {
        const auto left_count  = count_left();
        const auto right_count = count_right();

        if (left_count != right_count) {
            throw std::runtime_error(
                "Number of identifiers on the left side is not equal to the "
                "number of assigned expressions on the right");
        }
    }

    auto count_left() -> uint64_t {
        for (;;) {
            if (match({IDENTIFIER})) {
                const auto &next_token = peek();
                assert(next_token.value.has_value());
                const auto &variable_name = next_token.value.value();

                uint64_t    index;
                if (auto found_iter = var2index_.find(variable_name);
                    found_iter == var2index_.end()) {
                    var2index_.emplace(variable_name, var_count_);
                    index = var_count_++;
                } else {
                    index = found_iter->second;
                }

                dependent_.push_back(index);
                advance();
            } else if (match({COMMA})) {
                advance();
            } else if (match({ASSIGN})) {
                advance();
                break;
            } else {
                const auto &next_token = peek();
                throw std::runtime_error("Unexpected token type: "s +
                                         std::to_string(next_token.type));
            }
        }
        return dependent_.size();
    }

    auto count_right() -> uint64_t {
        uint64_t count = 0;
        for (;;) {
            expression();
            ++count;
            if (match({TokenType::COMMA})) {
                advance();
            }
            if (match({TokenType::END})) {
                break;
            }
        }
        return count;
    }

    auto expression() -> void {
        factor();
        while (match({TokenType::PLUS, TokenType::MINUS})) {
            advance();
            factor();
        }
    }

    auto factor() -> void {
        unary();
        while (match({TokenType::DIV, TokenType::MULT})) {
            advance();
            unary();
        }
    }

    auto unary() -> void {
        while (match({TokenType::PLUS, TokenType::MINUS})) {
            advance();
        }
        primary();
    }

    auto primary() -> void {
        if (match({TokenType::NUMBER})) {
            advance();
        }
        if (match({TokenType::L_PARENTHESIS})) {
            advance();
            expression();
            consume({TokenType::R_PARENTHESIS});
        }
        // consume({TokenType::END});
    }
};

TEST(Parser, Assignment) {
    const auto                               *program = "a = 1";
    const auto                                tokens  = Scanner::scan(program);
    std::unordered_map<std::string, uint64_t> var2index;
    uint64_t                                  var_count = 0;

    std::tuple<std::vector<uint64_t>, std::vector<uint64_t>> res;
    ASSERT_NO_THROW(res = Parser::parse(tokens.at(0), var2index, var_count););
    const auto [dependent, dependencies] = res;

    const std::vector<uint64_t> expected_dependent{0};
    const std::vector<uint64_t> expected_dependencies{};

    EXPECT_EQ(expected_dependent, dependent);
    EXPECT_EQ(expected_dependencies, dependencies);
}

// auto main() -> int {
// }
