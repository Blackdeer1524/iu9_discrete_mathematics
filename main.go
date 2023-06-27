package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type Tag int

const (
	ERROR  Tag = 1 << iota // Неправильная лексема
	NUMBER                 // Целое число
	VAR                    // Имя переменной
	PLUS                   // Знак +
	MINUS                  // Знак -
	MUL                    // Знак *
	DIV                    // Знак /
	LPAREN                 // Левая круглая скобка
	RPAREN                 // Правая круглая скобка
)

type Lexem struct {
	Tag
	Image string
}

func isDigit(c byte) bool {
	return '0' <= c && c <= '9'
}

func isAlpha(c byte) bool {
	return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z'
}

func lexer(expr string, lexems chan Lexem) {
	var index int
	for index != len(expr) {
		next := expr[index]
		switch next {
		case '+':
			lexems <- Lexem{Tag: PLUS, Image: "+"}
			index++
		case '-':
			lexems <- Lexem{Tag: MINUS, Image: "-"}
			index++
		case '*':
			lexems <- Lexem{Tag: MUL, Image: "*"}
			index++
		case '/':
			lexems <- Lexem{Tag: DIV, Image: "/"}
			index++
		case '(':
			lexems <- Lexem{Tag: LPAREN, Image: "("}
			index++
		case ')':
			lexems <- Lexem{Tag: RPAREN, Image: ")"}
			index++
		case '\t':
			fallthrough
		case ' ':
			fallthrough
		case '\r':
			fallthrough
		case '\n':
			index++
		default:
			start := index
			if isDigit(next) {
				for {
					index++
					if index == len(expr) {
						break
					}
					next = expr[index]
					if !isDigit(next) {
						break
					}
				}
				lexems <- Lexem{Tag: NUMBER, Image: expr[start:index]}
			} else if isAlpha(next) {
				for {
					index++
					if index == len(expr) {
						break
					}
					next = expr[index]
					if !isAlpha(next) && !isDigit(next) {
						break
					}
				}
				lexems <- Lexem{Tag: VAR, Image: expr[start:index]}
			} else {
				lexems <- Lexem{Tag: ERROR}
				index++
			}

		}
	}
	close(lexems)
}

/*
Program ::= EXPR
EXPR    ::= FACTOR ('+'|'-' FACTOR)*
FACTOR  ::= UNARY ('*'|'/' UNARY)*
UNARY   ::= ('-')* PRIMARY
PRIMARY ::= NUMBER | VAR | '(' EXPR ')'
*/

// https://craftinginterpreters.com/contents.html

type Computable interface {
	Compute() int
}

type BinPlusNode struct {
	left  Computable
	right Computable
}

func (astNode *BinPlusNode) Compute() int {
	leftRes := astNode.left.Compute()
	rightRes := astNode.right.Compute()
	return leftRes + rightRes
}

type BinMinusNode struct {
	left  Computable
	right Computable
}

func (astNode *BinMinusNode) Compute() int {
	leftRes := astNode.left.Compute()
	rightRes := astNode.right.Compute()
	return leftRes - rightRes
}

type MultiplicationNode struct {
	left  Computable
	right Computable
}

func (astNode *MultiplicationNode) Compute() int {
	leftRes := astNode.left.Compute()
	rightRes := astNode.right.Compute()
	return leftRes * rightRes
}

type DivisionNode struct {
	left  Computable
	right Computable
}

func (astNode *DivisionNode) Compute() int {
	leftRes := astNode.left.Compute()
	rightRes := astNode.right.Compute()
	return leftRes / rightRes
}

type UnaryMinusNode struct {
	operand Computable
}

func (astNode *UnaryMinusNode) Compute() int {
	res := astNode.operand.Compute()
	return -res
}

type NumberNodeNode struct {
	operand int
}

func (astNode *NumberNodeNode) Compute() int {
	return astNode.operand
}

type Parser struct {
	isBufferFull bool
	buffer       Lexem
	lexemsStream chan Lexem

	varValues          []int
	seenVariablesCount int
	varName2Index      map[string]int
}

func BuildParser(lexemsStream chan Lexem, varValues []int) Parser {
	return Parser{
		lexemsStream:  lexemsStream,
		varValues:     varValues,
		varName2Index: make(map[string]int),
	}
}

func (parser *Parser) Parse() (Computable, error) {
	res, err := parser.expr()

	if _, isStreamOpen := parser.peek(); isStreamOpen || parser.isBufferFull {
		return nil, fmt.Errorf("expected no tokens")
	}

	return res, err
}

func (parser *Parser) peek() (Lexem, bool) {
	isStreamOpen := true
	if !parser.isBufferFull {
		parser.buffer, isStreamOpen = <-parser.lexemsStream
		if isStreamOpen {
			parser.isBufferFull = true
		}
	}
	return parser.buffer, isStreamOpen
}

func (parser *Parser) match(expectedLexemTypes Tag) bool {
	next, isStreamOpen := parser.peek()
	if !isStreamOpen {
		return false
	}

	return (next.Tag & expectedLexemTypes) > 0
}

func (parser *Parser) advance() {
	parser.isBufferFull = false
}

func (parser *Parser) expr() (Computable, error) {
	node, err := parser.factor()
	if err != nil {
		return nil, err
	}

	for parser.match(PLUS | MINUS) {
		op, _ := parser.peek()
		parser.advance()

		var rightNode Computable
		rightNode, err = parser.factor()
		if err != nil {
			return nil, err
		}

		if op.Tag == PLUS {
			node = &BinPlusNode{left: node, right: rightNode}
		} else {
			node = &BinMinusNode{left: node, right: rightNode}
		}
	}
	return node, nil
}

func (parser *Parser) consume(expectedLexemTypes Tag) bool {
	if parser.match(expectedLexemTypes) {
		parser.advance()
		return true
	}
	return false
}

func (parser *Parser) factor() (Computable, error) {
	node, err := parser.unary()
	if err != nil {
		return nil, err
	}
	for parser.match(MUL | DIV) {
		op, _ := parser.peek()
		parser.advance()

		var rightNode Computable
		rightNode, err = parser.unary()
		if err != nil {
			return nil, err
		}

		if op.Tag == MUL {
			node = &MultiplicationNode{left: node, right: rightNode}
		} else {
			node = &DivisionNode{left: node, right: rightNode}
		}
	}
	return node, nil
}

func (parser *Parser) unary() (Computable, error) {
	applyMinus := false
	for parser.match(MINUS) {
		applyMinus = !applyMinus
		parser.advance()
	}
	node, err := parser.primary()
	if err != nil {
		return nil, err
	}

	if applyMinus {
		node = &UnaryMinusNode{operand: node}
	}
	return node, nil
}

func (parser *Parser) primary() (Computable, error) {
	if parser.match(NUMBER) {
		number, _ := parser.peek()
		parser.advance()

		if res, err := strconv.Atoi(number.Image); err == nil {
			return &NumberNodeNode{res}, nil
		}

		return nil, fmt.Errorf("couldn't convert number token value to int")
	}
	if parser.match(VAR) {
		variableName, _ := parser.peek()
		parser.advance()

		var node Computable
		if varIndex, ok := parser.varName2Index[variableName.Image]; !ok {
			parser.varName2Index[variableName.Image] = parser.seenVariablesCount
			varValue := parser.varValues[parser.seenVariablesCount]
			parser.seenVariablesCount++

			node = &NumberNodeNode{operand: varValue}
		} else {
			varValue := parser.varValues[varIndex]
			node = &NumberNodeNode{operand: varValue}
		}

		return node, nil
	}
	if !parser.consume(LPAREN) {
		return nil, fmt.Errorf("expected '('")
	}

	node, err := parser.expr()
	if err != nil {
		return nil, err
	}

	if !parser.consume(RPAREN) {
		return nil, fmt.Errorf("expected ')'")
	}
	return node, nil
}

func main() {
	expr := os.Args[1]

	// file, err := os.Open("/home/blackdeer/projects/discrete/txt_tests/main_input0.txt")
	// if err != nil {
	// 	panic(err.Error())
	// }
	// reader := bufio.NewReader(file)

	reader := bufio.NewReader(os.Stdin)
	// для локальных тестов
	// expr, _ := reader.ReadString('\n')
	var varValues []int
	for {
		strNums, err := reader.ReadString('\n')
		if len(strNums) == 0 {
			break
		}
		if strNums[len(strNums)-1] == '\n' {
			strNums = strNums[:len(strNums)-1]
		}

		numbers := strings.Split(strNums, " ")
		for _, v := range numbers {
			val, _ := strconv.Atoi(v)
			varValues = append(varValues, val)
		}

		if err != nil {
			break
		}
	}
	lexemsStream := make(chan Lexem)
	go lexer(expr, lexemsStream)
	parser := BuildParser(lexemsStream, varValues)

	ast, err := parser.Parse()
	if err != nil {
		fmt.Println("error")
		return
		// panic(err.Error())
	}
	res := ast.Compute()
	fmt.Println(res)
}
