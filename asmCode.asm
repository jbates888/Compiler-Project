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
	lw		$t1, num2
	slt		$t2, $t0, $t1
	sw		$t2, num4

	lw		$t0, num4
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 11
	li		$a0, 32
	syscall	
	lw		$t0, num1
	lw		$t1, num3
	sgt		$t2, $t0, $t1
	beq		$zero, $t2, L1

	li		$t0, 1000
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 11
	li		$a0, 32
	syscall	
L1:
	lw		$t0, num1
	lw		$t1, num1
	sgt		$t3, $t0, $t1
	beq		$zero, $t3, L2

	li		$t0, 1000
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 11
	li		$a0, 32
	syscall	
L2:
	lw		$t0, num2
	lw		$t1, num1
	sgt		$t4, $t0, $t1
	beq		$zero, $t4, L3

	li		$t0, 2
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 11
	li		$a0, 32
	syscall	
L3:
	lw		$t0, num1
	lw		$t1, num1
	sge		$t5, $t0, $t1
	beq		$zero, $t5, L4

	li		$t0, 3
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 11
	li		$a0, 32
	syscall	
L4:
	lw		$t0, num1
	lw		$t1, num2
	sge		$t6, $t0, $t1
	beq		$zero, $t6, L5

	li		$t0, 1000
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 11
	li		$a0, 32
	syscall	
L5:
	lw		$t0, num3
	lw		$t1, num2
	sge		$t7, $t0, $t1
	beq		$zero, $t7, L6

	li		$t0, 4
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 11
	li		$a0, 32
	syscall	
L6:
	li		$t0, 6
	lw		$t1, num1
	sgt		$t8, $t0, $t1
	beq		$zero, $t8, L7

	li		$t0, 5
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 11
	li		$a0, 32
	syscall	
L7:
	li		$v0, 10
	syscall	
	.data	
	.align		4
_nl:	.asciiz		"\n"
num1:	.word		0
num2:	.word		0
num3:	.word		0
num4:	.word		0
