li t0, 0x97812451  
li t1, 0x78964129  
mulhu a0, t0, t1   # Expected: High 32 bits of unsigned(0x97812451 * 0x78964129)

li t0, 0xFFFFFFFF  
li t1, 0xFFFFFFFF  
mulhu a1, t0, t1   # Expected: 0xFFFFFFFE (since max unsigned 32-bit * max = 0xFFFFFFFE00000001)

li t0, 0x12393  
li t1, 0x23456  
mulhu a2, t0, t1   # Expected: High 32 bits of unsigned(0x12393 * 0x23456)

li t0, 0x80000000  
li t1, 0x80000000  
mulhu a3, t0, t1   # Expected: 0x40000000 (since 0x80000000 * 0x80000000 = 0x4000000000000000)

li t0, 0  
li t1, 0  
mulhu a4, t0, t1   # Expected: 0 (0 * 0 = 0)

li t0, 0x123456789  
li t1, 0  
mulhu a5, t0, t1   # Expected: 0 (anything * 0 = 0)

li t0, 0  
li t1, 0x123456789  
mulhu a6, t0, t1   # Expected: 0 (0 * anything = 0)

jr ra
