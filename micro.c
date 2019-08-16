#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "micro.h"
#include "ioi.h"
#include "helper.h"
#include "instr.h"
#include "loader.h"

#define DEBUG 1

int main(int argc, char **argv)
{
  if (argc < 2) {
    printf("No program specified\nusage: micro [program]\n");
    return -1;
  }
  
  int mem_size = 0xffff;

  m_registers *regs = malloc(sizeof(m_registers));
  m_state *state = malloc(sizeof(m_state));
  byte *mem = (byte*) malloc(sizeof(byte) * mem_size);

  init_regs(regs);
  init_state(state);

  load_memory_from_file(mem, argv[1], state);

  /* enter main loop */
  while (state->running) {
    exec_instr(regs, mem, state);
  }

  /* check ending state */
  if (state->error != 0) {
    printf("Ended with an error: %d\n", state->error);
  }

  #ifdef DEBUG
  FILE *f = fopen("./memory.bin", "wb");
  fwrite(mem, sizeof(uint8_t), 256 * 256, f);
  fclose(f);
  #endif
  
  return 0;
}

void init_regs(m_registers* regs)
{
  regs->ps=0;
  regs->fg=0;
  
  regs->ar=0;
  regs->br=0;
  regs->cr=0;
  regs->dr=0;
  regs->er=0;
  regs->fr=0;
  regs->gr=0;
  regs->hr=0;
  regs->ar=0;
}

void init_state(m_state *state)
{
  state->running = true;
}
