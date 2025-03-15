li t0, -20
li t1, 4
sub a0, t0, t1  # a0 = -20 - 4

li t0, 1
li t1, -1
sub a1, t0, t1  # a1 = 1 - (-1)

li t0, -1
li t1, -1
sub a2, t0, t1  # a2 = -1 - (-1)

li t0, 1
li t1, 0
sub a3, t0, t1  # a3 = 1 - 0

li t0, 0x80000000
li t1, 1
sub a4, t0, t1  # a4 = 0x80000000 - 1 (overflow case)
jr ra