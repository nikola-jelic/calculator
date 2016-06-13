#include "calc_element.h"
#include <errno.h>
#include <fenv.h>
#include <math.h>
#include <stdlib.h>

CALC_ELEMENT *create_number(double val)
{
	CALC_ELEMENT *res = (CALC_ELEMENT *) malloc(sizeof(CALC_ELEMENT));
	res->calc_t = CALC_NUM;
	res->status = 0;
	res->value = val;
	res->left = NULL;
	res->right = NULL;
	return res;
}

CALC_ELEMENT *create_bin_op(char op, CALC_ELEMENT * l, CALC_ELEMENT * r)
{
	CALC_ELEMENT *res = (CALC_ELEMENT *) malloc(sizeof(CALC_ELEMENT));
	res->calc_t = CALC_BIN_OP;
	res->bin_op = (op != '-' ? op : '+');	/* simplification of substraction */
	res->status = l->status | r->status;
	if (op == '-')
		r->value *= -1.0;
	if (op == '/' && (res->status & STATUS_X_PRESENT))
		res->status |= STATUS_X_IN_DIV;
	res->value = 1.0;
	res->left = l;
	res->right = r;
	return res;
}

CALC_ELEMENT *create_x(void)
{
	CALC_ELEMENT *res = (CALC_ELEMENT *) malloc(sizeof(CALC_ELEMENT));
	res->calc_t = CALC_X;
	res->status = STATUS_X_PRESENT;
	res->value = 1.0;
	res->left = NULL;
	res->right = NULL;
	return res;
}

CALC_ELEMENT *create_log(CALC_ELEMENT * e)
{
	CALC_ELEMENT *res = (CALC_ELEMENT *) malloc(sizeof(CALC_ELEMENT));
	res->calc_t = CALC_LOG;
	res->status = e->status;
	if (res->status & STATUS_X_PRESENT)
		res->status |= STATUS_X_IN_LOG;
	res->value = 1.0;
	res->left = e;
	res->right = NULL;
	return res;
}

void free_calc_element(CALC_ELEMENT * e)
{
	if (e == NULL)
		return;
	switch (e->calc_t) {
	case CALC_NUM:
	case CALC_X:
		free(e);
		break;
	case CALC_BIN_OP:
		free_calc_element(e->left);
		free_calc_element(e->right);
		free(e);
		break;
	case CALC_LOG:
		free_calc_element(e->left);
		free(e);
		break;
	}
}

int calculate(CALC_ELEMENT ** e)
{
	CALC_ELEMENT *loc = *e;
	switch (loc->calc_t) {
	case CALC_NUM:
		return 0;
		break;
	case CALC_X:		/* should not ever happen */
		return -1;
		break;
	case CALC_BIN_OP:
		/* first validate the left and the right side of the equation */
		if ((calculate(&loc->left) == 0)
		    && (calculate(&loc->right) == 0)) {
			switch (loc->bin_op) {
			case '+':
				*e = create_number(loc->value *
						   (loc->left->value +
						    loc->right->value));
				free_calc_element(loc);
				return 0;
				break;
			case '*':
				*e = create_number(loc->value *
						   (loc->left->value *
						    loc->right->value));
				free_calc_element(loc);
				return 0;
				break;
			case '/':
				/* we must check the right side for zero */
				if (loc->right->value == 0.0) {
					loc->status |= STATUS_CALC_DIV_BY_ZERO;
					return -1;
				} else {
					*e = create_number(loc->value *
							   (loc->left->value /
							    loc->right->value));
					free_calc_element(loc);
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
		if (calculate(&loc->left) == 0) {
			if (loc->left->calc_t == CALC_NUM) {
				if (loc->left->value <= 0.0) {
					loc->status |= STATUS_CALC_DOMAIN;
					return -1;
				} else {
					/* replace the log element with a number */
					*e = create_number(log(loc->left->value)
							   * loc->value);
					free_calc_element(loc);
					return 0;
				}
			} else {	/* this should also never happen */
				return -1;
			}
		} else {
			loc->status |= loc->left->status;
			return -1;
		}
		break;
	}
	return 0;
}

int canonical_form(CALC_ELEMENT ** e)
{
	CALC_ELEMENT *loc = *e;
	double a1 = 0.0, b1 = 0.0, a2 = 0.0, b2 = 0.0;
	if (loc->status & STATUS_X_PRESENT) {	/* x present, transform to canonical form */
		if (loc->status & MASK_X_ERROR)	/* something is not just right... */
			return -1;
		/* everything is ok for now... */
		switch (loc->calc_t) {
		case CALC_X:
			/* nothing to be done */
			return 0;
			break;
		case CALC_BIN_OP:
			/* the interesting case and sub-cases */
			if ((canonical_form(&loc->left) != 0)
			    || (canonical_form(&loc->right) != 0)) {
				loc->status =
				    loc->left->status | loc->right->status;
				return -1;
			}
			/* nothing unexpected happened during the canonization */

			if ((get_ax_b(loc->left, &a1, &b1) != 0)
			    || (get_ax_b(loc->right, &a2, &b2) != 0))
				return -1;
			switch (loc->bin_op) {
			case '+':	/* covers both + and - */
				*e = create_ax_b(loc->value * (a1 + a2),
						 loc->value * (b1 + b2));
				free_calc_element(loc);
				return 0;
				break;
			case '*':
				if ((a1 != 0.0) && (a2 != 0.0)) {
					/* we have a non-linear equation on our hands */
					loc->status |= STATUS_X_NON_LINEAR;
					return -1;
				} else {
					/* we know that at least one a is a zero */
					*e = create_ax_b(a1 * b2 + a2 * b1,
							 b1 * b2);
					free_calc_element(loc);
					return 0;
				}
				break;
			}
			break;
		default:
			/* we should never get here, x can only be alone or in an arithmetic expression */
			return -1;
			break;
		}
	} else {		/* x absent, simply calculate */
		return calculate(e);
	}
	return 0;
}

CALC_ELEMENT *create_ax_b(double a, double b)
{
	if (a == 0.0) {
		return create_number(b);
	} else {
		if (b == 0.0) {
			CALC_ELEMENT *res = create_x();
			res->value *= a;
			return res;
		} else {
			CALC_ELEMENT *left = create_x();
			left->value *= a;
			CALC_ELEMENT *right = create_number(b);
			return create_bin_op('+', left, right);
		}
	}
}

int get_ax_b(CALC_ELEMENT * e, double *a, double *b)
{
	switch (e->calc_t) {
	case CALC_NUM:
		*a = 0.0;
		*b = e->value;
		return 0;
		break;
	case CALC_X:
		*b = 0.0;
		*a = e->value;
		return 0;
		break;
	case CALC_BIN_OP:
		if (e->bin_op == '+') {
			CALC_ELEMENT *num = NULL;
			CALC_ELEMENT *x = NULL;
			if ((e->left->calc_t == CALC_NUM)
			    && (e->right->calc_t == CALC_X)) {
				num = e->left;
				x = e->right;
			} else if ((e->left->calc_t == CALC_X)
				   && (e->right->calc_t == CALC_NUM)) {
				num = e->right;
				x = e->left;
			} else
				return -1;	/* should be impossible */
			*a = x->value * e->value;
			*b = num->value * e->value;
			return 0;
		} else		/* should be impossible */
			return -1;
		break;
	default:
		/* should be impossible */
		return -1;
		break;
	}
}
