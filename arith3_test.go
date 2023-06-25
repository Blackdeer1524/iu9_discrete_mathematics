package main

import "testing"

func TestLexer3(t *testing.T) {
	expr := " +- /  + )(  1017676669  (  (  )  -* / yjauj /  ) " +
		"*-  (  +**(  - 1578561866+  )  +  - 1235884185  ( " +
		"/- *if - (() - )  ) 394678641) ) / )( * +  / 791929465 " +
		"- - /  - 1717434326 )  1331201259 * ( +1288349395 nne " +
		")  y  1821794006 / * fieh3oag**  *  iroxnceke  +jalam " +
		"*  -  -*  ) z3zae  "
	lexems := make(chan Lexem)
	go lexer(expr, lexems)

	var lexem Lexem
	var ok bool
	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != PLUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался PLUS", lexem.Tag)
	}
	if lexem.Image != "+" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '+'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MINUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MINUS", lexem.Tag)
	}
	if lexem.Image != "-" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '-'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != DIV {
		t.Errorf("Неожиданный тег лексемы %v, ожидался DIV", lexem.Tag)
	}
	if lexem.Image != "/" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '/'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != PLUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался PLUS", lexem.Tag)
	}
	if lexem.Image != "+" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '+'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != RPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался RPAREN", lexem.Tag)
	}
	if lexem.Image != ")" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался ')'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != LPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался LPAREN", lexem.Tag)
	}
	if lexem.Image != "(" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '('", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "1017676669" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '1017676669'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != LPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался LPAREN", lexem.Tag)
	}
	if lexem.Image != "(" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '('", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != LPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался LPAREN", lexem.Tag)
	}
	if lexem.Image != "(" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '('", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != RPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался RPAREN", lexem.Tag)
	}
	if lexem.Image != ")" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался ')'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MINUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MINUS", lexem.Tag)
	}
	if lexem.Image != "-" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '-'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MUL {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MUL", lexem.Tag)
	}
	if lexem.Image != "*" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '*'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != DIV {
		t.Errorf("Неожиданный тег лексемы %v, ожидался DIV", lexem.Tag)
	}
	if lexem.Image != "/" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '/'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "yjauj" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался 'yjauj'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != DIV {
		t.Errorf("Неожиданный тег лексемы %v, ожидался DIV", lexem.Tag)
	}
	if lexem.Image != "/" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '/'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != RPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался RPAREN", lexem.Tag)
	}
	if lexem.Image != ")" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался ')'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MUL {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MUL", lexem.Tag)
	}
	if lexem.Image != "*" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '*'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MINUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MINUS", lexem.Tag)
	}
	if lexem.Image != "-" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '-'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != LPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался LPAREN", lexem.Tag)
	}
	if lexem.Image != "(" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '('", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != PLUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался PLUS", lexem.Tag)
	}
	if lexem.Image != "+" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '+'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MUL {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MUL", lexem.Tag)
	}
	if lexem.Image != "*" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '*'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MUL {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MUL", lexem.Tag)
	}
	if lexem.Image != "*" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '*'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != LPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался LPAREN", lexem.Tag)
	}
	if lexem.Image != "(" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '('", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MINUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MINUS", lexem.Tag)
	}
	if lexem.Image != "-" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '-'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "1578561866" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '1578561866'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != PLUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался PLUS", lexem.Tag)
	}
	if lexem.Image != "+" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '+'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != RPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался RPAREN", lexem.Tag)
	}
	if lexem.Image != ")" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался ')'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != PLUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался PLUS", lexem.Tag)
	}
	if lexem.Image != "+" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '+'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MINUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MINUS", lexem.Tag)
	}
	if lexem.Image != "-" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '-'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "1235884185" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '1235884185'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != LPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался LPAREN", lexem.Tag)
	}
	if lexem.Image != "(" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '('", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != DIV {
		t.Errorf("Неожиданный тег лексемы %v, ожидался DIV", lexem.Tag)
	}
	if lexem.Image != "/" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '/'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MINUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MINUS", lexem.Tag)
	}
	if lexem.Image != "-" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '-'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MUL {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MUL", lexem.Tag)
	}
	if lexem.Image != "*" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '*'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "if" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался 'if'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MINUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MINUS", lexem.Tag)
	}
	if lexem.Image != "-" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '-'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != LPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался LPAREN", lexem.Tag)
	}
	if lexem.Image != "(" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '('", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != LPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался LPAREN", lexem.Tag)
	}
	if lexem.Image != "(" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '('", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != RPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался RPAREN", lexem.Tag)
	}
	if lexem.Image != ")" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался ')'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MINUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MINUS", lexem.Tag)
	}
	if lexem.Image != "-" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '-'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != RPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался RPAREN", lexem.Tag)
	}
	if lexem.Image != ")" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался ')'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != RPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался RPAREN", lexem.Tag)
	}
	if lexem.Image != ")" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался ')'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "394678641" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '394678641'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != RPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался RPAREN", lexem.Tag)
	}
	if lexem.Image != ")" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался ')'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != RPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался RPAREN", lexem.Tag)
	}
	if lexem.Image != ")" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался ')'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != DIV {
		t.Errorf("Неожиданный тег лексемы %v, ожидался DIV", lexem.Tag)
	}
	if lexem.Image != "/" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '/'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != RPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался RPAREN", lexem.Tag)
	}
	if lexem.Image != ")" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался ')'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != LPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался LPAREN", lexem.Tag)
	}
	if lexem.Image != "(" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '('", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MUL {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MUL", lexem.Tag)
	}
	if lexem.Image != "*" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '*'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != PLUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался PLUS", lexem.Tag)
	}
	if lexem.Image != "+" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '+'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != DIV {
		t.Errorf("Неожиданный тег лексемы %v, ожидался DIV", lexem.Tag)
	}
	if lexem.Image != "/" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '/'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "791929465" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '791929465'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MINUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MINUS", lexem.Tag)
	}
	if lexem.Image != "-" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '-'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MINUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MINUS", lexem.Tag)
	}
	if lexem.Image != "-" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '-'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != DIV {
		t.Errorf("Неожиданный тег лексемы %v, ожидался DIV", lexem.Tag)
	}
	if lexem.Image != "/" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '/'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MINUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MINUS", lexem.Tag)
	}
	if lexem.Image != "-" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '-'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "1717434326" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '1717434326'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != RPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался RPAREN", lexem.Tag)
	}
	if lexem.Image != ")" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался ')'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "1331201259" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '1331201259'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MUL {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MUL", lexem.Tag)
	}
	if lexem.Image != "*" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '*'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != LPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался LPAREN", lexem.Tag)
	}
	if lexem.Image != "(" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '('", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != PLUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался PLUS", lexem.Tag)
	}
	if lexem.Image != "+" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '+'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "1288349395" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '1288349395'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "nne" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался 'nne'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != RPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался RPAREN", lexem.Tag)
	}
	if lexem.Image != ")" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался ')'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "y" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался 'y'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "1821794006" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '1821794006'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != DIV {
		t.Errorf("Неожиданный тег лексемы %v, ожидался DIV", lexem.Tag)
	}
	if lexem.Image != "/" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '/'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MUL {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MUL", lexem.Tag)
	}
	if lexem.Image != "*" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '*'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "fieh3oag" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался 'fieh3oag'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MUL {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MUL", lexem.Tag)
	}
	if lexem.Image != "*" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '*'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MUL {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MUL", lexem.Tag)
	}
	if lexem.Image != "*" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '*'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MUL {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MUL", lexem.Tag)
	}
	if lexem.Image != "*" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '*'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "iroxnceke" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался 'iroxnceke'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != PLUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался PLUS", lexem.Tag)
	}
	if lexem.Image != "+" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '+'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "jalam" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался 'jalam'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MUL {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MUL", lexem.Tag)
	}
	if lexem.Image != "*" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '*'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MINUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MINUS", lexem.Tag)
	}
	if lexem.Image != "-" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '-'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MINUS {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MINUS", lexem.Tag)
	}
	if lexem.Image != "-" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '-'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != MUL {
		t.Errorf("Неожиданный тег лексемы %v, ожидался MUL", lexem.Tag)
	}
	if lexem.Image != "*" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '*'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != RPAREN {
		t.Errorf("Неожиданный тег лексемы %v, ожидался RPAREN", lexem.Tag)
	}
	if lexem.Image != ")" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался ')'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "z3zae" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался 'z3zae'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if ok {
		t.Error("Канал должен быть пустым")
	}
}
