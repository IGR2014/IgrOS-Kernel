################################################################
#
#	x86_64 folder Make script
#
#	File:	Makefile
#	Date:	06 May 2019
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


export IGROS_TARGET			= x86_64

export IGROS_DIR_TARGET_BUILD		= $(IGROS_DIR_BUILD)/$(IGROS_TARGET)
export IGROS_DIR_TARGET_RELEASE		= $(IGROS_DIR_RELEASE)/$(IGROS_TARGET)

CXX		= g++
CXXFLAGS	= -m64 \
		  -std=c++11 \
		  -Wall \
		  -Wextra \
		  -w \
		  -pedantic \
		  -Werror \
		  -ffreestanding \
		  -fno-exceptions \
		  -fno-rtti \
		  -nostdlib \
		  -Os \
		  -mno-red-zone \
		  -mno-3dnow \
		  -mno-sse \
		  -mno-sse2 \
		  -mno-sse3 \
		  -g
CXXINCLUDE	= -I $(CURDIR) \
		  -I include -I ../../
RM		= rm -rf
BIN_NAME	= kernel.bin
BIN		= $(IGROS_DIR_TARGET_RELEASE)/$(BIN_NAME)
LINKER		= ld
LINKERFLAGS	= -m elf_x86_64 \
		  -z max-page-size=0x1000
LINKERSCRIPT	= link.ld
OBJ		= $(IGROS_DIR_TARGET_BUILD)/boot/multiboot.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/gdt.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/longMode.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/boot.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/port.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot.o \
		  $(IGROS_DIR_TARGET_BUILD)/gdt.o \
		  $(IGROS_DIR_TARGET_BUILD)/longMode.o \
		  $(IGROS_DIR_TARGET_BUILD)/vgaConsole.o

.PHONY: all-before all clean rebuild run deploy

all: all-before $(OBJ) $(BIN)

all-before:
	@mkdir -p $(IGROS_DIR_TARGET_BUILD)
	@mkdir -p $(IGROS_DIR_TARGET_RELEASE)
	@$(MAKE) -C boot

rebuild: clean all

clean:
	@$(RM) $(OBJ) $(BIN)

run: $(BIN)
	@echo "	RUN	$(BIN_NAME)"
	@qemu-system-x86_64 -kernel $(BIN) -enable-kvm -cpu host -machine pc,accel=kvm::tcg

$(IGROS_DIR_TARGET_BUILD)/%.o: %.cpp
	@echo "	C++	$<"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXINCLUDE)

$(BIN): $(OBJ)
	@echo "	LD	$(BIN_NAME)"
	@$(LINKER) -n $(OBJ) -o $(BIN) -T $(LINKERSCRIPT) $(LINKERFLAGS)

deploy: $(BIN)
	@echo "	DEPLOY	$(BIN_NAME)"
	@mkdir -p $(IGROS_DIR_TARGET_RELEASE)/boot
	@cp $(BIN) $(IGROS_DIR_TARGET_RELEASE)/boot
	@mkdir -p $(IGROS_DIR_TARGET_RELEASE)/boot/grub
	@cp $(IGROS_DIR_RELEASE)/../grub.cfg $(IGROS_DIR_TARGET_RELEASE)/boot/grub/grub.cfg
	@grub-mkrescue -o $(IGROS_DIR_RELEASE)/os.iso $(IGROS_DIR_TARGET_RELEASE)


