# Logical Shift Right (srl)
li t0, 0xF0000000
li t1, 4
srl a0, t0, t1  

li t0, 0x00FF0000
li t1, 8
srl a1, t0, t1  

li t0, 0x12345678
li t1, 2
srl a2, t0, t1  

li t0, 0xFFFFFFFF
li t1, 16
srl a3, t0, t1  

li t0, 0x80000000
li t1, 31
srl a4, t0, t1  

jr ra
