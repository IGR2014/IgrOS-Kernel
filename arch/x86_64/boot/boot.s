# kernel.s

.code32

.section .text
.balign	4
.global	kernelStart			# Kernel main function
.extern	kernelFunc			# Extern kernel C-function

kernelStart:				# Kernel starts here
	cli				# Turn off interrupts
	movl	$stackTop, %esp		# Set stack

	call	kernelFunc		# Call main func

	movw	$0x10, %ax		# Load data segment addr
	movw	%ax, %ds		# And copy it DS reg
	movw	%ax, %es		# And ES reg
	movw	%ax, %fs		# And FS reg
	movw	%ax, %gs		# And GS reg
	movw	%ax, %ss		# And even SS reg

haltCPU:
	hlt				# Stop CPU
	jmp	haltCPU			# Hang CPU

.section .bss
stackBottom:				# End of stack
	.skip	16384			# Stack size of 16kB
stackTop:				# Stack pointer

