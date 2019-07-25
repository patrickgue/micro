#ifndef ASMC_H
#define ASMC_H

#include <stdint.h>

#include "micro.h"

struct s_varstore {
  char varname[64];
  uint16_t memory_location;
};

typedef struct s_varstore varstore;

void setvar(char *varname, byte value);
byte getvar(char *varname);

#endif
