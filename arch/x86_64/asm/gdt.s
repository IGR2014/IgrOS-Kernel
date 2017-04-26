# gdt.s

.code32

.section .text
.balign 4
.global	gdtLoad				# Load GDT

gdtLoad:
	movl	4(%esp), %eax		# Get pointer from stack
	lgdt	(%eax)			# Load GDT from pointer
	movw	$0x10, %ax		# Load data segment addr
	movw	%ax, %ds		# And copy it DS reg
	movw	%ax, %es		# And ES reg
	movw	%ax, %fs		# And FS reg
	movw	%ax, %gs		# And GS reg
	movw	%ax, %ss		# And even SS reg
	jmp	$0x08, $farJump		# Far jump to update CS reg
farJump:
	ret				# We are done here

