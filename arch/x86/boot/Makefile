ASM		?= as
RM		= rm -rf
OBJ		= multiboot.o boot.o idt.o

all: $(OBJ)

clean:
	${RM} $(OBJ)

%.o: %.s
	$(ASM) --32 -o $@ $<

