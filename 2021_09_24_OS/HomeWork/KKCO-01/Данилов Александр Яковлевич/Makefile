all: test.exe

build: main.c
	@gcc main.c -lmcheck -o test.exe

run: test.exe
	@./test.exe

test.exe: build
	@./test.exe

debug: main.c
	@gcc -DDEBUG main.c -lmcheck -o test.exe

clean:
	@rm -f *.o *.so *.exe *.log
