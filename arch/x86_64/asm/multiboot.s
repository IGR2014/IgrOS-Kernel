# multiboot.s

.code32

# Some constants which are reqired
# by multiboot specification
.set	MAGIC,		0x1BADB002
.set	MBALIGN,	1<<0
.set	MEMINFO,	1<<1
.set	FLAGS,		MBALIGN | MEMINFO
.set	CHECKSUM,	-(MAGIC + FLAGS)

.section .boot				# Multyboot header
.balign	4
	.long	MAGIC			# Multiboot magic value
	.long	FLAGS			# Multiboot flags
	.long	CHECKSUM		# Multiboot checksum

