# Arithmetic Shift Right (sra)
li t0, 0xF0000000
li t1, 4
sra a0, t0, t1  

li t0, 0x80000000
li t1, 8
sra a1, t0, t1  

li t0, 0x7FFFFFFF
li t1, 1
sra a2, t0, t1  

li t0, -1
li t1, 4
sra a3, t0, t1  

li t0, 0x40000000
li t1, 2
sra a4, t0, t1  

jr ra
