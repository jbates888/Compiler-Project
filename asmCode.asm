	.text	
	.globl		main
main:
	li		$t0, 1
	sw		$t0, num1
	li		$t0, 2
	sw		$t0, num2
	lw		$t0, num1
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	lw		$t0, num2
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	li		$t0, 2
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	syscall	
	syscall	
	syscall	
	lw		$t0, num1
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	lw		$t0, num1
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	li		$t0, 4
	li		$v0, 1
	move		$a0, $t0
	syscall	
	lw		$t0, num1
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	li		$v0, 10
	syscall	
	.data	
	.align		4
_nl:	.asciiz		"\n"
num1:	.word		0
num2:	.word		0
