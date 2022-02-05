CC=gcc
CFLAGS:=-Wall
LDFLAGS=
LD=$(CC)

PYTHON:=$(shell which python)

all: main.exe

echo:
	echo "${PYTHON}"
noecho:
	@echo "${PYTHON}"

main.exe: main.o
	@echo "[LD] main.exe"
	@$(LD) -o main.exe main.o $(LDFLAGS)
%.o:%.c
	@echo "[CC] $@"
	@$(CC) $(CFLAGS) -c $< -o $@
python:	main.py
	@echo "[EXE] python sctypt"
	@$(PYTHON) main.py
clean:
	@echo "Build project"
	@rm -fv *.o *.exe *~
