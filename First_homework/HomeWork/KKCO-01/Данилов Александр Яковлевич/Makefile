all: test.exe

build: main.o libpushkin.so
	@gcc -o test.exe main.o -ldl

libpushkin.so: pushkin.o
	@gcc -shared -fPIC -o libpushkin.so pushkin.o

%.o:%.c
	@gcc -c $< -o $@

run: test.exe
	@./test.exe
clean:
	@rm -f *.o *.so *.exe *.sh