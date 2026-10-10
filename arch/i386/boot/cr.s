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


.code32

.section .text
.balign 4

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

	movl	%cr0, %eax
	retl

.size readCR0, . - readCR0


# Read CR2 register
.type readCR2, %function
readCR2:

	movl	%cr2, %eax
	retl

.size readCR2, . - readCR2


# Read CR3 register
.type readCR3, %function
readCR3:

	movl	%cr3, %eax
	retl

.size readCR3, . - readCR3


# Read CR4 register
.type readCR4, %function
readCR4:

	movl	%cr4, %eax
	retl

.size readCR4, . - readCR4



# Write CR0 register
.type writeCR0, %function
writeCR0:

	movl	4(%esp), %eax
	movl	%eax, %cr0
	retl

.size writeCR0, . - writeCR0


# Write CR3 register
.type writeCR3, %function
writeCR3:

	movl	4(%esp), %eax
	movl	%eax, %cr3
	retl

.size writeCR3, . - writeCR3


# Write CR4 register
.type writeCR4, %function
writeCR4:

	movl	4(%esp), %eax
	movl	%eax, %cr4
	retl

.size writeCR4, . - writeCR4


# Invalidate TLB entry of page
.type pageInvalidate, %function
pageInvalidate:

	movl	4(%esp), %eax		# Page address
	invlpg	(%eax)			# Invalidate TLB entry
	retl

.size pageInvalidate, . - pageInvalidate

