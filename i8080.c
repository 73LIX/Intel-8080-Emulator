#include "i8080.h"

//T-states for every 8-bit opcode
unsigned char clock_cycles8080[] = {
    4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4, //0x00..0x0F
    4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4, //0x10..0x1F
    4, 10, 16, 5, 5, 5, 7, 4, 4, 10, 16, 5, 5, 5 ,7, 4, //0x20..0x2F
    4, 10, 13, 5, 10, 10, 10, 4, 4, 10, 13, 5, 5, 5, 7, 4, //0x30..0x3F

    5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5, //0x40..0x4F
    5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5, //0x50..0x5F
    5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5, //0x60..0x6F
    7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 7, 5, //0x70..0x7F

    4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, //0x80..0x8F
    4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, //0x90..0x9F
    4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, //0xA0..0xAF
    4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, //0xB0..0xBF

    11, 10, 10, 10, 17, 11, 7, 11, 11, 10, 10, 10, 17, 17, 7, 11, //0xC0..0xCF
    11, 10, 10, 10, 17, 11, 7, 11, 11, 10, 10, 10, 17, 17, 7, 11, //0xD0..0xDF
    11, 10, 10, 18, 17, 11, 7, 11, 11, 5, 10, 4, 17, 17, 7, 11, //0xE0..0xEF
    11, 10, 10, 4, 17, 11, 7, 11, 11, 5, 10, 4, 17, 17, 7, 11, //0xF0..0xFF
};

int parity(int x, int size){
    int parity_count = 0;
    for(int i = 0; i < size; i++){
        parity_count += x & 1; //masks x with 1 to check LSB and increase by 1 or 0
        x = x >> 1; //shift right by 1 bit, so the next bit moves into the LSB position
    }
    return (parity_count % 2 == 0); //returns 1 for even and 0 for odd
}

void update_logic_flags(State8080 *state){
    state->SF.cy = state->SF.ac = 0;
    state->SF.zf = (state->A == 0);
    state->SF.s = (0x80 == (state->A & 0x80));
    state->SF.pf = parity(state->A, 8);
}

void update_arithmetic_flags(State8080 *state, uint16_t result){
    state->SF.zf = ((result & 0xff) == 0); //checks whether the result is 0 or not.
    state->SF.s = (0x80 == (result & 0x80)); //checks the MSB of the result to set the sign flag
    state->SF.pf = parity(result & 0xff, 8); //checks parity over the 8-bit result
    state->SF.ac = (result > 0x09); //compares result against 0x09 (9 in decimal)
}

void bcd_arithmetic_flags(State8080 *state, uint16_t result){
    state->SF.cy = ((result & 0x100) != 0); //sets the carry flag to 1 if any carry is generated at the 8th bit(9th-bit position)
    state->SF.zf = ((result & 0xff) == 0);
    state->SF.s = (0x80 == (result & 0x80));
    state->SF.pf = parity(result & 0xff, 8);
    state->SF.ac = (result > 0x90);
}

void unimplemented_instruction(State8080 *state){
    printf("Error: Unimplemented instruction\n");
    state->PC--;
    decode8080(state->memory, state->PC);
    printf("\n");
    exit(1);
}

int emulate(State8080 *state, bool printTrace){

}

//reads raw bytes from codebuffer stating at address pc, prints the memory add and readable 8080 assembly mnemonic
int decode8080(unsigned char *codebuffer, int pc){
    unsigned char *code = &codebuffer[pc];
    int opbytes = 1;
    printf("%04x", pc);
    switch(*code){
        //0x00..0x0F
        case 0x00:
            prinf("NOP");
            break;
        case 0x01:
            printf("LXI B,#$%02x%02x", code[2], code[1]); //little endian that's why first code[2] then code[1] to convert into big endian
            opbytes = 3;
            break;
        case 0x02:
            printf("STAX B");
            break;
        case 0x03:
            printf("INX B");
            break;
        case 0x04:
            printf("INR B");
            break;
        case 0x05:
            printf("DCR B");
            break;
        case 0x06:
            printf("MVI B, #$%02x", code[1]);
            opbytes = 2;
            break;
        case 0x07:
            printf("RLC");
            break;
        case 0x08:
            printf("NOP");
            break;
        case 0x09:
            printf("DAD B");
            break;
        case 0x0A:
            printf("LDAX B");
            break;
        case 0x0B:
            printf("DCX B");
            break;
        case 0x0C:
            printf("INR C");
            break;
        case 0x0D:
            printf("DCR C");
            break;
        case 0x0E:
            printf("MVI C, #$%02x", code[1]);
            opbytes = 2;
            break;
        case 0x0F:
            printf("RRC");
            break;
        
        //0x10..0x1F
        case 0x10:
            printf("NOP");
            break;
        case 0x11:
            printf("LXI D, #$%02x%02x", code[2], code[1]);
            opbytes = 3;
            break;
        case 0x12:
            printf("STAX D");
            break;
        case 0x13:
            printf("INX D");
            break;
        case 0x14:
            printf("INR D");
            break;
        case 0x15:
            printf("DCR D");
            break;
        case 0x16:
            printf("MVI D, #$%02x", code[1]);
            opbytes = 2;
            break;
        case 0x17:
            printf("RAL");
            break;
        case 0x18:
            printf("NOP");
            break;
        case 0x19:
            printf("DAD H");
            break;
        case 0x1A:
            printf("LDAX D");
            break;
        case 0x1B:
            printf("DCX D");
            break;
        case 0x1C:
            printf("INR E");
            break;
        case 0x1D:
            printf("DCR E");
            break;
        case 0x1E:
            printf("MVI E, #$%02x", code[1]);
            opbytes = 2;
            break;
        case 0x1F:
            printf("RAR");
            break;

        //[]TODO
        //0x20..0x2F
        case 0x20:
            printf("NOP");
            break;
        case 0x21:
            printf("LXI D, #$%02x%02x", code[2], code[1]);
            opbytes = 3;
            break;
        case 0x22:
            printf("SHLD $%02x%02x", code[2], code[1]);
            opbytes = 3;
            break; 
        case 0x23:
            printf("INX H");
            break;
        case 0x24:
            printf("INR H"); 
            break;
        case 0x25:
            printf("DCR H");
            break;
        case 0x26:
            printf("MVI H, #$%02x", code[1]);
            opbytes = 2;
            break;
        case 0x27:
            printf("DAA");
            break;
        case 0x28:
            printf("NOP");
            break;
        case 0x29:
            printf("DAD H");
            break;
        case 0x2A:
            printf("LHLD, $%02x%02x", code[2], code[1]);
            opbytes = 3;
            break;
        case 0x2B:
            printf("DCX H");
            break;
        case 0x2C:
            printf("INR L");
            break;
        case 0x2D:
            printf("DCR L");
            break;
        case 0x2E:
            printf("MVI L, #$%02x", code[1]);
            opbytes = 2;
            break;
        case 0x2F:
            printf("CMA");
            break;

        //0x30..0x3F
    }
}

void genInterrupt(State8080 *state, int interrupt_num){

}

void read_file_into_memory(State8080 *state, char *filename, uint32_t offset){

}

State8080 *init8080(void){

}