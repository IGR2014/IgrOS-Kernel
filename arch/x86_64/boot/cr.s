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

.global readCR0			# Read CR0 register
.global readCR2			# Read CR2 register
.global readCR3			# Read CR3 register
.global readCR4			# Read CR4 register
.global writeCR0		# Write CR0 register
.global writeCR3		# Write CR3 register
.global writeCR4		# Write CR4 register
.global pageInvalidate		# Invalidate TLB entry of page


# Read CR0 register
.type readCR0, %function
readCR0:

	cld				# Clear direction flag
	movq	%cr0, %rax
	retq

.size readCR0, . - readCR0


# Read CR2 register
.type readCR2, %function
readCR2:

	cld				# Clear direction flag
	movq	%cr2, %rax
	retq

.size readCR2, . - readCR2


# Read CR3 register
.type readCR3, %function
readCR3:

	cld				# Clear direction flag
	movq	%cr3, %rax
	retq

.size readCR3, . - readCR3


# Read CR4 register
.type readCR4, %function
readCR4:

	cld				# Clear direction flag
	movq	%cr4, %rax
	retq

.size readCR4, . - readCR4



# Write CR0 register
.type writeCR0, %function
writeCR0:

	cld				# Clear direction flag
	movq	%rdi, %cr0
	retq

.size writeCR0, . - writeCR0


# Write CR3 register
.type writeCR3, %function
writeCR3:

	cld				# Clear direction flag
	movq	%rdi, %cr3
	retq

.size writeCR3, . - writeCR3


# Write CR$ register
.type writeCR4, %function
writeCR4:

	cld				# Clear direction flag
	movq	%rdi, %cr4
	retq

.size writeCR4, . - writeCR4


# Invalidate TLB entry of page
.type pageInvalidate, %function
pageInvalidate:

	invlpg	(%rdi)			# Invalidate TLB entry
	retq

.size pageInvalidate, . - pageInvalidate

