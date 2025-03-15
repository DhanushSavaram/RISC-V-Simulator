 li t4, 6               # Load immediate value 6 into t4
    andi t1, t0, 0         # t1 = t0 & 0, expected 0
    andi t2, t0, 0xFFFFFF00 # t2 = t0 & 0xFFFFFF00, expected 0 (masking case)
    andi t3, t0, 0xF0      # t3 = t0 & 0xF0, expected 0
    andi t5, t4, 0xDE      # t5 = t4 & 0xDE, expected 0X6
    jr ra                  # Return execution
