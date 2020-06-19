#ifndef EMULATE_FETCH_H
#define EMULATE_FETCH_H

extern word getWord(byte *startAddress, bool isBigEndian);
extern void fetch(arm_t *state);
/* Takes in the ARM binary file's name and returns an ARM state pointer with
 * memory and register
 * pointers on heap, where memory is of size MEMORY_CAPACITY */
extern void initArm(arm_t *state, const char *fname);

#endif
