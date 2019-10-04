################################################################
#
#	Multiboot v1.0 header
#
#	File:	multiboot.s
#	Date:	04 Oct 2019
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


.code32

# Some constants which are reqired by multiboot specification
.set	MULTIBOOT_MAGIC,		0x1BADB002

.set	MULTIBOOT_ALIGN,		1<<0
.set	MULTIBOOT_MEMORY_INFO,		1<<1
.set	MULTIBOOT_GRAPHICS,		1<<2

.set	MULTIBOOT_FLAGS,		(MULTIBOOT_ALIGN | MULTIBOOT_MEMORY_INFO) #| MULTIBOOT_GRAPHICS)

.set	MULTIBOOT_CHECKSUM,		-(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

.section .multiboot				# Multiboot header
.balign	8
	.long	MULTIBOOT_MAGIC			# Multiboot magic value
	.long	MULTIBOOT_FLAGS			# Multiboot flags
	.long	MULTIBOOT_CHECKSUM		# Multiboot checksum
	#.long	0				# Multiboot File header address
	#.long	0				# Multiboot File load start address
	#.long	0				# Multiboot File load end address
	#.long	0				# Multiboot File BSS end address
	#.long	0				# Multiboot File Entry point address
	#.long	0				# Multiboot Graphics mode
	#.long	800				# Multiboot Graphics width
	#.long	600				# Multiboot Graphics height
	#.long	32				# Multiboot Graphics depth

