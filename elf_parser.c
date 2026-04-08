#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#include "elf_parser.h"

static int prompt_yes_no(const char *message)
{
    char choice;

    printf("%s", message);
    if (scanf(" %c", &choice) != 1) {
        return 0;
    }

    return choice == 'y' || choice == 'Y';
}

static void print_elf_identification(const unsigned char buffer[16])
{
    printf("%-20s = %-20s\n", "File type", "ELF magic detected");

    if (buffer[4] == ELFCLASS32) {
        printf("%-20s = %-20s\n", "Bit", "32");
    } else if (buffer[4] == ELFCLASS64) {
        printf("%-20s = %-20s\n", "Bit", "64");
    } else {
        printf("%-20s = %-20s\n", "Bit", "Invalid");
    }

    if (buffer[5] == ELFDATA2LSB) {
        printf("%-20s = %-20s\n", "Endian", "Little Endian");
    } else if (buffer[5] == ELFDATA2MSB) {
        printf("%-20s = %-20s\n", "Endian", "Big Endian");
    } else {
        printf("%-20s = %-20s\n", "Endian", "Invalid");
    }
}

static void print_separator(const char *title)
{
    printf("\n--------------------------------------------------------------\n");
    printf("%s\n", title);
    printf("--------------------------------------------------------------\n");
}

static void print_program_headers_64(FILE *fp, const Elf64_Ehdr *ehdr)
{
    Elf64_Phdr phdr;

    printf("------------------------------------------------\n");
    printf("Program header details:\n");
    printf("------------------------------------------------\n");

    fseek(fp, (long)ehdr->e_phoff, SEEK_SET);
    for (int i = 0; i < ehdr->e_phnum; ++i) {
        if (fread(&phdr, ehdr->e_phentsize, 1, fp) != 1) {
            break;
        }

        printf("--------------------\n");
        printf("pgmheader-->%d     |\n", i + 1);
        printf("--------------------\n");
        printf("Type=%u\n", phdr.p_type);
        printf("offset=0x%lx\n", (unsigned long)phdr.p_offset);
        printf("virtual address=0x%lx\n", (unsigned long)phdr.p_vaddr);
        printf("file size=%lu\n", (unsigned long)phdr.p_filesz);
        printf("memory size=%lu\n\n", (unsigned long)phdr.p_memsz);
    }
}

static void print_section_headers_64(FILE *fp, const Elf64_Ehdr *ehdr)
{
    Elf64_Shdr shdr;

    printf("-------------------------------\n");
    printf("Section header details:\n");
    printf("-------------------------------\n");

    fseek(fp, (long)ehdr->e_shoff, SEEK_SET);
    for (int i = 0; i < ehdr->e_shnum; ++i) {
        if (fread(&shdr, ehdr->e_shentsize, 1, fp) != 1) {
            break;
        }

        printf("-------------------------\n");
        printf("section header-->%d    |\n", i + 1);
        printf("-------------------------\n");
        printf("type=%u\n", shdr.sh_type);
        printf("offset=0x%lx\n", (unsigned long)shdr.sh_offset);
        printf("size=%lu\n", (unsigned long)shdr.sh_size);
        printf("address=0x%lx\n", (unsigned long)shdr.sh_addr);
    }
}

static int handle_elf64(FILE *fp)
{
    Elf64_Ehdr ehdr;

    if (fread(&ehdr, sizeof(ehdr), 1, fp) != 1) {
        fprintf(stderr, "Cannot read ELF header.\n");
        return 1;
    }

    print_separator("ELF header Details");
    printf("%-20s = %u\n", "Type", ehdr.e_type);
    printf("%-20s = %u\n", "Machine", ehdr.e_machine);
    printf("%-20s = %u\n", "Version", ehdr.e_version);
    printf("%-20s = 0x%lx\n", "Entry point", (unsigned long)ehdr.e_entry);
    printf("%-20s = %lu\n", "pgm header offset", (unsigned long)ehdr.e_phoff);
    printf("%-20s = %lu\n", "section header offset", (unsigned long)ehdr.e_shoff);
    printf("%-20s = %u\n", "pgrm header count", ehdr.e_phnum);
    printf("%-20s = %u\n", "section header count", ehdr.e_shnum);

    if (!prompt_yes_no("\nDo you want to print program header details? (y/n): ")) {
        return 0;
    }

    print_program_headers_64(fp, &ehdr);

    if (!prompt_yes_no("\nDo you want to print section header details? (y/n): ")) {
        return 0;
    }

    print_section_headers_64(fp, &ehdr);
    return 0;
}

static void print_program_headers_32(FILE *fp, const Elf32_Ehdr *ehdr)
{
    Elf32_Phdr phdr;

    printf("------------------------------------------------\n");
    printf("Program header details:\n");
    printf("------------------------------------------------\n");

    fseek(fp, (long)ehdr->e_phoff, SEEK_SET);
    for (int i = 0; i < ehdr->e_phnum; ++i) {
        if (fread(&phdr, ehdr->e_phentsize, 1, fp) != 1) {
            break;
        }

        printf("--------------------\n");
        printf("pgmheader-->%d     |\n", i + 1);
        printf("--------------------\n");
        printf("Type=%u\n", phdr.p_type);
        printf("offset=0x%x\n", phdr.p_offset);
        printf("virtual address=0x%x\n", phdr.p_vaddr);
        printf("file size=%u\n", phdr.p_filesz);
        printf("memory size=%u\n\n", phdr.p_memsz);
    }
}

static void print_section_headers_32(FILE *fp, const Elf32_Ehdr *ehdr)
{
    Elf32_Shdr shdr;

    printf("-------------------------------\n");
    printf("Section header details:\n");
    printf("-------------------------------\n");

    fseek(fp, (long)ehdr->e_shoff, SEEK_SET);
    for (int i = 0; i < ehdr->e_shnum; ++i) {
        if (fread(&shdr, ehdr->e_shentsize, 1, fp) != 1) {
            break;
        }

        printf("-------------------------\n");
        printf("section header-->%d    |\n", i + 1);
        printf("-------------------------\n");
        printf("type=%u\n", shdr.sh_type);
        printf("offset=0x%x\n", shdr.sh_offset);
        printf("size=%u\n", shdr.sh_size);
        printf("address=0x%x\n", shdr.sh_addr);
    }
}

static int handle_elf32(FILE *fp)
{
    Elf32_Ehdr ehdr;

    if (fread(&ehdr, sizeof(ehdr), 1, fp) != 1) {
        fprintf(stderr, "Cannot read ELF header.\n");
        return 1;
    }

    print_separator("ELF header Details");
    printf("%-20s = %u\n", "Type", ehdr.e_type);
    printf("%-20s = %u\n", "Machine", ehdr.e_machine);
    printf("%-20s = %u\n", "Version", ehdr.e_version);
    printf("%-20s = 0x%x\n", "Entry point", ehdr.e_entry);
    printf("%-20s = %u\n", "pgm header offset", ehdr.e_phoff);
    printf("%-20s = %u\n", "section header offset", ehdr.e_shoff);
    printf("%-20s = %u\n", "pgrm header count", ehdr.e_phnum);
    printf("%-20s = %u\n", "section header count", ehdr.e_shnum);

    if (!prompt_yes_no("\nDo you want to print program header details? (y/n): ")) {
        return 0;
    }

    print_program_headers_32(fp, &ehdr);

    if (!prompt_yes_no("\nDo you want to print section header details? (y/n): ")) {
        return 0;
    }

    print_section_headers_32(fp, &ehdr);
    return 0;
}

int analyze_elf_file(const char *file_name)
{
    FILE *fp;
    unsigned char buffer[16];
    int status = 0;

    fp = fopen(file_name, "rb");
    if (fp == NULL) {
        fprintf(stderr, "File not found.\n");
        return 1;
    }

    printf("File opened successfully\n");

    if (fread(buffer, 1, sizeof(buffer), fp) != sizeof(buffer)) {
        fprintf(stderr, "16 bytes reading unsuccessful.\n");
        fclose(fp);
        return 1;
    }

    if (buffer[0] != 0x7F || buffer[1] != 'E' || buffer[2] != 'L' || buffer[3] != 'F') {
        printf("Not an ELF file\n");
        fclose(fp);
        return 1;
    }

    print_elf_identification(buffer);

    fseek(fp, 0, SEEK_SET);
    if (buffer[4] == ELFCLASS64) {
        status = handle_elf64(fp);
    } else if (buffer[4] == ELFCLASS32) {
        status = handle_elf32(fp);
    } else {
        fprintf(stderr, "Unsupported ELF class.\n");
        status = 1;
    }

    fclose(fp);
    return status;
}
