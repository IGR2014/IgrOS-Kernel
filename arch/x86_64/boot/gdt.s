################################################################
#
#	Global descriptor table low-level operations
#
#	File:	gdt.s
#	Date:	13 Jun 2019
#
#	Copyright (c) 2017 - 2020, Igor Baklykov
#	All rights reserved.
#
#


.set	KERNEL_SEGMENT_DATA,	0x00		# Kernel data segment


.code64

.section .text
.balign 8

.global	gdtResetSegments			# Reset segments
.global	gdtLoad					# Load GDT

# Reset segments
gdtResetSegments:
	movabsq	$1f, %rax			# Move absolute address to RAX
	jmpq	*%rax				# Jump from RAX for CS changes to take affect
1:
	movw	$KERNEL_SEGMENT_DATA, %ax	# Set proper segment registers
	movw	%ax, %ds			# --//--
	movw	%ax, %es			# --//--
	movw	%ax, %fs			# --//--
	movw	%ax, %gs			# --//--
	movw	%ax, %ss			# --//--
	retq

gdtLoad:
	cld					# Clear direction flag
	lgdt	(%rdi)				# Load GDT from pointer
	callq	gdtResetSegments		# Reset segments after GDT change
	retq

