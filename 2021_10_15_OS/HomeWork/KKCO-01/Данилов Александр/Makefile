all: build

help:
	@echo -e "Список команд:\n" \
		"\tbuild - создать программу-загрузчик\n" \
		"\trun - запустить в отдельном окне\n" \
		"\trunConsole - запустить в терминале\n" \
		"\tclean - удаление фаилов *.exe и *.img"

build: encrypt.exe
	@nasm -f bin boothw.asm -o floppy.img
	@./encrypt.exe 

encrypt.exe: encrypt.c
	@gcc encrypt.c -o encrypt.exe

run: floppy.img
	@qemu-system-x86_64 -fda floppy.img

runConsole: floppy.img
	@qemu-system-x86_64 -fda floppy.img -curses

clean:
	@rm -f *.exe *.img
