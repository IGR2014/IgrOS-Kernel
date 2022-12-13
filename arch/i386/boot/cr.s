################################################################
#
#	CR0-CR4 in/out operations
#
#	File:	cr.s
#	Date:	13 Dec 2022
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
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
.type outCR0, @function
outCR0:

	movl	%cr0, %eax
	retl

.size outCR0, . - outCR0


# Read CR2 register
.type outCR2, @function
outCR2:

	movl	%cr2, %eax
	retl

.size outCR2, . - outCR2


# Read CR3 register
.type outCR3, @function
outCR3:

	movl	%cr3, %eax
	retl

.size outCR3, . - outCR3


# Read CR4 register
.type outCR4, @function
outCR4:

	movl	%cr4, %eax
	retl

.size outCR4, . - outCR4



# Write CR0 register
.type inCR0, @function
inCR0:

	movl	4(%esp), %eax
	movl	%eax, %cr0
	retl

.size inCR0, . - inCR0


# Write CR3 register
.type inCR3, @function
inCR3:

	movl	4(%esp), %eax
	movl	%eax, %cr3
	retl

.size inCR3, . - inCR3


# Write CR4 register
.type inCR4, @function
inCR4:

	movl	4(%esp), %eax
	movl	%eax, %cr4
	retl

.size inCR4, . - inCR4

