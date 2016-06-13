#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <math.h>

#include "parser.h"

#define LOWER_BOUND 0.0000001 /*!< Everything smaller than this is considered zero. */

char *input = NULL, *output = NULL, *errlog = NULL;

/**
 * Cleans up any memory potentially taken by the names of redirection files.
 */
void cleanup(void);
/**
 * Prints the command line arguments for the application and exits.
 */
void print_usage (void);

int main (int argc, char *argv[]) {
  int opt;
  atexit (cleanup);
  /* deal with the command line arguments */
  while ((opt = getopt (argc, argv, "i:o:e:vh")) != -1) {
    switch (opt) {
    case 'i': /* input file */
      if (input != NULL)
	free(input);
      input = strdup (optarg);
      break;
    case 'o': /* output file */
      if (output != NULL)
	free(output);
      output = strdup (optarg);
      break;
    case 'e': /* error log file */
      if (errlog != NULL)
	free(errlog);
      errlog = strdup (optarg);
      break;
    case 'v': /* version number, 1.0 for now */
      printf ("calculator version: 1.0\nauthor: Nikola Jelic <nikola.jelic83@gmail.com>\n");
      exit (EXIT_SUCCESS);
      break;
    case 'h': /* print usage help */
      print_usage ();
      exit (EXIT_SUCCESS);
      break;
    default:
      error(1, 0, "Try \"calc -h\" for more info");
    }
  }

  /* redirect anything that needs redirecting */
  if (input != NULL) {
    if (freopen (input, "r", stdin) == NULL)
      error (1, errno, "freopen %s", input);
    free (input);
    input = NULL;
  }
  if (output != NULL) {
    if (freopen (output, "w", stdout) == NULL)
      error (1, errno, "freopen %s", output);
    free (output);
    output = NULL;
  }
  if (errlog != NULL) {
    if (freopen (errlog, "a", stderr) == NULL)
      error (1, errno, "freopen %s", errlog);
    free (errlog);
    errlog = NULL;
  }
  
  while (have_more()) {
    CALC_ELEMENT *e1 = NULL, *e2 = NULL;
    get_line ();
    if (parse_line (&e1, &e2) == 0) {
      if (e1 == NULL) {
	/* nothing */
      } else if (e2 == NULL) { /* simple calculation */
	if (e1->status & STATUS_X_PRESENT) { /* check for x */
	  fprintf (stderr, "x present in the expression\n");
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
  
  exit (EXIT_SUCCESS);
}

void cleanup(void) {
  if (input != NULL)
    free(input);
  if (output != NULL)
    free(output);
  if (errlog != NULL)
    free(errlog);
}

void print_usage (void) {
  printf ("Usage: calc [OPTIONS]\n\n");
  printf ("Simple command line calculator and linear equations solver.\n");
  printf ("If no redirections are given, standard input and output are used.\n\n");
  printf ("\t-i <file>\t Will use <file> instead of standard input.\n");
  printf ("\t-o <file>\t Will use <file> instead of standard output.\n");
  printf ("\t-e <file>\t Will use <file> instead of standard error, and will append to it.\n");
  printf ("\t-v\t\t Prints version number and exits.\n");
  printf ("\t-h\t\t Prints this message and exits.\n");
}

