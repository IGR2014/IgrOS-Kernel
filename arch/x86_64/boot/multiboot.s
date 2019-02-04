################################################################
#
#	Multiboot v2.0 header
#
#	File:	multiboot.s
#	Date:	20 Nov. 2017
#
#	Copyright (c) 2017, Igor Baklykov
#	All rights reserved.
#
#


#.code32

# Some constants which are reqired
# by multiboot specification
#.set	MAGIC,		0xE85250D6
#.set	ARCH,		0
#.set	CHECKSUM,	-(MAGIC + ARCH + (multiboot_end - multiboot_start))

#.section .multiboot					# Multyboot header
#.balign	8
#multiboot_start:
#	.long	MAGIC					# Multiboot magic value
#	.long	ARCH					# Multiboot arch
#	.long	multiboot_end - multiboot_start		# Multiboot header size
#	.long	CHECKSUM				# Multiboot checksum
#	.word	0
#	.word	0
#	.long	8
#multiboot_end:

################################################################
#
#	Multiboot v1.0 header
#
#	File:	multiboot.s
#	Date:	20 Nov. 2017
#
#	Copyright (c) 2017, Igor Baklykov
#	All rights reserved.
#
#


.code32

# Some constants which are reqired
# by multiboot specification
.set	MAGIC,		0x1BADB002
.set	MBALIGN,	1<<0
.set	MEMINFO,	1<<1
.set	FLAGS,		MBALIGN | MEMINFO
.set	CHECKSUM,	-(MAGIC + FLAGS)

.section .multiboot			# Multyboot header
.balign	4
	.long	MAGIC			# Multiboot magic value
	.long	FLAGS			# Multiboot flags
	.long	CHECKSUM		# Multiboot checksum



