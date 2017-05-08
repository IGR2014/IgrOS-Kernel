# gdt.s

.code32

.section .text
.balign 4
.global	gdtLoad				# Load GDT

gdtLoad:
	movl	4(%esp), %eax		# Get pointer from stack
	lgdt	(%eax)			# Load GDT from pointer
	ret				# Done here

