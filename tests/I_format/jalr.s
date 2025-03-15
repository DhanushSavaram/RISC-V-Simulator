 # Set up initial registers
    li x2, 1000      # Load 1000 into x2 (target address for jump)
    li x3, 500       # Load 500 into x3 (return address)
    
    # Test jalr with an immediate
    jalr x1, x2, 4   # Jump to address in x2 + 4 (1000 + 4 = 1004), store return address in x1
    
    # After returning from jalr, continue here
    # This code will be executed after the jump
    li x4, 42        # Load 42 into x4 to signify continuation
    addi x5, x1, 10  # Add 10 to x1 and store in x5 (return address + 10)

    # End of program (infinite loop for simulation purposes)
end:
    j end
