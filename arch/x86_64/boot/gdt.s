################################################################
#
#	Global descriptor table low-level operations
#
#	File:	gdt.s
#	Date:	17 Apr. 2018
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign 4
.global	gdtLoad				# Load GDT

gdtLoad:
	movl	4(%esp), %eax		# Get pointer from stack
	lgdt	(%eax)			# Load GDT from pointer
	ljmp	$0x08, $1f
1:
	movw	$0x10, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss
	ret

.section .bss
PML4Table:
	.skip	4096
PDPTable:
	.skip	4096
PDTable:
	.skip	4096
PTable:
	.skip	4096

