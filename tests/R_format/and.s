li t0, 0xFFFFFFFF  
li t1, 0x12345678  
and a0, t0, t1  # a0 = 0xFFFFFFFF & 0x12345678 (result is 0x12345678)

li t0, 0xAAAAAAAA  
li t1, 0x55555555  
and a1, t0, t1  # a1 = 0xAAAAAAAA & 0x55555555 (result is 0x00000000, as no bits overlap)

li t0, 0x80000000  
li t1, 0x00000001  
and a2, t0, t1  # a2 = 0x80000000 & 0x00000001 (result is 0x00000000, as bits do not overlap)

li t0, 0x00000000  
li t1, 0xFFFFFFFF  
and a3, t0, t1  # a3 = 0x00000000 & 0xFFFFFFFF (result is 0x00000000)

li t0, 0xDEADBEEF  
li t1, 0xCAFEBABE  
and a4, t0, t1  # a4 = 0xDEADBEEF & 0xCAFEBABE (result is bitwise AND of both values)

jr ra  # Return to caller
