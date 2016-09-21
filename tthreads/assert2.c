#include "assert2.h"
#include "stdio.h"
#include "signal.h"
#include <sys/types.h>		/* for for getpid */
#include <unistd.h>		/* for getipid */

void __assert2(const char *filename, int linenum, const char *expr, char* msg)
{
  fprintf(stderr, "Assertion error: %s\n", msg);
  fprintf(stderr, " %s:%d (%s)\n", filename, linenum, expr);
  fflush(stderr);
  kill(getpid(), SIGKILL);	/* kill present proces */
}
