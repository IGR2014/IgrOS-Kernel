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


.set	KERNEL_SEGMENT_CODE,	0x08		# Kernel code segment
.set	KERNEL_SEGMENT_DATA,	0x10		# Kernel data segment


.code32

.section .text
.balign 4

.global	gdtResetSegments			# Reset segments
.global	gdtLoad					# Load GDT

# Reset segments
gdtResetSegments:
	ljmp	$KERNEL_SEGMENT_CODE, $1f	# Long jump for CS changes to take affect
1:
	movw	$KERNEL_SEGMENT_DATA, %ax	# Set proper segment registers
	movw	%ax, %ds			# --//--
	movw	%ax, %es			# --//--
	movw	%ax, %fs			# --//--
	movw	%ax, %gs			# --//--
	movw	%ax, %ss			# --//--
	ret

# Load GDT
gdtLoad:
	movl	4(%esp), %eax			# Get pointer from stack
	lgdt	(%eax)				# Load GDT from pointer
	call	gdtResetSegments		# Reset segments after GDT change
	ret

