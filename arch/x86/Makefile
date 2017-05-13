CXX		?= g++
CXXFLAGS	= -Wall -m32 -std=c++11 -Wextra -w -pedantic -Werror -ffreestanding -fno-exceptions -fno-rtti -nostdlib
CXXINCLUDE	= -I ${CURDIR} -I include -I ../../
RM		= rm -rf
BIN		?= kernel.bin
OBJ		= boot/multiboot.o boot/boot.o boot/idt.o boot/port.o boot/exceptions.o boot.o idt.o exceptions.o
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

#boot.o: boot.cpp
#	$(CXX) -c boot.cpp -o boot.o $(CXXFLAGS) $(CXXINCLUDE)

#idt.o: idt.cpp
#	$(CXX) -c idt.cpp -o idt.o $(CXXFLAGS) $(CXXINCLUDE)

#exceptions.o: exceptions.cpp
#	$(CXX) -c exceptions.cpp -o exceptions.o $(CXXFLAGS) $(CXXINCLUDE)

# videoMem.o: videoMem.cpp include/io.hpp
#	$(CXX) -c videoMem.cpp -o videoMem.o $(CXXFLAGS) -I ${CURDIR}
