################################################################
#
#	Global descriptor table low-level operations
#
#	File:	gdt.s
#	Date:	06 Jun 2019
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


.code64

.section .text
.balign 8
.global	gdtLoad				# Load GDT

gdtLoad:
	cld				# Clear direction flag
	lgdt	(%rdi)			# Load GDT from pointer
	movabsq	$1f, %rax
	jmpq	*%rax
1:
	movw	$0x00, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss
	retq


.section .bss
PML4Table:
	.skip	4096
PDPTable:
	.skip	4096
PDTable:
	.skip	4096
PTable:
	.skip	4096

