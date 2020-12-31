################################################################
#
#	CR0-CR4 in/out operations
#
#	File:	cr.s
#	Date:	20 Jan 2020
#
#	Copyright (c) 2017 - 2021, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign 4
.global outCR0			# Read CR0 register
.global outCR2			# Read CR2 register
.global outCR3			# Read CR3 register
.global outCR4			# Read CR4 register
.global inCR0			# Write CR0 register
.global inCR3			# Write CR3 register
.global inCR4			# Write CR4 register


# Read CR0 register
outCR0:
	movl	%cr0, %eax
	ret

# Read CR2 register
outCR2:
	movl	%cr2, %eax
	ret

# Read CR3 register
outCR3:
	movl	%cr3, %eax
	ret

# Read CR4 register
outCR4:
	movl	%cr4, %eax
	ret


# Write CR0 register
inCR0:
	movl	4(%esp), %eax
	movl	%eax, %cr0
	ret

# Write CR3 register
inCR3:
	movl	4(%esp), %eax
	movl	%eax, %cr3
	ret

# Write CR$ register
inCR4:
	movl	4(%esp), %eax
	movl	%eax, %cr4
	ret

