################################################################
#
#	IO low-level port operations
#
#	File:	port.s
#	Date:	20 Nov. 2017
#
#	Copyright (c) 2017, Igor Baklykov
#	All rights reserved.
#


.code32

.section .text
.balign 4
.global	portRByte			# Read byte from port
.global	portRWord			# Read word from port
.global	portRLong			# Read long from port

.global	portWByte			# Write byte to port
.global	portWWord			# Write word to port
.global	portWLong			# Write long to port


portRByte:				# Read byte from port function
	movw	4(%esp), %dx		# Port address
	inb	%dx, %al		# Read data
	ret				# Return

portRWord:				# Read word from port function
	movw	4(%esp), %dx		# Port address
	inw	%dx, %ax		# Read data
	ret				# Return

portRLong:				# Read long from port function
	movw	4(%esp), %dx		# Port address
	inl	%dx, %eax		# Read data
	ret				# Return


portWByte:				# Write byte to port function
	movw	4(%esp), %dx		# Port address
	movb	8(%esp), %al		# Data to write
	outb	%al, %dx		# Write data
	ret				# Return

portWWord:				# Write word to port function
	movw	4(%esp), %dx		# Port address
	movw	8(%esp), %ax		# Data to write
	outw	%ax, %dx		# Write data
	ret				# Return

portWLong:				# Write long to port function
	movw	4(%esp), %dx		# Port address
	movl	8(%esp), %eax		# Data to write
	outl	%eax, %dx		# Write data
	ret				# Return

