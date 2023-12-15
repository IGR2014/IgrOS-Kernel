################################################################
#
#	Low-level boot setup function
#
#	File:	boot.s
#	Date:	14 Dec 2023
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
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


# 32-bit code
.code32

# Boot code section
.section .boot
.balign	8

# Check funciotns
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

	# PAE
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
	leal	gdt32Ptr - KERNEL_VMA, %eax		# Load Long Mode GDT pointer address
	lgdtl	(%eax)					# Load GDT pointer address

	# Jump to Long Mode
	ljmpl	$0x08, $2f - KERNEL_VMA			# Long jump to 64 bit CS

# Hang on fail (32 bit)
1:
	hlt						# Stop CPU
	jmp	1b					# Hang CPU

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


# Data section
.section .data
.balign	4096

# Temporary boot page map level 4 table
bootPageMapLevel4:
	# Entry 0 = 8Mb (identity) mapped memory
	.quad	bootPageDirectoryPointer - KERNEL_VMA + 0x03
	# Zero entries
	.fill	PAGE_TEMP_KERNEL, 8, PAGE_ENTRY_INVALID
	# Entry 511 = 4Gb + 8Mb (higher-half) mapped memory
	.quad	bootPageDirectoryPointer - KERNEL_VMA + 0x03

# Temporary boot page directory pointer table
bootPageDirectoryPointer:
	# Entry 0 = 4Mb (identity) mapped memory
	.quad	bootPageDirectory - KERNEL_VMA + 0x03
	# Zero entries
	.fill	(PAGE_TEMP_KERNEL - 1), 8, PAGE_ENTRY_INVALID
	# Entry 510 = 3Gb + 4Mb (higher-half) mapped memory
	.quad	bootPageDirectory - KERNEL_VMA + 0x03
	# Entry 511 = 4Gb - 4Mb (identity) mapped memory
	.quad	bootPageMapLevel4 - KERNEL_VMA + 0x03

# Temporary boot page directory table
bootPageDirectory:
	# Entry 0 = 2Mb (identity) mapped memory
	.quad	PAGE_ENTRY_VALID
	# Entry 1 = 2Mb + 2Mb (identity) mapped memory
	.quad	PAGE_ENTRY_VALID + 0x200000
	# Zero entries
	.fill	(PAGE_TEMP_KERNEL - 1), 8, PAGE_ENTRY_INVALID
	# Entry 511 = 1Gb - 2Mb (identity) mapped memory
	.quad	bootPageMapLevel4 - KERNEL_VMA + PAGE_ENTRY_VALID


# Stack section
.section .bss
.balign	16

# Stack data
# Stack grows from stackTop to stackBottom
stackBottom:						# End of stack
	.skip	0x40000					# Stack size of 256kB
stackTop:						# Stack pointer

