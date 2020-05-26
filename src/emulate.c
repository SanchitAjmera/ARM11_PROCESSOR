#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MEMORY_CAPACITY = 65536
#define NO_REGISTERS = 17
#define ADDRESS_SIZE = 4
#define BITS_SET(value, mask, bits) ((values &mask) == bits)

//an unassigned 32 bit int for a word
typedef uint32_t word;

enum Register {PC =15, CPSR = 16};

//condition suffixes for their codes
enum Cond {EQ, NE GE=10, LT, GT, LE, AL};

typedef struct{
    //ARM machine memory
    uint8_t *memory;
    //0-12 general purpose , 13 SP, 14 LR, 15 PCM 16 CPSR
    word * registers;
}arm;

// CHECK IF CORRECTLY ASSIGNED MEMORY
void ptrValidate(const void * pointer, char * error){
    if (pointer == null){
        printf("Error: %s\n", error);
        exit(EXIT_FAILURE);
    }
}

// function to check conditions
//Parameter 1: code for Cond
//Parameter 2: current state of arm
bool checkCondition(unsigned int cond, arm state ){
    //CPSR FLAG BITS - WE USE 1,2,4,8 to extract the 1,2,3,4th bit in the cpsr address
    unsigned int n = state.registers[CPSR] & 8;
    unsigned int z = state.registers[CPSR] & 4;
    unsigned int c = state.registers[CPSR] & 2;
    unsigned int v = state.registers[CPSR] & 1;

    // conditions for instruction

    switch(cond){
        case EQ:
            return z;
        case NE:
            return !z;
        case GE:
            return n ==v;
        case LT:
            return n !=v;
        case GT:
            return !z && (n==v);
        case LE:
            return z || (n != v);;
        case AL:
            return true;
        default:
            return false;
    }
}

void sdti(arm state. word instruction) {
    if (!checkCond(instruction & 0xF0000000, state)){
        return;
    }
    // parts of the instruction
    unsigned int i = instruction & 0x02000000;
    unsigned int p = instruction & 0x01000000;
    unsigned int u = instruction & 0x00800000;
    unsigned int l = instruction & 0x00100000;
    unsigned int rn = instruction & 0x000F0000;
    unsigned int rd = instruction & 0x0000F000;
    unsigned int offset = instruction & 0x00000FFF;

    //Immediate Offset
    if(i){
        //offset is interpreted as a shifted register
    } else {
        // offset is interpreted as an immediate offset
    }
    //p doesn't change contents of base register for this exercise
    if(p){
        //add/subtract offset to the base register before transferring the data
    } else{
        //add/subtract offset to the base register after transferring
    }
    if(u){
        //add offset to base register
    } else{
        // subtract offset from base register
    }
    if(l){
        //word is loaded from memory
    } else {
        //word is stored in memory
    }
}
