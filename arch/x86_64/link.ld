/*OUTPUT_FORMAT(elf64-x86-64)*/

OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:x86-64)

ENTRY(kernelStart)

SECTIONS {

	. = 1M;

	.multiboot : {

		*(.multiboot)

	}

	.text : {

		*(.text)

	}

	.data : {

		*(.data)

	}

	.bss : {

		*(.bss)

	}

}
