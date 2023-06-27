package main

import "testing"

func TestLexer5(t *testing.T) {
	expr := "/  (  (  /) )  936125490  1951850894 ) +* 888940067 " +
		"/(*/  * ) 681242026  ( ( --^ lywyy  1349490957*) 987474184  " +
		"- -xoqfeeina+ -  fapsi  -/ / # 1933064760  sebuoge7 " +
		") (  -/ (  -"
	lexems := make(chan Lexem)
	go lexer(expr, lexems)

	var lexem Lexem
	var ok bool
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
	if lexem.Image != "936125490" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '936125490'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "1951850894" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '1951850894'", lexem.Image)
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
	if lexem.Image != "888940067" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '888940067'", lexem.Image)
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
	if lexem.Image != "681242026" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '681242026'", lexem.Image)
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
	if lexem.Tag != ERROR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался ERROR", lexem.Tag)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "lywyy" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался 'lywyy'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "1349490957" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '1349490957'", lexem.Image)
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
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "987474184" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '987474184'", lexem.Image)
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
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "xoqfeeina" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался 'xoqfeeina'", lexem.Image)
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
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "fapsi" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался 'fapsi'", lexem.Image)
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
	if lexem.Tag != ERROR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался ERROR", lexem.Tag)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "1933064760" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '1933064760'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "sebuoge7" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался 'sebuoge7'", lexem.Image)
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
	if ok {
		t.Error("Канал должен быть пустым")
	}
}
