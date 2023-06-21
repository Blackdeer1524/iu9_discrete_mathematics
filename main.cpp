#include <algorithm>
#include <assert.h>
#include <cstdint>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class SyntaxError : public std::runtime_error {
 public:
    explicit SyntaxError(const std::string &message)
        : std::runtime_error(message) {
    }

    [[nodiscard]] auto what() const noexcept -> const char * override {
        return what_message.c_str();
    }

 private:
    std::string what_message;
};

class CycleError : public std::runtime_error {
 public:
    explicit CycleError(const std::string &message)
        : std::runtime_error(message) {
    }

    [[nodiscard]] auto what() const noexcept -> const char * override {
        return what_message.c_str();
    }

 private:
    std::string what_message;
};

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
        -> std::tuple<std::vector<std::vector<Token>>,
                      std::vector<std::string>> {
        Scanner scanner(program);
        auto [statements, formulae] = scanner.scan();
        return {statements, formulae};
    }

 private:
    uint64_t                        index_{0};

    std::vector<std::string>        formulae_;
    std::vector<Token>              statement_tokens_;
    std::vector<std::vector<Token>> statements_;
    const std::string              &program_;

    explicit Scanner(const std::string &program) : program_(program) {
    }

    auto scan() noexcept(false) -> std::tuple<std::vector<std::vector<Token>>,
                                              std::vector<std::string>> {
        uint64_t start = 0;
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
                        formulae_.push_back(
                            program_.substr(start, index_ - start));
                        start = index_ + 1;
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
                        throw SyntaxError("Unexpected character: "s + c);
                    }
                    break;
            }
        }
        if (!statement_tokens_.empty()) {
            statement_tokens_.emplace_back(TokenType::END);
            formulae_.push_back(program_.substr(start, index_ - start));
            statements_.push_back(statement_tokens_);
            statement_tokens_.clear();
        }
        return {statements_, formulae_};
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

// #include <gtest/gtest.h>
//
// TEST(Scanner, Assignment) {
//     const auto *program           = "a = 1";
//     const auto [actual, formulae] = Scanner::scan(program);
//     const std::vector<std::vector<Token>> expected{
//         {
//          Token{TokenType::IDENTIFIER, "a"},
//          Token{TokenType::ASSIGN},
//          Token{TokenType::NUMBER},
//          Token{TokenType::END},
//          }
//     };
//
//     ASSERT_EQ(expected, actual);
// }
//
// TEST(Scanner, CompoundAssignment) {
//     const auto *program           = "a, b = 1, c";
//     const auto [actual, formulae] = Scanner::scan(program);
//     const std::vector<std::vector<Token>> expected{
//         {
//          Token{TokenType::IDENTIFIER, "a"},
//          Token{TokenType::COMMA},
//          Token{TokenType::IDENTIFIER, "b"},
//          Token{TokenType::ASSIGN},
//          Token{TokenType::NUMBER},
//          Token{TokenType::COMMA},
//          Token{TokenType::IDENTIFIER, "c"},
//          Token{TokenType::END},
//          }
//     };
//
//     ASSERT_EQ(expected, actual);
// }
//
// TEST(Scanner, CompoundAssignmentWithGrouping) {
//     const auto *program           = "a, b = (1 + c), 3";
//     const auto [actual, formulae] = Scanner::scan(program);
//     const std::vector<std::vector<Token>> expected{
//         {
//          Token{TokenType::IDENTIFIER, "a"},
//          Token{TokenType::COMMA},
//          Token{TokenType::IDENTIFIER, "b"},
//          Token{TokenType::ASSIGN},
//          Token{TokenType::L_PARENTHESIS},
//          Token{TokenType::NUMBER},
//          Token{TokenType::PLUS},
//          Token{TokenType::IDENTIFIER, "c"},
//          Token{TokenType::R_PARENTHESIS},
//          Token{TokenType::COMMA},
//          Token{TokenType::NUMBER},
//          Token{TokenType::END},
//          }
//     };
//
//     ASSERT_EQ(expected, actual);
// }
//
// TEST(Scanner, Multiline) {
//     const auto *program           = R"(
//         a = 3
//         b = a
// )";
//
//     const auto [actual, formulae] = Scanner::scan(program);
//     const std::vector<std::vector<Token>> expected{
//         {
//          Token{TokenType::IDENTIFIER, "a"},
//          Token{TokenType::ASSIGN},
//          Token{TokenType::NUMBER},
//          Token{TokenType::END},
//          },
//         {
//          Token{TokenType::IDENTIFIER, "b"},
//          Token{TokenType::ASSIGN},
//          Token{TokenType::IDENTIFIER, "a"},
//          Token{TokenType::END},
//          }
//     };
//
//     ASSERT_EQ(expected, actual);
// }

using std::string_literals::operator""s;

// Книжка по интерпретаторам для референса
// https://craftinginterpreters.com/
//
/* Хоть нам и не нужна приоритетность операций. Но почему нет
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

    // clang-format off
    auto peek() -> const Token &{
        return tokens_.at(index_);
    }

    // clang-format on

    auto                                       advance() -> void {
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
        throw SyntaxError("Unexpected token type: "s +
                          toString(next_token.type));
    }

    auto at_end() -> bool {
        return index_ >= tokens_.size();
    }

    auto statement() -> void {
        const auto left_count  = count_left();
        const auto right_count = count_right();

        if (left_count != right_count) {
            throw SyntaxError(
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
            } else {
                const auto &next_token = peek();
                throw SyntaxError("Unexpected token type: "s +
                                  toString(next_token.type));
            }

            if (match({COMMA})) {
                advance();
            } else if (match({ASSIGN})) {
                advance();
                break;
            } else {
                const auto &next_token = peek();
                throw SyntaxError("Unexpected token type: "s +
                                  toString(next_token.type));
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
        } else if (match({TokenType::L_PARENTHESIS})) {
            advance();
            expression();
            consume({TokenType::R_PARENTHESIS});
        } else if (match({TokenType::IDENTIFIER})) {
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

            dependincies_.push_back(index);
            advance();
        } else {
            const auto &next = peek();
            throw SyntaxError("Unexpected token type: "s + toString(next.type));
        }
        // consume({TokenType::END});
    }
};

// TEST(Parser, Assignment) {
//     const auto *program           = "a = 1";
//     const auto [tokens, formulae] = Scanner::scan(program);
//     std::unordered_map<std::string, uint64_t>                var2index;
//     uint64_t                                                 var_count = 0;
//
//     std::tuple<std::vector<uint64_t>, std::vector<uint64_t>> res;
//     ASSERT_NO_THROW(res = Parser::parse(tokens.at(0), var2index,
//     var_count);); const auto [dependent, dependencies] = res;
//
//     const std::vector<uint64_t> expected_dependent{0};
//     const std::vector<uint64_t> expected_dependencies{};
//
//     EXPECT_EQ(expected_dependent, dependent);
//     EXPECT_EQ(expected_dependencies, dependencies);
// }
//
// TEST(Parser, GrammarTest) {
//     const auto *program           = "a = (c + b) / c + d - ((-3)) * --(+4)";
//     const auto [tokens, formulae] = Scanner::scan(program);
//     std::unordered_map<std::string, uint64_t>                var2index;
//     uint64_t                                                 var_count = 0;
//
//     std::tuple<std::vector<uint64_t>, std::vector<uint64_t>> res;
//     ASSERT_NO_THROW(res = Parser::parse(tokens.at(0), var2index,
//     var_count);); const auto [dependent, dependencies] = res;
//
//     const std::vector<uint64_t> expected_dependent{0};
//     const std::vector<uint64_t> expected_dependencies{1, 2, 1, 3};
//
//     EXPECT_EQ(expected_dependent, dependent);
//     EXPECT_EQ(expected_dependencies, dependencies);
// }
//
// TEST(Parser, CompoundAssignment) {
//     const auto *program           = "a, b = 1, c";
//     const auto [tokens, formulae] = Scanner::scan(program);
//     std::unordered_map<std::string, uint64_t>                var2index;
//     uint64_t                                                 var_count = 0;
//
//     std::tuple<std::vector<uint64_t>, std::vector<uint64_t>> res;
//     ASSERT_NO_THROW(res = Parser::parse(tokens.at(0), var2index,
//     var_count);); const auto [dependent, dependencies] = res;
//
//     const std::vector<uint64_t> expected_dependent{0, 1};
//     const std::vector<uint64_t> expected_dependencies{2};
//
//     EXPECT_EQ(expected_dependent, dependent);
//     EXPECT_EQ(expected_dependencies, dependencies);
// }
//
// TEST(Parser, WrongParenthesesCount) {
//     const auto *program           = "a = ((C)";
//     const auto [tokens, formulae] = Scanner::scan(program);
//     std::unordered_map<std::string, uint64_t>                var2index;
//     uint64_t                                                 var_count = 0;
//
//     std::tuple<std::vector<uint64_t>, std::vector<uint64_t>> res;
//     try {
//         res = Parser::parse(tokens.at(0), var2index, var_count);
//         FAIL() << "Expected SyntaxError";
//     } catch (const SyntaxError &error) {
//         ASSERT_EQ("Unexpected token type: "s + toString(TokenType::END),
//                   error.what());
//     }
// }
//
// TEST(Parser, WrongAssignmentCount) {
//     const auto *program           = "a, b = 1";
//     const auto [tokens, formulae] = Scanner::scan(program);
//     std::unordered_map<std::string, uint64_t> var2index;
//     uint64_t                                  var_count = 0;
//
//     try {
//         Parser::parse(tokens.at(0), var2index, var_count);
//         FAIL() << "Expected SyntaxError";
//     } catch (const SyntaxError &error) {
//         ASSERT_EQ(
//             std::string(
//                 "Number of identifiers on the left side is not equal to the "
//                 "number of assigned expressions on the right"),
//             error.what());
//     }
// }
//
// TEST(Parser, EmptyLeftSide) {
//     const auto *program           = " = 1";
//     const auto [tokens, formulae] = Scanner::scan(program);
//     std::unordered_map<std::string, uint64_t> var2index;
//     uint64_t                                  var_count = 0;
//
//     try {
//         Parser::parse(tokens.at(0), var2index, var_count);
//         FAIL() << "Expected SyntaxError";
//     } catch (const SyntaxError &error) {
//         ASSERT_EQ("Unexpected token type: "s + toString(TokenType::ASSIGN),
//                   error.what());
//     }
// }
//
// TEST(Parser, EmptyRightSide) {
//     const auto *program           = "a = ";
//     const auto [tokens, formulae] = Scanner::scan(program);
//     std::unordered_map<std::string, uint64_t> var2index;
//     uint64_t                                  var_count = 0;
//
//     try {
//         Parser::parse(tokens.at(0), var2index, var_count);
//         FAIL() << "Expected SyntaxError";
//     } catch (const SyntaxError &error) {
//         ASSERT_EQ("Unexpected token type: END", std::string(error.what()));
//     }
// }

struct Vertex {
    uint64_t                     formula_index;
    std::unordered_set<uint64_t> next;
    std::unordered_set<uint64_t> parents;
    std::vector<uint64_t>        dependencies;
};

auto produce_graph(const std::string &program) -> std::
    tuple<std::vector<Vertex>, std::vector<std::string>, std::vector<bool>> {
    const auto [tokens, formulae] = Scanner::scan(program);
    std::unordered_map<std::string, uint64_t> var2index;
    uint64_t                                  var_count = 0;

    std::unordered_map<uint64_t, uint64_t>    dependents_origins;
    std::vector<Vertex>                       formulae_graph;
    std::vector<bool>                         alread_defined;
    for (uint64_t formula_i = 0; formula_i < tokens.size(); ++formula_i) {
        const auto &statement = tokens.at(formula_i);
        const auto [dependents, dependencies] =
            Parser::parse(statement, var2index, var_count);
        alread_defined.resize(var_count);

        for (const auto dependent : dependents) {
            if (alread_defined.at(dependent)) {
                for (const auto &[variable, index] : var2index) {
                    if (index == dependent) {
                        throw SyntaxError("Found redefinition of " + variable);
                    }
                }
                throw std::runtime_error("Unreachable");
            }
            alread_defined.at(dependent)  = true;
            dependents_origins[dependent] = formula_i;
        }
        formulae_graph.push_back(Vertex{formula_i, {}, {}, dependencies});
    }

    std::vector<bool> is_base(var_count);
    for (uint64_t current_vertex_i = 0;
         current_vertex_i < formulae_graph.size();
         ++current_vertex_i) {

        const auto &formula_vertex = formulae_graph.at(current_vertex_i);
        if (formula_vertex.dependencies.empty()) {
            is_base.at(current_vertex_i) = true;
        }

        for (const auto dependency : formula_vertex.dependencies) {
            const auto dependency_origin = dependents_origins.at(dependency);

            if (dependency_origin == current_vertex_i) {
                throw CycleError("Found self-loop: " +
                                 formulae.at(dependency_origin));
            }

            formulae_graph.at(dependency_origin).next.insert(current_vertex_i);
            formulae_graph.at(current_vertex_i)
                .parents.insert(dependency_origin);
        }
    }
    return {formulae_graph, formulae, is_base};
}

class DFSTraverser {
 public:
    static auto traverse(const std::vector<Vertex> &graph,
                         const std::vector<bool>   &is_base)
        -> std::vector<uint64_t> {
        DFSTraverser traverser(graph);
        for (uint64_t i = 0; i < graph.size(); ++i) {
            if (is_base.at(i)) {
                traverser.visit_vertex(i);
            }
        }

        for (uint64_t i = 0; i < graph.size(); ++i) {
            if (traverser.visited_.at(i) == VertexColor::WHITE) {
                throw CycleError("Found unvisited vertices after DFS => there "
                                 "is a strondly connected component => cycle");
            }
        }

        return traverser.sorted_vertices_;
    }

 private:
    enum class VertexColor {
        WHITE,
        GREY,
        BLACK,
    };

    explicit DFSTraverser(std::vector<Vertex> graph)
        : graph_(graph), visited_(graph.size()) {
        sorted_vertices_.reserve(graph.size());
    }

    auto visit_vertex(uint64_t start) -> void {
        if (visited_.at(start) == VertexColor::GREY) {
            throw CycleError("Found cycle");
        }
        assert(visited_.at(start) != VertexColor::BLACK);
        visited_.at(start) = VertexColor::GREY;

        auto &current      = graph_.at(start);
        sorted_vertices_.push_back(start);
        for (const auto neighbour_i : current.next) {
            auto      &neighbour              = graph_.at(neighbour_i);
            const auto current2neighbour_edge = neighbour.parents.find(start);
            assert(current2neighbour_edge != neighbour.parents.end());

            neighbour.parents.erase(current2neighbour_edge);
            if (neighbour.parents.empty()) {
                visit_vertex(neighbour_i);
            }
        }
        visited_.at(start) = VertexColor::BLACK;
    }

    std::vector<Vertex>      graph_;
    std::vector<VertexColor> visited_;
    std::vector<uint64_t>    sorted_vertices_;
};

auto main() -> int {
    // std::ifstream foo;
    // foo.open("/home/blackdeer/projects/discrete/txt_tests/main_input11.txt");
    auto       &foo = std::cin;

    std::string program;

    while (!foo.eof()) {
        std::string statement;
        std::getline(foo, statement);
        program += statement + '\n';
    }

    std::tuple<std::vector<Vertex>, std::vector<std::string>, std::vector<bool>>
        res;
    try {
        res = produce_graph(program);
    } catch (const SyntaxError &error) {
        std::cout << "syntax error" << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::runtime_error &error) {
        std::cout << "cycle" << std::endl;
        return EXIT_SUCCESS;
    }

    const auto [graph, formulae, is_base] = std::move(res);
    std::vector<uint64_t> formulae_ordering;
    try {
        formulae_ordering = DFSTraverser::traverse(graph, is_base);
    } catch (const std::runtime_error &error) {
        std::cout << "cycle" << std::endl;
        return EXIT_SUCCESS;
    }
    for (const auto formula_i : formulae_ordering) {
        std::cout << formulae.at(formula_i) << std::endl;
    }
}
