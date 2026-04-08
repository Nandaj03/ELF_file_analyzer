# ELF File Analyzer

Simple C program to inspect ELF binaries and print:

- ELF identification details
- ELF header details
- Program header details
- Section header details

## Project Structure

```text
.
|-- main.c
|-- elf_parser.c
|-- elf_parser.h
|-- Makefile
`-- README.md
```

## Build

```sh
make
```

This builds the executable `elf_analyzer` (or `elf_analyzer.exe` on Windows with MinGW).

## Run

```sh
./elf_analyzer
```

Then enter the path to an ELF file when prompted.

## Notes

- `main.c` handles user input and starts the program.
- `elf_parser.c` contains the ELF parsing and printing logic.
- `elf_parser.h` exposes the parser interface used by `main.c`.
