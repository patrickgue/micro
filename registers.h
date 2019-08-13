#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

#include "micro.h"

uint16_t read_register(m_registers*, uint8_t);
void write_register(m_registers*, uint8_t, uint16_t);

#endif
