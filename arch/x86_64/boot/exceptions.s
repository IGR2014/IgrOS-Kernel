################################################################
#
#	Exception low-level handlers
#
#	File:	exceptions.s
#	Date:	10 Oct 2019
#
#	Copyright (c) 2017 - 2021, Igor Baklykov
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
exHandler00:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x00			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 1 Debug Exception
exHandler01:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x01			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 2 Non-Maskable Interrupt Exception
exHandler02:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x02			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 3 Breakpoint Exception
exHandler03:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x03			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 4 Into Detected Overflow Exception
exHandler04:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x04			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 5 Out of Bounds Exception
exHandler05:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x05			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 6 Invalid Opcode Exception
exHandler06:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x06			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 7 No Coprocessor Exception
exHandler07:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x07			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 8 Double Fault Exception
exHandler08:
	cli				# Disable interrupts
	pushq	$0x08			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 9 Coprocessor Segment Overrun Exception
exHandler09:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x09			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 10 Bad TSS Exception
exHandler0A:
	cli				# Disable interrupts
	pushq	$0x0A			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 11 Segment Not Present Exception
exHandler0B:
	cli				# Disable interrupts
	pushq	$0x0B			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 12 Stack Fault Exception
exHandler0C:
	cli				# Disable interrupts
	pushq	$0x0C			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 13 General Protection Fault Exception
exHandler0D:
	cli				# Disable interrupts
	pushq	$0x0D			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 14 Page Fault Exception
exHandler0E:
	cli				# Disable interrupts
	pushq	$0x0E			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 15 Unknown Interrupt Exception
exHandler0F:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x0F			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 16 Coprocessor Fault Exception
exHandler10:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x10			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 17 Alignment Check Exception
exHandler11:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x11			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 18 Machine Check Exception
exHandler12:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x12			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 19 Reserved Exception
exHandler13:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x13			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 20 Reserved Exception
exHandler14:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x14			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 21 Reserved Exception
exHandler15:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x15			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 22 Reserved Exception
exHandler16:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x16			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 23 Reserved Exception
exHandler17:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x17			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 24 Reserved Exception
exHandler18:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x18			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 25 Reserved Exception
exHandler19:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x19			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 26 Reserved Exception
exHandler1A:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x1A			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 27 Reserved Exception
exHandler1B:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x1B			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 28 Reserved Exception
exHandler1C:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x1C			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 29 Reserved Exception
exHandler1D:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x1D			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 30 Reserved Exception
exHandler1E:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x1E			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

# 31 Reserved Exception
exHandler1F:
	cli				# Disable interrupts
	pushq	$0x00			# Fake parameter
	pushq	$0x1F			# Exception number
	jmp	interruptServiceRoutine	# Handle exception

