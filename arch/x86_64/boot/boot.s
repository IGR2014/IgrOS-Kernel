################################################################
#
#	Low-level boot setup function
#
#	File:	boot.s
#	Date:	20 Jan 2020
#
#	Copyright (c) 2017 - 2020, Igor Baklykov
#	All rights reserved.
#
#


.set	KERNEL_VMA,		0xFFFFFFFF80000000	# Kernel virtual memory offset

.set	PAGE_ENTRY_VALID,	0x0000000000000083	# Valid page entry
.set	PAGE_ENTRY_INVALID,	0x0000000000000000	# Invalid page entry

.set	PAGE_TEMP_KERNEL,	510			# Kernel PD index

.set	PAGE_BIT_PAE,		0x00000020		# Physical Address Extension bit
.set	PAGE_BIT_LME,		0x00000100		# Long Mode Enable bit
.set	PAGE_BIT_PE,		0x80000000		# Paging Enable bit

.set	MBOOT_STACK_PHOLDER,	0x00000000		# Multiboot stack placeholder (MSB 32 bits)


.code32

.section .boot
.balign	8

.global	kernelStart					# Kernel main function

.extern	checkCPUID
.extern checkLongMode

.extern	gdtLoad

.extern	kmain

# Kernel starts here
kernelStart:						# Kernel starts here
	cli						# Turn off interrupts

	# Initialize stack
	leal	stackTop - KERNEL_VMA, %esp		# Set stack

	# Save multiboot data for kmain
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
	testb	%al, %al				# Check if AL equals to zero
	jz	1f

	# Check if Long Mode available
	leal	checkLongMode - KERNEL_VMA, %eax	# Load address of Long Mode check func
	calll	*%eax					# Call Long Mode check
	testb	%al, %al				# Check if AL equals to zero
	jz	1f

	# Load new Page Map Level 4 table
	leal	bootPageMapLevel4 - KERNEL_VMA, %eax	# Load address of Page Map Level 4 table
	movl	%eax, %cr3				# Load page table

	# 
	movl	%cr4, %eax				# Load CR4 value
	orl	$PAGE_BIT_PAE, %eax			# Set Physical Address Extension bit
	movl	%eax, %cr4				# Set new CR4 value

	# Enable Long Mode
	movl	$0xC0000080, %ecx			# Load EFER register address
	rdmsr						# Read EFER value
	orl	$PAGE_BIT_LME, %eax			# Set Long Mode Enable bit
	wrmsr						# Write new EFER value

	# Enable paging
	movl	%cr0, %eax				# Load CR0 value
	orl	$PAGE_BIT_PE, %eax			# Set PE bit
	movl	%eax, %cr0				# Set new CR0 value

	# Set Long Mode GDT
	leal	gdt32Ptr - KERNEL_VMA, %eax		# Load 64-bit jump GDT pointer address
	lgdt	(%eax)					# Load GDT pointer address

	# Jump to long mode
	ljmp	$0x08, $2f - KERNEL_VMA			# Long jump to 64 bit CS

	# Hang on fail (32 bit)
1:
	hlt						# Stop CPU
	jmp	1b					# Hang CPU

.code64

# Long Mode (64 bit code)
2:
	# Set Long Mode GDT
	movq	$gdt64Ptr, %rdi				# Put pointer to GDT into RDI
	callq	gdtLoad					# Load new GDT and reset segments

	# Adjust stack to higher half
	addq	$KERNEL_VMA, %rsp			# Add virtual memory offset to RSP

	# Pop Multiboot header and magic from stack
	popq	%rdi					# Pop Multiboot header pointer
	popq	%rsi					# Pop Multiboot magic

	# Go to C++
	cld						# Clear direction flag
	callq	kmain					# Call main func

	# Hang on fail (64 bit)
3:
	hlt						# Stop CPU
	jmp	3b					# Hang CPU


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
	.quad	0x0020980000000000			# 64-bit code descriptor
	.quad	0x0020900000000000			# 64-bit data descriptor
gdt64End:


.section .data
.balign	4096

# Temporary boot page map level 4 table
bootPageMapLevel4:
	# Entry 0 - identity mapped lower-half memory
	.quad	bootPageDirectoryPointer - KERNEL_VMA + 3
	# Zero entries
	.fill	PAGE_TEMP_KERNEL, 8, PAGE_ENTRY_INVALID
	# Entry 0 - identity mapped higher-half memory
	.quad	bootPageDirectoryPointer - KERNEL_VMA + 3

# Temporary boot page directory pointer table
bootPageDirectoryPointer:
	# Entry 0 - identity mapped [0..1] Gb memory
	.quad	bootPageDirectory - KERNEL_VMA + 3
	# Zero entries
	.fill	PAGE_TEMP_KERNEL - 1, 8, PAGE_ENTRY_INVALID
	# Entry 0 - higher half mapped [-1..-0] Gb memory
	.quad	bootPageDirectory - KERNEL_VMA + 3
	# Zero entry
	.quad	PAGE_ENTRY_INVALID

# Temporary boot page directory table
bootPageDirectory:
	# Entry 0 - mapped [0..2] Mb memory
	.quad	PAGE_ENTRY_VALID
	# Entry 1 - mapped [2..4] Mb memory
	.quad	PAGE_ENTRY_VALID + 0x200000
	# Zero entries
	.fill	PAGE_TEMP_KERNEL, 8, PAGE_ENTRY_INVALID


.section .bss
.balign	16

# Stack data
# Stack grows from stackTop to stackBottom
stackBottom:						# End of stack
	.skip	0x40000					# Stack size of 256kB
stackTop:						# Stack pointer

