all:
	make build make run

build:
	make build_elf_changer
	make build_hello

build_elf_changer:
	gcc -Wall -Wextra -Wpedantic -Werror -o elf_changer main.c

build_hello:
	gcc -Wall -Wextra -Wpedantic -Werror -o hello hello.c

run:
	./elf_changer hello

clean:
	rm -f elf_changer hello










