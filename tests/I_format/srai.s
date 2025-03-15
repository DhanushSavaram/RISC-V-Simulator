 li t0, 5               # Load immediate value 5 into t0
    srai t1, t0, 2         # t1 = t0 signed >> 2, expected 1
    srai t2, t0, 0         # t2 = t0 signed >> 0, expected 5 (no change)
    srai t3, t0, 31        # t3 = t0 signed >> 31, boundary test
    jr ra                  # Return execution
