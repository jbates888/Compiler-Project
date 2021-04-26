	.text	
	.globl		main
main:

	addi		$t0, $zero, 1
	sw		$t0, x

	addi		$t1, $zero, 0
	sw		$t1, y
	lw		$t2, x
	lw		$t3, y
	and		$t4, $t2, $t3
	sw		$t4, z

	lw		$t2, z
	lw		$t2, z
	beq		$zero, $t2, L1
	li		$v0, 4
	la		$a0, true
	syscall	
	j		L2
L1:
	li		$v0, 4
	la		$a0, false
	syscall	
L2:
	li		$v0, 11
	li		$a0, 32
	syscall	
	li		$t2, 0
	addi		$t3, $zero, 0
L3:
	bgt		$t3, $t2, L4
	li		$v0, 4
	la		$a0, _nl
	syscall	
	addi		$t3, $t3, 1
	j		L3
L4:
	lw		$t2, x
	lw		$t3, y
	or		$t4, $t2, $t3
	sw		$t4, z

	lw		$t2, z
	lw		$t2, z
	beq		$zero, $t2, L5
	li		$v0, 4
	la		$a0, true
	syscall	
	j		L6
L5:
	li		$v0, 4
	la		$a0, false
	syscall	
L6:
	li		$v0, 11
	li		$a0, 32
	syscall	
	li		$t2, 0
	addi		$t3, $zero, 0
L7:
	bgt		$t3, $t2, L8
	li		$v0, 4
	la		$a0, _nl
	syscall	
	addi		$t3, $t3, 1
	j		L7
L8:
	lw		$t2, x
	sltu		$t3, $zero, $t2
	xori		$t3, $t3, 1
	sw		$t3, z

	lw		$t2, z
	lw		$t2, z
	beq		$zero, $t2, L9
	li		$v0, 4
	la		$a0, true
	syscall	
	j		L10
L9:
	li		$v0, 4
	la		$a0, false
	syscall	
L10:
	li		$v0, 11
	li		$a0, 32
	syscall	
	li		$t2, 0
	addi		$t3, $zero, 0
L11:
	bgt		$t3, $t2, L12
	li		$v0, 4
	la		$a0, _nl
	syscall	
	addi		$t3, $t3, 1
	j		L11
L12:
	lw		$t2, x
	lw		$t3, y
	and		$t4, $t2, $t3
	sltu		$t2, $zero, $t4
	xori		$t2, $t2, 1
	sw		$t2, z

	lw		$t2, z
	lw		$t2, z
	beq		$zero, $t2, L13
	li		$v0, 4
	la		$a0, true
	syscall	
	j		L14
L13:
	li		$v0, 4
	la		$a0, false
	syscall	
L14:
	li		$v0, 11
	li		$a0, 32
	syscall	
	li		$t2, 0
	addi		$t3, $zero, 0
L15:
	bgt		$t3, $t2, L16
	li		$v0, 4
	la		$a0, _nl
	syscall	
	addi		$t3, $t3, 1
	j		L15
L16:
	lw		$t2, x
	sltu		$t3, $zero, $t2
	xori		$t3, $t3, 1
	lw		$t2, y
	sltu		$t4, $zero, $t2
	xori		$t4, $t4, 1
	or		$t2, $t3, $t4
	sw		$t2, z

	lw		$t2, z
	lw		$t2, z
	beq		$zero, $t2, L17
	li		$v0, 4
	la		$a0, true
	syscall	
	j		L18
L17:
	li		$v0, 4
	la		$a0, false
	syscall	
L18:
	li		$v0, 11
	li		$a0, 32
	syscall	
	li		$t2, 0
	addi		$t3, $zero, 0
L19:
	bgt		$t3, $t2, L20
	li		$v0, 4
	la		$a0, _nl
	syscall	
	addi		$t3, $t3, 1
	j		L19
L20:
	lw		$t2, x
	sltu		$t3, $zero, $t2
	xori		$t3, $t3, 1
	sltu		$t2, $zero, $t3
	xori		$t2, $t2, 1
	sw		$t2, z

	lw		$t2, z
	lw		$t2, z
	beq		$zero, $t2, L21
	li		$v0, 4
	la		$a0, true
	syscall	
	j		L22
L21:
	li		$v0, 4
	la		$a0, false
	syscall	
L22:
	li		$v0, 11
	li		$a0, 32
	syscall	
	li		$t2, 0
	addi		$t3, $zero, 0
L23:
	bgt		$t3, $t2, L24
	li		$v0, 4
	la		$a0, _nl
	syscall	
	addi		$t3, $t3, 1
	j		L23
L24:
	li		$v0, 10
	syscall	
	.data	
	.align		4
x:	.word		0
y:	.word		0
z:	.word		0
_nl:	.asciiz		"\n"
true:	.asciiz		"true"
false:	.asciiz		"false"
