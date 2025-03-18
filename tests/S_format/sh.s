li a0, 0xf2f4
li a1, 200
sh a0, -100(a1)
lh a2, -100(a1)
sh a0, 100(a1)
lh a3, 100(a1)
sh a0, 500(a1)
lh a4, 200(a1)
sh a0, -20(a1)
lh a5, -20(a1)
add a6, a1, a2
sub a7, a3, a4
jr ra