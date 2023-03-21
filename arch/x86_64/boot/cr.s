################################################################
#
#	CR0-CR4 in/out operations
#
#	File:	cr.s
#	Date:	22 Mar 2023
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#


.code64

.section .text
.balign 8

.global outCR0			# Read CR0 register
.global outCR2			# Read CR2 register
.global outCR3			# Read CR3 register
.global outCR4			# Read CR4 register
.global inCR0			# Write CR0 register
.global inCR3			# Write CR3 register
.global inCR4			# Write CR4 register


# Read CR0 register
.type outCR0, %function
outCR0:

	cld				# Clear direction flag
	movq	%cr0, %rax
	retq

.size outCR0, . - outCR0


# Read CR2 register
.type outCR2, %function
outCR2:

	cld				# Clear direction flag
	movq	%cr2, %rax
	retq

.size outCR2, . - outCR2


# Read CR3 register
.type outCR3, %function
outCR3:

	cld				# Clear direction flag
	movq	%cr3, %rax
	retq

.size outCR3, . - outCR3


# Read CR4 register
.type outCR4, %function
outCR4:

	cld				# Clear direction flag
	movq	%cr4, %rax
	retq

.size outCR4, . - outCR4



# Write CR0 register
.type inCR0, %function
inCR0:

	cld				# Clear direction flag
	movq	%rdi, %cr0
	retq

.size inCR0, . - inCR0


# Write CR3 register
.type inCR3, %function
inCR3:

	cld				# Clear direction flag
	movq	%rdi, %cr3
	retq

.size inCR3, . - inCR3


# Write CR$ register
.type inCR4, %function
inCR4:

	cld				# Clear direction flag
	movq	%rdi, %cr4
	retq

.size inCR4, . - inCR4

