/***** interface.h **********************************************************
 * Description: Midpoint rooting of phylogenies
 * Author: Bernhard Haubold
 * Email: haubold@evolbio.mpg.de
 * License: GNU General Public License, https://www.gnu.org/licenses/gpl.html
 * Date: Tue Apr 30 10:39:30 2019
 ****************************************************************************/
#ifndef INTERFACE
#define INTERFACE

/* define argument container */
typedef struct args{
  char    h; /* help message?         */
  char    v; /* version message?      */
  char  err; /* error                 */
  char **fi; /* input files           */
  int    nf; /* number of input files */
} Args;

Args *getArgs(int argc, char *argv[]);
Args *newArgs();
void freeArgs(Args *args);
void printUsage();
void printSplash(Args *args);

#endif
