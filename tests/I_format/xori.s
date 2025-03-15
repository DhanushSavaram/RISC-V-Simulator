li t0, 5               # Load immediate value 5 into t0
   xori t1, t0, 0         # t1 = t0 ^ 0, expected 5 (no change)
   xori t2, t0, -1        # t2 = t0 ^ -1, expected 0xFFFFFFFA (bitwise inversion)
   xori t3, t0, 0xFF      # t3 = t0 ^ 0xFF, expected 0xFA
   jr ra                  # Return execution
