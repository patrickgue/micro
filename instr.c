#include "instr.h"

#include "micro.h"

#include <stdio.h>


void exec_instr(m_registers *regs, byte *mem, m_state *state)
{
  byte instr = mem[regs->ps];
  byte code = mem[regs->ps + 1];
  uint16_t value = mem[regs->ps + 2]  + (mem[regs->ps + 3]) * 0x100;
  
  printf("%0x %2x %4x\n", instr, code, value);


  switch(instr) {
  case INTE: // Interput
    intr(code, value, state);
    break;
  case MOVR:
    write_register(regs, code, value);
    break;
  }
  
  regs->ps+=4;
  
  if (state->running == true && regs->ps >= state->program_size) {
    state->error = ERR_PROG_COUNT_OVERFLOW;
    state->running = false;
  }
}


void intr(byte code, uint16_t value, m_state *state)
{
  printf("int %d\n", code);
  switch(code) {
  case EXIT:
    state->running = false;
    break;
  }
}
