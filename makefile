<<<<<<< HEAD
CC = g++
CFLAGS = -Ddebug
SRC = RISCV.cpp
OUT = RISCV

all: clean $(OUT)  # Ensure 'clean' is executed before compilation

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT) "example.mem" 0 0

clean:
	rm -f $(OUT)
=======
CC = g++
CFLAGS = -Ddebug
SRC = RISCV.cpp
OUT = RISCV

all: clean $(OUT)  # Ensure 'clean' is executed before compilation

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT) "example.mem" 0 0

clean:
	rm -f $(OUT)
>>>>>>> 2778d17fc2cfc635d1f09d8355ad88b89b9ae152
