#include "instr.h"

#include "micro.h"

#include <stdio.h>


void exec_instr(m_registers *regs, byte *mem, m_state *state)
{
  byte instr = mem[regs->ps];
  byte code = mem[regs->ps + 1];
  uint16_t value = mem[regs->ps + 2]  + (mem[regs->ps + 3]) * 0x100;
  
  switch(instr) {
  case INTE: // Interput
    intr(code, value, regs, state, mem);
    break;
  case MOVR:
    write_register(regs, code, value);
    break;
  case MOVM:
    mem[value] = read_register(regs, code);
    break;
  }
  
  regs->ps+=4;
  
  if (state->running == true && regs->ps >= state->program_size) {
    state->error = ERR_PROG_COUNT_OVERFLOW;
    state->running = false;
  }
}


void intr(byte code, uint16_t value, m_registers *regs, m_state *state, byte *mem)
{
  int i;

  switch(code) {
  case EXIT:
    state->running = false;
    break;
  case PRNT:
    for(i = regs->ar; i < regs->ar + regs->br; i++) {
      putchar(mem[i]);
    }
    break;
  }
}

void addi(m_registers *regs)
{
  regs->ar = regs->ar + regs->br;
}

void subt(m_registers *regs)
{

}


void mult(m_registers *regs)
{

}

void divi(m_registers *regs)
{

}
