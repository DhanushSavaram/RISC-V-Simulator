 li t0, 5               # Load immediate value 5 into t0
    slli t1, t0, 4         # t1 = t0 << 4, expected 80 (5 * 16)
    slli t2, t0, 0         # t2 = t0 << 0, expected 5 (no change)
    slli t3, t0, 31        # t3 = t0 << 31, boundary test
    jr ra                  # Return execution

