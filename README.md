# Computer Architecture Assignments

## Overview
This repository contains assignments for the Computer Architecture course. The assignments focus on RISC-V instruction decoding, simulation, and cache optimization.

## PA 1: RISC-V Binary Code
### Description
- Reads a binary file containing RISC-V machine code.
- Converts the machine code into its corresponding assembly representation.
- Prints the assembly instructions in a human-readable format.

### Implementation Details
- Parses binary files and decodes instructions.
- Supports RISC-V instruction formats such as R-type, I-type, S-type, B-type, U-type, and J-type.
- Outputs formatted assembly instructions.

## PA2: Single-cycle RISC-V CPU Simulator
### Description
- Implements an instruction simulator that supports a subset of single-cycle RISC-V instructions.
- Simulates execution of the instructions, updating registers and memory accordingly.

### Supported Instructions
#### Arithmetic and Logical Instructions
- `add`, `sub`, `addi`
- `xor`, `or`, `and`, `xori`, `ori`, `andi`
- `slli`, `srli`, `srai`, `sll`, `srl`, `sra`
- `slti`, `slt`

#### Control Flow Instructions
- `auipc`, `lui`
- `jal`, `jalr`
- `beq`, `bne`, `blt`, `bge`

#### Memory Instructions
- `lw`, `sw`

### Implementation Details
- Reads and executes RISC-V instructions step by step.
- Simulates register file and memory operations.
- Handles branching and jumps.

## PA3: Spike RISC-V Simulator
### Description
- Uses the RISC-V simulator’s cache simulation functionality to find the best cache configuration for given applications.
- Studies the cache simulation behavior in the Spike RISC-V instruction simulator.
- Modifies the simulator to implement the **LRU (Least Recently Used)** cache replacement algorithm.

### Implementation Details
- Tests different cache configurations to analyze performance.
- Implements and compares LRU with other cache replacement policies.
- Measures hit/miss rates and execution time to determine the optimal cache settings.

---
This repository is maintained for educational purposes only, and plagiarism of the code may result in penalties.
