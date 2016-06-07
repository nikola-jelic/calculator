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
  res->bin_op = (op != '-' ? op : '+'); /* simplification of substraction */
  res->status = l->status | r->status;
  if (op == '-')
    r->value += -1.0;
  if (op == '*' && (l->status & STATUS_X_PRESENT) && (r->status & STATUS_X_PRESENT))
    res->status |= STATUS_X_NON_LINEAR;
  if (op == '/' && (res->status & STATUS_X_PRESENT))
    res->status |= STATUS_X_IN_DIV;
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
  res->status = e->status;
  if (res->status & STATUS_X_PRESENT)
    res->status |= STATUS_X_IN_LOG;
  res->value = 1.0;
  res->left = e;
  res->right = NULL;
  return res;
}

void free_calc_element (CALC_ELEMENT * e) {
  if (e == NULL)
    return;
  switch (e->calc_t) {
  case CALC_NUM:
  case CALC_X:
    free (e);
    break;
  case CALC_BIN_OP:
    free_calc_element(e->left);
    free_calc_element(e->right);
    free (e);
    break;
  case CALC_LOG:
    free_calc_element(e->left);
    free (e);
    break;
  }
}

int calculate (CALC_ELEMENT ** e) {
  int res = 0;
  CALC_ELEMENT * loc = *e;
  switch (loc->calc_t) {
  case CALC_NUM:
    return 0;
    break;
  case CALC_X: /* should not ever happen */
    return -1;
    break;
  case CALC_BIN_OP:
    /* first validate the left and the right side of the equation */
    if ((calculate (&loc->left) == 0) && (calculate (&loc->right) == 0)) {
      switch (loc->bin_op) {
      case '+':
	*e = create_number (loc->value * (loc->left->value + loc->right->value));
	free_calc_element (loc);
	return 0;
	break;
      case '*':
	*e = create_number (loc->value * (loc->left->value * loc->right->value));
	free_calc_element (loc);
	return 0;
	break;
      case '/':
	/* we must check the right side for zero */
	if (loc->right-> value == 0.0) {
	  loc->status |= STATUS_CALC_DIV_BY_ZERO;
	  return -1;
	} else {
	  *e = create_number (loc->value * (loc->left->value / loc->right->value));
	  free_calc_element (loc);
	  return 0;
	}
	break;
      }
    } else {
      loc->status |= loc->left->status | loc->right->status;
      return -1;
    }
    break;
  case CALC_LOG:
    if (calculate (&loc->left) == 0) {
      if (loc->left->calc_t == CALC_NUM) {
	if (loc->left->value <= 0.0) {
	  loc->status |= STATUS_CALC_DOMAIN;
	  return -1;
	} else {
	  /* replace the log element with a number */
	  *e = create_number (log(loc->left->value) * loc->value);
	  free_calc_element (loc);
	  return 0;
	}
      } else { /* this should also never happen */
	return -1;
      }
    } else {
      loc->status |= loc->left->status;
      return -1;
    }
    break;
  }
  return res;
}
