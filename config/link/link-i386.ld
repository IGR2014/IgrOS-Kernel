/****************************************************
**
**	i386 kernel linker script
**
**	File:	link.ld
**	Date:	12 Dec 2022
**
**	Copyright (c) 2017 - 2022, Igor Baklykov
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

/* Kernel headers */
PHDRS {
	/* empty */
	HDR_EMPTY	PT_NULL		FLAGS(0x00);
	/* execute + read */
	HDR_TEXT	PT_LOAD		FLAGS(0x05);
	/* read */
	HDR_RODATA	PT_LOAD		FLAGS(0x04);
	/* write + read */
	HDR_DATA	PT_LOAD		FLAGS(0x06);
}

/* Kernel sections */
SECTIONS {

	/* Place kernel at precalculated virtual address */
	. = KERNEL_ADDRESS_VIRT;

	/* Start of the kernel */
	_SECTION_KERNEL_START_ = .;

	/* Kernel code section */
	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_OFFSET_VIRT) {
	_SECTION_TEXT_START_ = .;
		*(.multiboot)
		*(.boot)
		*(.text)
		*(.text.*)
	_SECTION_TEXT_END_ = .;
	} : HDR_TEXT

	/* Kernel read only section */
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_OFFSET_VIRT) {
	_SECTION_RODATA_START_ = .;
		*(.rodata)
		*(.rodata.*)
	_SECTION_RODATA_END_ = .;
	} : HDR_RODATA

	/* Kernel read write section */
	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_OFFSET_VIRT) {
	_SECTION_DATA_START_ = .;
		*(.data)
		*(.data.*)
	_SECTION_DATA_END_ = .;
	} : HDR_DATA

	/* Kernel stack section */
	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_OFFSET_VIRT) {
	_SECTION_BSS_START_ = .;
		*(COMMON)
		*(.bss)
		*(.bss.*)
	_SECTION_BSS_END_ = .;
	} : HDR_DATA

	/* End of the kernel */
	_SECTION_KERNEL_END_ = .;

}

