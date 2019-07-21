#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "loader.h"
#include "micro.h"

void load_memory_from_file(byte *mem, char *file, m_state *state)
{
  size_t program_file_size;
  FILE *program_file = fopen(file, "rb");
  byte *buffer;
  
  fseek (program_file, 0, SEEK_END);
  program_file_size = ftell (program_file);
  rewind (program_file);
  buffer = malloc(program_file_size);

  fread (buffer, 1, program_file_size, program_file);

  memcpy(mem, buffer, program_file_size);
  state->program_size = program_file_size;
  free (buffer);
  fclose (program_file);
}
