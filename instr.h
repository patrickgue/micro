#ifndef INSTR_H
#define INSTR_H

#include "micro.h"

void exec_instr(m_registers*, byte*, m_state*);


void addi(m_registers *regs);
void subt(m_registers *regs);
void mult(m_registers *regs);
void divi(m_registers *regs);

void intr(byte code, uint16_t value, m_registers *regs, m_state *state, byte *memory);

void jump(m_registers *regs, uint16_t value);
void skip_if_equal_zero(m_registers *regs);
void skip_if_not_equal_zero(m_registers *regs);

#endif
