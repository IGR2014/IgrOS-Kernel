########################################################################
#
#	General purpose, segment, stack and flags registers access
#
#	File:	port.s
#	Date:	22 Mar 2023
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign 4

# Segment registers
.global	outCS				# Read CS register value
.global	outDS				# Read DS register value
.global	outES				# Read ES register value
.global	outFS				# Read FS register value
.global	outGS				# Read GS register value
.global	outSS				# Read SS register value

# Stack registers
.global	outEBP				# Read EBP register value
.global	outESP				# Read ESP register value

# Eflags register
.global	outEFLAGS			# Read EFLAGS register value

# General purpose registers
.global	outEAX				# Read EAX register value
.global	outEBX				# Read EBX register value
.global	outECX				# Read ECX register value
.global	outEDX				# Read EDX register value
.global	outEDI				# Read EDI register value
.global	outESI				# Read ESI register value

# Instruction pointer
.global	outEIP				# Read EIP register value


# Read CS register value
.type outCS, %function
outCS:

	movw	%cs, %ax		# Get CS value
	retl

.size outCS, . - outCS


# Read DS register value
.type outDS, %function
outDS:

	movw	%ds, %ax		# Get DS value
	retl

.size outDS, . - outDS


# Read ES register value
.type outES, %function
outES:

	movw	%es, %ax		# Get ES value
	retl

.size outES, . - outES


# Read FS register value
.type outFS, %function
outFS:

	movw	%fs, %ax		# Get FS value
	retl

.size outFS, . - outFS


# Read GS register value
.type outGS, %function
outGS:

	movw	%gs, %ax		# Get GS value
	retl

.size outGS, . - outGS


# Read SS register value
.type outSS, %function
outSS:

	movw	%ss, %ax		# Get SS value
	retl

.size outSS, . - outSS



# Read EBP register value
.type outEBP, %function
outEBP:

	movl	%ebp, %eax		# Get EBP value
	retl

.size outEBP, . - outEBP

# Read ESP register value
.type outESP, %function
outESP:

	movl	%esp, %eax		# Get ESP value
	retl

.size outESP, . - outESP



# Read EFLAGS register value
.type outEFLAGS, %function
outEFLAGS:

	pushf				# Push EFLAGS to stack
	popl	%eax			# Get EFLAGS value from stack to output
	retl

.size outEFLAGS, . - outEFLAGS



# Read EAX register value
.type outEAX, %function
outEAX:

	retl				# EAX value already in EAX register :D

.size outEAX, . - outEAX


# Read EBX register value
.type outEBX, %function
outEBX:

	movl	%ebx, %eax		# Get EBX value
	retl

.size outEBX, . - outEBX


# Read ECX register value
.type outECX, %function
outECX:

	movl	%ecx, %eax		# Get ECX value
	retl

.size outECX, . - outECX


# Read EDX register value
.type outEDX, %function
outEDX:

	movl	%edx, %eax		# Get EDX value
	retl

.size outEDX, . - outEDX


# Read EDI register value
.type outEDI, %function
outEDI:

	movl	%edi, %eax		# Get EDI value
	retl

.size outEDI, . - outEDI


# Read ESI register value
.type outESI, %function
outESI:

	movl	%esi, %eax		# Get ESI value
	retl

.size outESI, . - outESI



# Read EIP register value
.type outEIP, %function
outEIP:

	leal	(%esp), %eax		# Get EIP value (Note - can only be read from stack after call!)
	retl

.size outEIP, . - outEIP

