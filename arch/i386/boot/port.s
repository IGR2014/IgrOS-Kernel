################################################################
#
#	IO low-level port operations
#
#	File:	port.s
#	Date:	13 Dec 2022
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign 4

.global	outPort8			# Read byte from port
.global	outPort16			# Read word from port
.global	outPort32			# Read long from port
.global	inPort8				# Write byte to port
.global	inPort16			# Write word to port
.global	inPort32			# Write long to port


# Read byte from port
.type outPort8, @function
outPort8:

	movw	4(%esp), %dx		# Port address
	inb	%dx, %al		# Read data
	retl

.size outPort8, . - outPort8


# Read word from port
.type outPort16, @function
outPort16:

	movw	4(%esp), %dx		# Port address
	inw	%dx, %ax		# Read data
	retl

.size outPort16, . - outPort16


# Read long from port
.type outPort32, @function
outPort32:

	movw	4(%esp), %dx		# Port address
	inl	%dx, %eax		# Read data
	retl

.size outPort32, . - outPort32



# Write byte to port
.type inPort8, @function
inPort8:

	movw	4(%esp), %dx		# Port address
	movb	8(%esp), %al		# Data to write
	outb	%al, %dx		# Write data
	retl

.size inPort8, . - inPort8


# Write word to port
.type inPort16, @function
inPort16:

	movw	4(%esp), %dx		# Port address
	movw	8(%esp), %ax		# Data to write
	outw	%ax, %dx		# Write data
	retl

.size inPort16, . - inPort16


# Write long to port
.type inPort32, @function
inPort32:

	movw	4(%esp), %dx		# Port address
	movl	8(%esp), %eax		# Data to write
	outl	%eax, %dx		# Write data
	retl

.size inPort32, . - inPort32

