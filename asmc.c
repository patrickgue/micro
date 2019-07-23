#include <stdio.h>

#include "asmhelper.h"

int main(int argc, char **argv)
{
  long input_file_size;
  size_t input_file_size_actual;
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
}
