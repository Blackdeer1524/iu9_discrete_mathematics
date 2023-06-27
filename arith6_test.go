package main

import "testing"

func TestLexer6(t *testing.T) {
	expr := "  !} 291906604  +-  * 12273124; )  - 310470301 -  " +
		"- /-495403259 ?704133513 )  * -* *  m 1053220205&  " +
		") ( 621875572 * (+ % * )  ( /  */ () )  ] {( - * /" +
		") 441491153 627080156 28505132 im4c5qa8 ]+*  1259106404  " +
		"+* 1640864249 *  +( /  /-  ( -- ) 328441566 364170848 " +
		"# /  + - *  *peuhyg  (444368037-"
	lexems := make(chan Lexem)
	go lexer(expr, lexems)

	var lexem Lexem
	var ok bool
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
	if lexem.Image != "291906604" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '291906604'", lexem.Image)
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
	if lexem.Image != "12273124" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '12273124'", lexem.Image)
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
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "310470301" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '310470301'", lexem.Image)
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
	if lexem.Image != "495403259" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '495403259'", lexem.Image)
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
	if lexem.Image != "704133513" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '704133513'", lexem.Image)
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
	if lexem.Image != "m" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался 'm'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "1053220205" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '1053220205'", lexem.Image)
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
	if lexem.Image != "621875572" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '621875572'", lexem.Image)
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
	if lexem.Tag != ERROR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался ERROR", lexem.Tag)
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
	if lexem.Tag != ERROR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался ERROR", lexem.Tag)
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
	if lexem.Image != "441491153" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '441491153'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "627080156" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '627080156'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "28505132" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '28505132'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != VAR {
		t.Errorf("Неожиданный тег лексемы %v, ожидался VAR", lexem.Tag)
	}
	if lexem.Image != "im4c5qa8" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался 'im4c5qa8'", lexem.Image)
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
	if lexem.Image != "1259106404" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '1259106404'", lexem.Image)
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
	if lexem.Image != "1640864249" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '1640864249'", lexem.Image)
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
	if lexem.Image != "328441566" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '328441566'", lexem.Image)
	}

	lexem, ok = <-lexems
	if !ok {
		t.Error("Канал не должен быть пустым")
	}
	if lexem.Tag != NUMBER {
		t.Errorf("Неожиданный тег лексемы %v, ожидался NUMBER", lexem.Tag)
	}
	if lexem.Image != "364170848" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '364170848'", lexem.Image)
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
	if lexem.Image != "peuhyg" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался 'peuhyg'", lexem.Image)
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
	if lexem.Image != "444368037" {
		t.Errorf("Неожиданный образ лексемы '%s', ожидался '444368037'", lexem.Image)
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
