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

#define IN_BUF_SIZE 1024

extern char in_line[IN_BUF_SIZE];
extern int line_len;
extern int line_pos;

double get_current_number (void);

PARSE_SYMBOL get_symbol (void);
void next_symbol(void);
bool accept (PARSE_SYMBOL s);
void clear_line (void);
bool have_more (void);
void get_line (void);

/* helper functions */
bool scan_log (void);
bool scan_number (void);

#endif
