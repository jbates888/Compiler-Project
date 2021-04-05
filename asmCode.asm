	.text	
	.globl		main
main:
	li		$t0, 0
	sw		$t0, num0
	li		$t0, 1
	sw		$t0, num1
	lw		$t0, num1
	lw		$t1, num1
	and		$t2, $t0, $t1
	sw		$t2, num3
	lw		$t0, num3
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	lw		$t0, num1
	lw		$t1, num1
	and		$t2, $t0, $t1
	beq		$zero, $t2, L1
	li		$t0, 2
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L1:
	lw		$t0, num1
	lw		$t1, num0
	and		$t3, $t0, $t1
	beq		$zero, $t3, L2
	li		$t0, 1000
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L2:
	lw		$t0, num1
	lw		$t1, num0
	or		$t4, $t0, $t1
	beq		$zero, $t4, L3
	li		$t0, 3
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L3:
	lw		$t0, num0
	lw		$t1, num0
	or		$t5, $t0, $t1
	beq		$zero, $t5, L4
	li		$t0, 1000
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L4:
	lw		$t0, num0
	sltu		$t1, $zero, $t0
	xori		$t1, $t1, 1
	beq		$zero, $t1, L5
	li		$t0, 4
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L5:
	lw		$t0, num1
	lw		$t6, num0
	sltu		$t7, $zero, $t6
	xori		$t7, $t7, 1
	and		$t6, $t0, $t7
	beq		$zero, $t6, L6
	li		$t0, 5
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L6:
	li		$v0, 10
	syscall	
	.data	
	.align		4
_nl:	.asciiz		"\n"
num0:	.word		0
num1:	.word		0
num3:	.word		0
num4:	.word		0
