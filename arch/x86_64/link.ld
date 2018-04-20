OUTPUT_FORMAT(elf64-x86-64)

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
