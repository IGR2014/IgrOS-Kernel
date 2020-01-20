################################################################
#
#	Interrupts descriptor table low-level operations
#
#	File:	idt.s
#	Date:	20 Jan 2020
#
#	Copyright (c) 2017, Igor Baklykov
#	All rights reserved.
#
#


.code64

.section .text
.balign	8

.global	idtLoad				# Load IDT


# Load IDT
idtLoad:
	cld				# Clear direction flag
	lidt	(%rdi)			# Load IDT from pointer
	retq				# Done here

