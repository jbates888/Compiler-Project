	.text	
	.globl		main
main:

	li		$v0, 5
	syscall	
	sw		$v0, a
	li		$v0, 5
	syscall	
	sw		$v0, b
	li		$v0, 5
	syscall	
	sw		$v0, c

	lw		$t0, a
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 11
	li		$a0, 32
	syscall	
	lw		$t1, b
	li		$v0, 1
	move		$a0, $t1
	syscall	
	li		$v0, 11
	li		$a0, 32
	syscall	
	lw		$t2, c
	li		$v0, 1
	move		$a0, $t2
	syscall	
	li		$v0, 11
	li		$a0, 32
	syscall	
	li		$v0, 10
	syscall	
	.data	
	.align		4
_nl:	.asciiz		"\n"
a:	.word		0
b:	.word		0
c:	.word		0
