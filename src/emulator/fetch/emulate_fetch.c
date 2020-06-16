#inlcude "../emulate_constants.h"
#include "../emulate_util.h"
#inlcude "../../common/constants.h"
#inlcude "../../common/util.h"

word getWord(byte *startAddress, bool isBigEndian) {
  word w = 0;
  for (int i = 0; i < WORD_SIZE_BYTES; i++) {
    uint byte_index = isBigEndian ? (WORD_SIZE_BYTES - 1 - i) : i;
    w += startAddress[i] << BYTE * byte_index;
  }
  return w;
}

void fetch(arm_t *state) {
  word memoryOffset = state->registers[PC];
  state->registers[PC] += WORD_SIZE_BYTES;
  state->fetched = getWord(state->memory + memoryOffset, NOT_BIG_ENDIAN);
}
