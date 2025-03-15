 li t0, 5               # Load immediate value 5 into t0
    sltiu t1, t0, 10       # t1 = (unsigned t0 < 10) ? 1 : 0, expected 1
    sltiu t2, t0, 5        # t2 = (unsigned t0 < 5) ? 1 : 0, expected 0
    sltiu t3, t0, 0        # t3 = (unsigned t0 < 0) ? 1 : 0, expected 0
    jr ra                  # Return execution
