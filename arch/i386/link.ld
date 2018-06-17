/****************************************************
**
**	Kernel linker script
**
**	File:	link.ld
**	Date:	18 Jun. 2018
**
**	Copyright (c) 2018, Igor Baklykov
**	All rights reserved.
*/


OUTPUT_FORMAT(elf32-i386)

ENTRY(kernelStart)

SECTIONS {

	. = 1M;

	.text ALIGN(4K) : AT(ADDR(.text)) {

		*(.multiboot)
		*(.text)

	}

	.data ALIGN(4K) : AT(ADDR(.data)) {

		*(.data)
		*(.rodata*)

	}

	.bss ALIGN(4K) : AT(ADDR(.bss)) {

		*(COMMON)
		*(.bss)

	}

}

