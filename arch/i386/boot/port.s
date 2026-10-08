################################################################
#
#	IO low-level port operations
#
#	File:	port.s
#	Date:	22 Mar 2023
#
#	Copyright (c) 2017 - 2022, Igor Baklykov
#	All rights reserved.
#
#


.code32

.section .text
.balign 4

.global	portRead8			# Read byte from port
.global	portRead16			# Read word from port
.global	portRead32			# Read long from port
.global	portWrite8			# Write byte to port
.global	portWrite16			# Write word to port
.global	portWrite32			# Write long to port


# Read byte from port
.type portRead8, %function
portRead8:

	movw	4(%esp), %dx		# Port address
	inb	%dx, %al		# Read data
	retl

.size portRead8, . - portRead8


# Read word from port
.type portRead16, %function
portRead16:

	movw	4(%esp), %dx		# Port address
	inw	%dx, %ax		# Read data
	retl

.size portRead16, . - portRead16


# Read long from port
.type portRead32, %function
portRead32:

	movw	4(%esp), %dx		# Port address
	inl	%dx, %eax		# Read data
	retl

.size portRead32, . - portRead32



# Write byte to port
.type portWrite8, %function
portWrite8:

	movw	4(%esp), %dx		# Port address
	movb	8(%esp), %al		# Data to write
	outb	%al, %dx		# Write data
	retl

.size portWrite8, . - portWrite8


# Write word to port
.type portWrite16, %function
portWrite16:

	movw	4(%esp), %dx		# Port address
	movw	8(%esp), %ax		# Data to write
	outw	%ax, %dx		# Write data
	retl

.size portWrite16, . - portWrite16


# Write long to port
.type portWrite32, %function
portWrite32:

	movw	4(%esp), %dx		# Port address
	movl	8(%esp), %eax		# Data to write
	outl	%eax, %dx		# Write data
	retl

.size portWrite32, . - portWrite32

