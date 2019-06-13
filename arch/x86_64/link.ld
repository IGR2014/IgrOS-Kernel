/****************************************************
**
**	Kernel linker script
**
**	File:	link.ld
**	Date:	13 Jun 2019
**
**	Copyright (c) 2017 - 2019, Igor Baklykov
**	All rights reserved.
**
*/


OUTPUT_FORMAT(elf32-i386)
/*OUTPUT_FORMAT(elf64-x86-64)*/
OUTPUT_ARCH(i386:x86-64)

ENTRY(kernelStart)

KERNEL_OFFSET_VIRT	= 0xFFFFFFFF80000000;
KERNEL_ADDRESS_PHYS	= 0x0000000000100000;
KERNEL_ADDRESS_VIRT	= KERNEL_ADDRESS_PHYS + KERNEL_OFFSET_VIRT;

SECTIONS {

	. = KERNEL_ADDRESS_VIRT;

	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_OFFSET_VIRT) {

		*(.multiboot)
		*(.boot)
		*(.text)

	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_OFFSET_VIRT) {

		*(.rodata)

	}

	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_OFFSET_VIRT) {

		*(.data)

	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_OFFSET_VIRT) {

		*(COMMON)
		*(.bss)

	}

}

