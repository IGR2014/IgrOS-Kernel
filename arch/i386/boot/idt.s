################################################################
#
#	Interrupts descriptor table low-level operations
#
#	File:	idt.s
#	Date:	08 Feb 2021
#
#	Copyright (c) 2017 - 2021, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign	4

.global	idtLoad				# Load IDT
.global	idtStore			# Store IDT


# Load IDT
idtLoad:
	movl	4(%esp), %eax		# Get pointer from stack
	lidtl	(%eax)			# Load IDT from pointer
	retl				# Done here


# Store IDT
idtStore:
	sidtl	(%eax)			# Store IDT to EAX
	retl

