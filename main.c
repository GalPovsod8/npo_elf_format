#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elf_changer.h"

int main(int argc, char *argv[]){
    int opt;
    int h_flag = 0, l_flag = 0, c_flag = 0;
    char *elf_path = NULL;
    char *variables[100];
    int var_count = 0;

    if (argc < 3) {
        printf("Uporaba: %s elf_loader [-hlc] [OPTIONS] elf_path\n", argv[0]);
        return 1;
    }
}