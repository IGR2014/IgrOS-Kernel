################################################################
#
#	IRQ low-level handlers
#
#	File:	interrupts.s
#	Date:	18 Jun. 2018
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign	4
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
	pushl	$0x00			# Fake parameter
	pushl	$0x20			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 1
irqHandler1:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x21			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 2
irqHandler2:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x22			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 3
irqHandler3:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x23			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 4
irqHandler4:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x24			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 5
irqHandler5:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x25			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 6
irqHandler6:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x26			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 7
irqHandler7:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x27			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 8
irqHandler8:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x28			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 9
irqHandler9:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x29			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 10
irqHandlerA:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x2A			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 11
irqHandlerB:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x2B			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 12
irqHandlerC:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x2C			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 13
irqHandlerD:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x2D			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 14
irqHandlerE:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x2E			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# IRQ 15
irqHandlerF:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x2F			# IRQ number
	jmp	interruptRoutine	# Handle IRQ

# Interrupt servicing routine
interruptRoutine:

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
	call	irqHandler		# Call interrupts handler
	popl	%eax			# Cleanup stack after us

	popl	%gs			# Restore segment registers
	popl	%fs			# ---//---
	popl	%es			# ---//---
	popl	%ds			# ---//---
	popal				# Restore "all" registers

	addl	$0x08, %esp		# Stack cleanup
	iretl				# Done here

# Enable interrupts
irqEnable:
	sti				# Enable interrupts
	ret

# Disable interrupts
irqDisable:
	cli				# Disable interrupts
	ret

