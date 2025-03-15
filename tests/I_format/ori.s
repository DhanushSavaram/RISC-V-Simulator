 li t0, 5               # Load immediate value 5 into t0
    ori t1, t0, 0          # t1 = t0 | 0, expected 5 (no change)
    ori t2, t0, 0xFF     # t2 = t0 | 0xFFFF, expected 0xFFFF
    ori t3, t0, 0x7F       # t3 = t0 | 0x7F, expected 0x7F
    jr ra                  # Return execution
