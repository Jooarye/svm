#include "stack.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum op {
  LDA,
  LDC,

  CMPLT,
  CMPGT,
  CMPLQ,
  CMPGQ,
  CMPEQ,
  CMPNE,

  JMPNZ,
  JMPZ,

  ADD,
  SUB,
  MUL,
  DIV,

  DUP,
  DROP,
  GET,

  CALL,
  RET,

  HLT,
};

struct Stack rs = {.index = -1};
struct Stack *rsp = &rs;

uint8_t *code;
uint64_t max_size;

void read_file(char *file) {
  FILE *f = fopen(file, "r");
  if (!f) {
    printf("error: can't open file\n");
    exit(-3);
  }

  fseek(f, 0, SEEK_END);
  max_size = ftell(f);
  fseek(f, 0, SEEK_SET);

  code = (uint8_t *)malloc(max_size + 1);
  fread(code, max_size, 1, f);
  fclose(f);
}

uint64_t ip = 0;
uint64_t fp = 0;

uint8_t read8() { return code[ip++]; }

uint64_t read64() {
  uint64_t res = 0;

  for (uint8_t i = 0; i < 8; i++) {
    res <<= 8;
    res |= code[ip++];
  }

  return res;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("usage: vm <file>\n");
    printf("missing file\n");
    exit(0);
  }
  read_file(argv[1]);

  uint8_t instruction;
  while ((instruction = read8()) != HLT) {

#ifdef DEBUG
    printf("   ip: %ld\n", ip - 1);
    printf("   fp: %ld\n", fp);
    printf("stack: ");
    const int64_t y = size(rsp);
    for (int64_t x = 0; x < y; x++) {
      printf("%ld ", at(rsp, x));
    }
    printf("\n");
#endif

    switch (instruction) {
    case LDA: {
      uint8_t argi = read8();
      push(rsp, at(rsp, fp - argi));

#ifdef DEBUG
      printf("   op: LDA %d\n", argi);
#endif

      break;
    }
    case LDC: {
      uint64_t arg = read64();
      push(rsp, arg);

#ifdef DEBUG
      printf("   op: LDC %ld\n", arg);
#endif

      break;
    }
    case CMPLT: {
      uint64_t b = pop(rsp);
      uint64_t a = pop(rsp);
      push(rsp, a < b);

#ifdef DEBUG
      printf("   op: CMPLT\n");
#endif

      break;
    }
    case CMPGT: {
      uint64_t b = pop(rsp);
      uint64_t a = pop(rsp);
      push(rsp, a > b);

#ifdef DEBUG
      printf("   op: CMPGT\n");
#endif

      break;
    }
    case CMPLQ: {
      uint64_t b = pop(rsp);
      uint64_t a = pop(rsp);
      push(rsp, a <= b);

#ifdef DEBUG
      printf("   op: CMPLQ\n");
#endif

      break;
    }
    case CMPGQ: {
      uint64_t b = pop(rsp);
      uint64_t a = pop(rsp);
      push(rsp, a >= b);

#ifdef DEBUG
      printf("   op: CMPGQ\n");
#endif

      break;
    }
    case CMPEQ: {
      uint64_t b = pop(rsp);
      uint64_t a = pop(rsp);
      push(rsp, a == b);

#ifdef DEBUG
      printf("   op: CMPEQ\n");
#endif

      break;
    }
    case CMPNE: {
      uint64_t b = pop(rsp);
      uint64_t a = pop(rsp);
      push(rsp, a != b);

#ifdef DEBUG
      printf("   op: CMPNE\n");
#endif

      break;
    }
    case ADD:
      push(rsp, pop(rsp) + pop(rsp));

#ifdef DEBUG
      printf("   op: ADD\n");
#endif

      break;
    case SUB: {
      uint64_t a = pop(rsp);
      uint64_t b = pop(rsp);
      push(rsp, (-a) + b);

#ifdef DEBUG
      printf("   op: SUB\n");
#endif

      break;
    }
    case MUL:
      push(rsp, pop(rsp) * pop(rsp));

#ifdef DEBUG
      printf("   op: MUL\n");
#endif

      break;
    case DIV: {
      uint64_t b = pop(rsp);
      uint64_t a = pop(rsp);

      if (b == 0) {
        printf("error: can't divide by zero\n");
        exit(-1);
      }

      push(rsp, a / b);

#ifdef DEBUG
      printf("   op: DIV\n");
#endif

      break;
    }
    case RET: {
      uint64_t result = pop(rsp);
      uint64_t tmp_ip = pop(rsp);
      fp = pop(rsp);

      uint8_t argc = read8();
      for (int i = 0; i < argc; i++)
        pop(rsp);

      ip = tmp_ip;
      push(rsp, result);

#ifdef DEBUG
      printf("   op: RET %d\n", argc);
#endif

      break;
    }
    case CALL: {
      uint64_t addr = read64();

      push(rsp, fp); // frame ptr
      fp = size(rsp) - 2;
      push(rsp, ip); // istr ptr
      ip = addr;

#ifdef DEBUG
      printf("   op: CALL %ld\n", addr);
#endif

      break;
    }
    case JMPZ: {
      uint64_t ts = read64();
      if (pop(rsp) == 0) {
        ip = ts;
      }

#ifdef DEBUG
      printf("   op: JMPZ %ld\n", ts);
#endif

      break;
    }
    case JMPNZ: {
      uint64_t ts = read64();
      if (pop(rsp) != 0) {
        ip = ts;
      }

#ifdef DEBUG
      printf("   op: JMPNZ %ld\n", ts);
#endif

      break;
    }
    case DUP: {
      uint64_t tmp = pop(rsp);
      push(rsp, tmp);
      push(rsp, tmp);

#ifdef DEBUG
      printf("   op: DUP\n");
#endif

      break;
    }
    case DROP: {
      pop(rsp);

#ifdef DEBUG
      printf("   op: DROP\n");
#endif

      break;
    }
    case GET: {
      uint8_t x = read8();
      push(rsp, at(rsp, size(rsp) - 1 - x));

#ifdef DEBUG
      printf("   op: GET %d\n", x);
#endif

      break;
    }
    default: {
      printf("error: not implemented %d\n", instruction);
      return -2;
    }
    }

#ifdef DEBUG
    printf("==========================\n");
#endif
  }

  printf("%ld\n", at(rsp, 0));
}