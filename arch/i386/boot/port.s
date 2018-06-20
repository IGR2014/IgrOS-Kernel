################################################################
#
#	IO low-level port operations
#
#	File:	port.s
#	Date:	21 Jun. 2018
#
#	Copyright (c) 2018, Igor Baklykov
#	All rights reserved.
#


.code32

.section .text
.balign 4
.global	inPort8				# Read byte from port
.global	inPort16			# Read word from port
.global	inPort32			# Read long from port
.global	outPort8			# Write byte to port
.global	outPort16			# Write word to port
.global	outPort32			# Write long to port


inPort8:				# Read byte from port function
	movw	4(%esp), %dx		# Port address
	inb	%dx, %al		# Read data
	ret				# Return

inPort16:				# Read word from port function
	movw	4(%esp), %dx		# Port address
	inw	%dx, %ax		# Read data
	ret				# Return

inPort32:				# Read long from port function
	movw	4(%esp), %dx		# Port address
	inl	%dx, %eax		# Read data
	ret				# Return


outPort8:				# Write byte to port function
	movw	4(%esp), %dx		# Port address
	movb	8(%esp), %al		# Data to write
	outb	%al, %dx		# Write data
	ret				# Return

outPort16:				# Write word to port function
	movw	4(%esp), %dx		# Port address
	movw	8(%esp), %ax		# Data to write
	outw	%ax, %dx		# Write data
	ret				# Return

outPort32:				# Write long to port function
	movw	4(%esp), %dx		# Port address
	movl	8(%esp), %eax		# Data to write
	outl	%eax, %dx		# Write data
	ret				# Return

