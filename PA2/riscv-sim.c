#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // uint32_t

#define REGISTER_SIZE 32
#define MEMORY_SIZE 65536
#define BASE_ADDRESS 0x10000000

uint32_t reg[REGISTER_SIZE] = {0};
uint32_t inst_mem[MEMORY_SIZE];
uint32_t data_mem[MEMORY_SIZE];
uint32_t pc = 0;

void ALU(uint32_t buffer){
    uint32_t opcode = buffer & 0x7F; // 7bit
    
    // R format
    if (opcode == 0x33){
        uint32_t rd = (buffer >> 7) & 0x1F; // 5bit
        uint32_t funct3 = (buffer >> 12) & 0x7; // 3bit
        uint32_t rs1 = (buffer >> 15) & 0x1F; // 5bit
        uint32_t rs2 = (buffer >> 20) & 0x1F; // 5bit
        uint32_t funct7 = (buffer >> 25) & 0x7F; // 7bit

        // arithmetic operation (add, sub)
        if ((funct3 == 0) && (funct7 == 0)){
            // add
            reg[rd] = reg[rs1] + reg[rs2];
        }
        else if ((funct3 == 0) && (funct7 == 0x20)){
            //sub
            reg[rd] = reg[rs1] - reg[rs2];
        }
        // logical shift operation (sll, srl)
        else if ((funct3 == 0x1) && (funct7 == 0)){
            // sll
            uint32_t lower_5bit = reg[rs2] & 0x1F;
            reg[rd] = reg[rs1] << lower_5bit;
        }
        else if ((funct3 == 0x05) && (funct7 == 0)){
            //srl
            uint32_t lower_5bit = reg[rs2] & 0x1F;
            reg[rd] = reg[rs1] >> lower_5bit;
        }
        // arithmetic shift operation (sra)
        else if ((funct3 == 0x05) && (funct7 == 0x20)){
            //sra
            uint32_t lower_5bit = reg[rs2] & 0x1F;
            reg[rd] = (int32_t)reg[rs1] >> lower_5bit;
        }
        // logical operation (or, xor, and)
        else if ((funct3 == 0x6) && (funct7 == 0)){
            //or
            reg[rd] = reg[rs1] | reg[rs2];
        }
        else if ((funct3 == 0x4) && funct7 == 0){
            // xor
            reg[rd] = reg[rs1] ^ reg[rs2];
        }
        else if ((funct3 == 0x7) && (funct7 == 0)){
            // and
            reg[rd] = reg[rs1] & reg[rs2];
        }
        // compare operation (slt)
        else if ((funct3 == 0x2) && (funct7 == 0)){
            //slt (signed)
            if ((int32_t)reg[rs1] < (int32_t)reg[rs2]){
                reg[rd] = 1;
            }
            else {reg[rd] = 0;}
        }

        if (rd == 0){
            reg[rd] = 0;
        }

        pc += 4;
    }

    // I format (opcode == 0x13) immediate instructions
    else if (opcode == 0x13){
        uint32_t rd = (buffer >> 7) & 0x1F; // 5bit
        uint32_t funct3 = (buffer >> 12) & 0x7; // 3bit
        uint32_t rs1 = (buffer >> 15) & 0x1F; // 5bit
        uint32_t imm = (buffer >> 20) & 0xFFF;// 12bit
        uint32_t shamt = imm & 0x1F; // 5bit
        uint32_t funct7 = (imm >> 5) & 0x7F;// 7bit

        // sign extension
        if (imm & 0x800) {
            imm |= 0xFFFFF000;
        }
        // casting
        int32_t signed_imm = (int32_t) imm;

        // immediate operation
        if (funct3 == 0){
            //addi
            reg[rd] = reg[rs1] + signed_imm;
        }
        else if (funct3 == 0x7){
            //andi
            reg[rd] = reg[rs1] & signed_imm;
        }
        else if (funct3 == 0x4){
            //xori
            reg[rd] = reg[rs1] ^ signed_imm;
        }
        else if (funct3 == 0x6){
            //ori
            reg[rd] = reg[rs1] | signed_imm;
        }
        // shift immediate operation
        else if ((funct3 == 0x1) && (funct7 == 0)){
            //slli
            reg[rd] = reg[rs1] << shamt;
        }
        else if ((funct3 == 0x5) && (funct7 == 0)){
            //srli
            reg[rd] = reg[rs1] >> shamt;
        }
        else if ((funct3 == 0x5) && (funct7 == 0x20)){
            //srai
            reg[rd] = (int32_t)reg[rs1] >> shamt;
        }
        // slti
        else if (funct3 == 0x2){
            //slti (signed comparison)
            if ((int32_t)reg[rs1] < signed_imm){
                reg[rd] = 1;
            }
            else {reg[rd] = 0;}
        }

        if (rd == 0){
            reg[rd] = 0;
        }

        pc += 4;    
    } 

    // I format (opcode == 0x03) memory instructions (lw)
    else if (opcode == 0x03){
        uint32_t rd = (buffer >> 7) & 0x1F; // 5bit
        uint32_t funct3 = (buffer >> 12) & 0x7; // 3bit
        uint32_t rs1 = (buffer >> 15) & 0x1F; // 5bit
        uint32_t imm = (buffer >> 20) & 0xFFF;// 12bit
        uint32_t shamt = imm & 0x1F; // 5bit
        uint32_t funct7 = (imm >> 5) & 0x7F;// 7bit

        // sign extension
        if (imm & 0x800) {
            imm |= 0xFFFFF000;
        }
        // casting
        int32_t signed_imm = (int32_t) imm;

        if (funct3 == 0x2){
            //lw (sign extension)
            uint32_t target_address = reg[rs1] + signed_imm;

            if (target_address == 0x20000000){
                int user_input;
                scanf("%d", &user_input);
                reg[rd] = user_input;
            }
            else{
                reg[rd] = data_mem[(target_address - BASE_ADDRESS) / 4];
            }
        }

        if (rd == 0){
            reg[rd] = 0;
        }

        pc += 4;
    }

    // I format (opcode == 0x67) jalr instruction
    else if (opcode == 0x67){
        uint32_t rd = (buffer >> 7) & 0x1F; // 5bit
        uint32_t funct3 = (buffer >> 12) & 0x7; // 3bit
        uint32_t rs1 = (buffer >> 15) & 0x1F; // 5bit
        uint32_t imm = (buffer >> 20) & 0xFFF;// 12bit
        uint32_t shamt = imm & 0x1F; // 5bit
        uint32_t funct7 = (imm >> 5) & 0x7F;// 7bit

        // sign extension
        if (imm & 0x800) {
            imm |= 0xFFFFF000;
        }

        // casting
        int32_t signed_imm = (int32_t) imm;

        uint32_t target_address = reg[rs1] + signed_imm;
        reg[rd] = pc + 4;
        pc = target_address;

        if (rd == 0){
            reg[rd] = 0;
        }

    }

    // S format (opcode == ) sw instruction
    else if (opcode == 0x23){
        uint32_t imm4_0 = (buffer >> 7) & 0x1F; // 5bit
        uint32_t funct3 = (buffer >> 12) & 0x7; // 3bit
        uint32_t rs1 = (buffer >> 15) & 0x1F; // 5bit
        uint32_t rs2 = (buffer >> 20) & 0x1F; // 5bit
        uint32_t imm11_5 = (buffer >> 25) & 0x7F;// 7bit
        uint32_t imm = (imm11_5 << 5) | imm4_0; // concate

        // sign extension
        if (imm & 0x800) {
            imm |= 0xFFFFF000;
        }

        // casting
        int32_t signed_imm = (int32_t) imm;

        if (funct3 == 0x2){
            uint32_t target_address = reg[rs1] + signed_imm;

            if (target_address == 0x20000000){
                printf("%c", reg[rs2]);
            }
            else {
                data_mem[(target_address - BASE_ADDRESS) / 4] = reg[rs2];
            }
        }

        pc += 4;
    }

    // U format (opcode == 0x37) lui instruction
    else if (opcode == 0x37){
        uint32_t rd = (buffer >> 7) & 0x1F; // 5bit
        uint32_t imm = (buffer >> 12) & 0xFFFFF; // 20bit

        // lower 12 bit concate
        imm = imm << 12;
        reg[rd] = imm;

        if (rd == 0){
            reg[rd] = 0;
        }

        pc += 4;
    }

    // U format (opcode == 0x17) auipc instruction
    else if (opcode == 0x17){
        uint32_t rd = (buffer >> 7) & 0x1F; // 5bit
        uint32_t imm = (buffer >> 12) & 0xFFFFF; // 20bit

        // lower 12 bit concate
        imm = imm << 12;

        reg[rd] = pc + imm;

        if (rd == 0){
            reg[rd] = 0;
        }

        pc += 4;
    }

    // UJ format (opcode == 0x6F) jal instrucion
    else if (opcode == 0x6F){
        uint32_t rd = (buffer >> 7) & 0x1F;// 5bit
        uint32_t imm19_12 = (buffer >> 12) & 0xFF; // 8bit
        uint32_t imm11 = (buffer >> 20) & 0x1; // 1bit
        uint32_t imm10_1 = (buffer >> 21) & 0x3FF; // 10bit
        uint32_t imm20 = (buffer >> 31) & 0x1; // 1bit
        uint32_t imm = (imm20 << 20) | (imm19_12 << 12) | (imm11 << 11) | (imm10_1 << 1);

        // sign extension
        if (imm & 0x100000) {
            imm |= 0xFFF00000;
        }

        int32_t signed_imm = ((int32_t)(imm << 11)) >> 11;

        reg[rd] = pc + 4;
        pc = pc + signed_imm;

        if (rd == 0){
            reg[rd] = 0;
        }
    }

    // SB format (branch instructions)
    else if (opcode == 0x63){
        uint32_t opcode = buffer & 0x7F; // 7bit
        uint32_t imm11 = (buffer >> 7) & 0x1; // 1bit
        uint32_t imm4_1 = (buffer >> 8) & 0xF;// 4bit
        uint32_t funct3 = (buffer >> 12) & 0x7; // 3bit
        uint32_t rs1 = (buffer >> 15) & 0x1F; // 5bit
        uint32_t rs2 = (buffer >> 20) & 0x1F; // 5bit
        uint32_t imm10_5 = (buffer >> 25) & 0x3F; // 6bit
        uint32_t imm12 = (buffer >> 31) & 0x1; // 1bit

        uint32_t imm = (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1); // concate

        // sign extension
        if (imm & 0x1000) {
            imm |= 0xFFFFE000;
        }
        
        // casting
        int32_t signed_imm = (int32_t) imm;   

        if (funct3 == 0){
            // beq
            if (reg[rs1] == reg[rs2]){
                pc = pc + signed_imm;
            }
            else {
                pc = pc + 4;
            }
        }
        else if (funct3 == 0x1){
            // bne
            if (reg[rs1] != reg[rs2]){
                pc = pc + signed_imm;
            }
            else {
                pc = pc + 4;
            }
        }
        else if (funct3 == 0x4){
            // blt
            if ((int32_t)reg[rs1] < (int32_t)reg[rs2]){
                pc = pc + signed_imm;
            }
            else {
                pc = pc + 4;
            }
        }
        else if (funct3 == 0x5){
            // bge
            if ((int32_t)reg[rs1] >= (int32_t)reg[rs2]){
                pc = pc + signed_imm;
            }
            else {
                pc = pc + 4;
            }
        }
    }
}

void show_reg(){
    int i;

    for (i = 0; i < REGISTER_SIZE; i++){
        printf("x%d: 0x%08x\n", i, reg[i]);
    }
}

int main(int argc, char *argv[]){
    const char *inst_file, *data_file;
    int N;
    int i;

    if (argc == 3){
        inst_file = argv[1];
        N = atoi(argv[2]);
    }

    else if (argc == 4){
        inst_file = argv[1];
        data_file = argv[2];
        N = atoi(argv[3]);
    }
    else {return 0;}

    uint32_t instruction;
    uint32_t data;

    // instruction memory load
    int inst_mem_size = 0;
    FILE *inst_p = fopen(inst_file, "rb");

    while (fread(&instruction, sizeof(uint32_t), 1, inst_p) == 1) {
        if (inst_mem_size >= MEMORY_SIZE) {
            fclose(inst_p);
        }
        inst_mem[inst_mem_size++] = instruction;
    }

    fclose(inst_p);

    FILE *data_p = NULL;

    // Initialize
    for (int k = 0; k < MEMORY_SIZE; k++) {
        data_mem[k] = 0xFF;
    }

    // data memory load
    if (data_file){
        int data_mem_size = 0;
        data_p = fopen(data_file, "rb");
        
        while (fread(&data, sizeof(uint32_t), 1, data_p) == 1) {
            if (data_mem_size >= MEMORY_SIZE) {
                fclose(data_p);
                return -1;
            }
            data_mem[data_mem_size++] = data;
        }
        fclose(data_p);
    }

    uint32_t buffer;

    // execution
    for (i = 0; i < N; i++){
        // fetch
        buffer = inst_mem[pc / 4];
        // decode and ALU
        ALU(buffer);
    }

    show_reg();

    return 0;
}
