package main

import "testing"

func TestLexer1(t *testing.T) {
	expr := " arqu  )+  -+( 1263529909+)sroz -  )  1285826528 903709596  " +
		"-  )*1835934868- /-187653863"

	lexems := make(chan Lexem)
	go lexer(expr, lexems)

	var lexem Lexem
	var ok bool
	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "arqu" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался 'arqu'", lexem.Image)
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
	if lexem.Image != "1263529909" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '1263529909'",
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
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "sroz" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался 'sroz'", lexem.Image)
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
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "1285826528" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '1285826528'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "903709596" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '903709596'",
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
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "1835934868" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '1835934868'",
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
	if lexem.Image != "187653863" {
		t.Errorf(
			"Неожиданный образ лексемы '%s', ожидался '187653863'",
			lexem.Image,
		)
	}

	lexem, ok = <-lexems
	if ok {
		t.Error("Канал должен быть пустым")
	}
}
