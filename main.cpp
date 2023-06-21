#include <algorithm>
#include <assert.h>
#include <cctype>
#include <cmath>
#include <complex>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <optional>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
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

     EQUAL,  // =
     WALRUS,  // := . Ну ведь действительно похоже на моржа

     QUESTION_MARK,
     COLON,  // :
     SEP,    // ;

     NOT_EQUAL,         // <>
     LESS,              // <
     GREATER,           // >
     LESS_OR_EQUAL,     // <=
     GREATER_OR_EQUAL,  // >=

     COMMA,
     END)

using std::string;
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
                        default:
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
                    tokens_.emplace_back(TokenType::EQUAL);
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
                        default:
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
                        default:
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

// #include <gtest/gtest.h>
//
// TEST(SCANNER, NoArgs) {
//     const auto *program = "123 foo () + - * / = := ? : ; <> < > <= >= ,";
//     const auto  actual  = Scanner::scan(program);
//     const std::vector<Token> expected{
//         Token{TokenType::NUMBER},
//         Token{TokenType::IDENTIFIER, "foo"},
//
//         Token{TokenType::L_PARENTHESIS},
//         Token{TokenType::R_PARENTHESIS},
//
//         Token{TokenType::PLUS},
//         Token{TokenType::MINUS},
//         Token{TokenType::MULT},
//         Token{TokenType::DIV},
//
//         Token{TokenType::EQUAL},
//         Token{TokenType::WALRUS},
//
//         Token{TokenType::QUESTION_MARK},
//         Token{TokenType::COLON},
//         Token{TokenType::SEP},
//
//         Token{TokenType::NOT_EQUAL},
//         Token{TokenType::LESS},
//         Token{TokenType::GREATER},
//         Token{TokenType::LESS_OR_EQUAL},
//         Token{TokenType::GREATER_OR_EQUAL},
//         Token{TokenType::COMMA},
//
//         Token{TokenType::END},
//     };
//
//     ASSERT_EQ(expected, actual);
// }

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

using AdjListT = std::vector<std::unordered_set<uint64_t>>;

class Parser {
 public:
    static auto parse(const std::vector<Token> &tokens)
        -> std::tuple<AdjListT, std::vector<std::string>> {
        Parser parser(tokens);
        parser.parse_program();

        std::vector<std::string> index2func_name(parser.func_count_);
        std::for_each(parser.func_name2index_.begin(),
                      parser.func_name2index_.end(),
                      [&index2func_name](
                          const std::pair<std::string, uint64_t> &mapping) {
                          const auto [name, index]  = mapping;
                          index2func_name.at(index) = name;
                      });

        for (uint64_t func_i = 0; func_i < index2func_name.size(); ++func_i) {
            if (!parser.already_defined_.at(func_i)) {
                const auto &undefined_function_name =
                    index2func_name.at(func_i);
                throw std::runtime_error("Couldn't find definition for " +
                                         undefined_function_name);
            }
        }

        return {parser.functions_, index2func_name};
    }

 private:
    const std::vector<Token>                  tokens_;
    uint64_t                                  index_{0};

    uint64_t                                  scope_variables_count_{0};
    std::unordered_set<std::string>           scope_variables_;

    uint64_t                                  parsing_func_index_{0};
    AdjListT                                  functions_;
    std::unordered_map<std::string, uint64_t> func_name2index_;
    std::vector<bool>                         already_defined_;
    uint64_t                                  func_count_{0};

    explicit Parser(const std::vector<Token> &tokens) : tokens_(tokens) {
    }

    [[nodiscard]] auto peek() const -> const Token & {
        return tokens_.at(index_);
    }

    auto advance() -> void {
        ++index_;
    }

    auto parse_program() -> void {
        while (!match({TokenType::END})) {
            function();
        }
    }

    auto consume(std::initializer_list<TokenType> expected) -> void {
        if (match(expected)) {
            advance();
            return;
        }
        const auto  next = peek();
        std::string expected_types_str =
            "[" +
            std::accumulate(
                expected.begin(),
                expected.end(),
                std::string(),
                [](const string &res, const TokenType expected_token_type) {
                    return res + toString(expected_token_type) + ',';
                }) +
            "]";

        throw std::runtime_error(
            "Unexpected token type: " + toString(next.type) + ". Expected " +
            expected_types_str);
    }

    auto match(std::initializer_list<TokenType> expected) -> bool {
        const auto next = peek();
        return std::any_of(expected.begin(),
                           expected.end(),
                           [next](const TokenType expected_type) {
                               return next.type == expected_type;
                           });
    }

    auto mangle_function_name(const std::string &name, const uint64_t arg_count)
        -> std::string {
        return name + "$" + std::to_string(arg_count);
    }

    auto function() -> void {
        if (!match({TokenType::IDENTIFIER})) {
            const auto not_ident = peek();
            throw std::runtime_error("Expected identifier. Got " +
                                     toString(not_ident.type));
        }
        const auto func_token = peek();
        assert(func_token.value.has_value());
        functions_.emplace_back();
        advance();

        formal_args_list();
        // поддержка перегрузки по количеству параметров
        const auto &mangled_function_name = mangle_function_name(
            func_token.value.value(), scope_variables_count_);
        const auto found = func_name2index_.find(mangled_function_name);
        if (found == func_name2index_.end()) {
            func_name2index_[mangled_function_name] = parsing_func_index_ =
                func_count_;
            already_defined_.resize(++func_count_);
        } else {
            parsing_func_index_ = found->second;
            if (already_defined_.at(parsing_func_index_)) {
                throw std::runtime_error("Found redefinition of " +
                                         mangled_function_name);
            }
        }

        consume({TokenType::WALRUS});
        expression();
        consume({TokenType::SEP});
        already_defined_.at(parsing_func_index_) = true;
        functions_.resize(func_count_);
    }

    auto formal_args_list() -> void {
        scope_variables_count_ = 0;
        scope_variables_.clear();
        scope_variables_.clear();

        consume({TokenType::L_PARENTHESIS});
        if (match({TokenType::R_PARENTHESIS})) {
            advance();
            return;
        }
        formal_arg();
        while (match({TokenType::COMMA})) {
            advance();
            formal_arg();
        }
        consume({TokenType::R_PARENTHESIS});
    }

    auto formal_arg() -> void {
        const auto &argument = peek();
        consume({TokenType::IDENTIFIER});
        assert(argument.value.has_value());

        const auto &argument_name = argument.value.value();
        const auto  found         = scope_variables_.find(argument_name);
        if (found != scope_variables_.end()) {
            throw std::runtime_error("Found redeclaration of formal argument");
        }
        scope_variables_.insert(argument_name);
        ++scope_variables_count_;
    }

    auto expression() -> void {
        comparison_expr();
        if (!match({TokenType::QUESTION_MARK})) {
            return;
        }
        advance();
        comparison_expr();
        consume({TokenType::COLON});
        expression();
    }

    auto comparison_expr() -> void {
        arith_expr();
        if (!match({TokenType::EQUAL,
                    TokenType::NOT_EQUAL,
                    TokenType::LESS,
                    TokenType::GREATER,
                    TokenType::LESS_OR_EQUAL,
                    TokenType::GREATER_OR_EQUAL})) {
            return;
        }
        advance();
        arith_expr();
    }

    // На НЕ нужен приоритет операций
    auto arith_expr() -> void {
        unary();
        while (match({TokenType::PLUS,
                      TokenType::MINUS,
                      TokenType::MULT,
                      TokenType::DIV})) {
            advance();
            unary();
        }
    }

    auto unary() -> void {
        while (match({TokenType::MINUS})) {
            advance();
        }
        primary();
    }

    auto primary() -> void {
        if (match({TokenType::NUMBER})) {
            advance();
            return;
        }
        if (match({TokenType::L_PARENTHESIS})) {
            advance();
            expression();
            consume({TokenType::R_PARENTHESIS});
            return;
        }

        if (!match({TokenType::IDENTIFIER})) {
            const auto &token = peek();
            throw std::runtime_error("Expected identifier. Got " +
                                     toString(token.type));
        }
        const auto &identifier = peek();
        assert(identifier.value.has_value());
        advance();

        if (match({TokenType::L_PARENTHESIS})) {
            const auto  actual_arg_list_length = actual_args_list();

            const auto &calling_function_name  = identifier.value.value();
            if (scope_variables_.find(calling_function_name) ==
                scope_variables_.end()) {

                const auto mangled_calling_function_name = mangle_function_name(
                    calling_function_name, actual_arg_list_length);

                uint64_t calling_func_index;
                if (const auto found =
                        func_name2index_.find(mangled_calling_function_name);
                    found == func_name2index_.end()) {
                    func_name2index_.emplace(mangled_calling_function_name,
                                             func_count_);
                    calling_func_index = func_count_;
                    already_defined_.resize(++func_count_);

                } else {
                    calling_func_index = found->second;
                }

                functions_.at(parsing_func_index_).insert(calling_func_index);
            }
        } else {
            const auto &identifier_name = identifier.value.value();
            if (const auto found = scope_variables_.find(identifier_name);
                found == scope_variables_.end()) {
                throw std::runtime_error("Unknown parameter: " +
                                         identifier_name);
            }
        }
    }

    auto actual_args_list() -> uint64_t {
        consume({TokenType::L_PARENTHESIS});
        if (match({TokenType::R_PARENTHESIS})) {
            advance();
            return 0;
        }
        expression();
        uint64_t actual_arg_list_length = 1;
        while (match({TokenType::COMMA})) {
            advance();
            expression();
            ++actual_arg_list_length;
        }
        consume({TokenType::R_PARENTHESIS});
        return actual_arg_list_length;
    }
};

// TEST(Parser, basic) {
//     const auto *program                 = "foo(x, y) := x + y;";
//     const auto  tokens                  = Scanner::scan(program);
//     const auto [graph, index2func_name] = Parser::parse(tokens);
//
//     AdjListT expected_graph{
//         {},
//     };
//     EXPECT_EQ(expected_graph, graph);
//     EXPECT_EQ(index2func_name[0], "foo$2");
// }
//
// TEST(Parser, recursion) {
//     const auto *program                 = R"(
//         foo(x, y) := x + y + foo(x, y);
//     )";
//     const auto  tokens                  = Scanner::scan(program);
//     const auto [graph, index2func_name] = Parser::parse(tokens);
//
//     AdjListT expected_graph{{0}};
//     EXPECT_EQ(expected_graph, graph);
//     EXPECT_EQ(index2func_name[0], "foo$2");
// }
//
// TEST(Parser, overload) {
//     const auto *program                 = R"(
//         foo(x, y) := x + y + foo(x, y, 1);
//         foo(x, y, z) := x + y + z;
//     )";
//     const auto  tokens                  = Scanner::scan(program);
//     const auto [graph, index2func_name] = Parser::parse(tokens);
//
//     AdjListT expected_graph{{1}, {}};
//     EXPECT_EQ(expected_graph, graph);
//     EXPECT_EQ(index2func_name[0], "foo$2");
//     EXPECT_EQ(index2func_name[1], "foo$3");
// }
//
// TEST(Parser, cycle) {
//     const auto *program                 = R"(
//         foo(x, y) := x + y + foo(x, y, 1);
//         foo(x, y, z) := x + y + z - foo(x, z);
//     )";
//     const auto  tokens                  = Scanner::scan(program);
//     const auto [graph, index2func_name] = Parser::parse(tokens);
//
//     AdjListT expected_graph{{1}, {0}};
//     EXPECT_EQ(expected_graph, graph);
//     EXPECT_EQ(index2func_name[0], "foo$2");
//     EXPECT_EQ(index2func_name[1], "foo$3");
// }
//
// TEST(Parser, GrammarTest) {
//     const auto *program                 = R"(
//         foo(x, y) := ((x = 0 ? x + y + foo(x - 1, y) : 1)) / (x + y + -3) -
//         ----1 ? 1111 : 2 / 31;
//     )";
//     const auto  tokens                  = Scanner::scan(program);
//     const auto [graph, index2func_name] = Parser::parse(tokens);
//
//     AdjListT expected_graph{{0}};
//     EXPECT_EQ(expected_graph, graph);
//     EXPECT_EQ(index2func_name[0], "foo$2");
// }
//
// TEST(Parser, Redefinition) {
//     const auto *program = R"(
//         foo(x, y) :=  x + y;
//         foo(x, z) :=  x - y;
//     )";
//     const auto  tokens  = Scanner::scan(program);
//
//     try {
//         Parser::parse(tokens);
//         FAIL() << "Expected redefinition error\n";
//     } catch (const std::runtime_error &error) {
//         ASSERT_EQ("Found redefinition of foo$2", std::string(error.what()));
//     }
// }

class TarjanTraverser {
 public:
    static auto build(const AdjListT &graph) -> uint64_t {
        auto traverser = TarjanTraverser(graph);
        return traverser.component_count;
    }

 private:
    AdjListT              graph_;
    std::vector<bool>     visited_;
    std::vector<bool>     on_stack_;
    std::stack<uint64_t>  stack_;
    std::vector<uint64_t> low_;
    std::vector<uint64_t> ids_;
    uint64_t              timestamp_{0};
    uint64_t              component_count{0};

    auto                  dfs(uint64_t start) -> void {
        stack_.push(start);
        on_stack_[start] = true;
        visited_[start]  = true;
        low_[start] = ids_[start] = timestamp_++;
        for (const auto child : graph_[start]) {
            if (!visited_[child]) {
                dfs(child);
            }
            if (on_stack_[child]) {
                low_[start] = std::min(low_[start], low_[child]);
            }
        }

        // нашли компоненту сильной связности
        if (low_[start] == ids_[start]) {
            ++component_count;
            while (!stack_.empty()) {
                const auto vertex = stack_.top();
                stack_.pop();
                on_stack_[vertex] = false;
                if (vertex == start) {
                    break;
                }
            }
        }
    }

    auto traverse() -> void {
        for (uint64_t i = 0; i < graph_.size(); ++i) {
            if (!visited_[i]) {
                dfs(i);
            }
        }
    }

    explicit TarjanTraverser(AdjListT graph)
        : graph_(std::move(graph)), visited_(graph_.size()),
          on_stack_(graph_.size()), low_(graph_.size()), ids_(graph_.size()) {
        traverse();
    }
};

// TEST(TarjanTraverser, DAG) {
//     AdjListT graph{
//         {1, 2},
//         {2},
//         {3},
//         {}
//     };
//     const auto res = TarjanTraverser::build(graph);
//     ASSERT_EQ(4, res);
// }
//
// TEST(TarjanTraverser, Cycle) {
//     AdjListT graph{
//         {1},
//         {0},
//     };
//     const auto res = TarjanTraverser::build(graph);
//     ASSERT_EQ(1, res);
// }
//
// TEST(TarjanTraverser, SelfLoop) {
//     AdjListT graph{
//         {0},
//     };
//     const auto res = TarjanTraverser::build(graph);
//     ASSERT_EQ(1, res);
// }
//
// TEST(TarjanTraverser, ColoringFurther) {
//     AdjListT graph{
//         {1},
//         {0, 2},
//         {3},
//         {}
//     };
//     const auto res = TarjanTraverser::build(graph);
//     ASSERT_EQ(3, res);
// }
//
// TEST(TarjanTraverser, Complexj) {
//     AdjListT graph{
//         {1},
//         {0, 2},
//         {3},
//         {},
//         {3, 5},
//         {6},
//         {4}
//     };
//     const auto res = TarjanTraverser::build(graph);
//     ASSERT_EQ(4, res);
// }

auto main() -> int {
    std::string program;
    while (!std::cin.eof()) {
        std::string current_line;
        std::getline(std::cin, current_line);
        program += current_line + '\n';
    }
    try {
        const auto tokens          = Scanner::scan(program);
        const auto [graph, _]      = Parser::parse(tokens);
        const auto component_count = TarjanTraverser::build(graph);
        std::cout << component_count << std::endl;
    } catch (const std::runtime_error &) {
        std::cout << "error" << std::endl;
    }
}
