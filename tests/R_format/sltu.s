# Set Less Than Unsigned (sltu)
li t0, -1  
li t1, 1
sltu a0, t0, t1  

li t0, 1
li t1, -1
sltu a1, t0, t1  

li t0, 0
li t1, 1
sltu a2, t0, t1  

li t0, 100
li t1, 50
sltu a3, t0, t1  

li t0, 0x80000000
li t1, 0x7FFFFFFF
sltu a4, t0, t1  

jr ra
