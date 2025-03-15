li t0, 0xFFFFFFFF  
li t1, 0x12345678  
or a0, t0, t1  # a0 = 0xFFFFFFFF | 0x12345678 (should remain 0xFFFFFFFF)

li t0, 0xAAAAAAAA  
li t1, 0x55555555  
or a1, t0, t1  # a1 = 0xAAAAAAAA | 0x55555555 (should become all 1s: 0xFFFFFFFF)

li t0, 0x80000000  
li t1, 0x00000001  
or a2, t0, t1  # a2 = 0x80000000 | 0x00000001 (sets both highest and lowest bits)

li t0, 0x00000000  
li t1, 0xFFFFFFFF  
or a3, t0, t1  # a3 = 0x00000000 | 0xFFFFFFFF (remains 0xFFFFFFFF)

li t0, 0xDEADBEEF  
li t1, 0xCAFEBABE  
or a4, t0, t1  # a4 = 0xDEADBEEF | 0xCAFEBABE (merges bits from both values)

jr ra  # Return to caller
