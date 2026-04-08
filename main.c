#include <stdio.h>

#include "elf_parser.h"

int main(void)
{
    char file_name[256];

    printf("Enter the file name: ");
    if (scanf("%255[^\n]", file_name) != 1) {
        fprintf(stderr, "Failed to read file name.\n");
        return 1;
    }

    return analyze_elf_file(file_name);
}
