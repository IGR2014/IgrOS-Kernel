################################################################
#
#	IRQ low-level handlers
#
#	File:	interrupts.s
#	Date:	06 Jun 2019
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


.code64

.section .text
.balign	8
.global irqHandler0			# 0
.global irqHandler1			# 1
.global irqHandler2			# 2
.global irqHandler3			# 3
.global irqHandler4			# 4
.global irqHandler5			# 5
.global irqHandler6			# 6
.global irqHandler7			# 7
.global irqHandler8			# 8
.global irqHandler9			# 9
.global irqHandlerA			# 10
.global irqHandlerB			# 11
.global irqHandlerC			# 12
.global irqHandlerD			# 13
.global irqHandlerE			# 14
.global irqHandlerF			# 15
.global irqEnable			# Interrupts
.global irqDisable			# No interrupts
.extern	irqHandler			# Extenral main interrupts handler


# IRQ 0
irqHandler0:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x20			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 1
irqHandler1:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x21			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 2
irqHandler2:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x22			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 3
irqHandler3:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x23			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 4
irqHandler4:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x24			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 5
irqHandler5:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x25			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 6
irqHandler6:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x26			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 7
irqHandler7:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x27			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 8
irqHandler8:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x28			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 9
irqHandler9:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x29			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 10
irqHandlerA:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x2A			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 11
irqHandlerB:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x2B			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 12
irqHandlerC:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x2C			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 13
irqHandlerD:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x2D			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 14
irqHandlerE:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x2E			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 15
irqHandlerF:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x2F			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# Interrupt servicing routine
interruptRoutine:

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
	callq	irqHandler		# Call interrupts handler

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

# Enable interrupts
irqEnable:
	cld				# Clear direction flag
	sti				# Enable interrupts
	retq

# Disable interrupts
irqDisable:
	cld				# Clear direction flag
	cli				# Disable interrupts
	retq

