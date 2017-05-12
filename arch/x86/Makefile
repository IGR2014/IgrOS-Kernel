CXX		?= g++
CXXFLAGS	= -Wall -m32 -std=c++11 -Wextra -w -pedantic -Werror -ffreestanding -fno-exceptions -fno-rtti -nostdlib
CXXINCLUDE	= -I ${CURDIR} -I include -I ../../
RM		= rm -rf
BIN		?= kernel.bin
OBJ		= boot/multiboot.o boot/boot.o boot/idt.o boot/port.o boot/exception.o boot.o idt.o exception.o
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

boot.o: boot.cpp
	$(CXX) -c boot.cpp -o boot.o $(CXXFLAGS) $(CXXINCLUDE)

idt.o: idt.cpp
	$(CXX) -c idt.cpp -o idt.o $(CXXFLAGS) $(CXXINCLUDE)

exception.o: exception.cpp
	$(CXX) -c exception.cpp -o exception.o $(CXXFLAGS) $(CXXINCLUDE)

# videoMem.o: videoMem.cpp include/io.hpp
#	$(CXX) -c videoMem.cpp -o videoMem.o $(CXXFLAGS) -I ${CURDIR}

