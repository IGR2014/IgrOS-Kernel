################################################################
#
#	Interrupts descriptor table low-level operations
#
#	File:	idt.s
#	Date:	20 Nov. 2017
#
#	Copyright (c) 2017, Igor Baklykov
#	All rights reserved.
#


.code32

.section .text
	.balign	4
	.global	idtLoad			# Load IDT

idtLoad:
	movl	4(%esp), %eax		# Get pointer from stack
	lidt	(%eax)			# Load IDT from pointer
	sti				# Enable interrupts
	ret				# Done here

