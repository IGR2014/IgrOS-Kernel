# kernel.asm

# Some constants which are reqired
# by multiboot specification
.set	MAGIC,		0x1BADB002
.set	MBALIGN,	1<<0
.set	MEMINFO,	1<<1
.set	FLAGS,		MBALIGN | MEMINFO
.set	CHECKSUM,	-(MAGIC + FLAGS)

.section .boot				# Multyboot header
	.align	4
	.long	MAGIC			# Multiboot magic value
	.long	FLAGS			# Multiboot flags
	.long	CHECKSUM		# Multiboot checksum

.section .text
.align	4
.global	kernelStart			# Kernel main function
.global	portWrite			# Read data from port
.global	portRead			# Write data to port
.extern	kernelFunc			# Extern kernel C-function

kernelStart:				# Kernel starts here
	cli				# Turn off interrupts
	movl	$stackTop, %esp		# Set stack
	call	kernelFunc		# Call main func
haltLoop:
	hlt				# Stop CPU
	jmp	haltLoop		# Hang CPU

portWrite:				# Write to port function
	movw	4(%esp),%dx		# Port address
	movb	8(%esp),%al		# Data to write
	outb	%al,%dx			# Write data
	ret				# Return

portRead:				# Read from port function
	movw	4(%esp),%dx		# Port address
	inb	%dx,%al			# Read data
	ret				# Return

.section .bss
	.align	16
	stackBottom:			# End of stack
	.skip	16384			# Stack size of 16kB
	stackTop:			# Stack pointer

