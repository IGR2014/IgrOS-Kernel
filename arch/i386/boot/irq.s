################################################################
#
#	IRQ low-level handlers
#
#	File:	irq.s
#	Date:	22 Mar 2023
#
#	Copyright (c) 2017 - 2026, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign	4

.extern	interruptServiceRoutine		# Extenral main interrupts handler

.global	irqHandlerTable			# IRQ handlers addresses
.global irqEnable			# Interrupts
.global irqDisable			# No interrupts
.global irqSave				# Save flags and disable interrupts
.global irqRestore			# Restore flags


# IRQ handler (IRQs are remapped to vectors 0x20..0x2F)
.macro	IRQ number
.type irqHandler\number, %function
irqHandler\number:

	cli				# Disable interrupts
	pushl	$0x00			# Fake error code
	pushl	$(0x20 + \number)	# Interrupt vector
	jmp	interruptServiceRoutine	# Handle IRQ

.size irqHandler\number, . - irqHandler\number
.endm


.irp	number, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
IRQ	\number
.endr


# Enable interrupts
.type irqEnable, %function
irqEnable:

	sti				# Enable interrupts
	retl

.size irqEnable, . - irqEnable


# Disable interrupts
.type irqDisable, %function
irqDisable:

	cli				# Disable interrupts
	retl

.size irqDisable, . - irqDisable


# Save flags and disable interrupts
.type irqSave, %function
irqSave:

	pushfl				# Save flags
	popl	%eax			# Return them
	cli				# Disable interrupts
	retl

.size irqSave, . - irqSave


# Restore flags (interrupts state)
.type irqRestore, %function
irqRestore:

	pushl	4(%esp)			# Saved flags
	popfl				# Restore them
	retl

.size irqRestore, . - irqRestore


.section .rodata
.balign	4

# IRQ handlers addresses (indexed by IRQ line)
irqHandlerTable:
.irp	number, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	.long	irqHandler\number
.endr

.size irqHandlerTable, . - irqHandlerTable

