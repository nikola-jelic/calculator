#include "calc_element.h"
#include <errno.h>
#include <fenv.h>
#include <math.h>
#include <stdlib.h>

CALC_ELEMENT * create_number (double val) {
  CALC_ELEMENT * res = (CALC_ELEMENT *)malloc(sizeof(CALC_ELEMENT));
  res->calc_t = CALC_NUM;
  res->status = 0;
  res->value = val;
  res->left = NULL;
  res->right = NULL;
  return res;
}

CALC_ELEMENT * create_bin_op (char op, CALC_ELEMENT * l, CALC_ELEMENT * r) {
  CALC_ELEMENT * res = (CALC_ELEMENT *)malloc(sizeof(CALC_ELEMENT));
  res->calc_t = CALC_BIN_OP;
  res->bin_op = op;
  res->status = l->status | r->status;
  if (op == '*' && (l->status & STATUS_X_PRESENT) && (r->status & STATUS_X_PRESENT))
    res->status |= STATUS_X_NON_LINEAR;
  res->value = 1.0;
  res->left = l;
  res->right = r;
  return res;
}

CALC_ELEMENT * create_x (void) {
  CALC_ELEMENT * res = (CALC_ELEMENT *)malloc(sizeof(CALC_ELEMENT));
  res->calc_t = CALC_X;
  res->status = STATUS_X_PRESENT;
  res->value = 1.0;
  res->left = NULL;
  res->right = NULL;
  return res;
}

CALC_ELEMENT * create_log (CALC_ELEMENT * e) {
  CALC_ELEMENT * res = (CALC_ELEMENT *)malloc(sizeof(CALC_ELEMENT));
  res->calc_t = CALC_LOG;
  res->status = 0;
  res->value = 1.0;
  res->left = e;
  res->right = NULL;
  return res;
}
