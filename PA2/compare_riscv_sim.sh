#!/bin/bash

# Variables
TEST_EXEC="~swe3005/2024f/proj2/riscv-sim"
USER_EXEC="./riscv-sim"
BIN_FILE="~swe3005/2024f/proj2/proj2_1_inst.bin"
RESULTS_DIR="results"
ARGS=(1 2 3 4 5 6 7 8 9 10) # Define the arguments to test

# Check if the results directory exists
if [ ! -d "$RESULTS_DIR" ]; then
    echo "Error: Directory '$RESULTS_DIR' does not exist. Please create it first."
    exit 1
fi

# Calculate the length of the binary file
FILE_SIZE=$(stat -c%s "$BIN_FILE")  # Get file size in bytes
INSTRUCTION_COUNT=$((FILE_SIZE / 4)) # Each instruction is 4 bytes

# Generate ARGS dynamically based on file size
ARGS=($(seq 1 $INSTRUCTION_COUNT))

# Loop through the arguments
for arg in "${ARGS[@]}"; do
    # Run the test program
    $TEST_EXEC $BIN_FILE $arg > $RESULTS_DIR/test_output_$arg.txt
    
    # Run your program
    $USER_EXEC $BIN_FILE $arg > $RESULTS_DIR/user_output_$arg.txt
    
    # Compare the outputs
    diff $RESULTS_DIR/test_output_$arg.txt $RESULTS_DIR/user_output_$arg.txt > $RESULTS_DIR/diff_$arg.txt
    if [ $? -eq 0 ]; then
        echo "Argument $arg: Outputs match!"
    else
        echo "Argument $arg: Outputs differ. See $RESULTS_DIR/diff_$arg.txt for details."
    fi
done

echo "Comparison complete. Results saved in '$RESULTS_DIR'."
