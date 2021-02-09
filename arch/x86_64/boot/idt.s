################################################################
#
#	Interrupts descriptor table low-level operations
#
#	File:	idt.s
#	Date:	08 Feb 2021
#
#	Copyright (c) 2017, Igor Baklykov
#	All rights reserved.
#
#


.code64

.section .text
.balign	8

.global	idtLoad				# Load IDT
.global	idtStore			# Store IDT


# Load IDT
idtLoad:
	cld				# Clear direction flag
	lidtq	(%rdi)			# Load IDT from pointer
	retq				# Done here


# Store IDT
idtStore:
	cld				# Clear direction flag
	sidtq	(%rax)			# Load IDT to pointer RAX
	retq

