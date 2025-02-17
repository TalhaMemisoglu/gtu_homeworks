GTU:
	bison -Wcounterexamples -d .\gpp_interpreter.y
	flex .\gpp_lexer.l
	gcc lex.yy.c gpp_interpreter.tab.c -o deneme
	./deneme.exe
