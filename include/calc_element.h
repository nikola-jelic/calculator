#ifndef CALC_ELEMENT_H
#define CALC_ELEMENT_H

#include <stdint.h>

#define STATUS_X_PRESENT 0x00000001
#define STATUS_X_IN_DIV 0x00000002
#define STATUS_X_IN_LOG 0x00000004
#define STATUS_X_NON_LINEAR 0x00000008

#define MASK_X_ERROR 0x0000000E

#define STATUS_CALC_DIV_BY_ZERO 0x00000100
#define STATUS_CALC_DOMAIN 0x00000200

#define MASK_CALC_ERROR 0x00000300

#define MASK_ANY_ARROR (MASK_X_ERROR | MASK_CALC_ERROR)

typedef enum {
  CALC_NUM,
  CALC_X,
  CALC_BIN_OP,
  CALC_LOG // could be placeholder for other unary functions
} CALC_TYPE;

typedef struct CALC_ELEMENT_T CALC_ELEMENT;

struct CALC_ELEMENT_T {
  CALC_TYPE calc_t;
  char bin_op;
  uint32_t status;
  double value;
  struct CALC_ELEMENT_T * left;
  struct CALC_ELEMENT_T * right;
};

CALC_ELEMENT * create_number (double);
CALC_ELEMENT * create_bin_op (char, CALC_ELEMENT *, CALC_ELEMENT *);
CALC_ELEMENT * create_x (void);
CALC_ELEMENT * create_log (CALC_ELEMENT *);

void free_calc_element (CALC_ELEMENT *);

int calculate (CALC_ELEMENT **);

int canonical_form (CALC_ELEMENT **);

CALC_ELEMENT * create_ax_b (double, double);
int get_ax_b (CALC_ELEMENT *, double *, double *);

#endif
