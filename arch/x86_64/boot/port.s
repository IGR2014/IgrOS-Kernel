################################################################
#
#	IO low-level port operations
#
#	File:	port.s
#	Date:	06 Jun 2019
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


.code64

.section .text
.balign 8
.global	inPort8				# Read byte from port
.global	inPort16			# Read word from port
.global	inPort32			# Read long from port
.global	outPort8			# Write byte to port
.global	outPort16			# Write word to port
.global	outPort32			# Write long to port


inPort8:				# Read byte from port function
	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	inb	%dx, %al		# Read data
	retq				# Return

inPort16:				# Read word from port function
	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	inw	%dx, %ax		# Read data
	retq				# Return

inPort32:				# Read long from port function
	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	inl	%dx, %eax		# Read data
	retq				# Return


outPort8:				# Write byte to port function
	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	movb	%sil, %al		# Data to write
	outb	%al, %dx		# Write data
	retq				# Return

outPort16:				# Write word to port function
	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	movw	%si, %ax		# Data to write
	outw	%ax, %dx		# Write data
	retq				# Return

outPort32:				# Write long to port function
	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	movl	%esi, %eax		# Data to write
	outl	%eax, %dx		# Write data
	retq				# Return

