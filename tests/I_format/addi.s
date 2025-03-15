 li t0, 5               # Load immediate value 5 into t0
    addi t1, t0, 10        # t1 = t0 + 10, expected 15
    addi t2, t1, -20       # t2 = t1 - 20, expected -5
    addi t3, t0, 0         # t3 = t0 + 0, expected 5 (no change)
    jr ra                  # Return execution
