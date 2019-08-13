#include <stdio.h>

#include "ioi.h"
#include "micro.h"

void print(m_registers *regs, byte *mem)
{
  int i;
  for(i = regs->ar; i < regs->ar + regs->br; i++) {
    putchar(mem[i]);
  }
}

void keyboard(m_registers *regs)
{
  regs->ar = getchar();
}
