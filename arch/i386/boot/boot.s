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


.set	KERNEL_VMA,		0xC0000000		# Kernel virtual memory offset (= KERNEL_OFFSET_VIRT)

.set	PAGE_ENTRY_VALID,	0x00000083		# Present | writable | 4 MB page
.set	PAGE_ENTRY_INVALID,	0x00000000		# Invalid page entry

.set	PAGE_TEMP_KERNEL,	KERNEL_VMA>>22		# Kernel PD index (768)

.set	CR4_BIT_PSE,		0x00000010		# CR4 Page Size Extension bit
.set	CR0_BIT_PG,		0x80000000		# CR0 Paging Enable bit

.set	EFLAGS_BIT_ID,		0x00200000		# EFLAGS.ID: CPUID available if it can be toggled
.set	CPUID_FEATURES,		0x00000001		# CPUID feature flags leaf
.set	CPUID_EDX_PSE,		0x00000008		# CPUID.1:EDX.PSE: 4 MB pages supported

.set	VGA_TEXT_PHYS,		0x000B8000		# VGA text memory (boot errors, paging off)
.set	VGA_ERROR_COLOR,	0x4F			# White on red


# Boot value of kernel offset: linker script checks it matches KERNEL_OFFSET_VIRT
.global	KERNEL_OFFSET_VIRT_BOOT
.set	KERNEL_OFFSET_VIRT_BOOT,	KERNEL_VMA


# 32-bit code
.code32

# Boot code section
.section .boot
.balign	4

# Main function
.extern	kmain						# Extern kernel function

# Export kernelStart function
.global	kernelStart					# Kernel main function


# Kernel starts here
.type kernelStart, %function
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

	# Check CPUID is available (try to toggle EFLAGS.ID)
	leal	errorNoPSE - KERNEL_VMA, %esi		# Error message (if check failed)
	pushfl						# Original EFLAGS
	popl	%eax					# --//--
	movl	%eax, %ecx				# --//--
	xorl	$EFLAGS_BIT_ID, %eax			# Toggle ID bit
	pushl	%eax					# --//--
	popfl						# --//--
	pushfl						# Read EFLAGS back
	popl	%eax					# --//--
	pushl	%ecx					# Restore original EFLAGS
	popfl						# --//--
	cmpl	%ecx, %eax				# ID bit didn't change - no CPUID
	je	1f

	# Check 4 MB pages are supported (boot page directory uses them)
	pushl	%ebx					# CPUID clobbers EBX
	movl	$CPUID_FEATURES, %eax			# Feature flags leaf
	cpuid						# --//--
	popl	%ebx					# --//--
	testl	$CPUID_EDX_PSE, %edx			# PSE supported
	jz	1f

	# Set new page directory (phys address)
	leal	bootPageDirectory - KERNEL_VMA, %eax	# Load temporary boot page directory phys address
	movl	%eax, %cr3				# Set new CR3 value

	# Enable Page Size Extension (4 MB pages)
	movl	%cr4, %eax				# Load CR4 value
	orl	$CR4_BIT_PSE, %eax			# Set PSE bit
	movl	%eax, %cr4				# Set new CR4 value

	# Enable paging
	movl	%cr0, %eax				# Load CR0 value
	orl	$CR0_BIT_PG, %eax			# Set PG bit
	movl	%eax, %cr0				# Set new CR0 value

	# Set Higher Half GDT
	leal	gdt32Ptr, %eax				# Load Higher Half GDT pointer address
	lgdtl	(%eax)					# Load GDT pointer address

	# Jump to Higher Half
	ljmpl	$0x08, $2f				# Long jump to Higher Half CS

# Boot error: print message (ESI - physical address) and hang
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

# continue kernel execution
2:
	# Adjust stack to higher half
	addl	$KERNEL_VMA, %esp			# Add virtual memory offset to ESP
	# Go to C++
	leal	kmain, %ebp				# Kernel main func address
	calll	*%ebp					# Call: kernel main func

# Hang on fail
3:
	hlt						# Stop CPU
	jmp	3b					# Hang CPU

# End of kernel function
.size kernelStart, . - kernelStart


# Read-only data section
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

# Boot error messages
errorNoPSE:
	.asciz	"IgrOS: CPU doesn't support 4 MB pages (CPUID PSE) - can't boot"


# Data section
.section .data
.balign	4096

# Temporary boot page directory: first 4 MB of physical memory mapped twice,
# at 0 (identity, boot code) and at KERNEL_VMA (higher half kernel);
# paging::init replaces it with kernel page tables
bootPageDirectory:
	# Entry 0 = physical 0 .. 4 MB (identity)
	.int	PAGE_ENTRY_VALID
	# Zeroes
	.fill	(PAGE_TEMP_KERNEL - 1), 4, PAGE_ENTRY_INVALID
	# Entry 768 = physical 0 .. 4 MB at KERNEL_VMA (higher half)
	.int	PAGE_ENTRY_VALID
	# Zeroes
	.fill	(1024 - PAGE_TEMP_KERNEL - 1), 4, PAGE_ENTRY_INVALID


# Stack section
.section .bss
.balign	4096

# Stack guard page: left unmapped by paging::init, stack overflow faults
# (double fault then runs on its own stack) instead of corrupting .bss
.global	_KERNEL_STACK_GUARD_
_KERNEL_STACK_GUARD_:
	.skip	0x1000

# Stack data
# Stack grows from stackTop to stackBottom
stackBottom:						# End of stack
	.skip	0x20000					# Stack size of 128kB
stackTop:						# Stack pointer

