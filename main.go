package main

import (
	"bufio"
	"fmt"
	"os"
)

type Parser struct {
	index   int
	program string
}

/*
PROGRAMM ::= EXPR
EXPR     ::= "(" OPERATOR OPERAND OPERAND ")"
OPERATOR ::= ('+'|'-'|'*')
OPERAND  ::= NUMBER | EXPR
*/

func (parser *Parser) Parse() int {
	res := parser.expr()
	return res
}

func (parser *Parser) skipWhiteSpaces() {
	for parser.index < len(parser.program) && parser.program[parser.index] == ' ' {
		parser.index++
	}
}

func (parser *Parser) peek() byte {
	if parser.index >= len(parser.program) {
		return 0
	}
	parser.skipWhiteSpaces()
	res := parser.program[parser.index]
	return res
}

func (parser *Parser) advance() {
	parser.skipWhiteSpaces()
	parser.index++
}

func (parser *Parser) expr() int {
	parser.advance() // пропуск "("

	op := parser.peek()
	parser.advance()

	left := parser.operand()
	right := parser.operand()

	result := 0
	switch op {
	case '+':
		result = left + right
	case '-':
		result = left - right
	case '*':
		result = left * right
	}

	parser.advance() // пропуск ")"
	return result
}

func (parser *Parser) operand() int {
	entity := parser.peek()
	if entity == '(' {
		return parser.expr()
	}
	parser.advance()
	return int(entity - '0')
}

func main() {
	// file, err := os.Open("/home/blackdeer/projects/discrete/txt_tests/main_input0.txt")
	// if err != nil {
	// 	panic(err.Error())
	// }

	reader := bufio.NewReader(os.Stdin)
	expression, _ := reader.ReadString('\n')

	parser := Parser{program: expression}
	res := parser.Parse()

	fmt.Println(res)
}
