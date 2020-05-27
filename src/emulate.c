#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

//Main assumptions about the emulator/ ARM
#define MEMORY_CAPACITY 65536
#define NO_REGISTERS 17
#define ADDRESS_SIZE 4
#define BITS_SET(value, mask, bits) ((value &mask) == bits)

//constants for masks
// Constants for Multiply
#define SDTI_I_MASK 0x02000000
#define SDTI_P_MASK 0x01000000
#define SDTI_U_MASK 0x00800000
#define SDTI_L_MASK 0x00100000
#define SDTI_RN_MASK 0x000F0000
#define SDTI_RD_MASK 0x0000F000
#define SDTI_OFFSET_MASK 0x00000FFF
#define SDTI_I_SHIFT 25
#define SDTI_P_SHIFT 24
#define SDTI_U_SHIFT 23
#define SDTI_L_SHIFT 20
#define SDTI_RN_SHIFT 16
#define SDTI_RD_SHIFT 12


//an unassigned 32 bit int for a word
typedef uint32_t word;

enum Register {PC =15, CPSR = 16};

//condition suffixes for their codes
enum Cond {EQ, NE, GE=10, LT, GT, LE, AL};

typedef struct{
    //ARM machine memory
    uint8_t *memory;
    //0-12 general purpose , 13 SP, 14 LR, 15 PCM 16 CPSR
    word * registers;
}arm;

// CHECK IF CORRECTLY ASSIGNED MEMORY
void ptrValidate(const void * pointer, char * error){
    if (pointer == NULL){
        printf("Error: %s\n", error);
        exit(EXIT_FAILURE);
    }
}

// function to check conditions
//Parameter 1: code for Cond
//Parameter 2: current state of arm
bool checkCond(word instruction, arm state ){
    //CPSR FLAG BITS - WE USE 1,2,4,8 to extract the 1,2,3,4th bit in the cpsr address
    unsigned int n = (state.registers[CPSR] & 0x80000000) >> 31;
    unsigned int z = (state.registers[CPSR] & 0x40000000) >> 30;
    unsigned int c = (state.registers[CPSR] & 0x20000000) >> 29;
    unsigned int v = (state.registers[CPSR] & 0x10000000) >> 28;
    unsigned int cond = instruction >> 28;


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


void sdti(arm state, word instruction) {
    if (!checkCond(instruction & 0xF0000000, state)){
      return;
    }
    // parts of the instruction
    unsigned int i = (instruction & SDTI_I_MASK) >> SDTI_I_SHIFT;
    unsigned int p = (instruction & SDTI_P_MASK) >> SDTI_P_SHIFT;
    unsigned int u = (instruction & SDTI_U_MASK) >> SDTI_U_SHIFT;
    unsigned int l = (instruction & SDTI_L_MASK) >> SDTI_L_SHIFT;
    unsigned int rn = (instruction & SDTI_RN_MASK) >> SDTI_RN_SHIFT;
    unsigned int rd = (instruction & SDTI_RD_MASK) >> SDTI_RD_SHIFT;
    unsigned int offset = (instruction & SDTI_OFFSET_MASK);

    //Immediate Offset
    if(i){
      //offset is interpreted as a shifted register
    } else {
      // offset is interpreted as an immediate offset
    }
    //p doesn't change contents of base register for this exercise
    if(p){
      // if flag is set then (pre-Indexing) and simply transfer data

    } else{

      if(u){
        //offset is added to base register if u is set
        rn += offset;

      } else {
        // subtract offset from base register
        rn -= offset;
      }
      //trasfer data
    }

    if(l){
      //word is loaded from memory
      // word ldr is the word stored within the source register rd
      //TODO: find way to load word from source register into variable ldr
    } else {
      //word str is the word from the updated base register rn
      // this word is stored within the destination/source register rd
    }
}

void decode(arm state, word instruction) {
  const word dpMask = 0x0C000000;
  const word dp = 0x00000000;
  const word multMask = 0x0FC000F0;
  const word mult = 0x0000090;
  const word sdtMask = 0x0C600000;
  const word sdt = 0x04000000;
  const word branchMask = 0x0F000000;
  const word branch = 0x0A000000;

  // TODO: determine how to differentiate ...
  // ... `data processing` from `multiply`

  if (BITS_SET(instruction, branchMask, branch)) {
    // function for branch instructions
  } else if (BITS_SET(instruction, sdtMask, sdt)) {
    // function for single data tranfser instructions
  } else if (BITS_SET(instruction, multMask, mult)) {
    // function for multiply instructions
  } else if (BITS_SET(instruction, dpMask, dp)) {
    // function for data processing instructions
  }
}

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("Please specify an ARM binary object code file.\n");
    exit(EXIT_FAILURE);
  }
  arm *state;

  // free memory before code termination
  free(state->memory);
  free(state->registers);
  free(state);
  return EXIT_SUCCESS;
}




