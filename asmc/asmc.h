#ifndef ASMC_H
#define ASMC_H

#include <stdint.h>

#include "../micro.h"

struct s_varstore {
  char varname[64];
  uint16_t memory_location;
  uint16_t size;
};

typedef struct s_varstore varstore;

struct s_instr_tokens {
  char *instr[64];
  char *arg1[64];
  char *arg2[64];
};

typedef struct s_instr_tokens instr_tokens;

void init_globals();

uint16_t setvar(char *varname, uint16_t size);
varstore *getvar(char *varname);
instr_tokens *parse_instr_token(char *line);

#endif
