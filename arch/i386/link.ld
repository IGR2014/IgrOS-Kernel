/****************************************************
**
**	Kernel linker script
**
**	File:	link.ld
**	Date:	22 May 2019
**
**	Copyright (c) 2017 - 2019, Igor Baklykov
**	All rights reserved.
**
*/


OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

ENTRY(kernelStart)

KERNEL_OFFSET_PHYS	= 0x00100000;
KERNEL_OFFSET_VIRT	= 0xC0000000;

SECTIONS {

	. = KERNEL_OFFSET_PHYS;

	.text ALIGN(4K) : AT(ADDR(.text)) {

		*(.multiboot)
		*(.boot)
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

	.bootstrap ALIGN(4K) : AT(ADDR(.bootstrap)) {

		*(.multiboot)
		*(.bootstrap)

	}

	. += KERNEL_OFFSET_PHYS + KERNEL_OFFSET_VIRT;

	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_OFFSET_VIRT) {

		*(.multiboot)
		*(.boot)
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

