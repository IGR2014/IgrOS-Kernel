CXX		?= g++
CXXFLAGS	= -Wall -m32 -std=c++11 -Wextra -w -pedantic -Werror -ffreestanding -fno-exceptions -fno-rtti -nostdlib
CXXINCLUDE	= -I ${CURDIR} -I include -I ../../
RM		= rm -rf
BIN		?= kernel.bin
OBJ		= boot/multiboot.o boot/boot.o boot/idt.o boot/port.o boot/exceptions.o boot/interrupts.o boot.o idt.o exceptions.o interrupts.o videoMem.o
LINKER		= ld
LINKERSCRIPT	= link.ld

all: all-before $(BIN)

all-before:
	$(MAKE) -C boot

clean:
	${RM} $(OBJ) $(BIN)

run: $(BIN)
	qemu-system-i386 -kernel $(BIN)

$(BIN): $(OBJ)
	$(LINKER) -n $(OBJ) -o $(BIN) -T $(LINKERSCRIPT) -m elf_i386

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXINCLUDE)
