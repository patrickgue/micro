#include <stdio.h>

#include "ioh.h"
#include "micro.h"

void print(m_registers *regs) {
  putchar(regs->ar);
}

void keyboard(m_registers *regs) {
  regs->ar = getchar();
}
