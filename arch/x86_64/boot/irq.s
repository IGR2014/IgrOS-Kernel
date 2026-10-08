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


.code64

.section .text
.balign	8

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
	pushq	$0x00			# Fake error code
	pushq	$(0x20 + \number)	# Interrupt vector
	jmp	interruptServiceRoutine	# Handle IRQ

.size irqHandler\number, . - irqHandler\number
.endm


.irp	number, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
IRQ	\number
.endr


# Enable interrupts
.type irqEnable, %function
irqEnable:

	cld				# Clear direction flag
	sti				# Enable interrupts
	retq

.size irqEnable, . - irqEnable


# Disable interrupts
.type irqDisable, %function
irqDisable:

	cld				# Clear direction flag
	cli				# Disable interrupts
	retq

.size irqDisable, . - irqDisable


# Save flags and disable interrupts
.type irqSave, %function
irqSave:

	pushfq				# Save flags
	popq	%rax			# Return them
	cli				# Disable interrupts
	retq

.size irqSave, . - irqSave


# Restore flags (interrupts state)
.type irqRestore, %function
irqRestore:

	pushq	%rdi			# Saved flags
	popfq				# Restore them
	retq

.size irqRestore, . - irqRestore


.section .rodata
.balign	8

# IRQ handlers addresses (indexed by IRQ line)
irqHandlerTable:
.irp	number, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	.quad	irqHandler\number
.endr

.size irqHandlerTable, . - irqHandlerTable

