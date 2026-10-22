################################################################
#
#	Global descriptor table low-level operations
#
#	File:	gdt.s
#	Date:	22 Oct 2026
#
#	Copyright (c) 2017 - 2026, Igor Baklykov
#	All rights reserved.
#
#


.set	KERNEL_SEGMENT_CODE,	0x08		# Kernel code segment (gdt.cpp entry 1)
.set	KERNEL_SEGMENT_DATA,	0x10		# Kernel data segment (gdt.cpp entry 2)


.code64

.section .text
.balign 8

.global	gdtResetSegments			# Reset segments
.global	gdtLoad					# Load GDT
.global	gdtStore				# Store GDT


# Reset segments (reload CS with far return, data segments directly)
.type gdtResetSegments, %function
gdtResetSegments:

	pushq	$KERNEL_SEGMENT_CODE		# New CS
	leaq	1f(%rip), %rax			# Return address
	pushq	%rax				# --//--
	lretq					# Far return: CS takes effect

1:
	movw	$KERNEL_SEGMENT_DATA, %ax	# Set proper segment registers
	movw	%ax, %ds			# --//--
	movw	%ax, %es			# --//--
	movw	%ax, %fs			# --//--
	movw	%ax, %gs			# --//--
	movw	%ax, %ss			# --//--
	retq

.size gdtResetSegments, . - gdtResetSegments


# Load GDT (RDI - pointer to GDT pointer)
.type gdtLoad, %function
gdtLoad:

	lgdtq	(%rdi)				# Load GDT from pointer
	jmp	gdtResetSegments		# Reset segments (tail call)

.size gdtLoad, . - gdtLoad


# Store GDT (RDI - pointer to GDT pointer to fill)
.type gdtStore, %function
gdtStore:

	sgdtq	(%rdi)				# Store GDT to pointer
	retq

.size gdtStore, . - gdtStore

