/****************************************************
**
**	Kernel linker script
**
**	File:	link.ld
**	Date:	13 Aug. 2018
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
		*(.rodata*)

	}

	.data ALIGN(4K) : AT(ADDR(.data)) {

		*(.data)

	}

	.bss ALIGN(4K) : AT(ADDR(.bss)) {

		*(COMMON)
		*(.bss)

	}

}

