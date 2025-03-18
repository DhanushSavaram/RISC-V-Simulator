li t0, 0x97812451  
li t1, 0x78964129  
mulh a0, t0, t1   # Expected: High 32 bits of signed(0x97812451 * 0x78964129)

li t0, -1  
li t1, -1  
mulh a1, t0, t1   # Expected: 0 (since -1 * -1 = 1, high bits = 0)

li t0, 0x12393  
li t1, 0x23456  
mulh a2, t0, t1   # Expected: High 32 bits of signed(0x12393 * 0x23456)

li t0, -0x80000000  
li t1, -1  
mulh a3, t0, t1   # Expected: 0x7FFFFFFF (since -MAX * -1 is positive)

li t0, 0  
li t1, 0  
mulh a4, t0, t1   # Expected: 0 (0 * 0 = 0)

li t0, 0x123456789  
li t1, 0  
mulh a5, t0, t1   # Expected: 0 (since anything * 0 = 0)

li t0, 0  
li t1, 0x123456789  
mulh a6, t0, t1   # Expected: 0 (since 0 * anything = 0)

jr ra
