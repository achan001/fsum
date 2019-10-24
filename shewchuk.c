#include <string.h>
#include <stdlib.h>
#include <math.h>
#define SC_STACK  64        // 2098 bit / 53 = min 40 doubles

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
  if (i > 0 && isnan(x)) { sum->last = 0; return; }
  sum->p[ sum->last = i ] = x;
  if (i == SC_STACK - 1) sc_add(0.0, sum);
}

double sc_total(sc_partials *sum)
{
  for(;;) {
    int n = sum->last + 1;      // number of partials
    double prev[SC_STACK];
    memcpy(prev, sum->p, n * sizeof(double));
    sc_add(0.0, sum);           // remove partials overlap
    if (n == sum->last + 1)
      if (memcmp(prev, sum->p, n * sizeof(double)) == 0) break;
  }
  double x = sum->p[0], lo = sum->p[1];
  if (sum->last > 1 && (lo < 0) == (sum->p[2] < 0)) {
    double hi = x + (lo *= 2);
    if (lo == (hi - x)) x = hi; // half-way case
  }
  return x;
}
