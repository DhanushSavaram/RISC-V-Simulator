# Define toolchain commands using aliases
RV32AS = $(RISCV)/bin/riscv32-unknown-elf-as
RV32OBJDUMP = $(RISCV)/bin/riscv32-unknown-elf-objdump
RV32GCC = $(RISCV)/bin/riscv32-unknown-elf-gcc
RV32LD = $(RISCV)/bin/riscv32-unknown-elf-ld

# Compilation flags
ASFLAGS = -ahld

# Default file, can be overridden via command line (e.g., make FILE=my_program all)
FILE ?= test_add

# C++ Compiler and flags
CC = g++
CFLAGS =
SRC = RISCV.cpp
OUT = RISCV

# Targets
all: clean assemble $(OUT)  # Clean before building

# Assemble the RISC-V file
assemble: $(FILE).o $(FILE).dis $(FILE).mem

$(FILE).o: $(FILE).s
	$(RV32AS) $(ASFLAGS) $< -o $@ 

$(FILE).dis: $(FILE).o
	$(RV32OBJDUMP) -d $< > $@

$(FILE).mem: $(FILE).o
	$(RV32OBJDUMP) -d $< | grep -o '^[[:blank:]]*[[:xdigit:]]*:[[:blank:]][[:xdigit:]]*' > $@

# Compile the C++ simulator
$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

# Run the simulator with the specified memory file
run: $(OUT) $(FILE).mem
	./$(OUT) "$(FILE).mem" 1

# Clean all generated files
clean:
	rm -f *.o *.dis *.mem prog.lst $(OUT)