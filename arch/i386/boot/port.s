################################################################
#
#	IO low-level port operations
#
#	File:	port.s
#	Date:	17 Jan 2020
#
#	Copyright (c) 2017 - 2020, Igor Baklykov
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
outPort8:
	movw	4(%esp), %dx		# Port address
	inb	%dx, %al		# Read data
	ret				# Return

# Read word from port
outPort16:
	movw	4(%esp), %dx		# Port address
	inw	%dx, %ax		# Read data
	ret				# Return

# Read long from port
outPort32:
	movw	4(%esp), %dx		# Port address
	inl	%dx, %eax		# Read data
	ret				# Return


# Write byte to port
inPort8:
	movw	4(%esp), %dx		# Port address
	movb	8(%esp), %al		# Data to write
	outb	%al, %dx		# Write data
	ret				# Return

# Write word to port
inPort16:
	movw	4(%esp), %dx		# Port address
	movw	8(%esp), %ax		# Data to write
	outw	%ax, %dx		# Write data
	ret				# Return

# Write long to port
inPort32:
	movw	4(%esp), %dx		# Port address
	movl	8(%esp), %eax		# Data to write
	outl	%eax, %dx		# Write data
	ret				# Return

