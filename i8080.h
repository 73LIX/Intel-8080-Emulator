#include <stdint.h>
#include <stdbool.h>

/*status flags:
zf - zero flag
    set to 1 when result of an operation is 0

s - sign flag
    set to 1 when bit 7(MSB) of the result is 1. Representing a negative num in 2's complement.

pf - parity flag
    set 1 when the result has even parity(even number of 1 bits)

cy - carry flag
    set to 1 when carry is generated

ac - auxiliary carry flag
    set 1 when there is a carry or borrow out of bit 3 into bit 4(mainly for BCD)

pad - rest are just padding to fill out the remaining space
*/
typedef struct StatusFlags{
    uint8_t zf : 1; //declares a 1bit field (allocating only 1 bit of memory)
    uint8_t s : 1;
    uint8_t pf : 1;
    uint8_t cy : 1;
    uint8_t ac : 1;
    uint8_t pad : 3; //padding (3bits) unused bits that fill out the remaining space of the 8-bits 
} StatusFlags;

/* Registers
A - Accumulator
    The primary 8-bit accumulator

BC - register pair
DE - register pair
HL - register pair
    register pair that is often paired with instructions referencing memory
    used any time data is read or written

SP - stack pointer
    holds the 16-bit memory address of the top of the stack

PC - program counter
    holds the address of the next instruction to fetch and execute

Emulator level fields:
memory - a pointer to the emulated address space. On real hardware, memory lives on external RAM/ROM chips connected via the address and data buses rather than inside the CPUs register files.

interrupt_enable - tracks whether hardware interrupts are currently enabled(EI instruction sets it, DI instruction clears it). On the real 8080 this is an internal control latch.
*/
typedef struct State8080{
    uint8_t A;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t H;
    uint8_t L;
    uint16_t SP;
    uint16_t PC;
    uint8_t *memory;
    struct StatusFlags SF;
    uint8_t interrupt_enable;
} State8080;

//fetches, decodes and executes a single 8080 instruction
int emulate(State8080 *state, bool printTrace);

//Decodes one 8080 machine code instruction into readable assembly text
int decode8080(unsigned char *codebuffer, int pc);

//simulates a hardware interrupt for the 8080cpu
void genInterrupt(State8080 *state, int interrupt_num);

//loads rom or program butes from disk into the 8080's memory map at specified offset
void read_file_into_memory(State8080 *state, char *filename, uint32_t offset);

//allocate CPU struct
//allocate and initialize the emulated 8080 machine
State8080 *init8080(void);