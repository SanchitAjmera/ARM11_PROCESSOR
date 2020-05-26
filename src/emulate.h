#ifndef EMULATE_H
#define EMULATE_H

typedef uint32_t Word;
typedef uint8_t Byte;

struct arm {
	Byte* memory;

	/* 0-12 general purpose, 13 SP, 14 LR, 15 PC, 16 CPSR */
	Word* registers;
};

extern void check_ptr(const void* ptr, char* error_msg);


/* Takes in the ARM binary file's name and returns an ARM state pointer with memory and register
 * pointers on heap, where memory is of size MEM_LIMIT bytes */
extern void init_arm(struct arm* state, char* fname);

#endif
