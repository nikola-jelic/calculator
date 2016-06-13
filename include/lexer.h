#ifndef LEXER_H
#define LEXER_H

#include "symbols.h"
#include <errno.h>
#include <error.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define IN_BUF_SIZE 1024 /*!< The size of the input buffer. */

extern char in_line[IN_BUF_SIZE]; /*!< Input buffer. */
extern int line_len; /*!< Length of the line in the [input buffer](@ref in_line). */
extern int line_pos; /*!< Current position of the lexer in the input buffer. */

/**
 * Gets the last floating point number recognized by the lexer.
 * @returns The floating point number.
 */
double get_current_number (void);

/**
 * Gets the current symbol recognized by the lexer. If current symbol is accepted, calls
 * [next_symbol](@ref next_symbol) internally before returning the symbol.
 * @returns Current symbol.
 */
PARSE_SYMBOL get_symbol (void);
/**
 * Advances to the next symbol, changes only internal states of the lexer. Takes special
 * care to recognize implicit multiplications in two forms 'nx' or 'n(exp)', where n is any
 * floating point number; in such cases, [multiplication](@ref PARSE_MULT) symbol will be
 * emitted, even though the character '*' is absent. Not that no spaces are allowed between
 * the number and the following symbol.
 */
void next_symbol(void);
/**
 * Tries to accept an expected symbol.
 * @param[in] s Expected symbol.
 * @returns true if symbol is accepted, false otherwise.
 */
bool accept (PARSE_SYMBOL s);
/**
 * Resets internal states of the lexer and clears the input buffer.
 */
void clear_line (void);
/**
 * Retrieves the position of the current symbol in the input buffer.
 * @returns The position of the symbol.
 */
int get_current_symbol_pos (void);
/**
 * Checks the input stream for the availability of more input.
 * @returns true if more data can be read from the input stream, false otherwise.
 */
bool have_more (void);
/**
 * Reads new line into the input buffer and sets internal states of the lexer accordingly.
 */
void get_line (void);

/* helper functions */
/**
 * Tries to scan [logarithm](@ref PARSE_LOG).
 * @returns true for success, false for failure.
 */
bool scan_log (void);
/**
 * Tries to scan a [number](@ref PARSE_NUMBER).
 * @returns true for success, false for failure.
 */
bool scan_number (void);

#endif
