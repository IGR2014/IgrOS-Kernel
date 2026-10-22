################################################################
#
#	Low-level boot setup function
#
#	File:	boot.s
#	Date:	22 Oct 2026
#
#	Copyright (c) 2017 - 2026, Igor Baklykov
#	All rights reserved.
#
#


.set	KERNEL_VMA,		0xFFFFFFFF80000000	# Kernel virtual memory offset (= KERNEL_OFFSET_VIRT)

.set	PAGE_TABLE_ENTRY,	0x0000000000000003	# Present | writable (points to next level table)
.set	PAGE_LARGE_ENTRY,	0x0000000000000083	# Present | writable | 2 MB page
.set	PAGE_ENTRY_INVALID,	0x0000000000000000	# Invalid page entry

.set	PML4_KERNEL_INDEX,	511			# PML4 index of KERNEL_VMA (bits 47..39)
.set	PDPT_KERNEL_INDEX,	510			# PDPT index of KERNEL_VMA (bits 38..30)

.set	CR4_BIT_PAE,		0x00000020		# CR4 Physical Address Extension bit
.set	EFER_BIT_LME,		0x00000100		# EFER Long Mode Enable bit
.set	CR0_BIT_PG,		0x80000000		# CR0 Paging Enable bit
.set	MSR_EFER,		0xC0000080		# EFER MSR number

.set	MBOOT_STACK_PHOLDER,	0x00000000		# Multiboot stack placeholder (MSB 32 bits)

.set	VGA_TEXT_PHYS,		0x000B8000		# VGA text memory (boot errors, paging off)
.set	VGA_ERROR_COLOR,	0x4F			# White on red


# Boot value of kernel offset: linker script checks it matches KERNEL_OFFSET_VIRT
.global	KERNEL_OFFSET_VIRT_BOOT
.set	KERNEL_OFFSET_VIRT_BOOT,	KERNEL_VMA


# 32-bit code
.code32

# Boot code section
.section .boot
.balign	8

# Check functions
.extern	checkCPUID					# Extern check CPUID function
.extern	checkLongMode					# Extern check Long Mode function
# GDT function
.extern	gdtLoad						# Extern load GDT function
# Main function
.extern	kmain						# Extern kernel function

# Export kernelStart function
.global	kernelStart					# Kernel main function


# Kernel starts here
.type kernelStart, %function
kernelStart:

	cli						# Turn off interrupts

	# Initialize stack
	leal	stackTop - KERNEL_VMA, %esp		# Set stack

	# Save multiboot data for kmain as two 64-bit values
	# Note - since now till kmain call nothing
	# should NOT be pushed on stack without
	# poping from it!
	pushl	$MBOOT_STACK_PHOLDER			# Push stack placeholder
	pushl	%eax					# Push Multiboot magic value
	pushl	$MBOOT_STACK_PHOLDER			# Push stack placeholder
	pushl	%ebx					# Push Multiboot header address

	# Check if CPUID available
	leal	checkCPUID - KERNEL_VMA, %eax		# Load address of CPUID check func
	calll	*%eax					# Call CPUID check
	leal	errorNoCPUID - KERNEL_VMA, %esi		# Error message (if check failed)
	testb	%al, %al				# Check if AL equals to zero
	jz	1f

	# Check if Long Mode available
	leal	checkLongMode - KERNEL_VMA, %eax	# Load address of Long Mode check func
	calll	*%eax					# Call Long Mode check
	leal	errorNoLongMode - KERNEL_VMA, %esi	# Error message (if check failed)
	testb	%al, %al				# Check if AL equals to zero
	jz	1f

	# Load new Page Map Level 4 table
	leal	bootPageMapLevel4 - KERNEL_VMA, %eax	# Load address of Page Map Level 4 table
	movl	%eax, %cr3				# Load page table

	# PAE
	movl	%cr4, %eax				# Load CR4 value
	orl	$CR4_BIT_PAE, %eax			# Set Physical Address Extension bit
	movl	%eax, %cr4				# Set new CR4 value

	# Enable Long Mode
	movl	$MSR_EFER, %ecx				# Load EFER register address
	rdmsr						# Read EFER value
	orl	$EFER_BIT_LME, %eax			# Set Long Mode Enable bit
	wrmsr						# Write new EFER value

	# Enable paging
	movl	%cr0, %eax				# Load CR0 value
	orl	$CR0_BIT_PG, %eax			# Set PG bit
	movl	%eax, %cr0				# Set new CR0 value

	# Set Long Mode GDT
	leal	gdt32Ptr - KERNEL_VMA, %eax		# Load Long Mode GDT pointer address
	lgdtl	(%eax)					# Load GDT pointer address

	# Jump to Long Mode
	ljmpl	$0x08, $2f - KERNEL_VMA			# Long jump to 64 bit CS

# Boot error: print message (ESI - physical address) and hang (32 bit)
1:
	cld						# String operations go forward
	movl	$VGA_TEXT_PHYS, %edi			# Top-left corner of screen
	movb	$VGA_ERROR_COLOR, %ah			# Message color
4:
	lodsb						# Next message symbol
	testb	%al, %al				# End of message
	jz	5f
	stosw						# Symbol + color
	jmp	4b
5:
	hlt						# Stop CPU
	jmp	5b					# Hang CPU

# 64-bit code
.code64

# Long Mode (64 bit code)
2:
	# Long Mode GDT pointer
	leaq	gdt64Ptr, %rdi				# Put pointer to GDT into RDI

	# Set Long Mode GDT
	movq	$gdtLoad, %rax				# Load new GDT and reset segments
	callq	*%rax

	# Adjust stack to higher half
	movl	%esp, %esp				# Zero-extend RSP (upper half is undefined after mode switch)
	addq	$KERNEL_VMA, %rsp			# Add virtual memory offset to RSP

	# Pop Multiboot header and magic from stack
	popq	%rdi					# Pop Multiboot header pointer
	popq	%rsi					# Pop Multiboot magic

	# Go to C++
	movq	$kmain, %rax				# Call main func
	callq	*%rax

# Hang on fail (64 bit)
3:
	hlt						# Stop CPU
	jmp	3b					# Hang CPU

# End of kernel function
.size kernelStart, . - kernelStart


# Read-only data section
.section .rodata
.balign	4096

# GDT pointer for jump from protected mode to long mode
gdt32Ptr:
	.word	gdt64End - gdt64Start - 1		# GDT size
	.long	gdt64Start - KERNEL_VMA			# GDT pointer

# GDT pointer for long mode
gdt64Ptr:
	.word	gdt64End - gdt64Start - 1		# GDT size
	.quad	gdt64Start				# GDT pointer

# GDT with 64-bit code and data descriptors
gdt64Start:
	.quad	0x0000000000000000			# Empty
	.quad	0x00A09A0000000000			# 64-bit code descriptor
	.quad	0x00A0920000000000			# 64-bit data descriptor
	.quad	0x0000000000000000			# Empty
gdt64End:

# Boot error messages
errorNoCPUID:
	.asciz	"IgrOS: CPUID instruction is not supported - can't boot x86_64 kernel"
errorNoLongMode:
	.asciz	"IgrOS: CPU doesn't support long mode - use i386 kernel"


# Data section
.section .data
.balign	4096

# Temporary boot page tables: first 4 MB of physical memory mapped twice,
# at 0 (identity, boot code) and at KERNEL_VMA (higher half kernel).
# Both mappings share one PDPT and one page directory; paging::init
# replaces them with kernel page tables.

# Temporary boot page map level 4 table
bootPageMapLevel4:
	# Entry 0 = 0 .. 512 GB (identity)
	.quad	bootPageDirectoryPointer - KERNEL_VMA + PAGE_TABLE_ENTRY
	# Zero entries
	.fill	(PML4_KERNEL_INDEX - 1), 8, PAGE_ENTRY_INVALID
	# Entry 511 = top 512 GB (higher half)
	.quad	bootPageDirectoryPointer - KERNEL_VMA + PAGE_TABLE_ENTRY

# Temporary boot page directory pointer table
bootPageDirectoryPointer:
	# Entry 0 = 0 .. 1 GB (identity)
	.quad	bootPageDirectory - KERNEL_VMA + PAGE_TABLE_ENTRY
	# Zero entries
	.fill	(PDPT_KERNEL_INDEX - 1), 8, PAGE_ENTRY_INVALID
	# Entry 510 = KERNEL_VMA .. KERNEL_VMA + 1 GB (higher half)
	.quad	bootPageDirectory - KERNEL_VMA + PAGE_TABLE_ENTRY
	# Entry 511 = unused
	.quad	PAGE_ENTRY_INVALID

# Temporary boot page directory table
bootPageDirectory:
	# Entry 0 = physical 0 .. 2 MB
	.quad	PAGE_LARGE_ENTRY
	# Entry 1 = physical 2 .. 4 MB
	.quad	PAGE_LARGE_ENTRY + 0x200000
	# Zero entries
	.fill	510, 8, PAGE_ENTRY_INVALID


# Stack section
.section .bss
.balign	16

# Stack data
# Stack grows from stackTop to stackBottom
stackBottom:						# End of stack
	.skip	0x40000					# Stack size of 256kB
stackTop:						# Stack pointer

