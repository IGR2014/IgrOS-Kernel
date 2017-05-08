CXX		?= g++
CXXFLAGS	= -O2 -Wall -m32 -std=c++11 -Wextra -w -pedantic -Werror -ffreestanding -fno-exceptions -fno-rtti -nostdlib
CXXINCLUDE	= -I ${CURDIR} -I include -I ../../
RM		= rm -rf
BIN		?= kernel.bin
OBJ		= boot/multiboot.o boot/gdt.o boot/longMode.o boot/boot.o boot/port.o boot.o gdt.o
LINKER		= ld
LINKERSCRIPT	= link.ld

all: all-before $(BIN)

all-before:
	$(MAKE) -C boot

clean:
	${RM} $(OBJ) $(BIN)

run: $(BIN)
	qemu-system-x86_64 -kernel $(BIN) -enable-kvm -cpu host

$(BIN): $(OBJ)
	$(LINKER) -n $(OBJ) -o $(BIN) -T $(LINKERSCRIPT) -m elf_i386

boot.o: boot.cpp
	$(CXX) -c boot.cpp -o boot.o $(CXXFLAGS) $(CXXINCLUDE)

gdt.o: gdt.cpp
	$(CXX) -c gdt.cpp -o gdt.o $(CXXFLAGS) $(CXXINCLUDE)

# videoMem.o: videoMem.cpp include/io.hpp
#	$(CXX) -c videoMem.cpp -o videoMem.o $(CXXFLAGS) -I ${CURDIR}

