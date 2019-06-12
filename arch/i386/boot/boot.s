################################################################
#
#	Low-level boot setup function
#
#	File:	boot.s
#	Date:	12 Jun 2019
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
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

.section .text
.balign	4
.global	kernelStart					# Kernel main function
.extern	kmain						# Extern kernel C-function

# Kernel starts here
kernelStart:
	cli						# Turn off interrupts

	# Set new page directory (phys address)
	movl	$bootPageDirectory - KERNEL_VMA, %ecx	# Load temporary boot page directory phys address
	movl	%ecx, %cr3				# Set new CR3 value

	# Enable Page Size Extension (4 Mb pages)
	movl	%cr4, %ecx				# Get CR4 value
	orl	$PAGE_BIT_PSE, %ecx			# Set PSE bit
	movl	%ecx, %cr4				# Set new CR4 value

	# Enable paging
	movl	%cr0, %ecx				# Get CR0 value
	orl	$PAGE_BIT_PE, %ecx			# Set PE bit
	movl	%ecx, %cr0				# Set new CR0 value

	# Reload CS
	leal	1f, %ecx				# Load new CS related address
	jmp	*%ecx					# Long jump to it
1:
	# Unmap first entry (disable identity mapping)
	movl	$bootPageDirectory, PAGE_ENTRY_INVALID	# Make identity entry invalid
	movl	%cr3, %ecx				# Reset page directory
	movl	%ecx, %cr3				# ---//---

	# Start kernel code
	movl	$stackTop, %esp				# Set stack
	pushl	%eax					# Multiboot magic value
	pushl	%ebx					# Multiboot header address
	calll	kmain					# Call main func

	# Hang on fail
2:
	hlt						# Stop CPU
	jmp	2b					# Hang CPU


.section .data
.balign	4096

# Temporary boot page directory
bootPageDirectory:
	# Entry 0 - identyty mapped
	.int	PAGE_ENTRY_VALID
	# Zeroes
	.fill	(PAGE_TEMP_KERNEL - 1), 4, PAGE_ENTRY_INVALID
	# Entry 227 - 3Gb + 1Mb mapped
	.int	PAGE_ENTRY_VALID
	# Zeroes
	.fill	(1024 - PAGE_TEMP_KERNEL - 1), 4, PAGE_ENTRY_INVALID


.section .bss
.balign 4096

# Stack data
# Stack grows from stackTop to stackBottom
stackBottom:						# End of stack
	.skip	0x40000					# Stack size of 256kB
stackTop:						# Stack pointer

