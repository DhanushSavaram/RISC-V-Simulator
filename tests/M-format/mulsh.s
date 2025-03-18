li t0, 0x97812451  
li t1, 0x78964129  
mulhsu a0, t0, t1   # Expected: High 32 bits of signed(0x97812451 * 0x78964129)

li t0, -1  
li t1, 1  
mulhsu a1, t0, t1   # Expected: -1 (since -1 * 1 = -1)

li t0, 0x12393  
li t1, 0x23456  
mulhsu a2, t0, t1   # Expected: High 32 bits of signed(0x12393 * 0x23456)

li t0, -0x80000000  
li t1, 0xFFFFFFFF  
mulhsu a3, t0, t1   # Expected: -0x80000000 (since -MAX * MAX is large negative)

li t0, 0  
li t1, 0  
mulhsu a4, t0, t1   # Expected: 0 (0 * 0 = 0)

li t0, 0x123456789  
li t1, 0  
mulhsu a5, t0, t1   # Expected: 0 (anything * 0 = 0)

li t0, 0  
li t1, 0x123456789  
mulhsu a6, t0, t1   # Expected: 0 (0 * anything = 0)

jr ra
