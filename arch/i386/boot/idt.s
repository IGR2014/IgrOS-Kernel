################################################################
#
#	Interrupts descriptor table low-level operations
#
#	File:	idt.s
#	Date:	18 Jun. 2018
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign	4
.global	idtLoad				# Load IDT


# Load IDT
idtLoad:
	movl	4(%esp), %eax		# Get pointer from stack
	lidt	(%eax)			# Load IDT from pointer
	ret				# Done here

