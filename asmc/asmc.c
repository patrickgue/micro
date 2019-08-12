#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "asmc.h"
#define DEBUG 1

varstore **vars;
int var_count;
label_store *labels;
int label_count;
uint16_t heap_offset, label_offset;

int main(int argc, char **argv)
{
  init_globals();
  
  long input_file_size, input_file_size_actual, memory_position = 0;
  char *buffer, *token;
  
  FILE *input_file;
  FILE *output_file;
  
  if(argc < 3) {
    printf("Not enough arguments\n");
    return 1;
  }

  input_file = fopen(argv[1], "r");
  output_file = fopen(argv[2], "w");

  if (!input_file || !output_file) {
    printf("Error opening files");
    return 1;
  }


  /* process file line by line */
  char *line, *tofree, *asms_line;
  fseek (input_file , 0 , SEEK_END);
  input_file_size = ftell (input_file);
  rewind (input_file);

  // offsets: approximations. TODO: calculate actual offset
  label_offset = input_file_size * 2;
  heap_offset = label_offset + (256 * 4); // allow 0xff labels

  buffer = malloc(input_file_size);
  input_file_size_actual = fread (buffer, 1, input_file_size, input_file);
  fclose(input_file);
  
  tofree = strdup(buffer);
  
  while ((line = strsep(&buffer, "\n")) != NULL) {
    instr_tokens *token = parse_instr_token(line);
    printf("%s | %s | %s\n",*token->instr, *token->arg1, *token->arg2);
    if(strcmp(*token->instr, "end") == 0) {
      asms_line = malloc(4 * 5 * sizeof(char));
      sprintf(asms_line, "0x%02x 0x%02x 0x00 0x00\n", INTE, EXIT);
      fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
      free(asms_line);
      memory_position += 4;
    }
    else if (strcmp(*token->instr, "print") == 0) {
      asms_line = malloc(3 * 5 * sizeof(char));
      varstore *var = getvar(*token->arg1);
      sprintf(asms_line, "0x%02x 0x%02x 0x%02x 0x%02x\n", MOVR, AR, var->memory_location & 0xff, var->memory_location >> 8);
      sprintf(asms_line, "%s0x%02x 0x%02x 0x%02x 0x%02x\n", asms_line, MOVR, BR, var->size & 0xff, var->size >> 8);
      sprintf(asms_line, "%s0x%02x 0x%02x 0x%02x 0x%02x\n", asms_line, INTE, PRNT, 0, 0);
      fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
      free(asms_line);
      memory_position += 4 * 3;
    }
    else if (strcmp(*token->instr, "var") == 0) {
      asms_line = malloc(2 * 5 * 4 * sizeof(char));
      char *varname = malloc(64 * sizeof(char));
      strcpy(varname, *token->arg1);
      uint16_t number = atoi(*token->arg2);
      uint16_t memloc = setvar(varname, 1);
      uint8_t num_lower = number & 0xff;
      uint8_t num_higher = number >> 8;
      uint8_t mem_lower = memloc & 0xff;
      uint8_t mem_higher = memloc >> 8;
      sprintf(asms_line, "0x%02x 0x%02x 0x%02x 0x%02x\n", MOVR, AR, num_lower, num_higher);
      sprintf(asms_line, "%s0x%02x 0x%02x 0x%02x 0x%02x\n", asms_line, MOVM, AR, mem_lower, mem_higher);
      fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
      free(asms_line);
      memory_position += 4 * 2;
    }
    else if (strcmp(*token->instr, "str") == 0) {
      int strsize = strlen(*token->arg2) - 1, i;
      char *varname = malloc(64 * sizeof(char));
      strcpy(varname, *token->arg1);
      uint16_t memloc = setvar(varname, strsize);
      uint8_t mem_lower = memloc & 0xff;
      uint8_t mem_higher = memloc >> 8;
      
      for(i = 1; i < strsize; i++) {
	asms_line = malloc(sizeof(char) * (2 * 5 * 4 + 1));
	uint16_t number = (*token->arg2)[i];
	uint8_t num_lower = number & 0xff;
	uint8_t num_higher = number >> 8;
	memloc++;
	uint8_t mem_lower = memloc & 0xff;
	uint8_t mem_higher = memloc >> 8;
      
	sprintf(asms_line, "0x%02x 0x%02x 0x%02x 0x%02x\n", MOVR, AR, num_lower, num_higher);
	sprintf(asms_line, "%s0x%02x 0x%02x 0x%02x 0x%02x\n", asms_line, MOVM, AR, mem_lower, mem_higher);
	fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
	free(asms_line);
	memory_position += 4 * 2;
      }
    }
    else if (strcmp(*token->instr, "label") == 0) {
      addlabel(*token->arg1, memory_position);
    }
    else if (strcmp(*token->instr, "jump") == 0) {
      asms_line = malloc(sizeof(char) * (5 * 4 + 1));
      uint16_t memloc = getlabel(*token->arg1);
      uint8_t mem_lower = memloc & 0xff;
      uint8_t mem_higher = memloc >> 8;
      sprintf(asms_line, "0x%02x 0x00 0x%02x 0x%02x\n", JUMP, mem_lower, mem_higher);
      fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
      free(asms_line);
      memory_position += 4;
    }
    /*else if (strcmp(*token->instr, "skipeqzero") == 0) {
      asms_line = malloc(sizeof(char) * (5 * 4 + 1));
      sprintf(asms_line, "0x%02x 0x00 0x%02x 0x%02x\n", );
      fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
      free(asms_line);
      }*/
    else if (strcmp(*token->instr, ";") == 0) {
      // comment
    }
  }

  int label_index, to_offset_index;
  for(to_offset_index = memory_position; to_offset_index < label_offset; to_offset_index += 4) {
    asms_line = malloc(sizeof(char) * (5 * 4));
    strcpy(asms_line, "0x00 0x00 0x00 0x00\n");
    fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
    free(asms_line);
  }
  for(label_index = 0; label_index < label_count; label_index++) {
    asms_line = malloc(sizeof(char) * (5 * 4));
    uint16_t memloc = labels[label_index].memory_location;
    uint8_t mem_lower = memloc & 0xff;
    uint8_t mem_higher = memloc >> 8;
    sprintf(asms_line, "0x%02x 0x00 0x%02x 0x%02x\n", JUMP, mem_lower, mem_higher);
    fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
    free(asms_line);
    #ifdef DEBUG
    printf("Label %-12s: mem: 0x%04x ref: 0x%04x\n", labels[label_index].labelname, labels[label_index].memory_location, labels[label_index].reference_location);
    #endif
  }

  fclose(output_file);
  free(tofree);
  free(buffer);

  #ifdef DEBUG
  int j;
  for(j = 0; j < var_count; j++) {
    printf("%-12s memloc: %02x size: %02x\n", vars[j]->varname, vars[j]->memory_location, vars[j]->size);
  }
  #endif
  
  return 0;
}

void init_globals()
{
  vars = malloc(0);
  var_count = 0;
  labels = malloc(0);
  label_count = 0;
}

uint16_t setvar(char *varname, uint16_t size)
{
  uint16_t memloc;
  int i;
  for (i = 0; i < var_count; i++) {
    memloc += vars[i]->size;
  }

  memloc += heap_offset;

  vars = realloc(vars, sizeof(varstore) * (var_count + 1));
  vars[var_count] = (varstore*) malloc(sizeof(varstore));
  strcpy(vars[var_count]->varname, varname);
  vars[var_count]->memory_location = memloc;
  vars[var_count]->size = size;
  var_count++;
  return memloc;
}

varstore *getvar(char *varname)
{
  int i;
  for (i = 0; i < var_count; i++) {
    if (strcmp(varname, vars[i]->varname) == 0) {
      return vars[i];
    }
  }
  return NULL;
}


instr_tokens *parse_instr_token(char *line)
{
  instr_tokens *tokens = malloc(sizeof(instr_tokens));
  char *buffer, *tofree;
  buffer = tofree = strdup(line);
  *tokens->instr = strdup(strsep(&buffer, " "));
  char *arg1tmpval = strsep(&buffer, " ");
  *tokens->arg1 = strdup(arg1tmpval ? arg1tmpval : "");
  *tokens->arg2 = strdup(buffer ? buffer : "");
  free(tofree);
  return tokens;
}

uint16_t addlabel(char *labelname, uint16_t location)
{
  /* if label was already used in jump instruction */
  int i;
  for (i = 0; i < label_count; i++) {
    if (strcmp(labels[i].labelname, labelname) == 0) {
      labels[i].memory_location = location;
      return labels[i].reference_location;
    }
  }
  /* if this is a new label to be jumped to in the future */
  labels = realloc(labels, sizeof(label_store) * (label_count + 1));
  strcpy(labels[label_count].labelname, labelname);
  labels[label_count].memory_location = location;
  labels[label_count].reference_location = label_offset + (label_count -1) * 4 * sizeof(uint8_t) + 6;
  return labels[label_count++].reference_location;
}

uint16_t getlabel(char *labelname)
{
  int i;
  for (i = 0; i < label_count; i++) {
    if (strcmp(labels[i].labelname, labelname) == 0) {
      return labels[i].reference_location;
    }
  }
  /* if no label was found, add it */
  return addlabel(labelname, 0x00);
}
