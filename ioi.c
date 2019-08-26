#include <stdio.h>
#include <stdlib.h>

#include "ioi.h"
#include "micro.h"

void print(m_registers *regs, byte *mem)
{
  int i;
  for(i = regs->ar; i < regs->ar + regs->br; i++) {
    putchar(mem[i]);
  }
}

void read(m_registers *regs, byte *mem)
{
  char *buffer = (char*) malloc(regs->br);
  fgets(buffer, regs->br, stdin);
  int i;
  for(i = 0; i < regs->br; i++) {
    mem[regs->ar + i] = buffer[i];
  }
  free(buffer);
}

void keyboard(m_registers *regs)
{
  regs->ar = getchar();
}
