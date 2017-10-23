OUTPUT_FORMAT(elf32-i386)

ENTRY(kernelFunc)

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
