#ifndef EMULATE_FETCH_H
#define EMULATE_FETCH_H

extern word getWord(byte *start_addr, bool isBigEndian);
extern void fetch(arm_t *state);

#endif
