#ifndef PARSER_H
#define PARSER_H

#include "calc_element.h"
#include "lexer.h"
#include "symbols.h"

int parse_line (CALC_ELEMENT **, CALC_ELEMENT**);
int parse_expression (CALC_ELEMENT **);
int parse_term (CALC_ELEMENT **);
int parse_factor (CALC_ELEMENT **);
int parse_x (CALC_ELEMENT **);
int parse_number (CALC_ELEMENT **);
int parse_log (CALC_ELEMENT **);
int parse_paren_expr (CALC_ELEMENT **);

void parser_error (char *);

#endif
