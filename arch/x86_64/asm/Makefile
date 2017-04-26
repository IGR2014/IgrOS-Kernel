ASM		?= as
RM		= rm -rf
OBJ		= multiboot.o kernel.o io.o gdt.o

all: $(OBJ)

clean:
	${RM} $(OBJ)

%.o: %.s
	$(ASM) --32 -o $@ $<

