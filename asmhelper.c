#include "asmhelper.h"

bool init_file_pointers(FILE *input_file, FILE *output_file ,int argc, char **argv)
{
  if(argc < 3) {
    printf("Not enough arguments\n");
    return false;
  }

  input_file = fopen(argv[1], "r");
  output_file = fopen(argv[2], "wb");

  if (!input_file || !output_file) {
    printf("Error opening files");
    return false;
  }
  

  return true;
}
