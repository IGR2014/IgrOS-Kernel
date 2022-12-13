################################################################
#
#	Low-level boot setup function
#
#	File:	boot.s
#	Date:	13 Dec 2022
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#


.set	KERNEL_VMA,		0xC0000000		# Kernel virtual memory offset

.set	PAGE_ENTRY_VALID,	0x00000083		# Valid page entry
.set	PAGE_ENTRY_INVALID,	0x00000000		# Invalid page entry

.set	PAGE_TEMP_KERNEL,	KERNEL_VMA>>22		# Kernel PD index

.set	PAGE_BIT_PSE,		0x00000010		# Page Size Extension bit
.set	PAGE_BIT_PE,		0x80000000		# Paging Enable bit


.code32

.section .boot
.balign	4

.extern	outCR0						# Extern write CR0 function
.extern	outCR3						# Extern write CR3 function
.extern	outCR4						# Extern write CR4 function

.extern	inCR0						# Extern read CR0 function
.extern	inCR4						# Extern read CR4 function

.extern	kmain						# Extern kernel function

.global	kernelStart					# Kernel main function


# Kernel starts here
.type kernelStart, @function
kernelStart:

	cli						# Turn off interrupts

	# Setup stack pointer
	leal	stackTop - KERNEL_VMA, %esp		# Set stack

	# Save multiboot data for kmain
	# Note - since now till kmain call nothing
	# should NOT be pushed on stack without
	# poping from it!
	pushl	%eax					# Multiboot magic value
	pushl	%ebx					# Multiboot header address

	# Set new page directory (phys address)
	leal	bootPageDirectory - KERNEL_VMA, %eax	# Load temporary boot page directory phys address
	pushl	%eax					# Set new CR3 value
	leal	inCR3  - KERNEL_VMA, %ebx
	calll	*%ebx
	addl	$4, %esp

	# Enable Page Size Extension (4 Mb pages)
	leal	outCR4  - KERNEL_VMA, %ebx		# Get CR4 value
	calll	*%ebx
	orl	$PAGE_BIT_PSE, %eax			# Set PSE bit
	pushl	%eax					# Set new CR4 value
	leal	inCR4  - KERNEL_VMA, %ebx
	calll	*%ebx
	addl	$4, %esp

	# Enable paging
	leal	outCR0  - KERNEL_VMA, %ebx		# Get CR0 value
	calll	*%ebx
	orl	$PAGE_BIT_PE, %eax			# Set PE bit
	pushl	%eax					# Set new CR0 value
	leal	inCR0  - KERNEL_VMA, %ebx
	calll	*%ebx
	addl	$4, %esp

	# Set Higher Half GDT
	leal	gdt32Ptr - KERNEL_VMA, %eax		# Load Higher Half GDT pointer address
	lgdtl	(%eax)					# Load GDT pointer address

	# Jump to Higher Half
	ljmpl	$0x08, $2f				# Long jump to Higher Half CS

	# Hang on fail
1:
	hlt						# Stop CPU
	jmp	1b					# Hang CPU

2:
	# Adjust stack to higher half
	addl	$KERNEL_VMA, %esp			# Add virtual memory offset to ESP
	# Go to C++
	leal	kmain, %ebp				# Call main func
	calll	*%ebp

	# Hang on fail
3:
	hlt						# Stop CPU
	jmp	3b					# Hang CPU

.size kernelStart, . - kernelStart


.section .rodata
.balign 4096

# GDT pointer for jump from Lower Half to Higher Half
gdt32Ptr:
	.word	gdt32End - gdt32Start - 1		# GDT size
	.long	gdt32Start - KERNEL_VMA			# GDT pointer

# GDT with 32-bit code and data descriptors
gdt32Start:
	.quad	0x0000000000000000			# Empty
	.quad	0x00CF9A000000FFFF			# 32-bit code descriptor
	.quad	0x00CF92000000FFFF			# 32-bit data descriptor
	.quad	0x0000000000000000			# Empty
gdt32End:


.section .data
.balign	4096

# Temporary boot page directory
bootPageDirectory:
	# Entry 0 = 1Mb offset (identity) mapped
	.int	PAGE_ENTRY_VALID
	# Zeroes
	.fill	(PAGE_TEMP_KERNEL - 1), 4, PAGE_ENTRY_INVALID
	# Entry 227 = 3Gb + 1Mb offset (higher half) mapped
	.int	PAGE_ENTRY_VALID
	# Zeroes
	.fill	(1024 - PAGE_TEMP_KERNEL - 1), 4, PAGE_ENTRY_INVALID


.section .bss
.balign 16

# Stack data
# Stack grows from stackTop to stackBottom
stackBottom:						# End of stack
	.skip	0x20000					# Stack size of 128kB
stackTop:						# Stack pointer

