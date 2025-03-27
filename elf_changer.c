#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include "elf_changer.h"

void elf_28878_glava(char* zacetekElf){
    int fd = open(zacetekElf, O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    Elf64_Ehdr ehdr;
    if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("ELF header\n");
    printf("  Magic number:       %x %x %x %x\n", ehdr.e_ident[0], ehdr.e_ident[1], ehdr.e_ident[2], ehdr.e_ident[3]);
    printf("  Class:             %s\n", (ehdr.e_ident[EI_CLASS] == ELFCLASS64) ? "64-bit" : "32-bit");
    printf("  Data:              %s\n", (ehdr.e_ident[EI_DATA] == ELFDATA2LSB) ? "Little Endian" : "Big Endian");
    printf("  Type:              %d\n", ehdr.e_type);
    printf("  Machine:           %d\n", ehdr.e_machine);
    printf("  Entry point:       0x%lx\n", ehdr.e_entry);

    close(fd);
}

void elf_28878_simboli(char* zacetekElf){

}

void elf_28878_menjaj(char* zacetekElf, char *sprem[], int stevSprem){

}

