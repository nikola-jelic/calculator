#ifndef PARSER_H
#define PARSER_H

#include "calc_element.h"
#include "lexer.h"
#include "symbols.h"

/**
 * Tries to parse one input line.
 * 
 * It will accept three types of lines:
 * * empty line, '\\n'
 * * simple expression 'exp\\n'
 * * linear equation 'exp=exp\\n'
 * @param[out] e1 Simple expression or left expression for linear equation; NULL for an 
 * empty line.
 * @param[out] e2 Right expression for linear equation, NULL otherwise.
 * @returns Zero for success, lesser that zero for failure.
 */
int parse_line (CALC_ELEMENT ** e1, CALC_ELEMENT ** e2);
/**
 * Tries to parse 'term((-|+)term)*'.
 * @param[out] e The resulting expression.
 * @returns Zero for success, lesser than zero for failure.
 */
int parse_expression (CALC_ELEMENT ** e);
/**
 * Tries to parse 'factor((*|/)factor)*'.
 * @param[out] e The resulting expression.
 * @returns Zero for success, lesser than zero for failure.
 */
int parse_term (CALC_ELEMENT ** e);
/**
 * Tries to parse the 'num|x|(exp)|log(exp)'.
 * @param[out] e The resulting expression.
 * @returns Zero for success, lesser than zero for failure.
 */
int parse_factor (CALC_ELEMENT ** e);
/**
 * Tries to parse an expression in the form of 'log(exp)'.
 * @param[out] e The resulting expression.
 * @returns Zero for success, lesser than zero for failure.
 */
int parse_log (CALC_ELEMENT ** e);
/**
 * Tries to parse an expression in the form of '(exp)'.
 * @param[out] e The resulting expression.
 * @returns Zero for success, lesser than zero for failure.
 */
int parse_paren_expr (CALC_ELEMENT ** e);

/**
 * Prints the error message to the error stream, taking into account the type of symbol
 * which caused the error, its position and the expected symbols.
 * @param[in] expected The string with the list of the expected symbols.
 */
void parser_error (char * expected);

#endif
