ASM		?= as
RM		= rm -rf
OBJ		= multiboot.o gdt.o longMode.o boot.o io.o

all: $(OBJ)

clean:
	${RM} $(OBJ)

%.o: %.s
	$(ASM) --32 -o $@ $<

