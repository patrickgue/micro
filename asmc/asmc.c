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

  const char *asms_template = "0x%02x 0x%02x 0x%02x 0x%02x\n";
  const char *asms_template_add = "%s0x%02x 0x%02x 0x%02x 0x%02x\n";
  
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
  label_offset = (input_file_size * 2) - (input_file_size * 2) % 4;
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
      sprintf(asms_line, asms_template, INTE, EXIT, 0, 0);
      fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
      free(asms_line);
      memory_position += 4;
    }
    else if (strcmp(*token->instr, "print") == 0) {
      asms_line = malloc(3 * 5 * sizeof(char));
      varstore *var = getvar(*token->arg1);
      sprintf(asms_line, asms_template,
	      MOVR,
	      AR,
	      split_word_bytes(var->memory_location).lower,
	      split_word_bytes(var->memory_location).higher);
      
      sprintf(asms_line, asms_template_add, asms_line,
	      MOVR,
	      BR,
	      split_word_bytes(var->size).lower,
	      split_word_bytes(var->size).higher);
      
      sprintf(asms_line, asms_template_add, asms_line, INTE, PRNT, 0, 0);
      fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
      free(asms_line);
      memory_position += 4 * 3;
    }
    else if (strcmp(*token->instr, "var") == 0 ||
	     strcmp(*token->instr, "set") == 0) {
      asms_line = malloc(2 * 5 * 4 * sizeof(char));
      uint16_t memloc;
      char *varname = malloc(64 * sizeof(char));
      strcpy(varname, *token->arg1);
      uint16_t number = atoi(*token->arg2);
      if (strcmp(*token->instr, "var") == 0) {
	memloc = setvar(varname, 1);
      }
      else if (strcmp(*token->instr, "set") == 0) {
	memloc = getvar(varname)->memory_location;
      }
      sprintf(asms_line, asms_template,
	      MOVR,
	      AR,
	      split_word_bytes(number).lower,
	      split_word_bytes(number).higher);
      
      sprintf(asms_line, asms_template_add, asms_line,
	      MOVM,
	      AR,
	      split_word_bytes(memloc).lower,
	      split_word_bytes(memloc).higher);
      
      fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
      free(asms_line);
      memory_position += 4 * 2;
    }
    else if (strcmp(*token->instr, "str") == 0) {
      int strsize = strlen(*token->arg2) - 1, i;
      char *varname = malloc(64 * sizeof(char));
      strcpy(varname, *token->arg1);
      uint16_t memloc = setvar(varname, strsize);
      
      for(i = 1; i < strsize; i++) {
	asms_line = malloc(sizeof(char) * (2 * 5 * 4 + 1));
	uint16_t number = (*token->arg2)[i];
	memloc++;
      
	sprintf(asms_line, asms_template,
		MOVR,
		AR,
		split_word_bytes(number).lower,
		split_word_bytes(number).higher);
	
	sprintf(asms_line, asms_template_add, asms_line,
		MOVM,
		AR,
		split_word_bytes(memloc).lower,
		split_word_bytes(memloc).higher);

	fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
	free(asms_line);
	memory_position += 4 * 2;
      }
    }
    else if (strcmp(*token->instr, "arr") == 0) {
      char *varname = malloc(64 * sizeof(char));
      int arrsize = 1, i;
      for(i = 0; i < strlen(*token->arg2); i++) {
	if ((*token->arg2)[i] == ',') {
	  arrsize++;
	}
      }
      strcpy(varname, *token->arg1);
      uint16_t memloc = setvar(varname, arrsize);

      char *array_string = strdup(*token->arg2), *number_str;
      while( (number_str = strsep(&array_string, ",")) != NULL) {
	asms_line = malloc(sizeof(char) * (2 * 5 * 4 + 1));
	uint16_t number = atoi(number_str);
	memloc++;
      
	sprintf(asms_line, asms_template,
		MOVR,
		AR,
		split_word_bytes(number).lower,
		split_word_bytes(number).higher);
	
	sprintf(asms_line, asms_template_add, asms_line,
		MOVM,
		AR,
		split_word_bytes(memloc).lower,
		split_word_bytes(memloc).higher);

	fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
	free(asms_line);
	memory_position += 4 * 2;
      }
    }
    else if (strcmp(*token->instr, "mem") == 0) {
      asms_line = malloc(sizeof(char) * (2 * 5 * 4 + 1));
      char *arg2 = strdup(*token->arg2);
      char *varname = strdup(*token->arg1);
      uint16_t location = strtol(strsep(&arg2, " "), NULL, 16);
      uint16_t size = atol(strsep(&arg2, " "));      
      uint16_t memloc = setvar_raw(varname, location, size);
      free(asms_line);
      free(arg2);
    }
    else if (strcmp(*token->instr, "addn") == 0 ||
	     strcmp(*token->instr, "subn") == 0 ||
	     strcmp(*token->instr, "muln") == 0 ||
	     strcmp(*token->instr, "divn") == 0) {
      uint8_t instruction;

      if(strcmp(*token->instr, "addn") == 0)      { instruction = ADDI; }
      else if(strcmp(*token->instr, "subn") == 0) { instruction = SUBT; }
      else if(strcmp(*token->instr, "muln") == 0) { instruction = MULT; }
      else if(strcmp(*token->instr, "divn") == 0) { instruction = DIVI; }
      
      asms_line = malloc(sizeof(char) * (4 * 5 * 4 + 1));
      uint16_t memloc = getvar(*token->arg1)->memory_location;
      uint16_t number = atoi(*token->arg2);
      sprintf(asms_line, asms_template,
	      LOAD,
	      AR,
	      split_word_bytes(memloc).lower,
	      split_word_bytes(memloc).higher);
      
      sprintf(asms_line, asms_template_add, asms_line,
	      MOVR,
	      BR,
	      split_word_bytes(number).lower,
	      split_word_bytes(number).higher);
      
      sprintf(asms_line, "%s0x%02x 0x00 0x00 0x00\n", asms_line,
	      instruction, 0, 0, 0);
      
      sprintf(asms_line, asms_template_add, asms_line,
	      MOVM,
	      AR,
	      split_word_bytes(memloc).lower,
	      split_word_bytes(memloc).higher);
      
      fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
      free(asms_line);
      memory_position += 4 * 4;
    }
    else if (strcmp(*token->instr, "addv") == 0 ||
	     strcmp(*token->instr, "subv") == 0 ||
	     strcmp(*token->instr, "mulv") == 0 ||
	     strcmp(*token->instr, "divv") == 0) {
      uint8_t instruction;

      if(strcmp(*token->instr, "addv") == 0) {
	instruction = ADDI;
      }
      else if(strcmp(*token->instr, "subv") == 0) {
	instruction = SUBT;
      }
      else if(strcmp(*token->instr, "mulv") == 0) {
	instruction = MULT;
      }
      else if(strcmp(*token->instr, "divv") == 0) {
	instruction = DIVI;
      }

      asms_line = malloc(sizeof(char) * (4 * 5 * 4 + 1));
      uint16_t memaddr1 = getvar(*token->arg1)->memory_location;
      uint16_t memaddr2 = getvar(*token->arg2)->memory_location;

      sprintf(asms_line, asms_template,
	      LOAD,
	      AR,
	      split_word_bytes(memaddr1).lower,
	      split_word_bytes(memaddr1).higher);
      
      sprintf(asms_line, asms_template_add, asms_line,
	      LOAD,
	      BR,
	      split_word_bytes(memaddr2).lower,
	      split_word_bytes(memaddr2).higher);
      
      sprintf(asms_line, asms_template_add, asms_line,
	      instruction, 0, 0, 0);
      
      sprintf(asms_line, asms_template_add, asms_line,
	      MOVM,
	      AR,
	      split_word_bytes(memaddr1).lower,
	      split_word_bytes(memaddr1).higher);
      
      fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
      free(asms_line);
      memory_position += 4 * 4;
    }
    else if (strcmp(*token->instr, "label") == 0) {
      addlabel(*token->arg1, memory_position);
    }
    else if (strcmp(*token->instr, "jump") == 0) {
      asms_line = malloc(sizeof(char) * (5 * 4 + 1));
      uint16_t memloc = getlabel(*token->arg1);
      sprintf(asms_line, asms_template,
	      JUMP,
	      0,
	      split_word_bytes(memloc).lower,
	      split_word_bytes(memloc).higher);
      
      fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
      free(asms_line);
      memory_position += 4;
    }
    else if (strcmp(*token->instr, "ifzero") == 0 || strcmp(*token->instr, "ifnzero") == 0) {
      uint8_t instruction;

      if(strcmp(*token->instr, "ifzero") == 0) { instruction = SKNZ; }
      else if(strcmp(*token->instr, "ifnzero")== 0) { instruction = SKZE; }
      uint16_t memloc = getvar(*token->arg1)->memory_location;
      asms_line = malloc(sizeof(char) * (2 * 5 * 4 + 1));
      sprintf(asms_line, asms_template,
	      LOAD,
	      AR,
	      split_word_bytes(memloc).lower,
	      split_word_bytes(memloc).higher);

      sprintf(asms_line, asms_template_add, asms_line,
	      instruction, 0, 0, 0);
      
      fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
      free(asms_line);
      memory_position += 4 * 2;
    }
    else if (strcmp(*token->instr, ";") == 0) {
      // comment
    }
  }

  int label_index, to_offset_index;
  for(to_offset_index = memory_position; to_offset_index < label_offset; to_offset_index += 4) {
    asms_line = malloc(sizeof(char) * (5 * 4));
    sprintf(asms_line, asms_template, 0, 0, 0, 0);
    fwrite(asms_line, sizeof(char), strlen(asms_line), output_file);
    free(asms_line);
  }
  for(label_index = 0; label_index < label_count; label_index++) {
    asms_line = malloc(sizeof(char) * (5 * 4));
    uint16_t memloc = labels[label_index].memory_location;

    sprintf(asms_line, asms_template,
	    JUMP,
	    0,
	    split_word_bytes(memloc).lower,
	    split_word_bytes(memloc).higher);

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
    if(vars[i]->type == autom) {
      memloc += vars[i]->size;
    }
  }

  memloc += heap_offset;

  vars = realloc(vars, sizeof(varstore) * (var_count + 1));
  vars[var_count] = (varstore*) malloc(sizeof(varstore));
  strcpy(vars[var_count]->varname, varname);
  vars[var_count]->memory_location = memloc;
  vars[var_count]->size = size;
  vars[var_count]->type = autom;
  var_count++;
  return memloc;
}

uint16_t setvar_raw(char *varname, uint16_t location, uint16_t size) {
  vars = realloc(vars, sizeof(varstore) * (var_count + 1));
  vars[var_count] = (varstore*) malloc(sizeof(varstore));
  strcpy(vars[var_count]->varname, varname);
  vars[var_count]->memory_location = location;
  vars[var_count]->size = size;
  vars[var_count]->type = fixed;
  var_count++;
  return location;
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
  labels[label_count].reference_location = label_offset + ((label_count) * 4 * sizeof(uint8_t));
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

split_word split_word_bytes(uint16_t word)
{
  uint8_t lower = word & 0xff;
  uint8_t higher = word >> 8;
  split_word split = {lower, higher};
  return split;
}
