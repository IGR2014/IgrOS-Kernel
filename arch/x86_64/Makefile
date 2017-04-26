CXX		?= g++
CXXFLAGS	= -O2 -Wall -m32 -std=c++11 -Wextra -w -pedantic -Werror -ffreestanding -fno-exceptions -fno-rtti -nostdlib
CXXINCLUDE	= -I ${CURDIR} -I include -I ../../
RM		= rm -rf
BIN		?= kernel.bin
OBJ		= asm/multiboot.o asm/kernel.o asm/io.o asm/gdt.o kernelFunc.o gdt.o
LINKER		= ld
LINKERSCRIPT	= link.ld

all: all-before $(BIN)

all-before:
	$(MAKE) -C asm

clean:
	${RM} $(OBJ) $(BIN)

run: $(BIN)
	qemu-system-x86_64 -kernel $(BIN) -enable-kvm -cpu host

$(BIN): $(OBJ)
	$(LINKER) $(OBJ) -o $(BIN) -T $(LINKERSCRIPT) -m elf_i386

kernelFunc.o: kernelFunc.cpp
	$(CXX) -c kernelFunc.cpp -o kernelFunc.o $(CXXFLAGS) $(CXXINCLUDE)

gdt.o: gdt.cpp
	$(CXX) -c gdt.cpp -o gdt.o $(CXXFLAGS) $(CXXINCLUDE)

# videoMem.o: videoMem.cpp include/io.hpp
#	$(CXX) -c videoMem.cpp -o videoMem.o $(CXXFLAGS) -I ${CURDIR}

