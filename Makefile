

all:compiler

compiler:compiler.cpp
	g++ compiler.cpp -o compiler
	./compiler.exe