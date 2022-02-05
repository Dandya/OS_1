CFLAGS=-Wall -pedantic
BIN=client.exe server.exe clienthalfsend.exe clientwithoutsend.exe clientWriteInProc.exe

all: clean $(BIN) create_user

help:
	@echo "*.exe - create *.exe"
	@echo "debug - create all exe with -DDEBUG"	
	@echo "create_user - create user with name 'server'"
	@echo "delete_user - delete user with name 'server'"
	@echo "clean - delete files"
	@echo "to launch server.exe use: 'sudo su -c \"/home/server/server.exe\" server'"
%.exe:%.c
	@echo "Build $@"
	@gcc $(CFLAGS) $< -o $@
	
debug: 
	@echo "Build debug server.exe"
	@gcc $(CFLAGS) -DDEBUG server.c -o server.exe
	@echo "Build debug client.exe"
	@gcc $(CFLAGS) -DDEBUG client.c -o client.exe
	@echo "Build debug clienthalfsend.exe"
	@gcc $(CFLAGS) -DDEBUG clienthalfsend.c -o clienthalfsend.exe
	@echo "Build debug clientwithoutsend.exe"
	@gcc $(CFLAGS) -DDEBUG clientwithoutsend.c -o clientwithoutsend.exe
	@echo "Build debug clientWriteInProc.exe"
	@gcc $(CFLAGS) -DDEBUG clientWriteInProc.c -o clientWriteInProc.exe

create_user:
	sudo useradd -m server

clean:
	@echo "Clean project"
	@rm -f *.exe file? fi? *.out

delete_user:
	sudo userdel -r server
