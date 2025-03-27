CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror
SRCS = main.c elf_changer.c
OBJS = $(SRCS:.c=.o)

all: build run

build: build_elf_changer build_hello

build_elf_changer: $(OBJS)
	$(CC) $(CFLAGS) -o elf_changer $(OBJS)

build_hello:
	$(CC) $(CFLAGS) -o hello hello.c

run:
	./elf_changer

clean:
	- rm -f elf_changer hello *.o