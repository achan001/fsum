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

void sc_add(double x, sc_partials *sum)
{
  int i=0;
  double y, hi, lo;
  for(int j=0; j <= sum->last; j++) {
    y = sum->p[j];
    hi = x + y;
    lo = (fabs(x) < fabs(y)) ? x - (hi - y) : y - (hi - x);
    x = hi;
    if (lo) sum->p[i++] = x, x = lo;
  }
  if (isnan(x)) { sum->last = 0; return; }
  sum->p[ sum->last = i ] = x;
  if (i == SC_STACK - 1) sc_add(0.0, sum);
}

double sc_total(sc_partials *sum)
{
  for(;; sc_add(0.0, sum)) {
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
