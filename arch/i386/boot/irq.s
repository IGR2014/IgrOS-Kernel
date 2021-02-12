################################################################
#
#	IRQ low-level handlers
#
#	File:	irq.s
#	Date:	11 Feb 2021
#
#	Copyright (c) 2017 - 2021, Igor Baklykov
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

.extern	interruptServiceRoutine		# Extenral main interrupts handler


# IRQ 0
.type irqHandler0, @function
irqHandler0:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x20			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
.size irqHandler0, . - irqHandler0

# IRQ 1
.type irqHandler1, @function
irqHandler1:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x21			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
.size irqHandler1, . - irqHandler1

# IRQ 2
.type irqHandler2, @function
irqHandler2:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x22			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
.size irqHandler2, . - irqHandler2

# IRQ 3
.type irqHandler3, @function
irqHandler3:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x23			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
.size irqHandler3, . - irqHandler3

# IRQ 4
.type irqHandler4, @function
irqHandler4:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x24			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
.size irqHandler4, . - irqHandler4

# IRQ 5
.type irqHandler5, @function
irqHandler5:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x25			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
.size irqHandler5, . - irqHandler5

# IRQ 6
.type irqHandler6, @function
irqHandler6:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x26			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
	.size irqHandler6, . - irqHandler6

# IRQ 7
.type irqHandler7, @function
irqHandler7:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x27			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
.size irqHandler7, . - irqHandler7

# IRQ 8
.type irqHandler8, @function
irqHandler8:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x28			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
	.size irqHandler8, . - irqHandler8

# IRQ 9
irqHandler9:
.type irqHandler9, @function
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x29			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
.size irqHandler9, . - irqHandler9

# IRQ 10
.type irqHandlerA, @function
irqHandlerA:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x2A			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
.size irqHandlerA, . - irqHandlerA

# IRQ 11
.type irqHandlerB, @function
irqHandlerB:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x2B			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
.size irqHandlerB, . - irqHandlerB

# IRQ 12
.type irqHandlerC, @function
irqHandlerC:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x2C			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
.size irqHandlerC, . - irqHandlerC

# IRQ 13
.type irqHandlerD, @function
irqHandlerD:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x2D			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
.size irqHandlerD, . - irqHandlerD

# IRQ 14
.type irqHandlerE, @function
irqHandlerE:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x2E			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
.size irqHandlerE, . - irqHandlerE

# IRQ 15
.type irqHandlerF, @function
irqHandlerF:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x2F			# IRQ number
	jmp	interruptServiceRoutine	# Handle IRQ
.size irqHandlerF, . - irqHandlerF


# Enable interrupts
.type irqEnable, @function
irqEnable:
	sti				# Enable interrupts
	retl
.size irqEnable, . - irqEnable

# Disable interrupts
.type irqDisable, @function
irqDisable:
	cli				# Disable interrupts
	retl
.size irqDisable, . - irqDisable

