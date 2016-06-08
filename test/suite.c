#include "calc_element.h"
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

int main () {
  CU_pSuite cf_suite;
  CU_initialize_registry ();
  cf_suite = CU_add_suite ("Create and free expression elements", NULL, NULL);
  CU_add_test (cf_suite, "Create and free number test", test_number_cf);
  CU_add_test (cf_suite, "Create and free variable x test", test_x_cf);
  CU_add_test (cf_suite, "Create and free log test", test_log_cf);
  CU_add_test (cf_suite, "Create and free binary expression test", test_binop_cf);

  CU_basic_set_mode (CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry ();
}
