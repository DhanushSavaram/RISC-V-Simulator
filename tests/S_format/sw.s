li a0, 0xf123f2f4
li a1, 200
sw a0, -100(a1)
lw a2, -100(a1)
sw a0, 100(a1)
lw a3, 100(a1)
sw a0, 500(a1)
lw a4, 500(a1)
sw a0, -20(a1)
lw a5, -20(a1)
add a6, a1, a2
sub a7, a3, a4
jr ra