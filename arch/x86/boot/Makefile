ASM		?= as
RM		= rm -rf
OBJ		= multiboot.o boot.o idt.o port.o exception.o

all: $(OBJ)

clean:
	${RM} $(OBJ)

%.o: %.s
	$(ASM) --32 -o $@ $<

