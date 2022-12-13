################################################################
#
#	Interrupts descriptor table low-level operations
#
#	File:	idt.s
#	Date:	13 Dec 2022
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
.type idtLoad, @function
idtLoad:

	cld				# Clear direction flag
	lidtq	(%rdi)			# Load IDT from pointer
	retq				# Done here

.size idtLoad, . - idtLoad


# Store IDT
.type idtStore, @function
idtStore:

	cld				# Clear direction flag
	sidtq	(%rax)			# Store IDT to pointer RAX
	retq

.size idtStore, . - idtStore

