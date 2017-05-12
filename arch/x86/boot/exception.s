# irq.s

.code32

.section .text
	.balign	4
	.extern	isrHandler
	.global isrException0
	.global isrException1
	.global isrException2
	.global isrException3
	.global isrException4
	.global isrException5
	.global isrException6
	.global isrException7
	.global isrException8
	.global isrException9
	.global isrException10
	.global isrException11
	.global isrException12
	.global isrException13
	.global isrException14
	.global isrException15
	.global isrException16
	.global isrException17
	.global isrException18
	.global isrException19
	.global isrException20
	.global isrException21
	.global isrException22
	.global isrException23
	.global isrException24
	.global isrException25
	.global isrException26
	.global isrException27
	.global isrException28
	.global isrException29
	.global isrException30
	.global isrException31

# Division By Zero Exception
isrException0:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x00			# IRQ number
	jmp	isr			# Handle exception

# Debug Exception
isrException1:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x01			# IRQ number
	jmp	isr			# Handle exception

# Non-Maskable Interrupt Exception
isrException2:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x02			# IRQ number
	jmp	isr			# Handle exception

# Breakpoint Exception
isrException3:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x03			# IRQ number
	jmp	isr			# Handle exception

# Into Detected Overflow Exception
isrException4:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x04			# IRQ number
	jmp	isr			# Handle exception

# Out of Bounds Exception
isrException5:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x05			# IRQ number
	jmp	isr			# Handle exception

# Invalid Opcode Exception
isrException6:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x06			# IRQ number
	jmp	isr			# Handle exception

# No Coprocessor Exception
isrException7:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x07			# IRQ number
	jmp	isr			# Handle exception

# Double Fault Exception
isrException8:
	cli				# Disable interrupts
	pushl	$0x08			# IRQ number
	jmp	isr			# Handle exception

# Coprocessor Segment Overrun Exception
isrException9:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x09			# IRQ number
	jmp	isr			# Handle exception

# Bad TSS Exception
isrException10:
	cli				# Disable interrupts
	pushl	$0x0A			# IRQ number
	jmp	isr			# Handle exception

# Segment Not Present Exception
isrException11:
	cli				# Disable interrupts
	pushl	$0x0B			# IRQ number
	jmp	isr			# Handle exception

# Stack Fault Exception
isrException12:
	cli				# Disable interrupts
	pushl	$0x0C			# IRQ number
	jmp	isr			# Handle exception

# General Protection Fault Exception
isrException13:
	cli				# Disable interrupts
	pushl	$0x0D			# IRQ number
	jmp	isr			# Handle exception

# Page Fault Exception
isrException14:
	cli				# Disable interrupts
	pushl	$0x0E			# IRQ number
	jmp	isr			# Handle exception

# Unknown Interrupt Exception
isrException15:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x0F			# IRQ number
	jmp	isr			# Handle exception

# Coprocessor Fault Exception
isrException16:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x10			# IRQ number
	jmp	isr			# Handle exception

# Alignment Check Exception
isrException17:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x11			# IRQ number
	jmp	isr			# Handle exception

# Machine Check Exception
isrException18:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x12			# IRQ number
	jmp	isr			# Handle exception

# Reserved Exception
isrException19:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x13			# IRQ number
	jmp	isr			# Handle exception

# Reserved Exception
isrException20:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x14			# IRQ number
	jmp	isr			# Handle exception

# Reserved Exception
isrException21:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x15			# IRQ number
	jmp	isr			# Handle exception

# Reserved Exception
isrException22:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x16			# IRQ number
	jmp	isr			# Handle exception

# Reserved Exception
isrException23:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x17			# IRQ number
	jmp	isr			# Handle exception

# Reserved Exception
isrException24:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x18			# IRQ number
	jmp	isr			# Handle exception

# Reserved Exception
isrException25:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x19			# IRQ number
	jmp	isr			# Handle exception

# Reserved Exception
isrException26:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x1A			# IRQ number
	jmp	isr			# Handle exception

# Reserved Exception
isrException27:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x1B			# IRQ number
	jmp	isr			# Handle exception

# Reserved Exception
isrException28:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x1C			# IRQ number
	jmp	isr			# Handle exception

# Reserved Exception
isrException29:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x1D			# IRQ number
	jmp	isr			# Handle exception

# Reserved Exception
isrException30:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x1E			# IRQ number
	jmp	isr			# Handle exception

# Reserved Exception
isrException31:
	cli				# Disable interrupts
	pushl	$0x00			# Fake parameter
	pushl	$0x1F			# IRQ number
	jmp	isr			# Handle exception

# IRQ servicing routine
isr:
	
	pushal				# Save "all" regisers
	pushl	%ds			# Save segment registers
	pushl	%es			# ---//---
	pushl	%fs			# ---//---
	pushl	%gs			# ---//---

	movl	$0x10, %eax		# Load data segment
	movw	%ax, %ds		# To all segment registers
	movw	%ax, %es		# ---//---
	movw	%ax, %fs		# ---//---
	movw	%ax, %gs		# ---//---

	movl	%esp, %eax		# Save pointer
	pushl	%eax			# Save struct pointer to stack
	call	isrHandler		# Call isr handler
	popl	%eax			# Cleanup stack after us

	popl	%gs			# Restore segment registers
	popl	%fs			# ---//---
	popl	%es			# ---//---
	popl	%ds			# ---//---
	popal				# Restore "all" registers

	addl	$0x08, %esp		# Stack cleanup
	iret				# Done here

