################################################################
#
#	Low-level boot setup function
#
#	File:	boot.s
#	Date:	03 Jul 2020
#
#	Copyright (c) 2017 - 2021, Igor Baklykov
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

.global	kernelStart					# Kernel main function

.extern outCR0
.extern outCR3
.extern outCR4
.extern inCR0
.extern inCR4
.extern	kmain						# Extern kernel C-function

# Kernel starts here
kernelStart:
	cli						# Turn off interrupts

	# Start kernel code
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

	# Reload CS
	leal	1f, %eax				# Load new CS related address
	jmp	*%eax					# Long jump to it

1:
	# Adjust stack to higher half
	addl	$KERNEL_VMA, %esp			# Add virtual memory offset to ESP
	# Go to C++
	calll	kmain					# Call main func

	# Hang on fail
2:
	hlt						# Stop CPU
	jmp	2b					# Hang CPU


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

