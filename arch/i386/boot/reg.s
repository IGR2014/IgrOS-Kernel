########################################################################
#
#	General purpose, segment, stack and flags registers access
#
#	File:	port.s
#	Date:	21 Jan 2020
#
#	Copyright (c) 2017 - 2020, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign 4

# Segment registers
.global	outCS			        # Read CS register value
.global	outDS   			# Read DS register value
.global	outES   			# Read ES register value
.global	outFS   			# Read FS register value
.global	outGS   			# Read GS register value
.global	outSS   			# Read SS register value

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
outCS:
	movw	%cs, %ax		# Get CS value
	ret

# Read DS register value
outDS:
	movw	%ds, %ax		# Get DS value
	ret

# Read ES register value
outES:
	movw	%es, %ax		# Get ES value
	ret

# Read FS register value
outFS:
	movw	%fs, %ax		# Get FS value
	ret

# Read GS register value
outGS:
	movw	%gs, %ax		# Get GS value
	ret

# Read SS register value
outSS:
	movw	%ss, %ax		# Get SS value
	ret


# Read EBP register value
outEBP:
	movl	%ebp, %eax		# Get EBP value
	ret

# Read ESP register value
outESP:
	movl	%esp, %eax		# Get ESP value
	ret


# Read EFLAGS register value
outEFLAGS:
        pushf                           # Push EFLAGS to stack
	popl	%eax    		# Get EFLAGS value from stack to output
	ret


# Read EAX register value
outEAX:
	ret                             # EAX value already in EAX register :D

# Read EBX register value
outEBX:
	movl	%ebx, %eax		# Get EBX value
	ret

# Read ECX register value
outECX:
	movl	%ecx, %eax		# Get ECX value
	ret

# Read EDX register value
outEDX:
	movl	%edx, %eax		# Get EDX value
	ret

# Read EDI register value
outEDI:
	movl	%edi, %eax		# Get EDI value
	ret

# Read ESI register value
outESI:
	movl	%esi, %eax		# Get ESI value
	ret


# Read EIP register value
outEIP:
	leal	(%esp), %eax		# Get EIP value (Note - can only be read from stack after call!)
	ret

