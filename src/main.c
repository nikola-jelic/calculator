#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <math.h>

#include "parser.h"

#define LOWER_BOUND 0.0000001

int main () {
  /* deal with the command line arguments: TODO! */
  while (have_more()) {
    CALC_ELEMENT *e1 = NULL, *e2 = NULL;
    get_line ();
    if (parse_line (&e1, &e2) == 0) {
      if (e1 == NULL) {
	/* nothing */
      } else if (e2 == NULL) { /* simple calculation */
	if (e1->status & STATUS_X_PRESENT) { /* check for x */
	  fprintf (stderr, "x present in the \n");
	} else { /* calculation */
	  if (calculate (&e1) == 0) {
	    fprintf (stdout, "%g\n", e1->value);
	  } else { /* check for illegal conditions */
	    fprintf (stderr,
		     "the following errors were detected during the calculation:\n");
	    if (e1->status & STATUS_CALC_DIV_BY_ZERO)
	      fprintf (stderr, "\tdivison by zero\n");
	    if (e1->status & STATUS_CALC_DOMAIN)
	      fprintf (stderr, "\tlogarithm's argument smaller or equal to zero\n");
	  }
	}
	free_calc_element (e1);
      } else { /* linear equation */
	if (((e1->status & STATUS_X_PRESENT) == 0) && ((e2->status & STATUS_X_PRESENT) == 0))
	  fprintf (stderr, "x absent from the linear equation, checking the result anyway\n");
	
	if ((canonical_form (&e1) != 0) || (canonical_form (&e2) != 0)) {
	  int status = e1->status | e2->status;
	  fprintf (stderr,
		   "the following errors were detected while solving linear equation:\n");
	  if (status & STATUS_X_IN_DIV)
	    fprintf (stderr, "\tx present in division\n");
	  if (status & STATUS_X_IN_LOG)
	    fprintf (stderr, "\tx present in logarithm\n");
	  if (status & STATUS_X_NON_LINEAR)
	    fprintf (stderr, "\tequation is not linear (possible multiplications of two or more unknown variables)\n");
	  if (status & STATUS_CALC_DIV_BY_ZERO)
	    fprintf (stderr, "\tdivison by zero\n");
	  if (status & STATUS_CALC_DOMAIN)
	    fprintf (stderr, "\tlogarithm's argument smaller or equal to zero\n");
	  
	} else { /* we have something */
	  double a1, b1, a2, b2;
	  /* should not be possible, but checking anyway */
	  if ((get_ax_b (e1, &a1, &b1) != 0) || (get_ax_b (e2, &a2, &b2) != 0)) {
	    fprintf (stderr, "unknown error while solving linear equation\n");
	  } else {
	    if (fabs(a1 - a2) < LOWER_BOUND) { /* effectively zero */
	      if (fabs (b2 - b1) < LOWER_BOUND) {
		fprintf (stderr, "x could be any number\n");
	      } else {
		fprintf (stderr, "no solutions\n");
	      }
	    } else {
	      fprintf (stdout, "x = %g\n", (b2 - b1)/(a1 - a2));
	    }
	  }
	}
	free_calc_element (e1);
	free_calc_element (e2);
      }
    }
    clear_line();
  }
  
  return 0;
}
