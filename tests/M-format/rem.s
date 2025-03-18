    # REM Tests (Signed Remainder)
    li t0, 0xFFFFFFFF   # -1
    li t1, 0xFFFFFFFF   # -1
    rem a0, t0, t1      # (-1 % -1) -> 0

    li t0, 5  
    li t1, 5  
    rem a1, t0, t1      # (5 % 5) -> 0

    li t0, -5  
    li t1, 5  
    rem a2, t0, t1      # (-5 % 5) -> 0

    li t0, 5  
    li t1, -5  
    rem a3, t0, t1      # (5 % -5) -> 0

    li t0, 7  
    li t1, 3  
    rem a4, t0, t1      # (7 % 3) -> 1

    li t0, -7  
    li t1, 3  
    rem a5, t0, t1      # (-7 % 3) -> -1

    li t0, 7  
    li t1, -3  
    rem a6, t0, t1      # (7 % -3) -> 1

    li t0, -7  
    li t1, -3  
    rem a7, t0, t1      # (-7 % -3) -> -1

    li t0, 0  
    li t1, 5  
    rem t2, t0, t1      # (0 % 5) -> 0

    li t0, 5  
    li t1, 0  
    rem t3, t0, t1      # Division by zero (should return t0)

    li t0, -2147483648  # INT32_MIN
    li t1, -1  
    rem t4, t0, t1      # Undefined behavior (should return 0)

    jr ra
