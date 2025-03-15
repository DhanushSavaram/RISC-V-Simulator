li zero, 5               # Load immediate value 5 into t0
    srli t1, zero, 2         # t1 = t0 >> 2, expected 1
    srli t2, zero, 0         # t2 = t0 >> 0, expected 5 (no change)
    srli t3, zero, 31        # t3 = t0 >> 31, boundary test
    jr ra                  # Return execution
