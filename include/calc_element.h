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

/**
 * Types of arithmetic expressions.
 */
typedef enum {
  CALC_NUM, /*!< Floating point number*/
  CALC_X, /*!< Unknown variable x */
  CALC_BIN_OP, /*!< Binary operator, '+', '-', '*' or '/' */
  CALC_LOG /*!< Logarithm */
} CALC_TYPE;

typedef struct CALC_ELEMENT_T CALC_ELEMENT;

/**
 * Data structure for arithmetic expression and its elements.
 */
struct CALC_ELEMENT_T {
  CALC_TYPE calc_t; /*!< Type of the expression */
  char bin_op; /*!< In the case of the binary operand, the operand type */
  uint32_t status; /*!< The status of the expression, propagated to its parents */
  double value; /*!< For numbers, the value of the number; for all others, the multiplicator */
  struct CALC_ELEMENT_T * left; /*!< Left sub-expression for binary operators, the sub-expression for logarithm; NULL for all others */
  struct CALC_ELEMENT_T * right; /*!< Right sub-expression for binary operators; NULL for all others */
};

/**
 * Creates an arithmetic expression whose value is a single floating point number.
 * @param[in] val The number for which we create the expression.
 * @returns The expression.
 */
CALC_ELEMENT * create_number (double val);
/**
 * Creates an arithmetic expression corresponding to a binary operator with two sub-expressions.
 * @param[in] op The binary operand, could be '+', '-', '*' or '/'.
 * @param[in] l The first (left) subexpression.
 * @param[in] r The second (right) subexpression.
 * @returns The expression.
 */
CALC_ELEMENT * create_bin_op (char op, CALC_ELEMENT * l, CALC_ELEMENT * r);
/**
 * Creates an arithmetic expression containing an unknown variable 'x'. Used only in the linear equation mode.
 * @returns The expression.
 */
CALC_ELEMENT * create_x (void);
/**
 * Craetes an arithmetic expression for logarithm.
 * @param[in] e The sub-expression.
 * @returns The expression.
 */
CALC_ELEMENT * create_log (CALC_ELEMENT * e);

/**
 * Frees the memory of the expression tree, taking into account the types of the sub-expressions.
 * @param[in,out] e The expression.
 */
void free_calc_element (CALC_ELEMENT * e);

/**
 * Tries to calculate the numerical value of the expression. The presence of an 'x',
 * division by zero or logarithm with the sub-expression lesser then or equal to zero will
 * result in an error, which will be indicated by the status of the parent expression.
 * This function will do multiple tree rewrites, so the original tree is not saved.
 * @param[in,out] e The expression to be calculated.
 * @returns Zero for success, lesser than zero for failure.
 */
int calculate (CALC_ELEMENT ** e);

/**
 * Tries to transform one part of a linear equation to a canonical form of 'ax + b'. In the
 * case of the absence of 'x', it will behave exactly as [calculate](@ref calculate)
 * function. If 'x' is involved in division or logarithm, or if the equation is non-linear,
 * the function will fail and the status of the paren expression will be set accordingly.
 * Like the [calculate](@ref calculate) function, it will perform multiple tree rewrites.
 * @param[in,out] e The expression to be transformed to the canonical form.
 * @return Zero for success, lesser than zero for failure.
 */
int canonical_form (CALC_ELEMENT ** e);

/**
 * A helper function to quickly create 'ax + b'.
 * @param[in] a The coefficient next to 'x'.
 * @param[in] b The additive coefficient.
 * @returns For 'a = 0', an expression containing a single number 'b'; for 'b = 0', an
 * expression containing 'x' multiplied by 'a'; in all other cases, a binary operator '+'
 * with left side being equal to 'x' multiplied by 'a', and with right side being equal to
 * the number 'b'.
 */
CALC_ELEMENT * create_ax_b (double a, double b);
/**
 * A helper function to quickly retrieve coefficients 'a' and 'b' from a canonical form.
 * If the equation is not in the canonical form, the function will fail.
 * @param[in] e The queried expression.
 * @param[out] a The coefficient next to 'x'.
 * @param[out] b the additive coefficient.
 * @returns Zero fir success, lesser than zero for failure.
 */
int get_ax_b (CALC_ELEMENT * e, double * a, double * b);

#endif
