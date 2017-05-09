# idt.s

.code32

.section .text
.balign	4
.global	idtLoad				# Load IDT

idtLoad:
	movl	4(%esp), %eax		# Get pointer from stack
	lidt	(%eax)			# Load IDT from pointer
	sti				# Enable interrupts
	ret				# Done here
