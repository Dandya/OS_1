CC=gcc

all:
	@echo "Использовать:"
	@echo "main - cоздание просто программы"
	@echo "castum - cоздание программы с ключем"
	@echo "debug - программка с отладочным кодом"
	@echo "nodebug - программка без отладочного кода"
	@echo "hack - немного веселья"

main: main.c
	$(CC) main.c
castum: main.c
	$(CC) -DMY_NAME="\"Student\"" main.c
debug: debug.c
	$(CC) -DDEBUG_MODE debug.c
nodebug: debug.c
	$(CC) debug.c
hack:
	$(CC) -DMY_NAME="({char* msg; fprintf(stderr,\"Hack the system\\n\"); msg = \"World\";})" main.c
clean:
	rm -f *.o *~ a.out
