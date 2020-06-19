#ifndef EMULATE_EXECUTE_DPI_H
#define EMULATE_EXECUTE_DPI_H

#include "../emulate_util.h"

extern void setCPSR(arm_t *state, word result, uint carryOut);
extern void executeDPI(arm_t *state, dp_t *decoded);
extern operation_t *opImmediate(arm_t *state, uint op2);
extern operation_t *opRegister(arm_t *state, uint op2);

#endif
