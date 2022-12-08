################################################################
#
#	CR0-CR4 in/out operations
#
#	File:	cr.s
#	Date:	20 Jan 2020
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
outCR0:
	cld				# Clear direction flag
	movq	%cr0, %rax
	retq

# Read CR2 register
outCR2:
	cld				# Clear direction flag
	movq	%cr2, %rax
	retq

# Read CR3 register
outCR3:
	cld				# Clear direction flag
	movq	%cr3, %rax
	retq

# Read CR4 register
outCR4:
	cld				# Clear direction flag
	movq	%cr4, %rax
	retq


# Write CR0 register
inCR0:
	cld				# Clear direction flag
	movq	%rdi, %cr0
	retq

# Write CR3 register
inCR3:
	cld				# Clear direction flag
	movq	%rdi, %cr3
	retq

# Write CR$ register
inCR4:
	cld				# Clear direction flag
	movq	%rdi, %cr4
	retq

