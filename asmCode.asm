	.text	
	.globl		main
main:
	li		$t0, 10
	sw		$t0, num1
	li		$t0, 2
	sw		$t0, num2
	lw		$t0, num1
	lw		$t1, num1
	mul		$t2, $t0, $t1
	lw		$t0, num1
	lw		$t1, num2
	div		$t3, $t0, $t1
	add		$t0, $t2, $t3
	sw		$t0, num3
	li		$t0, 10
	li		$t1, 2
	div		$t2, $t0, $t1
	sw		$t2, num4
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
	lw		$t0, num3
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	lw		$t0, num4
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	li		$t0, 100
	sw		$t0, num4
	li		$t0, 9
	sub		$t1, $zero, $t0
	li		$t0, 8
	sub		$t2, $t1, $t0
	li		$t0, 2
	add		$t1, $t2, $t0
	sw		$t1, num5
	li		$t0, 38
	li		$t1, 6
	div		$t2, $t0, $t1
	MFHI		$t2
	sw		$t2, num6
	lw		$t0, num6
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	lw		$t0, num5
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
num3:	.word		0
num4:	.word		0
num5:	.word		0
num6:	.word		0
