################################################################
#
#	Global descriptor table low-level operations
#
#	File:	gdt.s
#	Date:	20 Nov. 2017
#
#	Copyright (c) 2017, Igor Baklykov
#	All rights reserved.
#


.code32

.section .text
.balign 4
.global	gdtLoad				# Load GDT

gdtLoad:
	movl	4(%esp), %eax		# Get pointer from stack
	lgdt	(%eax)			# Load GDT from pointer
	ret				# Done here

