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

KERNEL_OFFSET_PHYS	= 1M;
KERNEL_OFFSET_VIRT	= 0xE0000000;

SECTIONS {

	. = KERNEL_OFFSET_PHYS;

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

/*

	. = KERNEL_OFFSET_PHYS;

	.boot ALIGN(4K) : AT(ADDR(.boot)) {

		*(.multiboot)

	}

	. += KERNEL_OFFSET_VIRT;

	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_OFFSET_VIRT) {

		*(.text)
		*(.rodata*)

	}

	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_OFFSET_VIRT) {

		*(.data)

	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_OFFSET_VIRT) {

		*(COMMON)
		*(.bss)

	}

*/

}

