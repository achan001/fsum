#include <string.h>
#include <stdlib.h>
#include <math.h>
#define SC_STACK  48

// max bits = 1023 - (-1022) + 53 = 2098
// min doubles = 2098/53 <= 40 (all halfway cases)
// min SC_STACK = 40+1 = 41

typedef struct {
  int last;
  double p[SC_STACK];
} sc_partials;

void sc_init(sc_partials *sum)
{
  sum->p[sum->last = 0] = 0.0;
}

void sc_iadd(sc_partials *sum, double x)
{
  int i=0;
  double y, hi, lo;
  for(int j=0; j <= sum->last; j++) {
    y = sum->p[j];
    hi = x + y;
#ifdef SC_BRANCH
    lo = (fabs(x) < fabs(y)) ? x - (hi - y) : y - (hi - x);
#else
    lo = hi - x;
    lo = (y - lo) + (x - (hi - lo));
#endif
    x = hi;
    if (lo) sum->p[i++] = x, x = lo;
  }
  if (x != x) { sum->last = 0; return; }
  sum->p[ sum->last = i ] = x;
  if (i == SC_STACK - 1) sc_iadd(sum, 0.0);
}

double sc_total(sc_partials *sum)
{
  for(;; sc_iadd(sum, 0.0)) {
    int n = sum->last;
    if (n == 0) return sum->p[0];
    if (n == 1) return sum->p[0] + sum->p[1];
    double y, *x = &sum->p[n];
    while(y=0.5 * *x--, --n && *x == *x + y) ;
    if (*x == *x + y) {         // partials no overlap
      double r = *x + (y += y); // *x + 2y => r + y/2
      y = 2*(y - (r - *x));
      if (y != y+r-r) return r; // 0 < |y| < 1 ULP
      return (y<0) == (x[2]<0) && x[2] ? r+y : r;
    }
  }
}
