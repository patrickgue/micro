#ifndef MICRO_H
#define MICRO_H

#include <stdint.h>
#include <stdbool.h>

/* registers */

/* 16 bit */
#define AR 0x00
#define BR 0x01
#define CR 0x02
#define DR 0x03
#define ER 0x04
#define FR 0x05
#define GR 0x06
#define HR 0x07

/* lower byte */
#define AL 0x10
#define BL 0x11
#define CL 0x12
#define DL 0x13
#define EL 0x14
#define FL 0x15
#define GL 0x16
#define HL 0x17

/* upper byte */
#define AU 0x18
#define BU 0x19
#define CU 0x1a
#define DU 0x1b
#define EU 0x1c
#define FU 0x1d
#define GU 0x1e
#define HU 0x1f

/* general purpose */
#define PS 0x08
#define FG 0x09

/* instructions */
#define INTE 0x00 // interupt

#define MOVR 0x01 // move 16bit value to register
#define MOVM 0x02 // move value from register to memory

#define ADDI 0x10 // add register br to ar. save result to ar. possible overflow error
#define SUBT 0x11 // subtract br from ar. save result to ar. possible subzero error
#define MULT 0x12 // multiply br and ar. save result to ar. possible overflow error
#define DIVI 0x13 // devide register ar through br. possible zero through zero error

/* interupts */
#define EXIT 0x00 // halt machine
#define PRNT 0x01 // print text at memory location stored in ar and size stored in br
#define KYIN 0x02 // read text from keyboard, save at memory location stored in ar and size stored in br
#define MEME 0x03 // memory error
#define GENE 0x04 // 

/* error codes */
#define ERR_PROG_COUNT_OVERFLOW 1


struct s_m_state {
  bool running;
  uint16_t program_size;
  uint8_t error;
};

struct s_m_registers {
  uint16_t ps;
  uint16_t ar;
  uint16_t br;
  uint16_t cr;
  uint16_t dr;
  uint16_t er;
  uint16_t fr;
  uint16_t gr;
  uint16_t hr;
  uint8_t  fg;
  /*
    0000 0000
    |||| ||||
    |||| |||
    |||| ||
    |||| |
    ||||
    ||| subzero error
    || division through zero error
    | overflow error
   */
};

typedef struct s_m_registers m_registers;
typedef struct s_m_state m_state;

typedef uint8_t byte;

uint16_t read_register(m_registers*, uint8_t);
void write_register(m_registers*, uint8_t regi, uint16_t value);

void init_regs(m_registers*);
void init_state(m_state*);

#endif
