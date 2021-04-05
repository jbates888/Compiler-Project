	.text	
	.globl		main
main:
	li		$t0, 1
	sw		$t0, num1
	li		$t0, 2
	sw		$t0, num2
	li		$t0, 3
	sw		$t0, num3
	lw		$t0, num1
	lw		$t1, num1
	seq		$t2, $t0, $t1
	beq		$zero, $t2, L1
	li		$t0, 1
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L1:
	lw		$t0, num1
	lw		$t1, num3
	sne		$t3, $t0, $t1
	beq		$zero, $t3, L2
	li		$t0, 2
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L2:
	lw		$t0, num1
	lw		$t1, num1
	sne		$t4, $t0, $t1
	beq		$zero, $t4, L3
	li		$t0, 1000
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L3:
	lw		$t0, num1
	lw		$t1, num2
	slt		$t5, $t0, $t1
	beq		$zero, $t5, L4
	li		$t0, 3
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L4:
	lw		$t0, num1
	lw		$t1, num1
	slt		$t6, $t0, $t1
	beq		$zero, $t6, L5
	li		$t0, 1000
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L5:
	lw		$t0, num1
	lw		$t1, num1
	sle		$t7, $t0, $t1
	beq		$zero, $t7, L6
	li		$t0, 4
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L6:
	lw		$t0, num1
	lw		$t1, num2
	sle		$t8, $t0, $t1
	beq		$zero, $t8, L7
	li		$t0, 5
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L7:
	lw		$t0, num3
	lw		$t1, num1
	sle		$t9, $t0, $t1
	beq		$zero, $t9, L8
	li		$t0, 1000
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L8:
	li		$v0, 10
	syscall	
	.data	
	.align		4
_nl:	.asciiz		"\n"
num1:	.word		0
num2:	.word		0
num3:	.word		0
num4:	.word		0
