#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "asmhelper.h"
#include "asmc.h"

varstore *vars;
int var_count;

int main(int argc, char **argv)
{
  init_globals();
  
  long input_file_size, input_file_size_actual;
  char *buffer, *token_line, *token;

  FILE *input_file;
  FILE *output_file;
  
  /*if(!init_file_pointers(&input_file, &output_file, argc, argv)) {
    return 1;
    }*/

  if(argc < 3) {
    printf("Not enough arguments\n");
    return 1;
  }

  input_file = fopen(argv[1], "r");
  output_file = fopen(argv[2], "wb");

  if (!input_file || !output_file) {
    printf("Error opening files");
    return 1;
  }


  /* process file line by line */
  char *line, *tofree;
  fseek (input_file , 0 , SEEK_END);
  input_file_size = ftell (input_file);
  rewind (input_file);

  buffer = malloc(input_file_size);
  input_file_size_actual = fread (buffer, 1, input_file_size, input_file);

  tofree = strdup(buffer);
  
  while ((line = strsep(&buffer, "\n")) != NULL) {
    printf("%s\n", line);
  }

  fclose(input_file);
  fclose(output_file);
  free(tofree);
  free(buffer);
}

void init_globals()
{
  vars = malloc(0);
  var_count = 0;
}

void setvar(char *varname, byte value)
{
  
}

byte getvar(char *varname)
{

}
