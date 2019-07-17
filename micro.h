#ifndef MICRO_H
#define MICRO_H

#include <stdint.h>

struct s_m_registers {
  uint8_t ps;
  uint16_t ar;
  uint16_t br;
  uint16_t cr;
  uint16_t dr;
};

typedef struct s_m_registers m_registers;

#endif
