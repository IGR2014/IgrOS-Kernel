OUTPUT_FORMAT(elf32-i386)

ENTRY(kernelStart)

SECTIONS {

	. = 1M;

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
