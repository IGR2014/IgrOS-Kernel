################################################################
#
#	Interrupt servising routine handler
#
#	File:	isr.s
#	Date:	10 Oct 2019
#
#	Copyright (c) 2017 - 2021, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign	4

.global interruptServiceRoutine		# ISR
.extern	isrHandler			# Extenral main interrupt service routine handler


# Interrupt service routine
interruptServiceRoutine:

	pushal				# Save "all" regisers
	pushl	%ds			# Save segment registers
	pushl	%es			# ---//---
	pushl	%fs			# ---//---
	pushl	%gs			# ---//---

	movl	$0x10, %eax		# Load kernel data segment
	movw	%ax, %ds		# To all segment registers
	movw	%ax, %es		# ---//---
	movw	%ax, %fs		# ---//---
	movw	%ax, %gs		# ---//---

	movl	%esp, %eax		# Take pointer to stack
	pushl	%eax			# Pass it as a regs struct pointer
	call	isrHandler		# Call interrupt service routine handler
	popl	%eax			# Cleanup stack after us

	popl	%gs			# Restore segment registers
	popl	%fs			# ---//---
	popl	%es			# ---//---
	popl	%ds			# ---//---
	popal				# Restore "all" registers

	addl	$0x08, %esp		# Stack cleanup

	iretl				# Done here

