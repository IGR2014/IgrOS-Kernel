################################################################
#
#	Global descriptor table low-level operations
#
#	File:	gdt.s
#	Date:	22 Mar 2023
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#


.set	KERNEL_SEGMENT_DATA,	0x00		# Kernel data segment


.code64

.section .text
.balign 8

.global	gdtResetSegments			# Reset segments
.global	gdtLoad					# Load GDT
.global	gdtStore				# Store GDT


# Reset segments
.type gdtResetSegments, %function
gdtResetSegments:

	cld					# Clear direction flag
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

.size gdtResetSegments, . - gdtResetSegments


# Load GDT
.type gdtLoad, %function
gdtLoad:

	cld					# Clear direction flag
	lgdtq	(%rdi)				# Load GDT from pointer
	movq	$gdtResetSegments, %rax		# Reset segments after GDT change
	callq	*%rax
	retq

.size gdtLoad, . - gdtLoad


# Store GDT
.type gdtStore, %function
gdtStore:

	cld					# Clear direction flag
	sgdtq	(%rax)				# Load GDT to pointer RAX
	retq

.size gdtStore, . - gdtStore

