# Logical Shift Left (sll)
li t0, 0x00000001
li t1, 1
sll a0, t0, t1  

li t0, 0x000000FF
li t1, 8
sll a1, t0, t1  

li t0, 0x0000FFFF
li t1, 4
sll a2, t0, t1  

li t0, 0x7FFFFFFF
li t1, 1
sll a3, t0, t1  

li t0, 0x80000000
li t1, 2
sll a4, t0, t1  

jr ra
