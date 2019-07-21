#include <stdio.h>

#include "ioi.h"
#include "micro.h"

void print(m_registers *regs)
{
  putchar(regs->ar);
}

void keyboard(m_registers *regs)
{
  regs->ar = getchar();
}
