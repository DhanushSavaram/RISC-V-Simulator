    # REMU Tests (Unsigned Remainder)
    li t0, 0xFFFFFFFF   # Unsigned max (4294967295)
    li t1, 5  
    remu a0, t0, t1     # (0xFFFFFFFF % 5) -> 0

    li t0, 10  
    li t1, 3  
    remu a1, t0, t1     # (10 % 3) -> 1

    li t0, 15  
    li t1, 4  
    remu a2, t0, t1     # (15 % 4) -> 3

    li t0, 0xFFFFFFFF   
    li t1, 7  
    remu a3, t0, t1     # (0xFFFFFFFF % 7) -> 3

    li t0, 10  
    li t1, 0xFFFFFFFD   # -3 (interpreted as 4294967293)
    remu a4, t0, t1     # (10 % 4294967293) -> 10

    li t0, 0  
    li t1, 5  
    remu t2, t0, t1     # (0 % 5) -> 0

    li t0, 5  
    li t1, 0  
    remu t3, t0, t1     # Division by zero (should return t0)

    jr ra
