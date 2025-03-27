#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elf_changer.h"

int main(int argc, char *argv[]){
    if (argc < 3) {
        printf("Uporaba: %s elf_loader [-hlc] [OPTIONS] elf_path\n", argv[0]);
        return 1;
    }

    char *option = argv[1];
    char *elf_path = argv[2];

    if (strcmp(option, "-h") == 0) {
        elf_28878_glava(elf_path);
    } else if (strcmp(option, "-l") == 0) {
        elf_28878_simboli(elf_path);
    } else if (strcmp(option, "-c") == 0) {
        if (argc < 4) {
            printf("Napaka: Napišite imena spremenljivk za spremembno.\n");
            return 1;
        }
        elf_28878_menjaj(elf_path, &argv[3], argc - 3);
    } else {
        printf("Nepoznan ukaz: %s\n", option);
        printf("Uporaba: %s elf_loader [-hlc] [OPTIONS] elf_path\n", argv[0]);
        return 1;
    }

    return 0;
}