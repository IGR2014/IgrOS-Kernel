/****************************************************
**
**	i386 kernel linker script
**
**	File:	link.ld
**	Date:	24 Jan 2020
**
**	Copyright (c) 2017 - 2020, Igor Baklykov
**	All rights reserved.
**
*/


/* Create i386 kernel */
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

/* Kernel entry function. Bootloader will jump here to run kernel */
ENTRY(kernelStart)

/* Kernel virtual offset = 3Gb */
KERNEL_OFFSET_VIRT	= 0xC0000000;
/* Kernel physical address = 1Mb */
KERNEL_ADDRESS_PHYS	= 0x00100000;
/* Kernel virtual address = 1Mb + 3Gb */
KERNEL_ADDRESS_VIRT	= KERNEL_ADDRESS_PHYS + KERNEL_OFFSET_VIRT;

/* Kernel sections */
SECTIONS {

	/* Place kernel at precalculated virtual address */
	. = KERNEL_ADDRESS_VIRT;

	/* Start of the kernel */
	_SECTION_KERNEL_START_ = .;

	/* Kernel code section */
	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_OFFSET_VIRT) {
	_SECTION_TEXT_START_ = .;
		KEEP(*(.multiboot))
		*(.boot)
		*(.text)
	_SECTION_TEXT_END_ = .;
	}

	/* Kernel read only section */
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_OFFSET_VIRT) {
	_SECTION_RODATA_START_ = .;
		*(.rodata)
	_SECTION_RODATA_END_ = .;
	}

	/* Kernel read write section */
	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_OFFSET_VIRT) {
	_SECTION_DATA_START_ = .;
		*(.data)
	_SECTION_DATA_END_ = .;
	}

	/* Kernel stack section */
	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_OFFSET_VIRT) {
	_SECTION_BSS_START_ = .;
		*(COMMON)
		*(.bss)
	_SECTION_BSS_END_ = .;
	}

	/* End of the kernel */
	_SECTION_KERNEL_END_ = .;

}

