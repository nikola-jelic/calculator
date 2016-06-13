#ifndef SYMBOLS_H
#define SYMBOLS_H

/**
 * Types of parsing symbols.
 */
typedef enum {
	PARSE_NUMBER,		/*!< A floating point number in the form [0-9]*.?[0-9]+ */
	PARSE_X,		/*!< Unknown variable 'x' */
	PARSE_ADD,		/*!< Addition '+' */
	PARSE_SUB,		/*!< Substraction '-' */
	PARSE_MULT,		/*!< Multiplication '*' or implied, look [next_symbol](@ref next_symbol) */
	PARSE_DIV,		/*!< Division '/' */
	PARSE_LOG,		/*!< Logarithm 'log' */
	PARSE_LPAREN,		/*!< Left parenthesis '(' */
	PARSE_RPAREN,		/*!< Right parenthesis ')' */
	PARSE_EQUAL,		/*!< Equals '=' */
	PARSE_NLINE,		/*!< New line '\\n' */
	PARSE_BAD		/*!< Any unrecognized symbol by the lexer */
} PARSE_SYMBOL;

#endif
