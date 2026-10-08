################################################################
#
#	Exception low-level handlers
#
#	File:	exceptions.s
#	Date:	22 Mar 2023
#
#	Copyright (c) 2017 - 2026, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign	4

.extern	interruptServiceRoutine		# Extenral main exception handler

.global	exHandlerTable			# Exception handlers addresses


# Exception handler (CPU pushes error code only for some exceptions,
# others get fake one, so stack layout is always the same)
.macro	EXCEPTION number, hasError
.type exHandler\number, %function
exHandler\number:

	cli				# Disable interrupts
.if	\hasError == 0
	pushl	$0x00			# Fake error code
.endif
	pushl	$\number		# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler\number, . - exHandler\number
.endm


EXCEPTION	0,  0			# Divide by zero
EXCEPTION	1,  0			# Debug
EXCEPTION	2,  0			# Non-maskable interrupt
EXCEPTION	3,  0			# Breakpoint
EXCEPTION	4,  0			# Overflow
EXCEPTION	5,  0			# Bound range exceeded
EXCEPTION	6,  0			# Invalid opcode
EXCEPTION	7,  0			# Device not available
EXCEPTION	8,  1			# Double fault
EXCEPTION	9,  0			# Coprocessor segment overrun
EXCEPTION	10, 1			# Invalid TSS
EXCEPTION	11, 1			# Segment not present
EXCEPTION	12, 1			# Stack-segment fault
EXCEPTION	13, 1			# General protection fault
EXCEPTION	14, 1			# Page fault
EXCEPTION	15, 0			# Reserved
EXCEPTION	16, 0			# x87 floating-point exception
EXCEPTION	17, 1			# Alignment check
EXCEPTION	18, 0			# Machine check
EXCEPTION	19, 0			# SIMD floating-point exception
EXCEPTION	20, 0			# Virtualization exception
EXCEPTION	21, 1			# Control protection exception
EXCEPTION	22, 0			# Reserved
EXCEPTION	23, 0			# Reserved
EXCEPTION	24, 0			# Reserved
EXCEPTION	25, 0			# Reserved
EXCEPTION	26, 0			# Reserved
EXCEPTION	27, 0			# Reserved
EXCEPTION	28, 0			# Hypervisor injection exception
EXCEPTION	29, 1			# VMM communication exception
EXCEPTION	30, 1			# Security exception
EXCEPTION	31, 0			# Reserved


.section .rodata
.balign	4

# Exception handlers addresses (indexed by exception number)
exHandlerTable:
.irp	number, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
	.long	exHandler\number
.endr

.size exHandlerTable, . - exHandlerTable

