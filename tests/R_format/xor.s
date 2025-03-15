li t0, 0xFFFFFFFF  
li t1, 0x12345678  
xor a0, t0, t1  # a0 = 0xFFFFFFFF ^ 0x12345678 (flips bits of t1)

li t0, 0xAAAAAAAA  
li t1, 0x55555555  
xor a1, t0, t1  # a1 = 0xAAAAAAAA ^ 0x55555555 (alternating bit pattern)

li t0, 0x80000000  
li t1, 0x7FFFFFFF  
xor a2, t0, t1  # a2 = 0x80000000 ^ 0x7FFFFFFF (sign bit flip)

li t0, 0x00000000  
li t1, 0xFFFFFFFF  
xor a3, t0, t1  # a3 = 0x00000000 ^ 0xFFFFFFFF (all bits flip)

li t0, 0xDEADBEEF  
li t1, 0xCAFEBABE  
xor a4, t0, t1  # a4 = 0xDEADBEEF ^ 0xCAFEBABE (random large values)

jr ra  # Return to caller
