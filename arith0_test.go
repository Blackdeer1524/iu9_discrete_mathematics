package main

import "testing"

var expr = "  2017486365+  + * (630158001  * -  *  z  vyebza*  " +
	"- )  ( )  2035541385  * dulo2u- - / /) -iqdkiuy"

func TestLexer(t *testing.T) {
	lexems := make(chan Lexem)
	go lexer(expr, lexems)

	var lexem Lexem
	var ok bool
	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "2017486365" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '2017486365'",
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
	if lexem.Image != "630158001" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '630158001'",
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
	if lexem.Image != "z" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался 'z'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "vyebza" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался 'vyebza'",
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
	if lexem.Image != "2035541385" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '2035541385'",
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
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "dulo2u" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался 'dulo2u'",
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
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "iqdkiuy" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался 'iqdkiuy'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if ok {
		t.Error("Канал должен быть пустым")
	}
}
