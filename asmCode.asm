	.text	
	.globl		main
main:
	li		$t0, 0
	sw		$t0, num0
	li		$t0, 1
	sw		$t0, num1
	li		$t0, 1
	li		$t1, 0
	and		$t2, $t0, $t1
	li		$t0, 1
	or		$t1, $t2, $t0
	sltu		$t0, $zero, $t1
	xori		$t0, $t0, 1
	li		$t1, 2
	slt		$t2, $t0, $t1
	sw		$t2, num3
	lw		$t0, num3
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
num0:	.word		0
num1:	.word		0
num3:	.word		0
