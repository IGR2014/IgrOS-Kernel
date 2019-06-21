################################################################
#
#	arch folder Make script
#
#	File:	Makefile
#	Date:	13 Jun 2019
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


export	IGROS_DIR_BUILD		= $(CURDIR)/../build
export	IGROS_DIR_RELEASE	= $(CURDIR)/../release

export	IGROS_ARCH		?= x86_64

export	CXX			?= g++

export	CXXFLAGS		= -m64					\
				  -std=c++11				\
				  -Wall					\
				  -Wextra				\
				  -w					\
				  -pedantic				\
				  -Werror				\
				  -ffreestanding			\
				  -fno-exceptions			\
				  -fno-rtti				\
				  -nostdlib				\
				  -fno-pic				\
				  -fno-pie				\
				  -Os					\
				  -s					\
				  -mno-red-zone				\
				  -mno-3dnow				\
				  -mno-sse				\
				  -mno-sse2				\
				  -mno-sse3				\
				  -mno-mmx				\
				  -mcmodel=large

export	CXXINCLUDE		= -I $(CURDIR)/$(IGROS_ARCH)/include/	\
				  -I $(CURDIR)/../include/

export	AS			= as

export	ASFLAGS			= --64

export	LD			= ld

export	LDFLAGS			= -n					\
				  -m elf_$(IGROS_ARCH)			\
				  -z max-page-size=0x1000

export	RM			= rm -rf

export	BIN_NAME		= kernel.bin


.PHONY: all clean distclean rebuild run deploy

all: $(IGROS_DIR_BUILD) $(IGROS_DIR_RELEASE)
	@echo "	ECHO	$(IGROS_DIR_BUILD)"
	@echo "	ECHO	$(IGROS_DIR_RELEASE)"
	@$(MAKE) -C $(CURDIR)/../multiboot
	@$(MAKE) -C $(CURDIR)/../klib
	@$(MAKE) -C $(IGROS_ARCH)

clean:
	@$(MAKE) -C $(CURDIR)/../multiboot clean
	@$(MAKE) -C $(CURDIR)/../klib clean
	@$(MAKE) -C $(IGROS_ARCH) clean

distclean:
	@$(MAKE) -C $(IGROS_ARCH) distclean

rebuild:
	@$(MAKE) -C $(CURDIR)/../multiboot rebuild
	@$(MAKE) -C $(CURDIR)/../klib rebuild
	@$(MAKE) -C $(IGROS_ARCH) rebuild

run: all
	@$(MAKE) -C $(IGROS_ARCH) run

deploy: all
	@echo "	DEPLOY	$(BIN_NAME)"
	@mkdir -p $(IGROS_DIR_RELEASE)/iso/
	@mkdir -p $(IGROS_DIR_RELEASE)/iso/boot/
	@cp $(IGROS_DIR_RELEASE)/$(IGROS_ARCH)/$(BIN_NAME) $(IGROS_DIR_RELEASE)/iso/boot/
	@mkdir -p $(IGROS_DIR_RELEASE)/iso/boot/grub/
	@cp $(IGROS_DIR_RELEASE)/../grub.cfg $(IGROS_DIR_RELEASE)/iso/boot/grub/grub.cfg
	@grub-mkrescue -o $(IGROS_DIR_RELEASE)/os-$(IGROS_ARCH).iso $(IGROS_DIR_RELEASE)/iso

$(IGROS_DIR_BUILD):
	@echo "	MKDIR	$(IGROS_DIR_BUILD)/$(IGROS_ARCH)"
	@mkdir -p $(IGROS_DIR_BUILD)/$(IGROS_ARCH)

$(IGROS_DIR_RELEASE):
	@echo "	MKDIR	$(IGROS_DIR_RELEASE)/$(IGROS_ARCH)"
	@mkdir -p $(IGROS_DIR_RELEASE)/$(IGROS_ARCH)

