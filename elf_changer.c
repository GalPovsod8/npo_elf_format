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
}

void elf_28878_simboli(char* zacetekElf){

}

void elf_28878_menjaj(char* zacetekElf, char *sprem[], int stevSprem){

}

