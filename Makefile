all: compile run

compile: 
	gcc -o main.exe main.c

run:
	.\main.exe