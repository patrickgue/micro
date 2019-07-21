#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int main(int argc, char **argv)
{
  long input_file_size;
  size_t input_file_size_actual;
  char *buffer, *token_line, *token;
  
  if(argc < 3) {
    printf("Not enough arguments\n");
    return 1;
  }

  FILE *input_file =  fopen(argv[1], "r");
  FILE *output_file = fopen(argv[2], "wb");

  if (!input_file || !output_file) {
    printf("Error opening files");
  }
  
  fseek (input_file , 0 , SEEK_END);
  input_file_size = ftell (input_file);
  rewind (input_file);

  buffer = malloc(input_file_size);
  input_file_size_actual = fread (buffer, 1, input_file_size, input_file);

  while ((token_line = strsep(&buffer, "\n")) != NULL) {
    printf("\n%s\n", token_line);
    while ((token = strsep(&token_line, " ")) != NULL) {
      uint8_t intval = strtol(token, NULL, 16);
      printf("%i ", intval);
      unsigned char charval = intval;
      fwrite(&charval,1,1, output_file);
    }
  }

  fclose(output_file);
  fclose(input_file);
  
  
  return 0;
}
