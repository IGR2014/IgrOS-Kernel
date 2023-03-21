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


.code64

.section .text
.balign 8

.global	outPort8			# Read byte from port
.global	outPort16			# Read word from port
.global	outPort32			# Read long from port
.global	inPort8				# Write byte to port
.global	inPort16			# Write word to port
.global	inPort32			# Write long to port


# Read byte from port function
.type outPort8, %function
outPort8:

	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	inb	%dx, %al		# Read data
	retq				# Return

.size outPort8, . - outPort8


# Read word from port function
.type outPort16, %function
outPort16:

	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	inw	%dx, %ax		# Read data
	retq				# Return

.size outPort16, . - outPort16


# Read long from port function
.type outPort32, %function
outPort32:

	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	inl	%dx, %eax		# Read data
	retq				# Return

.size outPort32, . - outPort32


# Write byte to port function
.type inPort8, %function
inPort8:

	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	movb	%sil, %al		# Data to write
	outb	%al, %dx		# Write data
	retq				# Return

.size inPort8, . - inPort8


# Write word to port function
.type inPort16, %function
inPort16:

	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	movw	%si, %ax		# Data to write
	outw	%ax, %dx		# Write data
	retq				# Return

.size inPort16, . - inPort16


# Write long to port function
.type inPort32, %function
inPort32:

	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	movl	%esi, %eax		# Data to write
	outl	%eax, %dx		# Write data
	retq				# Return

.size inPort32, . - inPort32

