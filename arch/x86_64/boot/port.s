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

.global	portRead8			# Read byte from port
.global	portRead16			# Read word from port
.global	portRead32			# Read long from port
.global	portWrite8			# Write byte to port
.global	portWrite16			# Write word to port
.global	portWrite32			# Write long to port


# Read byte from port function
.type portRead8, %function
portRead8:

	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	inb	%dx, %al		# Read data
	retq				# Return

.size portRead8, . - portRead8


# Read word from port function
.type portRead16, %function
portRead16:

	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	inw	%dx, %ax		# Read data
	retq				# Return

.size portRead16, . - portRead16


# Read long from port function
.type portRead32, %function
portRead32:

	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	inl	%dx, %eax		# Read data
	retq				# Return

.size portRead32, . - portRead32


# Write byte to port function
.type portWrite8, %function
portWrite8:

	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	movb	%sil, %al		# Data to write
	outb	%al, %dx		# Write data
	retq				# Return

.size portWrite8, . - portWrite8


# Write word to port function
.type portWrite16, %function
portWrite16:

	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	movw	%si, %ax		# Data to write
	outw	%ax, %dx		# Write data
	retq				# Return

.size portWrite16, . - portWrite16


# Write long to port function
.type portWrite32, %function
portWrite32:

	cld				# Clear direction flag
	movw	%di, %dx		# Port address
	movl	%esi, %eax		# Data to write
	outl	%eax, %dx		# Write data
	retq				# Return

.size portWrite32, . - portWrite32

