################################################################
#
#	Exception low-level handlers
#
#	File:	exceptions.s
#	Date:	22 Mar 2023
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#


.code64

.section .text
.balign	8

.extern	interruptServiceRoutine		# Extenral main exception handler

.global exHandler00			# 0
.global exHandler01			# 1
.global exHandler02			# 2
.global exHandler03			# 3
.global exHandler04			# 4
.global exHandler05			# 5
.global exHandler06			# 6
.global exHandler07			# 7
.global exHandler08			# 8
.global exHandler09			# 9
.global exHandler0A			# 10
.global exHandler0B			# 11
.global exHandler0C			# 12
.global exHandler0D			# 13
.global exHandler0E			# 14
.global exHandler0F			# 15
.global exHandler10			# 16
.global exHandler11			# 17
.global exHandler12			# 18
.global exHandler13			# 19
.global exHandler14			# 20
.global exHandler15			# 21
.global exHandler16			# 22
.global exHandler17			# 23
.global exHandler18			# 24
.global exHandler19			# 25
.global exHandler1A			# 26
.global exHandler1B			# 27
.global exHandler1C			# 28
.global exHandler1D			# 29
.global exHandler1E			# 30
.global exHandler1F			# 31


# 0 Division By Zero Exception
.type exHandler00, %function
exHandler00:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x00			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler00, . - exHandler00


# 1 Debug Exception
.type exHandler01, %function
exHandler01:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x01			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler01, . - exHandler01


# 2 Non-Maskable Interrupt Exception
.type exHandler02, %function
exHandler02:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x02			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler02, . - exHandler02


# 3 Breakpoint Exception
.type exHandler03, %function
exHandler03:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x03			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler03, . - exHandler03


# 4 Into Detected Overflow Exception
.type exHandler04, %function
exHandler04:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x04			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler04, . - exHandler04


# 5 Out of Bounds Exception
.type exHandler05, %function
exHandler05:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x05			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler05, . - exHandler05


# 6 Invalid Opcode Exception
.type exHandler06, %function
exHandler06:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x06			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler06, . - exHandler06


# 7 No Coprocessor Exception
.type exHandler07, %function
exHandler07:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x07			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler07, . - exHandler07


# 8 Double Fault Exception
.type exHandler08, %function
exHandler08:

	cli				# Disable interrupts
	pushq	$0x08			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler08, . - exHandler08


# 9 Coprocessor Segment Overrun Exception
.type exHandler09, %function
exHandler09:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x09			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler09, . - exHandler09


# 10 Bad TSS Exception
.type exHandler0A, %function
exHandler0A:

	cli				# Disable interrupts
	pushq	$0x0A			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler0A, . - exHandler0A


# 11 Segment Not Present Exception
.type exHandler0B, %function
exHandler0B:

	cli				# Disable interrupts
	pushq	$0x0B			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler0B, . - exHandler0B


# 12 Stack Fault Exception
.type exHandler0C, %function
exHandler0C:

	cli				# Disable interrupts
	pushq	$0x0C			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler0C, . - exHandler0C


# 13 General Protection Fault Exception
.type exHandler0D, %function
exHandler0D:

	cli				# Disable interrupts
	pushq	$0x0D			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler0D, . - exHandler0D


# 14 Page Fault Exception
.type exHandler0E, %function
exHandler0E:

	cli				# Disable interrupts
	pushq	$0x0E			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler0E, . - exHandler0E


# 15 Unknown Interrupt Exception
.type exHandler0F, %function
exHandler0F:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x0F			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler0F, . - exHandler0F


# 16 Coprocessor Fault Exception
.type exHandler10, %function
exHandler10:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x10			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler10, . - exHandler10


# 17 Alignment Check Exception
.type exHandler11, %function
exHandler11:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x11			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler11, . - exHandler11


# 18 Machine Check Exception
.type exHandler12, %function
exHandler12:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x12			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler12, . - exHandler12


# 19 Reserved Exception
.type exHandler13, %function
exHandler13:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x13			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler13, . - exHandler13


# 20 Reserved Exception
.type exHandler14, %function
exHandler14:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x14			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler14, . - exHandler14


# 21 Reserved Exception
.type exHandler15, %function
exHandler15:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x15			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler15, . - exHandler15


# 22 Reserved Exception
.type exHandler16, %function
exHandler16:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x16			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler16, . - exHandler16


# 23 Reserved Exception
.type exHandler17, %function
exHandler17:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x17			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler17, . - exHandler17


# 24 Reserved Exception
.type exHandler18, %function
exHandler18:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x18			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler18, . - exHandler18


# 25 Reserved Exception
.type exHandler19, %function
exHandler19:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x19			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler19, . - exHandler19


# 26 Reserved Exception
.type exHandler1A, %function
exHandler1A:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x1A			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler1A, . - exHandler1A


# 27 Reserved Exception
.type exHandler1B, %function
exHandler1B:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x1B			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler1B, . - exHandler1B


# 28 Reserved Exception
.type exHandler1C, %function
exHandler1C:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x1C			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler1C, . - exHandler1C


# 29 Reserved Exception
.type exHandler1D, %function
exHandler1D:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x1D			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler1D, . - exHandler1D


# 30 Reserved Exception
.type exHandler1E, %function
exHandler1E:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x1E			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler1E, . - exHandler1E


# 31 Reserved Exception
.type exHandler1F, %function
exHandler1F:

	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x1F			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

.size exHandler1F, . - exHandler1F

