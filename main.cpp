#include <cctype>
#include <cstdint>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

/*
fib(n) := fibrec(1,1,n);
fibrec(a,b,n) := n=1 ? a : fibrec(b,a+b,n-1);

<program> ::= <function> <program>.

<function> ::= <ident> ( <formal-args-list> ) := <expr> ; .

<formal-args-list> ::= <ident-list> | .

<ident-list> ::= <ident> | <ident> , <ident-list>.

<expr> ::=
  <comparison_expr> ? <comparison_expr> : <expr>
  | <comparison_expr>.

<comparison_expr> ::=
  <arith_expr> <comparison_op> <arith_expr>
  | <arith_expr>.

<comparison_op> ::= = | <> | < | > | <= | >= .

<arith_expr> ::=
  <arith_expr> + <term>
  | <arith_expr> - <term>
  | <term>.

<term> ::=
  <term> * <factor>
  | <term> / <factor>
  | <factor>.

<factor> ::=
  <number>
  | <ident>
  | <ident> ( <actual_args_list> )
  | ( <expr> )
  | - <factor>.

<actual_args_list> ::= <expr-list> | .

<expr-list> ::= <expr> | <expr> , <expr-list>.
*/

// Чтобы получить из enum строки
// https://stackoverflow.com/a/9150779
auto split(const std::string &text, char sep) -> std::vector<std::string> {
    std::vector<std::string> tokens;
    uint64_t                 start = 0;
    uint64_t                 end   = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 2;
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

     ASSIGNMENT,  // =
     WALRUS,  // := . Ну ведь действительно похоже на моржа

     QUESTION_MARK,
     COLON,
     SEP,  // ;

     NOT_EQUAL,         // <>
     LESS,              // <
     GREATER,           // >
     LESS_OR_EQUAL,     // <=
     GREATER_OR_EQUAL,  // >=

     COMMA,
     END)

using std::string_literals::operator""s;

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
    static auto scan(const std::string &program) -> std::vector<Token> {
        Scanner scanner(program);
        auto    tokens = scanner.scan();
        return tokens;
    }

 private:
    explicit Scanner(const std::string &program) : program_(program) {
    }

    const std::string &program_;
    uint64_t           index_{0};
    std::vector<Token> tokens_;

    auto               scan() -> std::vector<Token> {
        while (!at_end()) {
            const auto c = peek();
            switch (c) {
                case ' ':
                case '\n':
                    advance();
                    break;
                case '(':
                    tokens_.emplace_back(TokenType::L_PARENTHESIS);
                    advance();
                    break;
                case ')':
                    tokens_.emplace_back(TokenType::R_PARENTHESIS);
                    advance();
                    break;
                case '+':
                    tokens_.emplace_back(TokenType::PLUS);
                    advance();
                    break;
                case '-':
                    tokens_.emplace_back(TokenType::MINUS);
                    advance();
                    break;
                case '*':
                    tokens_.emplace_back(TokenType::MULT);
                    advance();
                    break;
                case '/':
                    tokens_.emplace_back(TokenType::DIV);
                    advance();
                    break;
                case '?':
                    tokens_.emplace_back(TokenType::QUESTION_MARK);
                    advance();
                    break;
                case ':': {
                    const auto next = peek_next();
                    switch (next) {
                        case '=':
                            tokens_.emplace_back(TokenType::WALRUS);
                            advance();
                            break;
                        case ' ':
                            tokens_.emplace_back(TokenType::COLON);
                            break;
                    }
                    advance();
                    break;
                }
                case ';':
                    tokens_.emplace_back(TokenType::SEP);
                    advance();
                    break;
                case '=':
                    tokens_.emplace_back(TokenType::ASSIGNMENT);
                    advance();
                    break;
                case '<': {
                    const auto next = peek_next();
                    switch (next) {
                        case '>':
                            tokens_.emplace_back(TokenType::NOT_EQUAL);
                            advance();
                            break;
                        case '=':
                            tokens_.emplace_back(TokenType::LESS_OR_EQUAL);
                            advance();
                            break;
                        case ' ':
                            tokens_.emplace_back(TokenType::LESS);
                            break;
                    }
                    advance();
                    break;
                }
                case '>': {
                    const auto next = peek_next();
                    switch (next) {
                        case '=':
                            tokens_.emplace_back(TokenType::GREATER_OR_EQUAL);
                            advance();
                            break;
                        case ' ':
                            tokens_.emplace_back(TokenType::GREATER);
                            break;
                    }
                    advance();
                    break;
                }
                case ',':
                    tokens_.emplace_back(TokenType::COMMA);
                    advance();
                    break;
                default:
                    if (isdigit(c)) {
                        number();
                    } else if (isalpha(c)) {
                        identifier();
                    } else {
                        throw std::runtime_error("Unexpected symbol: "s + c);
                    }
                    break;
            }
        }
        tokens_.emplace_back(TokenType::END);
        return tokens_;
    }

    [[nodiscard]] auto at_end() const -> bool {
        return index_ >= program_.length();
    }

    [[nodiscard]] auto peek() const -> char {
        return at_end() ? '\0' : program_.at(index_);
    }

    [[nodiscard]] auto peek_next() const -> char {

        return index_ + 1 >= program_.length() ? '\0' : program_.at(index_ + 1);
    }

    auto advance() -> void {
        ++index_;
    }

    auto number() -> void {
        while (isdigit(peek())) {
            advance();
        }
        tokens_.emplace_back(TokenType::NUMBER);
    }

    auto identifier() -> void {
        const auto start = index_;
        while (isalphanum(peek())) {
            advance();
        }
        const auto scanned_ident = program_.substr(start, index_ - start);
        tokens_.emplace_back(TokenType::IDENTIFIER, scanned_ident);
    }

    static auto isalphanum(char c) -> bool {
        return isdigit(c) || isalpha(c);
    }
};

#include <gtest/gtest.h>

TEST(SCANNER, NoArgs) {
    const auto *program = "123 foo () + - * / = := ? : ; <> < > <= >= ,";
    const auto  actual  = Scanner::scan(program);
    const std::vector<Token> expected{
        Token{TokenType::NUMBER},
        Token{TokenType::IDENTIFIER, "foo"},

        Token{TokenType::L_PARENTHESIS},
        Token{TokenType::R_PARENTHESIS},

        Token{TokenType::PLUS},
        Token{TokenType::MINUS},
        Token{TokenType::MULT},
        Token{TokenType::DIV},

        Token{TokenType::ASSIGNMENT},
        Token{TokenType::WALRUS},

        Token{TokenType::QUESTION_MARK},
        Token{TokenType::COLON},
        Token{TokenType::SEP},

        Token{TokenType::NOT_EQUAL},
        Token{TokenType::LESS},
        Token{TokenType::GREATER},
        Token{TokenType::LESS_OR_EQUAL},
        Token{TokenType::GREATER_OR_EQUAL},
        Token{TokenType::COMMA},

        Token{TokenType::END},
    };

    ASSERT_EQ(expected, actual);
}

// auto main() -> int {
// }
