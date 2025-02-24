#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // uint32_t

int i = 0;

int rformat(uint32_t buffer){
    uint32_t opcode = buffer & 0x7F; // 7bit
    uint32_t rd = (buffer >> 7) & 0x1F; // 5bit
    uint32_t funct3 = (buffer >> 12) & 0x7; // 3bit
    uint32_t rs1 = (buffer >> 15) & 0x1F; // 5bit
    uint32_t rs2 = (buffer >> 20) & 0x1F; // 5bit
    uint32_t funct7 = (buffer >> 25) & 0x7F; // 7bit

    if (opcode == 0x33){
        // arithmetic operation (add, sub)
        if ((funct3 == 0) && (funct7 == 0)){
            // add
            printf("inst %d: %08x add x%d, x%d, x%d\n", i++, buffer, rd, rs1, rs2);
            return 1;
        }
        else if ((funct3 == 0) && (funct7 == 0x20)){
            //sub
            printf("inst %d: %08x sub x%d, x%d, x%d\n", i++, buffer, rd, rs1, rs2);
            return 1;
        }
        // logical shift operation (sll, srl)
        else if ((funct3 == 0x1) && (funct7 == 0)){
            // sll
            printf("inst %d: %08x sll x%d, x%d, x%d\n", i++, buffer, rd, rs1, rs2);
            return 1;
        }
        else if ((funct3 == 0x05) && (funct7 == 0)){
            //srl
            printf("inst %d: %08x srl x%d, x%d, x%d\n", i++, buffer, rd, rs1, rs2);
            return 1;
        }
        // arithmetic shift operation (sra)
        else if ((funct3 == 0x05) && (funct7 == 0x20)){
            //sra
            printf("inst %d: %08x sra x%d, x%d, x%d\n", i++, buffer, rd, rs1, rs2);
            return 1;
        }
        // logical operation (or, xor, and)
        else if ((funct3 == 0x6) && (funct7 == 0)){
            //or
            printf("inst %d: %08x or x%d, x%d, x%d\n", i++, buffer, rd, rs1, rs2);
            return 1;
        }
        else if ((funct3 == 0x4) && funct7 == 0){
            // xor
            printf("inst %d: %08x xor x%d, x%d, x%d\n", i++, buffer, rd, rs1, rs2);
            return 1;
        }
        else if ((funct3 == 0x7) && (funct7 == 0)){
            // and
            printf("inst %d: %08x and x%d, x%d, x%d\n", i++, buffer, rd, rs1, rs2);
            return 1;
        }
        // compare operation (slt, sltu)
        else if ((funct3 == 0x2) && (funct7 == 0)){
            //slt (signed)
            printf("inst %d: %08x slt x%d, x%d, x%d\n", i++, buffer, rd, rs1, rs2);
            return 1;
        }
        else if ((funct3 == 0x3) && (funct7 == 0)){
            //sltu (unsigned)
            printf("inst %d: %08x sltu x%d, x%d, x%d\n", i++, buffer, rd, rs1, rs2);
            return 1;
        }
        else {return 0;}
    }
    else {return 0;}
}

int iformat(uint32_t buffer){
    uint32_t opcode = buffer & 0x7F; // 7bit
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

    if (opcode == 0x13){
        // immediate operation
        if (funct3 == 0){
            //addi
            printf("inst %d: %08x addi x%d, x%d, %d\n", i++, buffer, rd, rs1, signed_imm);
            return 1;
        }
        else if (funct3 == 0x7){
            //andi
            printf("inst %d: %08x andi x%d, x%d, %d\n", i++, buffer, rd, rs1, signed_imm);
            return 1;
        }
        else if (funct3 == 0x4){
            //xori
            printf("inst %d: %08x xori x%d, x%d, %d\n", i++, buffer, rd, rs1, signed_imm);
            return 1;
        }
        else if (funct3 == 0x6){
            //ori
            printf("inst %d: %08x ori x%d, x%d, %d\n", i++, buffer, rd, rs1, signed_imm);
            return 1;
        }
        // shift immediate operation
        else if ((funct3 == 0x1) && (funct7 == 0)){
            //slli
            printf("inst %d: %08x slli x%d, x%d, %d\n", i++, buffer, rd, rs1, shamt);
            return 1;
        }
        else if ((funct3 == 0x5) && (funct7 == 0)){
            //srli
            printf("inst %d: %08x srli x%d, x%d, %d\n", i++, buffer, rd, rs1, shamt);
            return 1;
        }
        else if ((funct3 == 0x5) && (funct7 == 0x20)){
            //srai
            printf("inst %d: %08x srai x%d, x%d, %d\n", i++, buffer, rd, rs1, shamt);
            return 1;
        }
        // slti, sltiu
        else if (funct3 == 0x2){
            //slti (signed comparison)
            printf("inst %d: %08x slti x%d, x%d, %d\n", i++, buffer, rd, rs1, signed_imm);
            return 1;
        }
        else if (funct3 == 0x3){
            //sltiu
            printf("inst %d: %08x sltiu x%d, x%d, %d\n", i++, buffer, rd, rs1, imm);
            return 1;
        }
        else {return 0;}
    }
    else if (opcode == 0x03){
        // memory instructions
        if (funct3 == 0x2){
            //lw (sign extension)
            printf("inst %d: %08x lw x%d, %d(x%d)\n", i++, buffer, rd, signed_imm, rs1);
            return 1;
        }
        else if (funct3 == 0x1){
            //lh (sign extension)
            printf("inst %d: %08x lh x%d, %d(x%d)\n", i++, buffer, rd, signed_imm, rs1);
            return 1;
        }
        else if (funct3 == 0){
            //lb (sign extension)
            printf("inst %d: %08x lb x%d, %d(x%d)\n", i++, buffer, rd, signed_imm, rs1);
            return 1;
        }
        else if (funct3 == 0x4){
            //lbu (zero extension)
            printf("inst %d: %08x lbu x%d, %d(x%d)\n", i++, buffer, rd, imm, rs1);
            return 1;
        }
        else if (funct3 == 0x5){
            //lhu (zero extension)
            printf("inst %d: %08x lhu x%d, %d(x%d)\n", i++, buffer, rd, imm, rs1);
            return 1;
        }
        else {return 0;}
    }
    else if (opcode == 0x67){
        //jalr
        printf("inst %d: %08x jalr x%d, %d(x%d)\n", i++, buffer, rd, signed_imm, rs1);
        return 1;
    }
    else {return 0;}
}

int sformat(uint32_t buffer){
    uint32_t opcode = buffer & 0x7F; // 7bit
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
    
    // store operation (sw, sh, sb)
    if (opcode == 0x23){
        if (funct3 == 0x2){
            //sw
            printf("inst %d: %08x sw x%d, %d(x%d)\n", i++, buffer, rs2, signed_imm, rs1);
            return 1;
        }
        else if (funct3 == 0x1){
            //sh
            printf("inst %d: %08x sh x%d, %d(x%d)\n", i++, buffer, rs2, signed_imm, rs1);
            return 1;
        }
        else if (funct3 == 0){
            //sb
            printf("inst %d: %08x sb x%d, %d(x%d)\n", i++, buffer, rs2, signed_imm, rs1);
            return 1;
        }
        else {return 0;}
    }
    else {return 0;}
}

int sbformat(uint32_t buffer){
    uint32_t opcode = buffer & 0x7F; // 7bit
    uint32_t imm11 = (buffer >> 7) & 0x1; // 1bit
    uint32_t imm4_1 = (buffer >> 8) & 0xF;// 4bit
    uint32_t funct3 = (buffer >> 12) & 0x7; // 3bit
    uint32_t rs1 = (buffer >> 15) & 0x1F; // 5bit
    uint32_t rs2 = (buffer >> 20) & 0x1F; // 5bit
    uint32_t imm10_5 = (buffer >> 25) & 0x3F; // 6bit
    uint32_t imm12 = (buffer >> 31) & 0x1; // 1bit
    uint32_t imm = (imm12 << 11) | (imm11 << 10) | (imm10_5 << 4) | imm4_1; // concate
    imm = imm << 1; // 13bit

    // sign extension
    if (imm & 0x1000) {
        imm |= 0xFFFFF000;
    }

    // casting
    int32_t signed_imm = (int32_t) imm;

    // branch instruction
    if (opcode == 0x63){
        if (funct3 == 0){
            //beq
            printf("inst %d: %08x beq x%d, x%d, %d\n", i++, buffer, rs1, rs2, signed_imm);
            return 1;
        }
        else if (funct3 == 0x1){
            //bne
            printf("inst %d: %08x bne x%d, x%d, %d\n", i++, buffer, rs1, rs2, signed_imm);
            return 1;
        }
        else if (funct3 == 0x4){
            //blt
            printf("inst %d: %08x blt x%d, x%d, %d\n", i++, buffer, rs1, rs2, signed_imm);
            return 1;
        }
        else if (funct3 == 0x5){
            //bge
            printf("inst %d: %08x bge x%d, x%d, %d\n", i++, buffer, rs1, rs2, signed_imm);
            return 1;
        }
        else if (funct3 == 0x6){
            //bltu
            printf("inst %d: %08x bltu x%d, x%d, %d\n", i++, buffer, rs1, rs2, imm);
            return 1;
        }
        else if (funct3 == 0x7){
            //bgeu
            printf("inst %d: %08x bgeu x%d, x%d, %d\n", i++, buffer, rs1, rs2, imm);
            return 1;
        }
        else {return 0;}
    }
    else {return 0;}
}

int ujformat(uint32_t buffer){
    uint32_t opcode = buffer & 0x7F; // 7bit
    uint32_t rd = (buffer >> 7) & 0x1F;// 5bit
    uint32_t imm19_12 = (buffer >> 12) & 0xFF; // 8bit
    uint32_t imm11 = (buffer >> 20) & 0x1; // 1bit
    uint32_t imm10_1 = (buffer >> 21) & 0x3FF; // 10bit
    uint32_t imm20 = (buffer >> 31) & 0x1; // 1bit
    uint32_t imm = (imm20 << 19) | (imm19_12 << 11) | (imm11 << 10) | imm10_1; // concate
    imm = imm << 1; //lower 1bit == 0

    // sign extension
    if (imm & 0x100000) {
        imm |= 0xFFF00000;
    }

    // casting
    int32_t signed_imm = (int32_t) imm;

    if (opcode == 0x6F){
        //jal
        printf("inst %d: %08x jal x%d, %d\n", i++, buffer, rd, signed_imm);
        return 1;
    }
    else {return 0;}
}

int uformat(uint32_t buffer){
    uint32_t opcode = buffer & 0x7F; // 7bit
    uint32_t rd = (buffer >> 7) & 0x1F; // 5bit
    uint32_t imm = (buffer >> 12) & 0xFFFFF; // 20bit

    // lower 12 bit concate
    imm = imm << 12;

    if (opcode == 0x37){
        // lui
        printf("inst %d: %08x lui x%d, %d\n", i++, buffer, rd, imm);
        return 1;
    }
    else if (opcode == 0x17){
        //auipc
        printf("inst %d: %08x auipc x%d, %d\n", i++, buffer, rd, imm);
        return 1;
    }
    else {return 0;}
}

int main(int argc, char *argv[]){
    const char *filename = argv[1];

    FILE *file = fopen(filename, "rb");
    
    uint32_t buffer;
    size_t bytesRead;

    int r_val, i_val, s_val, sb_val, uj_val, u_val; 

    while ((bytesRead = fread(&buffer, sizeof(uint32_t), 1, file)) > 0) {
        r_val = rformat(buffer); // 0 or 1
        i_val = iformat(buffer);
        s_val = sformat(buffer);
        sb_val = sbformat(buffer);
        uj_val = ujformat(buffer);
        u_val = uformat(buffer);

        if ((r_val + i_val + s_val + sb_val + uj_val + u_val == 0)){
            printf("inst %d: %08x unknown instruction\n", i++, buffer);
        }
    }

    fclose(file);
    return 0;
}
