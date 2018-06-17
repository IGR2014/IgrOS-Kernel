################################################################
#
#	IO low-level port operations
#
#	File:	port.s
#	Date:	18 Jun. 2018
#
#	Copyright (c) 2018, Igor Baklykov
#	All rights reserved.
#


.code32

.section .text
.balign 4
.global	inPortB				# Read byte from port
.global	inPortW				# Read word from port
.global	inPortL				# Read long from port
.global	outPortB			# Write byte to port
.global	outPortW			# Write word to port
.global	outPortL			# Write long to port


inPortB:				# Read byte from port function
	movw	4(%esp), %dx		# Port address
	inb	%dx, %al		# Read data
	ret				# Return

inPortW:				# Read word from port function
	movw	4(%esp), %dx		# Port address
	inw	%dx, %ax		# Read data
	ret				# Return

inPortL:				# Read long from port function
	movw	4(%esp), %dx		# Port address
	inl	%dx, %eax		# Read data
	ret				# Return


outPortB:				# Write byte to port function
	movw	4(%esp), %dx		# Port address
	movb	8(%esp), %al		# Data to write
	outb	%al, %dx		# Write data
	ret				# Return

outPortW:				# Write word to port function
	movw	4(%esp), %dx		# Port address
	movw	8(%esp), %ax		# Data to write
	outw	%ax, %dx		# Write data
	ret				# Return

outPortL:				# Write long to port function
	movw	4(%esp), %dx		# Port address
	movl	8(%esp), %eax		# Data to write
	outl	%eax, %dx		# Write data
	ret				# Return

