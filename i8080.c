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

void UpdateLogicFlags(State8080 *state){
    state->SF.cy = state->SF.ac = 0;
    state->SF.zf = (state->A == 0);
    state->SF.s = (0x80 == (state->A & 0x80));
    state->SF.pf = parity(state->A, 8);
}

int emulate(State8080 *state, bool printTrace){

}

int decode8080(unsigned char *codebuffer, int pc){

}

void genInterrupt(State8080 *state, int interrupt_num){

}

void read_file_into_memory(State8080 *state, char *filename, uint32_t offset){

}

State8080 *init8080(void){

}