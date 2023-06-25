package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

/*
PROGRAM  ::= OPERAND
EXPR     ::= "(" OPERATOR OPERAND OPERAND ")"
OPERATOR ::= ("$"|"@"|"#")
OPERAND  ::= IDENT | EXPR
IDENT    ::= 'a'..'z'
*/

type Parser struct {
	program string
	index   uint64
	result  strings.Builder
}

func (parser *Parser) Parse() string {
	if len(parser.program) <= 1 {
		return parser.program
	}

	parser.operand()
	return parser.result.String()
}

func (parser *Parser) expr() {
	parser.index++ // пропускаем "("

	operator := parser.program[parser.index]
	parser.index++

	parser.operand()
	parser.operand()

	parser.index++ // пропускаем ")"
	parser.result.WriteByte(operator)
}

func (parser *Parser) operand() {
	if parser.program[parser.index] == '(' {
		parser.expr()
	} else {
		ident := parser.program[parser.index]
		parser.result.WriteByte(ident)
		parser.index++
	}
}

type Deque struct {
	elements []string
	front    int
	back     int
	size     int
}

func (d *Deque) IsEmpty() bool {
	return d.size == 0
}

func (d *Deque) Size() int {
	return d.size
}

func (d *Deque) PushFront(element string) {
	if d.size == len(d.elements) {
		d.resize()
	}

	d.front = (d.front - 1 + len(d.elements)) % len(d.elements)
	d.elements[d.front] = element
	d.size++
}

func (d *Deque) PushBack(element string) {
	if d.size == len(d.elements) {
		d.resize()
	}

	d.elements[d.back] = element
	d.back = (d.back + 1) % len(d.elements)
	d.size++
}

func (d *Deque) PopFront() string {
	if d.IsEmpty() {
		return ""
	}

	front := d.elements[d.front]
	d.elements[d.front] = "$NULL$"
	d.front = (d.front + 1) % len(d.elements)
	d.size--

	return front
}

func (d *Deque) PopBack() string {
	if d.IsEmpty() {
		return ""
	}

	back := d.elements[(d.back-1+len(d.elements))%len(d.elements)]
	d.elements[(d.back-1+len(d.elements))%len(d.elements)] = "$NULL$"
	d.back = (d.back - 1 + len(d.elements)) % len(d.elements)
	d.size--

	return back
}

func (d *Deque) resize() {
	newCapacity := d.size * 2
	newElements := make([]string, newCapacity)

	for i := 0; i < d.size; i++ {
		newElements[i] = d.elements[(d.front+i)%len(d.elements)]
	}

	d.elements = newElements
	d.front = 0
	d.back = d.size
}

func main() {
	reader := bufio.NewReader(os.Stdin)
	expression, _ := reader.ReadString('\n')
	expression = strings.TrimSpace(expression)

	parser := Parser{program: expression}
	reversePolish := parser.Parse()

	counter := 0
	alreadyComputed := make(map[string]struct{})

	// Вроде достаточно и простоо стека, но как бы да
	// не стерать же
	operationsDeque := Deque{elements: make([]string, 1)}

	index := 0
	for index < len(reversePolish) {
		entity := reversePolish[index]
		index++
		if entity == '$' || entity == '@' || entity == '#' {
			left := operationsDeque.PopFront()
			right := operationsDeque.PopFront()

			operation := fmt.Sprintf("%s%s%c", left, right, entity)
			if _, ok := alreadyComputed[operation]; !ok {
				alreadyComputed[operation] = struct{}{}
				counter++
			}

			operationsDeque.PushFront(operation)
		} else {
			operationsDeque.PushFront(string(entity))
		}
	}

	fmt.Println(counter)
}
