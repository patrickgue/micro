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
  
  int mem_size = 0x10000;

  m_registers *regs = malloc(sizeof(m_registers));
  m_state *state = malloc(sizeof(m_state));
  byte *mem = (byte*) malloc(sizeof(byte) * mem_size);

  init_regs(regs);
  init_state(state);

  load_memory_from_file(mem, argv[1], state);

  /* enter main loop */
  while (state->running) {
    exec_instr(regs, mem, state);
    //printf("--\nps: %2x flags: %d\n%2x %2x %2x %2x\n%2x %2x %2x %2x\n--\n", regs->ps, regs->fg, regs->ar, regs->br, regs->cr, regs->dr, regs->er, regs->fr, regs->gr, regs->hr);
  }

  if (state->error != 0) {
    printf("Ended with an error: %d\n", state->error);
  }

  #ifdef DEBUG
  FILE *f = fopen("/tmp/memory.bin", "wb");
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

uint16_t read_register(m_registers *regs, uint8_t regi)
{
  switch(regi) {
    /* full 16 bit */
  case AR:
    return regs->ar;
    break;
  case BR:
    return regs->br;
    break;
  case CR:
    return regs->cr;
    break;
  case DR:
    return regs->dr;
    break;
  case ER:
    return regs->er;
    break;
  case FR:
    return regs->fr;
    break;
  case GR:
    return regs->gr;
    break;
  case HR:
    return regs->hr;
    break;
    /* lower byte: */
  case AL:
    return regs->ar & 0xff;
    break;
  case BL:
    return regs->br & 0xff;
    break;
  case CL:
    return regs->cr & 0xff;
    break;
  case DL:
    return regs->dr & 0xff;
    break;
  case EL:
    return regs->er & 0xff;
    break;
  case FL:
    return regs->fr & 0xff;
    break;
  case GL:
    return regs->gr & 0xff;
    break;
  case HL:
    return regs->hr & 0xff;
    break;
    /* upper byte */
  case AU:
    return regs->ar >> 8;
    break;
  case BU:
    return regs->br >> 8;
    break;
  case CU:
    return regs->cr >> 8;
    break;
  case DU:
    return regs->dr >> 8;
    break;
  case EU:
    return regs->er >> 8;
    break;
  case FU:
    return regs->fr >> 8;
    break;
  case GU:
    return regs->gr >> 8;
    break;
  case HU:
    return regs->hr >> 8;
    break;
  default:
    return 0;
  }
}

void write_register(m_registers *regs, uint8_t regi, uint16_t value)
{
  switch(regi) {
    /* full 16 bit */
  case AR:
    regs->ar = value;
    break;
  case BR:
    regs->br = value;
    break;
  case CR:
    regs->cr = value;
    break;
  case DR:
    regs->dr = value;
    break;
  case ER:
    regs->er = value;
    break;
  case FR:
    regs->fr = value;
    break;
  case GR:
    regs->gr = value;
    break;
  case HR:
    regs->hr = value;
    break;
    /* lower byte: */
  case AL:
    regs->ar = value & 0xff;
    break;
  case BL:
    regs->br = value & 0xff;
    break;
  case CL:
    regs->cr = value & 0xff;
    break;
  case DL:
    regs->dr = value & 0xff;
    break;
  case EL:
    regs->er = value & 0xff;
    break;
  case FL:
    regs->fr = value & 0xff;
    break;
  case GL:
    regs->gr = value & 0xff;
    break;
  case HL:
    regs->hr = value & 0xff;
    break;
    /* upper byte */
  case AU:
    regs->ar = value >> 8;
    break;
  case BU:
    regs->br = value >> 8;
    break;
  case CU:
    regs->cr = value >> 8;
    break;
  case DU:
    regs->dr = value >> 8;
    break;
  case EU:
    regs->er = value >> 8;
    break;
  case FU:
    regs->fr = value >> 8;
    break;
  case GU:
    regs->gr = value >> 8;
    break;
  case HU:
    regs->hr = value >> 8;
    break;
  }
}
