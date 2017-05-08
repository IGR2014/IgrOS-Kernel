ASM		?= as
RM		= rm -rf
OBJ		= multiboot.o gdt.o longMode.o boot.o port.o

all: $(OBJ)

clean:
	${RM} $(OBJ)

%.o: %.s
	$(ASM) --32 -o $@ $<

