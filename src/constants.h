// general constants
#define MEMORY_CAPACITY 65536
#define NO_REGISTERS 17
#define ADDRESS_SIZE 4
#define BITS_SET(value, mask, bits) ((value & mask) == bits)

// Constants for Multiply
#define MULT_RDEST_MASK 0x000F0000
#define MULT_REG_S_MASK 0x00000F00
#define MULT_REG_M_MASK 0x0000000F
#define MULT_REG_N_MASK 0x0000F000
#define MULT_REG_S_SHIFT 8
#define MULT_REG_N_SHIFT 12
#define MULT_RDEST_SHIFT 16
#define ACCUMULATE_FLAG (1 << 21)
#define UPDATE_CPSR (1 << 20)
#define CPSR_N (1 << 31)
#define CPSR_Z (1 << 30)
