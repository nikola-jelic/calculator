#include "lexer.h"

char in_line[IN_BUF_SIZE];
int line_len = 0;
int line_pos = 0;

static double number = 0.0;
static int symbol_pos = 0;
static bool implicit_mult = false;
static bool symbol_accepted = true;
static PARSE_SYMBOL symbol;

PARSE_SYMBOL get_symbol(void)
{
	if (symbol_accepted) {
		next_symbol();
		symbol_accepted = false;
		return symbol;
	} else
		return symbol;
}

void next_symbol(void)
{
	if (line_pos >= line_len) {
		implicit_mult = false;
		symbol = PARSE_NLINE;
	} else {
		if (implicit_mult && ((in_line[line_pos] == '(')
				      || (in_line[line_pos] == 'x'))) {
			implicit_mult = false;
			symbol_pos = line_pos;
			symbol = PARSE_MULT;
			return;
		} else {
			implicit_mult = false;
			while ((line_pos < line_len)
			       && isblank(in_line[line_pos]))
				line_pos++;
			if (line_pos == line_len) {
				symbol_pos = line_pos;
				symbol = PARSE_NLINE;
				return;
			}
			symbol_pos = line_pos;
			switch (in_line[line_pos]) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (scan_number())
					symbol = PARSE_NUMBER;
				else
					symbol = PARSE_BAD;
				break;
			case 'x':
				symbol = PARSE_X;
				line_pos++;
				break;
			case '+':
				symbol = PARSE_ADD;
				line_pos++;
				break;
			case '-':
				symbol = PARSE_SUB;
				line_pos++;
				break;
			case '*':
				symbol = PARSE_MULT;
				line_pos++;
				break;
			case '/':
				symbol = PARSE_DIV;
				line_pos++;
				break;
			case 'l':
				if (scan_log()) {
					symbol = PARSE_LOG;
				} else {
					symbol = PARSE_BAD;
				}
				break;
			case '(':
				symbol = PARSE_LPAREN;
				line_pos++;
				break;
			case ')':
				symbol = PARSE_RPAREN;
				line_pos++;
				break;
			case '=':
				symbol = PARSE_EQUAL;
				line_pos++;
				break;
			case '\n':
			case '\r':
				symbol = PARSE_NLINE;
				line_pos++;
				break;
			default:
				symbol = PARSE_BAD;
				line_pos++;
			}
		}
	}
}

bool accept(PARSE_SYMBOL s)
{
	if (s == get_symbol()) {
		symbol_accepted = true;
		return true;
	} else {
		symbol_accepted = false;
		return false;
	}
}

void clear_line(void)
{
	line_pos = 0;
	line_len = 0;
	implicit_mult = false;
	symbol_accepted = true;
	symbol_pos = 0;
}

int get_current_symbol_pos(void)
{
	return symbol_pos;
}

bool have_more(void)
{
	return (feof(stdin) == 0);
}

void get_line(void)
{
	char *res = fgets(in_line, IN_BUF_SIZE, stdin);
	if (res == NULL)	/* nothing read */
		line_len = 0;
	else
		line_len = strlen(in_line);
}

double get_current_number(void)
{
	return number;
}

bool scan_log(void)
{
	if ((line_len - line_pos >= 3)
	    && strncmp(&in_line[line_pos], "log", 3) == 0) {
		line_pos += 3;
		return true;
	} else {
		line_pos += 1;
		return false;
	}
}

bool scan_number(void)
{
	int range = 0;
	/* [0-9]*.?[0-9]+ */
	/* first we try with plain numbers */
	while ((line_pos + range < line_len)
	       && isdigit(in_line[line_pos + range]))
		range++;
	/* decimal dot */
	if ((line_pos + range < line_len) && in_line[line_pos + range] == '.') {
		range++;
		/* mor numbers */
		while ((line_pos + range < line_len)
		       && isdigit(in_line[line_pos + range]))
			range++;
	}
	if (range == 0)
		return false;
	else {
		/* we must limit the range of function strtod, so as to avoid any undesired effects */
		char *tmp = (char *)malloc(sizeof(char) * (range + 1));
		strncpy(tmp, &in_line[line_pos], range);
		tmp[range] = '\0';
		number = strtod(tmp, NULL);
		free(tmp);
		line_pos += range;
		implicit_mult = true;
		return true;
	}
}
