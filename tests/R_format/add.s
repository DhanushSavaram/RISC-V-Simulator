li t0, -20
li t1, 4
add a0, t0, t1  # a0 = -20 + 4

li t0, 1
li t1, -1
add a1, t0, t1  # a1 = 1 + (-1)

li t0, -1
li t1, -1
add a2, t0, t1  # a2 = -1 + (-1)

li t0, 1
li t1, 0
add a3, t0, t1  # a3 = 1 + 0

li t0, 0x7FFFFFFF
li t1, 1
add a4, t0, t1  # a4 = 0x7FFFFFFF + 1 (overflow case)
jr ra 