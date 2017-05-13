# exceptions.s

.code32

.section .text
	.balign	4
	.extern	exHandler		# Extenral main exception handler
	.global exHandler0		# 0
	.global exHandler1		# 1
	.global exHandler2		# 2
	.global exHandler3		# 3
	.global exHandler4		# 4
	.global exHandler5		# 5
	.global exHandler6		# 6
	.global exHandler7		# 7
	.global exHandler8		# 8
	.global exHandler9		# 9
	.global exHandler10		# 10
	.global exHandler11		# 11
	.global exHandler12		# 12
	.global exHandler13		# 13
	.global exHandler14		# 14
	.global exHandler15		# 15
	.global exHandler16		# 16
	.global exHandler17		# 17
	.global exHandler18		# 18
	.global exHandler19		# 19
	.global exHandler20		# 20
	.global exHandler21		# 21
	.global exHandler22		# 22
	.global exHandler23		# 23
	.global exHandler24		# 24
	.global exHandler25		# 25
	.global exHandler26		# 26
	.global exHandler27		# 27
	.global exHandler28		# 28
	.global exHandler29		# 29
	.global exHandler30		# 30
	.global exHandler31		# 31

# 0 Division By Zero Exception
exHandler0:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x00			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 1 Debug Exception
exHandler1:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x01			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 2 Non-Maskable Interrupt Exception
exHandler2:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x02			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 3 Breakpoint Exception
exHandler3:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x03			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 4 Into Detected Overflow Exception
exHandler4:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x04			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 5 Out of Bounds Exception
exHandler5:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x05			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 6 Invalid Opcode Exception
exHandler6:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x06			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 7 No Coprocessor Exception
exHandler7:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x07			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 8 Double Fault Exception
exHandler8:
	cli				# Disable interrupts
	pushl	$0x08			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 9 Coprocessor Segment Overrun Exception
exHandler9:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x09			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 10 Bad TSS Exception
exHandler10:
	cli				# Disable interrupts
	pushl	$0x0A			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 11 Segment Not Present Exception
exHandler11:
	cli				# Disable interrupts
	pushl	$0x0B			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 12 Stack Fault Exception
exHandler12:
	cli				# Disable interrupts
	pushl	$0x0C			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 13 General Protection Fault Exception
exHandler13:
	cli				# Disable interrupts
	pushl	$0x0D			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 14 Page Fault Exception
exHandler14:
	cli				# Disable interrupts
	pushl	$0x0E			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 15 Unknown Interrupt Exception
exHandler15:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x0F			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 16 Coprocessor Fault Exception
exHandler16:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x10			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 17 Alignment Check Exception
exHandler17:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x11			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 18 Machine Check Exception
exHandler18:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x12			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 19 Reserved Exception
exHandler19:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x13			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 20 Reserved Exception
exHandler20:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x14			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 21 Reserved Exception
exHandler21:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x15			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 22 Reserved Exception
exHandler22:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x16			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 23 Reserved Exception
exHandler23:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x17			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 24 Reserved Exception
exHandler24:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x18			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 25 Reserved Exception
exHandler25:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x19			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 26 Reserved Exception
exHandler26:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x1A			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 27 Reserved Exception
exHandler27:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x1B			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 28 Reserved Exception
exHandler28:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x1C			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 29 Reserved Exception
exHandler29:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x1D			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 30 Reserved Exception
exHandler30:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x1E			# Exception number
	jmp	exceptionRoutine	# Handle exception

# 31 Reserved Exception
exHandler31:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x1F			# Exception number
	jmp	exceptionRoutine	# Handle exception

# Exceptions servicing routine
exceptionRoutine:

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
	call	exHandler		# Call exception handler
	popl	%eax			# Cleanup stack after us

	popl	%gs			# Restore segment registers
	popl	%fs			# ---//---
	popl	%es			# ---//---
	popl	%ds			# ---//---
	popal				# Restore "all" registers

	addl	$0x08, %esp		# Stack cleanup
	iretl				# Done here
