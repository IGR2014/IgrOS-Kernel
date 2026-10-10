/****************************************************
**
**	x86_64 kernel linker script
**
**	File:	link-x86_64.ld
**	Date:	13 Dec 2022
**
**	Copyright (c) 2017 - 2026, Igor Baklykov
**	All rights reserved.
**
*/


/* Output format */
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

/* Kernel entry function (bootloader converts it to physical address) */
ENTRY(kernelStart)

/* Kernel virtual offset = -2 GB (top of address space) */
KERNEL_OFFSET_VIRT	= 0xFFFFFFFF80000000;
/* Kernel physical address = 1 MB */
KERNEL_ADDRESS_PHYS	= 0x0000000000100000;
/* Kernel virtual address = 1 MB + offset */
KERNEL_ADDRESS_VIRT	= KERNEL_ADDRESS_PHYS + KERNEL_OFFSET_VIRT;

/* Kernel segments */
PHDRS {
	/* read + execute */
	HDR_TEXT	PT_LOAD		FLAGS(0x05);
	/* read */
	HDR_RODATA	PT_LOAD		FLAGS(0x04);
	/* read + write */
	HDR_DATA	PT_LOAD		FLAGS(0x06);
}

/* Kernel sections: linked at virtual (higher half) addresses, loaded at physical ones.
   Boot code runs before paging is enabled and adjusts its addresses itself. */
SECTIONS {

	/* Kernel virtual address */
	. = KERNEL_ADDRESS_VIRT;

	/* Start of the kernel */
	_SECTION_KERNEL_START_ = .;

	/* Kernel code section */
	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_OFFSET_VIRT) {
	_SECTION_TEXT_START_ = .;
		/* Multiboot header must be within first 8 KB of kernel file */
		KEEP(*(.multiboot))
		/* Boot code (runs at physical addresses) */
		*(.boot)
		*(.text)
		*(.text.*)
		/* Large code model (x86_64) */
		*(.ltext)
		*(.ltext.*)
	_SECTION_TEXT_END_ = .;
	} : HDR_TEXT

	/* Kernel read only section */
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_OFFSET_VIRT) {
	_SECTION_RODATA_START_ = .;
		*(.rodata)
		*(.rodata.*)
		/* Large code model (x86_64) */
		*(.lrodata)
		*(.lrodata.*)
		/* Unwind tables */
		*(.eh_frame)
		*(.eh_frame_hdr)
		/* Global constructors (called by kmain) */
		. = ALIGN(8);
		_SECTION_INIT_ARRAY_START_ = .;
		KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
		KEEP(*(.init_array))
		_SECTION_INIT_ARRAY_END_ = .;
	_SECTION_RODATA_END_ = .;
	} : HDR_RODATA

	/* Kernel read write section */
	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_OFFSET_VIRT) {
	_SECTION_DATA_START_ = .;
		*(.data)
		*(.data.*)
		/* Large code model (x86_64) */
		*(.ldata)
		*(.ldata.*)
		/* Global offset tables (if any position independent code) */
		*(.got)
		*(.got.plt)
		*(.igot.plt)
	_SECTION_DATA_END_ = .;
	} : HDR_DATA

	/* Kernel zero-initialized section (not stored in file) */
	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_OFFSET_VIRT) {
	_SECTION_BSS_START_ = .;
		*(COMMON)
		*(.bss)
		*(.bss.*)
		/* Large code model (x86_64) */
		*(.lbss)
		*(.lbss.*)
		*(LARGE_COMMON)
	_SECTION_BSS_END_ = .;
	} : HDR_DATA

	/* End of the kernel */
	_SECTION_KERNEL_END_ = .;

	/* Discard comments and notes */
	/DISCARD/ : {
		*(.comment)
		*(.comment.*)
		*(.note.GNU-stack)
		*(.note.gnu.property)
	}

}
