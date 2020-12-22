################################################################
#
#	Interrupt servising routine handler
#
#	File:	isr.s
#	Date:	10 Oct 2019
#
#	Copyright (c) 2017 - 2020, Igor Baklykov
#	All rights reserved.
#
#


.code64

.section .text
.balign	8

.global interruptServiceRoutine		# ISR
.extern	isrHandler			# Extenral main interrupt service routine handler


# Interrupt service routine
interruptServiceRoutine:

	pushq	%rax			# Save "all" regisers
	pushq	%rcx			# ---//---
	pushq	%rdx			# ---//---
	pushq	%rbx			# ---//---
	pushq	%rbp			# ---//---
	pushq	%rsi			# ---//---
	pushq	%rdi			# ---//---
	pushq	%r8			# ---//---
	pushq	%r9			# ---//---
	pushq	%r10			# ---//---
	pushq	%r11			# ---//---
	pushq	%r12			# ---//---
	pushq	%r13			# ---//---
	pushq	%r14			# ---//---
	pushq	%r15			# ---//---

	cld				# Clear direction flag
	leaq	(%rsp), %rdi		# Take pointer to stack
	callq	isrHandler		# Call external main interrupt service routine handler

	popq	%r15			# Restore "all" registers
	popq	%r14			# ---//---
	popq	%r13			# ---//---
	popq	%r12			# ---//---
	popq	%r11			# ---//---
	popq	%r10			# ---//---
	popq	%r9			# ---//---
	popq	%r8			# ---//---
	popq	%rdi			# ---//---
	popq	%rsi			# ---//---
	popq	%rbp			# ---//---
	popq	%rbx			# ---//---
	popq	%rdx			# ---//---
	popq	%rcx			# ---//---
	popq	%rax			# ---//---

	addq	$0x10, %rsp		# Stack cleanup

	iretq				# Done here

