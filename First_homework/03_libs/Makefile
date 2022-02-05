PROGS = simple.exe static.exe shared.exe

help:
	@echo "Как использовать"
	@echo "all - собрать все exe файлы"
	@echo "simple.exe - собрать из двух файликов"
	@echo "static.exe - собрать статический бинарь"
	@echo "shared.exe - собрать с динамической библиотекой"
	@echo "python - запуск python-скрипта, который использует динамическую библиотеку"
	@echo "shared.exe HACK=1 - собрать с динамические библиотеки,"
	@echo "    одна из которых с 'плохой' реализацией."
	@echo "    Ее нужно копировать в /tmp/libpprint.so."

all: $(PROGS)

simple.exe: main.o pprint.o
	gcc -o simple.exe main.o pprint.o

static.exe: main.o libpprint.a
	gcc -o static.exe main.o -static -L. -lpprint

shared.exe: main.o libpprint.so
	gcc -o shared.exe main.o -L. -lpprint -Wl,-rpath,'/tmp:.'

libpprint.a: pprint.o
	ar cr libpprint.a pprint.o

ifeq ($(HACK), 1)
libpprint.so: pprint.o badcode.o
	gcc -shared -fPIC -o libpprint.so pprint.o
	gcc -shared -fPIC -o libpprint.bad.so badcode.o
else
libpprint.so: pprint.o
	gcc -shared -fPIC -o libpprint.so pprint.o
endif

math.exe: math.o
	gcc -o math.exe math.o -ldl

python: libpprint.so main.py
	python main.py

%.o:%.c
	gcc -c $< -o $@
clean:
	rm -f *.o *.so *.a *.exe /tmp/libpprint.so *~
