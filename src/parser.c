#include "parser.h"

int parse_line(CALC_ELEMENT ** e1, CALC_ELEMENT ** e2)
{
	if (accept(PARSE_NLINE)) {
		return 0;
	}
	if (parse_expression(e1) != 0) {
		return -1;
	}
	if (accept(PARSE_EQUAL)) {
		if (parse_expression(e2) != 0) {
			free_calc_element(*e1);
			return -1;
		}
	}
	if (!accept(PARSE_NLINE)) {
		free_calc_element(*e1);
		if (*e2 != NULL) {
			free_calc_element(*e2);
			parser_error("\'\\n\'");
		} else
			parser_error("\'\\n\' or \'=\'");

		return -1;
	}
	return 0;
}

int parse_expression(CALC_ELEMENT ** e)
{
	CALC_ELEMENT *t1 = NULL, *t2 = NULL;
	PARSE_SYMBOL s;
	if (parse_term(&t1) != 0)
		return -1;
	/* we have one here, which is nice... */
	s = get_symbol();
	while ((s == PARSE_ADD) || (s == PARSE_SUB)) {
		accept(s);
		if (parse_term(&t2) != 0) {
			free_calc_element(t1);
			return -1;
		}
		if (s == PARSE_ADD)
			t1 = create_bin_op('+', t1, t2);
		else
			t1 = create_bin_op('-', t1, t2);
		t2 = NULL;
		s = get_symbol();
	}
	*e = t1;
	return 0;
}

int parse_term(CALC_ELEMENT ** e)
{
	CALC_ELEMENT *t1 = NULL, *t2 = NULL;
	PARSE_SYMBOL s;
	if (parse_factor(&t1) != 0)
		return -1;
	s = get_symbol();
	while ((s == PARSE_MULT) || (s == PARSE_DIV)) {
		accept(s);
		if (parse_factor(&t2) != 0) {
			free_calc_element(t1);
			return -1;
		}
		if (s == PARSE_MULT)
			t1 = create_bin_op('*', t1, t2);
		else
			t1 = create_bin_op('/', t1, t2);
		t2 = NULL;
		s = get_symbol();
	}
	*e = t1;
	return 0;
}

int parse_factor(CALC_ELEMENT ** e)
{
	PARSE_SYMBOL s = get_symbol();
	switch (s) {
	case PARSE_NUMBER:
		*e = create_number(get_current_number());
		accept(s);
		return 0;
		break;
	case PARSE_X:
		*e = create_x();
		accept(s);
		return 0;
		break;
	case PARSE_LOG:
		return parse_log(e);
		break;
	case PARSE_LPAREN:
		return parse_paren_expr(e);
		break;
	default:
		parser_error("\'number\', \'x\', \'log\' or \'(\'");
		return -1;
	}
}

int parse_log(CALC_ELEMENT ** e)
{
	CALC_ELEMENT *t = NULL;
	if (!accept(PARSE_LOG)) {
		parser_error("\'log\'");
		return -1;
	}
	if (parse_paren_expr(&t) == -1)
		return -1;
	else {
		*e = create_log(t);
		return 0;
	}
}

int parse_paren_expr(CALC_ELEMENT ** e)
{
	CALC_ELEMENT *loc = NULL;
	if (!accept(PARSE_LPAREN)) {
		parser_error("\'(\'");
		return -1;
	}
	if (parse_expression(&loc) != 0) {
		if (loc != NULL)
			free_calc_element(loc);
		return -1;
	}
	if (!accept(PARSE_RPAREN)) {
		parser_error("\')\'");
		if (loc != NULL)
			free_calc_element(loc);
		return -1;
	}
	*e = loc;
	return 0;
}

void parser_error(char *expected)
{
	fprintf(stderr, "parser error: unexpected symbol ");
	switch (get_symbol()) {
	case PARSE_NUMBER:
		fprintf(stderr, "\'number %g\'", get_current_number());
		break;
	case PARSE_X:
		fprintf(stderr, "\'x\'");
		break;
	case PARSE_ADD:
		fprintf(stderr, "\'+\'");
		break;
	case PARSE_SUB:
		fprintf(stderr, "\'-\'");
		break;
	case PARSE_MULT:
		fprintf(stderr, "\'*\'");
		break;
	case PARSE_DIV:
		fprintf(stderr, "\'/\'");
		break;
	case PARSE_LOG:
		fprintf(stderr, "\'log\'");
		break;
	case PARSE_LPAREN:
		fprintf(stderr, "\'(\'");
		break;
	case PARSE_RPAREN:
		fprintf(stderr, "\')\'");
		break;
	case PARSE_EQUAL:
		fprintf(stderr, "\'=\'");
		break;
	case PARSE_NLINE:
		fprintf(stderr, "\'\\n\'");
		break;
	case PARSE_BAD:
		fprintf(stderr, "\'unrecognized %c\'",
			in_line[get_current_symbol_pos()]);
		break;
	}
	fprintf(stderr, " at position %d, expected %s, skipping line\n",
		get_current_symbol_pos(), expected);
}
