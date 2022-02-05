all:
	@echo -e "Использовать так:\n" \
	"\tserver - запустить сервер\n" \
	"\tmount - подключить файловую систему\n" \
	"\tumount - отключить файловую систему\n" \
	"Дополнительные опции:\n" \
	"\tupdate - обновляет библиотеку сервера\n" \
	"\tclean - удаляет папку mnt\n"

mount:
	@mkdir -p mnt
	@python3 client.py mnt
umount:
	@fusermount -u mnt
server:
	@python3 server.py
update:
	@rm ./library/library.json
clean: 
	@rm -r ./mnt
