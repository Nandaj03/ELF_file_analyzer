OBJ:=$(patsubst %.c,%.o,$(wildcard *.c))
elf.exe:$(OBJ)
	gcc -o $@ $^
%.o:%.c
	gcc -c $<
clean:
	rm *.exe *.o