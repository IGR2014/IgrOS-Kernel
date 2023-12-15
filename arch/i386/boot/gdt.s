################################################################
#
#	Global descriptor table low-level operations
#
#	File:	gdt.s
#	Date:	14 Dec 2023
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#


.set	KERNEL_SEGMENT_CODE,	0x08		# Kernel code segment
.set	KERNEL_SEGMENT_DATA,	0x10		# Kernel data segment


.code32

.section .text
.balign 4

.global	gdtResetSegments			# Reset segments
.global	gdtLoad					# Load GDT
.global	gdtStore				# Store GDT


# Reset segments
.type gdtResetSegments, %function
gdtResetSegments:

	ljmpl	$KERNEL_SEGMENT_CODE, $1f	# Long jump for CS changes to take affect

1:
	movw	$KERNEL_SEGMENT_DATA, %ax	# Set proper segment registers
	movw	%ax, %ds			# --//--
	movw	%ax, %es			# --//--
	movw	%ax, %fs			# --//--
	movw	%ax, %gs			# --//--
	movw	%ax, %ss			# --//--
	retl

.size gdtResetSegments, . - gdtResetSegments


# Load GDT
.type gdtLoad, %function
gdtLoad:

	movl	4(%esp), %eax			# Get pointer from stack
	lgdtl	(%eax)				# Load GDT from pointer
	leal	gdtResetSegments, %ebp		# Reset segments after GDT change
	calll	*%ebp
	retl

.size gdtLoad, . - gdtLoad


# Store GDT
.type gdtStore, %function
gdtStore:

	sgdtl	(%eax)				# Store GDT to EAX
	retl

.size gdtStore, . - gdtStore

