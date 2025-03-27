#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int fd = open(zacetekElf, O_RDONLY);
    if (fd == -1) {
        perror("Problem pri odpiranju datoteke elf");
        return;
    }

    Elf64_Ehdr elfHeader;
    if (read(fd, &elfHeader, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr)) {
        perror("Problem pri branju glave ELF");
        close(fd);
        return;
    }

    if (elfHeader.e_ident[EI_MAG0] != 0x7f || elfHeader.e_ident[EI_MAG1] != 'E' || elfHeader.e_ident[EI_MAG2] != 'L' || elfHeader.e_ident[EI_MAG3] != 'F') {
        fprintf(stderr, "Datoteka, ki ste jo podali ni pravi format (ELF)\n");
        close(fd);
        return;
    }

    Elf64_Shdr *sectionHeaders = malloc(elfHeader.e_shnum * sizeof(Elf64_Shdr));
    if (sectionHeaders == NULL) {
        perror("Problem pri alociranju spomina za glave odsekov");
        close(fd);
        return;
    }

    lseek(fd, elfHeader.e_shoff, SEEK_SET);
    if (read(fd, sectionHeaders, elfHeader.e_shnum * sizeof(Elf64_Shdr)) != (ssize_t)(elfHeader.e_shnum * sizeof(Elf64_Shdr))){
        perror("Problem pri branju glave odsekov");
        free(sectionHeaders);
        close(fd);
        return;
    }

    Elf64_Shdr stringTableHeader = sectionHeaders[elfHeader.e_shstrndx];
    char *stringTable = malloc(stringTableHeader.sh_size);
    if (stringTable == NULL) {
        perror("Problem pri alociranju spomina za tabele nizov");
        free(sectionHeaders);
        close(fd);
        return;
    }

    lseek(fd, stringTableHeader.sh_offset, SEEK_SET);
    if (read(fd, stringTable, stringTableHeader.sh_size) != (ssize_t)(stringTableHeader.sh_size)){
        perror("Problem pri branju tabele nizov");
        free(stringTable);
        free(sectionHeaders);
        close(fd);
        return;
    }

    Elf64_Sym *symbols = NULL;
    int numSymbols = 0;

    for (int i = 0; i < elfHeader.e_shnum; i++) {
        if (strcmp(&stringTable[sectionHeaders[i].sh_name], ".symtab") == 0) {
            Elf64_Shdr symtabHeader = sectionHeaders[i];
            symbols = malloc(symtabHeader.sh_size);
            if (symbols == NULL) {
                perror("Problem pri alociranju spomina za simbole");
                free(stringTable);
                free(sectionHeaders);
                close(fd);
                return;
            }

            lseek(fd, symtabHeader.sh_offset, SEEK_SET);
            numSymbols = symtabHeader.sh_size / sizeof(Elf64_Sym);
            if (read(fd, symbols, symtabHeader.sh_size) != (ssize_t)(symtabHeader.sh_size)){
                perror("Problem pri branju tabele simbolov");
                free(symbols);
                free(stringTable);
                free(sectionHeaders);
                close(fd);
                return;
            }
            break;
        }
    }

    printf("  [Nr]   Value           Size    Name\n");

    for (int i = 0; i < numSymbols; i++) {
        if (ELF64_ST_TYPE(symbols[i].st_info) == STT_FUNC && symbols[i].st_size > 4) {
            printf("  [%02d]   0x%08lx      %d      %s\n", i, (unsigned long)symbols[i].st_value, (int)symbols[i].st_size, &stringTable[sectionHeaders[elfHeader.e_shstrndx].sh_offset + symbols[i].st_name]);
        }
    }

    free(symbols);
    free(stringTable);
    free(sectionHeaders);
    close(fd);
}

void elf_28878_menjaj(char* zacetekElf, char *sprem[], int stevSprem){
    int fd = open(zacetekElf, O_RDWR);
    if (fd == -1) {
        perror("Napaka pri odpiranju ELF datoteke");
        return;
    }

    Elf64_Ehdr elfHeader;
    if (read(fd, &elfHeader, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr)) {
        perror("Napaka pri branju ELF glave");
        close(fd);
        return;
    }

    if (elfHeader.e_ident[EI_MAG0] != 0x7f || elfHeader.e_ident[EI_MAG1] != 'E' || elfHeader.e_ident[EI_MAG2] != 'L' || elfHeader.e_ident[EI_MAG3] != 'F') {
        fprintf(stderr, "To ni veljavna ELF datoteka\n");
        close(fd);
        return;
    }

    Elf64_Shdr *sectionHeaders = malloc(elfHeader.e_shnum * sizeof(Elf64_Shdr));
    if (sectionHeaders == NULL) {
        perror("Napaka pri dodeljevanju pomnilnika za naslove sekcij");
        close(fd);
        return;
    }

    lseek(fd, elfHeader.e_shoff, SEEK_SET);
    if (read(fd, sectionHeaders, elfHeader.e_shnum * sizeof(Elf64_Shdr)) != elfHeader.e_shnum * sizeof(Elf64_Shdr)) {
        perror("Napaka pri branju naslovov sekcij");
        free(sectionHeaders);
        close(fd);
        return;
    }

    Elf64_Shdr stringTableHeader = sectionHeaders[elfHeader.e_shstrndx];
    char *stringTable = malloc(stringTableHeader.sh_size);
    if (stringTable == NULL) {
        perror("Napaka pri dodeljevanju pomnilnika za tabelo znakov");
        free(sectionHeaders);
        close(fd);
        return;
    }

    lseek(fd, stringTableHeader.sh_offset, SEEK_SET);
    if (read(fd, stringTable, stringTableHeader.sh_size) != stringTableHeader.sh_size) {
        perror("Napaka pri branju tabele znakov");
        free(stringTable);
        free(sectionHeaders);
        close(fd);
        return;
    }

    Elf64_Sym *symbols = NULL;
    int numSymbols = 0;
    for (int i = 0; i < elfHeader.e_shnum; i++) {
        if (strcmp(&stringTable[sectionHeaders[i].sh_name], ".data") == 0 || strcmp(&stringTable[sectionHeaders[i].sh_name], ".bss") == 0) {
            Elf64_Shdr dataSectionHeader = sectionHeaders[i];
            unsigned char *data = malloc(dataSectionHeader.sh_size);
            if (data == NULL) {
                perror("Napaka pri dodeljevanju pomnilnika za podatke sekcije");
                free(stringTable);
                free(sectionHeaders);
                close(fd);
                return;
            }

            lseek(fd, dataSectionHeader.sh_offset, SEEK_SET);
            if (read(fd, data, dataSectionHeader.sh_size) != dataSectionHeader.sh_size) {
                perror("Napaka pri branju podatkovne sekcije");
                free(data);
                free(stringTable);
                free(sectionHeaders);
                close(fd);
                return;
            }

            printf("IME     NASLOV     VREDNOST     NOVA_VREDNOST\n");
            for (int j = 0; j < stevSprem; j++) {
                char *varName = sprem[j];
                int found = 0;

                for (int k = 0; k < elfHeader.e_shnum; k++) {
                    if (sectionHeaders[k].sh_type == SHT_SYMTAB) {
                        Elf64_Sym *symtab = malloc(sectionHeaders[k].sh_size);
                        lseek(fd, sectionHeaders[k].sh_offset, SEEK_SET);
                        read(fd, symtab, sectionHeaders[k].sh_size);

                        for (int l = 0; l < sectionHeaders[k].sh_size / sizeof(Elf64_Sym); l++) {
                            if (ELF64_ST_TYPE(symtab[l].st_info) == STT_OBJECT && strcmp(&stringTable[sectionHeaders[elfHeader.e_shstrndx].sh_offset + symtab[l].st_name], varName) == 0) {
                                unsigned long *varValue = (unsigned long *)(data + symtab[l].st_value - dataSectionHeader.sh_offset);
                                printf("%s   0x%lx    0x%lx    0x%lx\n", varName, symtab[l].st_value, *varValue, *varValue + 3);
                                *varValue += 3;
                                found = 1;
                                break;
                            }
                        }
                        free(symtab);
                    }
                    if (found) break;
                }
            }

            lseek(fd, dataSectionHeader.sh_offset, SEEK_SET);
            if (write(fd, data, dataSectionHeader.sh_size) != dataSectionHeader.sh_size) {
                perror("Napaka pri pisanju spremenjene podatkovne sekcije");
            }

            free(data);
            break;
        }
    }
}

