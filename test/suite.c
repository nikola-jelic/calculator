#include "calc_element.h"
#include "lexer.h"
#include "parser.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void test_number_cf (void) {
  CALC_ELEMENT * t1 = create_number (15.0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 15.0);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);
}

void test_x_cf (void) {
  CALC_ELEMENT * t1 = create_x ();
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_X);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_PRESENT);
  CU_ASSERT_EQUAL (t1->value, 1.0);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);
}

void test_log_cf (void) {
  CALC_ELEMENT * t1 = create_number (1.0);
  t1 = create_log (t1);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_LOG);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 1.0);
  CU_ASSERT_PTR_NOT_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);

  t1 = create_x ();
  t1 = create_log (t1);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_LOG);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_PRESENT | STATUS_X_IN_LOG);
  CU_ASSERT_EQUAL (t1->value, 1.0);
  CU_ASSERT_PTR_NOT_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);
}

void test_binop_cf (void) {
  /* + */
  CALC_ELEMENT * e1 = create_number (3.0), * e2 = create_number (-4.0);
  CALC_ELEMENT * expr = create_bin_op ('+', e1, e2);
  CU_ASSERT_PTR_NOT_NULL (expr);
  CU_ASSERT_EQUAL (expr->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (expr->bin_op, '+');
  CU_ASSERT_EQUAL (expr->status, 0);
  CU_ASSERT_EQUAL (expr->value, 1.0);
  CU_ASSERT_PTR_NOT_NULL (expr->left);
  CU_ASSERT_PTR_NOT_NULL (expr->right);
  free_calc_element (expr);

  /* - */
  e1 = create_number (-7.0);
  e2 = create_number (9.0);
  expr = create_bin_op ('-', e1, e2);
  CU_ASSERT_PTR_NOT_NULL (expr);
  CU_ASSERT_EQUAL (expr->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (expr->bin_op, '+');
  CU_ASSERT_EQUAL (expr->status, 0);
  CU_ASSERT_EQUAL (expr->value, 1.0);
  CU_ASSERT_PTR_NOT_NULL (expr->left);
  CU_ASSERT_PTR_NOT_NULL (expr->right);
  CU_ASSERT_EQUAL (expr->right->value, -9.0);
  free_calc_element (expr);

  /* * */
  e1 = create_number (3.0);
  e2 = create_number (11.5);
  expr = create_bin_op ('*', e1, e2);
  CU_ASSERT_PTR_NOT_NULL (expr);
  CU_ASSERT_EQUAL (expr->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (expr->bin_op, '*');
  CU_ASSERT_EQUAL (expr->status, 0);
  CU_ASSERT_EQUAL (expr->value, 1.0);
  CU_ASSERT_PTR_NOT_NULL (expr->left);
  CU_ASSERT_PTR_NOT_NULL (expr->right);
  free_calc_element (expr);

  /* /, only numbers */
  e1 = create_number (-7.0);
  e2 = create_number (5.0);
  expr = create_bin_op ('/', e1, e2);
  CU_ASSERT_PTR_NOT_NULL (expr);
  CU_ASSERT_EQUAL (expr->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (expr->bin_op, '/');
  CU_ASSERT_EQUAL (expr->status, 0);
  CU_ASSERT_EQUAL (expr->value, 1.0);
  CU_ASSERT_PTR_NOT_NULL (expr->left);
  CU_ASSERT_PTR_NOT_NULL (expr->right);
  free_calc_element (expr);

  /* /, with an x */
  e1 = create_x ();
  e2 = create_number (2.0);
  expr = create_bin_op ('/', e1, e2);
  CU_ASSERT_PTR_NOT_NULL (expr);
  CU_ASSERT_EQUAL (expr->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (expr->bin_op, '/');
  CU_ASSERT_EQUAL (expr->status, STATUS_X_PRESENT | STATUS_X_IN_DIV);
  CU_ASSERT_EQUAL (expr->value, 1.0);
  CU_ASSERT_PTR_NOT_NULL (expr->left);
  CU_ASSERT_PTR_NOT_NULL (expr->right);
  free_calc_element (expr);
}

void test_create_access_axb (void) {
  CALC_ELEMENT * t1 = create_ax_b (0.0, 0.0);
  double a, b;
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 0.0);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  get_ax_b (t1, &a, &b);
  CU_ASSERT_EQUAL (a, 0.0);
  CU_ASSERT_EQUAL (b, 0.0);
  free_calc_element (t1);

  t1 = create_ax_b (6.0, 0.0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_X);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_PRESENT);
  CU_ASSERT_EQUAL (t1->value, 6.0);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  get_ax_b (t1, &a, &b);
  CU_ASSERT_EQUAL (a, 6.0);
  CU_ASSERT_EQUAL (b, 0.0);
  free_calc_element (t1);

  t1 = create_ax_b (-3.0, 2.25);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_PRESENT);
  CU_ASSERT_EQUAL (t1->value, 1.0);
  CU_ASSERT_PTR_NOT_NULL (t1->left);
  CU_ASSERT_PTR_NOT_NULL (t1->right);
  get_ax_b (t1, &a, &b);
  CU_ASSERT_EQUAL (a, -3.0);
  CU_ASSERT_EQUAL (b, 2.25);
  free_calc_element (t1);

  t1 = create_ax_b (2, -3);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_PRESENT);
  t1->value *= -0.5;
  get_ax_b (t1, &a, &b);
  CU_ASSERT_EQUAL (a, -1.0);
  CU_ASSERT_EQUAL (b, 1.5);
  free_calc_element (t1);
}

void test_bad_axb (void) {
  CALC_ELEMENT * t1, *t2, *t3;
  double a, b;
  /* log */
  t2 = create_number(6.0);
  t1 = create_log (t2);
  CU_ASSERT_EQUAL (get_ax_b (t1, &a, &b), -1);
  free_calc_element (t1);
  /* bin op different from + */
  t2 = create_number (-9.0);
  t3 = create_x ();
  t1 = create_bin_op ('*', t2, t3);
  CU_ASSERT_EQUAL (get_ax_b (t1, &a, &b), -1);
  free_calc_element (t1);
  /* two numbers */
  t2 = create_number (2.0);
  t3 = create_number (4.2);
  t1 = create_bin_op ('+', t2, t3);
  CU_ASSERT_EQUAL (get_ax_b (t1, &a, &b), -1);
  free_calc_element (t1);
  /* two xs */
  t2 = create_x ();
  t3 = create_x ();
  t1 = create_bin_op ('*', t2, t3);
  CU_ASSERT_EQUAL (get_ax_b (t1, &a, &b), -1);
  free_calc_element (t1);
  /* something wonky, like 4 * 3 + 2 */
  t2 = create_number (4);
  t3 = create_number (3);
  t2 = create_bin_op ('*', t2, t3);
  t3 = create_number (2);
  t1 = create_bin_op ('+', t2, t3);
  CU_ASSERT_EQUAL (get_ax_b (t1, &a, &b), -1);
  free_calc_element (t1);
}

void test_calc_good (void) {
  CALC_ELEMENT *t1, *t2;
  /* single number */
  t1 = create_number (12.0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 12.0);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);
  
  /* addition */
  t1 = create_number (2.0);
  t2 = create_number (-3.5);
  t1 = create_bin_op ('+', t1, t2);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, -1.5);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);

  /* substraction */
  t1 = create_number (-3.0);
  t2 = create_number (-4.25);
  t1 = create_bin_op ('-', t1, t2);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 1.25);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);

  /* multiplication */
  t1 = create_number (4);
  t2 = create_number (1.25);
  t1 = create_bin_op ('*', t1, t2);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 5.0);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);

  /* division */
  t1 = create_number (8.0);
  t2 = create_number (16.0);
  t1 = create_bin_op ('/', t1, t2);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 0.5);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);
  
  /* natural logarithm */
  t1 = create_number (1.0);
  t1 = create_log (t1);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 0.0);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);

  t1 = create_number (M_E);
  t1 = create_log (t1);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 1.0);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);

  /* a slightly more complex example (3 + (4 - 1)) * 5 */
  t1 = create_number (4);
  t2 = create_number (1);
  t2 = create_bin_op ('-', t1, t2);
  t1 = create_number (3);
  t1 = create_bin_op ('+', t1, t2);
  t2 = create_number (5);
  t1 = create_bin_op ('*', t1, t2);
  CU_ASSERT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 30.0);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);
}

void test_calc_bad (void) {
  CALC_ELEMENT *t1, *t2;
  /* division by zero, simple */
  t1 = create_number (9.0);
  t2 = create_number (0.0);
  t1 = create_bin_op ('/', t1, t2);
  CU_ASSERT_NOT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_EQUAL (t1->status & STATUS_CALC_DIV_BY_ZERO, STATUS_CALC_DIV_BY_ZERO);
  free_calc_element (t1);
  /* division by zero, complex */
  t1 = create_bin_op ('/', create_number(4.0), create_bin_op('-', create_number (2.0),
							     create_number (2.0)));
  CU_ASSERT_NOT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_EQUAL (t1->status & STATUS_CALC_DIV_BY_ZERO, STATUS_CALC_DIV_BY_ZERO);
  free_calc_element (t1);
  /* log zero */
  t1 = create_log (create_number (0.0));
  CU_ASSERT_NOT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_EQUAL (t1->status & STATUS_CALC_DOMAIN, STATUS_CALC_DOMAIN);
  free_calc_element (t1);
  /* log less than zero */
  t1 = create_log (create_number (-3.0));
  CU_ASSERT_NOT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_EQUAL (t1->status & STATUS_CALC_DOMAIN, STATUS_CALC_DOMAIN);
  free_calc_element (t1);
  /* log zero, complex */
  t1 = create_log ( create_bin_op('*', create_number (0.0), create_number (28.0)));
  CU_ASSERT_NOT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_EQUAL (t1->status & STATUS_CALC_DOMAIN, STATUS_CALC_DOMAIN);
  free_calc_element (t1);
  /* x in the expression, simple */
  t1 = create_x ();
  CU_ASSERT_NOT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_EQUAL (t1->status & STATUS_X_PRESENT, STATUS_X_PRESENT);
  free_calc_element (t1);
  /* x in the expression, complex */
  t1 = create_bin_op ('-', create_x(), create_number(17));
  CU_ASSERT_NOT_EQUAL (calculate (&t1), 0);
  CU_ASSERT_EQUAL (t1->status & STATUS_X_PRESENT, STATUS_X_PRESENT);
  free_calc_element (t1);
}

void test_canon_calc_good (void) {
  CALC_ELEMENT *t1, *t2;
  /* single number */
  t1 = create_number (12.0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 12.0);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);
  
  /* addition */
  t1 = create_number (2.0);
  t2 = create_number (-3.5);
  t1 = create_bin_op ('+', t1, t2);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, -1.5);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);

  /* substraction */
  t1 = create_number (-3.0);
  t2 = create_number (-4.25);
  t1 = create_bin_op ('-', t1, t2);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 1.25);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);

  /* multiplication */
  t1 = create_number (4);
  t2 = create_number (1.25);
  t1 = create_bin_op ('*', t1, t2);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 5.0);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);

  /* division */
  t1 = create_number (8.0);
  t2 = create_number (16.0);
  t1 = create_bin_op ('/', t1, t2);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 0.5);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);
  
  /* natural logarithm */
  t1 = create_number (1.0);
  t1 = create_log (t1);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 0.0);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);

  t1 = create_number (M_E);
  t1 = create_log (t1);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 1.0);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);

  /* a slightly more complex example (3 + (4 - 1)) * 5 */
  t1 = create_number (4);
  t2 = create_number (1);
  t2 = create_bin_op ('-', t1, t2);
  t1 = create_number (3);
  t1 = create_bin_op ('+', t1, t2);
  t2 = create_number (5);
  t1 = create_bin_op ('*', t1, t2);
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_PTR_NOT_NULL (t1);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (t1->status, 0);
  CU_ASSERT_EQUAL (t1->value, 30.0);
  CU_ASSERT_PTR_NULL (t1->left);
  CU_ASSERT_PTR_NULL (t1->right);
  free_calc_element (t1);
}

void test_canon_calc_bad (void) {
  CALC_ELEMENT *t1, *t2;
  /* division by zero, simple */
  t1 = create_number (9.0);
  t2 = create_number (0.0);
  t1 = create_bin_op ('/', t1, t2);
  CU_ASSERT_NOT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->status & STATUS_CALC_DIV_BY_ZERO, STATUS_CALC_DIV_BY_ZERO);
  free_calc_element (t1);
  /* division by zero, complex */
  t1 = create_bin_op ('/', create_number(4.0), create_bin_op('-', create_number (2.0),
							     create_number (2.0)));
  CU_ASSERT_NOT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->status & STATUS_CALC_DIV_BY_ZERO, STATUS_CALC_DIV_BY_ZERO);
  free_calc_element (t1);
  /* log zero */
  t1 = create_log (create_number (0.0));
  CU_ASSERT_NOT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->status & STATUS_CALC_DOMAIN, STATUS_CALC_DOMAIN);
  free_calc_element (t1);
  /* log less than zero */
  t1 = create_log (create_number (-3.0));
  CU_ASSERT_NOT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->status & STATUS_CALC_DOMAIN, STATUS_CALC_DOMAIN);
  free_calc_element (t1);
  /* log zero, complex */
  t1 = create_log ( create_bin_op('*', create_number (0.0), create_number (28.0)));
  CU_ASSERT_NOT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->status & STATUS_CALC_DOMAIN, STATUS_CALC_DOMAIN);
  free_calc_element (t1);
}

void test_canon_x (void) {
  CALC_ELEMENT * t1;
  double a, b;
  /* addition x + a * x */
  t1 = create_bin_op ('+', create_x(), create_bin_op ('*', create_number (2.5),
						      create_x()));
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_X);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_PRESENT);
  CU_ASSERT_EQUAL (t1->value, 3.5);
  free_calc_element (t1);
  
  /* addition a * x + b */
  t1 = create_bin_op ('+', create_bin_op ('*', create_number (1.25), create_x()),
		      create_number (14));
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_PRESENT);
  CU_ASSERT_EQUAL (get_ax_b (t1, &a, &b), 0);
  CU_ASSERT_EQUAL (a, 1.25);
  CU_ASSERT_EQUAL (b, 14);
  free_calc_element (t1);

  /* addition a * x + b + c */
  t1 = create_bin_op ('+', create_bin_op ('+', create_bin_op ('*', create_number (0.75),
							      create_x()),
					  create_number (6)), create_number (-3.5));
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_PRESENT);
  CU_ASSERT_EQUAL (get_ax_b (t1, &a, &b), 0);
  CU_ASSERT_EQUAL (a, 0.75);
  CU_ASSERT_EQUAL (b, 2.5);
  free_calc_element (t1);
  
  /* addition (a1 * x + b1) + (a2 * x + b2) */
  t1 = create_bin_op ('+', create_ax_b (9.0, -1.25), create_ax_b (2.0, 6.5));
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_PRESENT);
  CU_ASSERT_EQUAL (get_ax_b (t1, &a, &b), 0);
  CU_ASSERT_EQUAL (a, 11.0);
  CU_ASSERT_EQUAL (b, 5.25);
  free_calc_element (t1);
  
  /* multiplication a * x */
  t1 = create_bin_op ('*', create_x (), create_number (-9.0));
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_X);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_PRESENT);
  CU_ASSERT_EQUAL (get_ax_b (t1, &a, &b), 0);
  CU_ASSERT_EQUAL (a, -9.0);
  CU_ASSERT_EQUAL (b, 0);
  free_calc_element (t1);

  /* multiplication x * a */
  t1 = create_bin_op ('*', create_number (7.5), create_x ());
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_X);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_PRESENT);
  CU_ASSERT_EQUAL (get_ax_b (t1, &a, &b), 0);
  CU_ASSERT_EQUAL (a, 7.5);
  CU_ASSERT_EQUAL (b, 0);
  free_calc_element (t1);

  /* multiplication c * (a * x + b) */
  t1 = create_bin_op ('*', create_number (19), create_ax_b (4, -0.5));
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_PRESENT);
  CU_ASSERT_EQUAL (get_ax_b (t1, &a, &b), 0);
  CU_ASSERT_EQUAL (a, 76);
  CU_ASSERT_EQUAL (b, -9.5);
  free_calc_element (t1);
  
  /* multiplication (a * x + b) * c */
  t1 = create_bin_op ('*', create_ax_b (-6, 4.5), create_number (0.5));
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_PRESENT);
  CU_ASSERT_EQUAL (get_ax_b (t1, &a, &b), 0);
  CU_ASSERT_EQUAL (a, -3.0);
  CU_ASSERT_EQUAL (b, 2.25);
  free_calc_element (t1);

  /* multiplication x * x */
  t1 = create_bin_op ('*', create_x(), create_x());
  CU_ASSERT_NOT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_NON_LINEAR | STATUS_X_PRESENT);
  free_calc_element (t1);

  /* multiplication x * (a * x + b) */
  t1 = create_bin_op ('*', create_x(), create_x());
  CU_ASSERT_NOT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_NON_LINEAR | STATUS_X_PRESENT);
  free_calc_element (t1);

  /* multiplication (2*x - (2*x - 3)) * (3x - 4) - false square, actually linear */
  t1 = create_bin_op ('*', create_bin_op ('-', create_ax_b (2.0, 0.0),
					  create_ax_b(2, -3)), create_ax_b (3.0, -4.0));
  CU_ASSERT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (get_ax_b (t1, &a, &b), 0);
  CU_ASSERT_EQUAL (a, 9.0);
  CU_ASSERT_EQUAL (b, -12.0);
  free_calc_element (t1);

  /* division x / a */
  t1 = create_bin_op ('/', create_x(), create_number(2.0));
  CU_ASSERT_NOT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_BIN_OP);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_IN_DIV | STATUS_X_PRESENT);
  free_calc_element (t1);

  /* log (ax + b) */
  t1 = create_log (create_ax_b (9.0, -3.0));
  CU_ASSERT_NOT_EQUAL (canonical_form (&t1), 0);
  CU_ASSERT_EQUAL (t1->calc_t, CALC_LOG);
  CU_ASSERT_EQUAL (t1->status, STATUS_X_IN_LOG | STATUS_X_PRESENT);
  free_calc_element (t1);
}

void test_lexer (void) {
  strcpy(in_line, "14.5");
  line_len = 4;
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (14.5, get_current_number());
  clear_line();
  
  strcpy (in_line, "(3+(4-1))*5");
  line_len = 11;
  CU_ASSERT_TRUE (accept (PARSE_LPAREN));
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (3, get_current_number());
  CU_ASSERT_TRUE (accept (PARSE_ADD));
  CU_ASSERT_TRUE (accept (PARSE_LPAREN));
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (4, get_current_number());
  CU_ASSERT_TRUE (accept (PARSE_SUB));
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (1, get_current_number());
  CU_ASSERT_TRUE (accept (PARSE_RPAREN));
  CU_ASSERT_TRUE (accept (PARSE_RPAREN));
  CU_ASSERT_TRUE (accept (PARSE_MULT));
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (5, get_current_number());
  CU_ASSERT_TRUE (accept (PARSE_NLINE));
  clear_line();

  strcpy (in_line, "2 * x + 0.5 = 1");
  line_len = 15;
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (2, get_current_number());
  CU_ASSERT_TRUE (accept (PARSE_MULT));
  CU_ASSERT_TRUE (accept (PARSE_X));
  CU_ASSERT_TRUE (accept (PARSE_ADD));
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (0.5, get_current_number());
  CU_ASSERT_TRUE (accept (PARSE_EQUAL));
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (1, get_current_number());
  CU_ASSERT_TRUE (accept (PARSE_NLINE));
  clear_line();

  strcpy (in_line, "2x + 1 = 2(1-x)");
  line_len = 15;
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (2, get_current_number());
  CU_ASSERT_TRUE (accept (PARSE_MULT));
  CU_ASSERT_TRUE (accept (PARSE_X));
  CU_ASSERT_TRUE (accept (PARSE_ADD));
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (1, get_current_number());
  CU_ASSERT_TRUE (accept (PARSE_EQUAL));
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (2, get_current_number());
  CU_ASSERT_TRUE (accept (PARSE_MULT));
  CU_ASSERT_TRUE (accept (PARSE_LPAREN));
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (1, get_current_number());
  CU_ASSERT_TRUE (accept (PARSE_SUB));
  CU_ASSERT_TRUE (accept (PARSE_X));
  CU_ASSERT_TRUE (accept (PARSE_RPAREN));
  CU_ASSERT_TRUE (accept (PARSE_NLINE));
  clear_line();

  strcpy (in_line, "8a - 4b = 3");
  line_len = 11;
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (8, get_current_number());
  CU_ASSERT_TRUE (accept (PARSE_BAD));
  CU_ASSERT_TRUE (accept (PARSE_SUB));
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (4, get_current_number());
  CU_ASSERT_TRUE (accept (PARSE_BAD));
  CU_ASSERT_TRUE (accept (PARSE_EQUAL));
  CU_ASSERT_TRUE (accept (PARSE_NUMBER));
  CU_ASSERT_EQUAL (3, get_current_number());
  CU_ASSERT_TRUE (accept (PARSE_NLINE));
  clear_line();
}

void test_parser (void) {
  CALC_ELEMENT *e1 = NULL, *e2 = NULL;
  double a, b;
  strcpy (in_line, "(3+(4-1))*5");
  line_len = 11;
  CU_ASSERT_EQUAL (parse_line (&e1, &e2), 0);
  CU_ASSERT_PTR_NOT_NULL (e1);
  CU_ASSERT_PTR_NULL (e2);
  CU_ASSERT_EQUAL (canonical_form (&e1), 0);
  CU_ASSERT_EQUAL (e1->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (e1->value, 30);
  free_calc_element (e1);
  clear_line();

  strcpy (in_line, "2 * x + 0.5 = 1");
  line_len = 15;
  CU_ASSERT_EQUAL (parse_line (&e1, &e2), 0);
  CU_ASSERT_PTR_NOT_NULL (e1);
  CU_ASSERT_PTR_NOT_NULL (e2);
  CU_ASSERT_EQUAL (canonical_form (&e1), 0);
  CU_ASSERT_EQUAL (canonical_form (&e2), 0);
  CU_ASSERT_EQUAL (get_ax_b (e1, &a, &b), 0);
  CU_ASSERT_EQUAL (a, 2);
  CU_ASSERT_EQUAL (b, 0.5);
  CU_ASSERT_EQUAL (e2->calc_t, CALC_NUM);
  CU_ASSERT_EQUAL (e2->value, 1);
  free_calc_element (e1);
  free_calc_element (e2);
  clear_line();
  
  strcpy (in_line, "2x + 1 = 2(1-x)");
  line_len = 15;
  CU_ASSERT_EQUAL (parse_line (&e1, &e2), 0);
  CU_ASSERT_PTR_NOT_NULL (e1);
  CU_ASSERT_PTR_NOT_NULL (e2);
  CU_ASSERT_EQUAL (canonical_form (&e1), 0);
  CU_ASSERT_EQUAL (canonical_form (&e2), 0);
  CU_ASSERT_EQUAL (get_ax_b (e1, &a, &b), 0);
  CU_ASSERT_EQUAL (a, 2);
  CU_ASSERT_EQUAL (b, 1);
  CU_ASSERT_EQUAL (get_ax_b (e2, &a, &b), 0);
  CU_ASSERT_EQUAL (a, -2);
  CU_ASSERT_EQUAL (b, 2);
  free_calc_element (e1);
  free_calc_element (e2);
  clear_line();
}

int main () {
  CU_pSuite cf_suite, axb_suite, calc_suite, canon_suite, lexer_suite, parser_suite;
  CU_initialize_registry ();
  cf_suite = CU_add_suite ("Create and free expression elements", NULL, NULL);
  CU_add_test (cf_suite, "Create and free number test", test_number_cf);
  CU_add_test (cf_suite, "Create and free variable x test", test_x_cf);
  CU_add_test (cf_suite, "Create and free log test", test_log_cf);
  CU_add_test (cf_suite, "Create and free binary expression test", test_binop_cf);
  axb_suite = CU_add_suite ("a*x + b creation and access", NULL, NULL);
  CU_add_test (axb_suite, "Create, free and access a*x + b test", test_create_access_axb);
  CU_add_test (axb_suite, "Bad a*x + b access test", test_bad_axb);
  calc_suite = CU_add_suite ("Simple calculations", NULL, NULL);
  CU_add_test (calc_suite, "Simple and correct calculations", test_calc_good);
  CU_add_test (calc_suite, "Bad calculations", test_calc_bad);
  canon_suite = CU_add_suite ("Canonical form, simple calculations", NULL, NULL);
  CU_add_test (canon_suite, "Canonical form, simple and correct calculations", test_canon_calc_good);
  CU_add_test (canon_suite, "Canonical form, bad calculations", test_canon_calc_bad);
  CU_add_test (canon_suite, "Canonical form, with an x", test_canon_x);
  lexer_suite = CU_add_suite ("Lexer", NULL, NULL);
  CU_add_test (lexer_suite, "Multiple tests", test_lexer);
  parser_suite = CU_add_suite ("Parser", NULL, NULL);
  CU_add_test (parser_suite, "Multiple tests", test_parser);
  CU_basic_set_mode (CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry ();
}
