################################################################
#
#	Interrupts descriptor table low-level operations
#
#	File:	idt.s
#	Date:	11 Feb 2021
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign	4

.global	idtLoad				# Load IDT
.global	idtStore			# Store IDT


# Load IDT
.type idtLoad, @function
idtLoad:
	movl	4(%esp), %eax		# Get pointer from stack
	lidtl	(%eax)			# Load IDT from pointer
	retl
.size idtLoad, . - idtLoad


# Store IDT
.type idtStore, @function
idtStore:
	sidtl	(%eax)			# Store IDT to EAX
	retl
.size idtStore, . - idtStore

