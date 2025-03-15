# Set Less Than (slt)
li t0, 5
li t1, 10
slt a0, t0, t1  

li t0, -10
li t1, 10
slt a1, t0, t1  

li t0, 10
li t1, -10
slt a2, t0, t1  

li t0, -1
li t1, 0
slt a3, t0, t1  

li t0, 100
li t1, 100
slt a4, t0, t1  

jr ra
