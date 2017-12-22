#include <stdlib.h>
#include <math.h>
#define SHEWCHUK_STACK  64      // 2098 bit / 53 = min 40 doubles

typedef struct {
  int last;
  double p[SHEWCHUK_STACK];
} partials;

void sum_init(partials *sum)
{
  sum->last = -1;               // no numbers yet
}

void shewchuk_add(double x, partials *sum)
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
  if (i == SHEWCHUK_STACK - 1) shewchuk_add(0.0, sum);
}

double shewchuk_total(partials *sum)
{
  for(;;) {
    int n = sum->last + 1;      // number of partials
    double prev[n];
    memcpy(prev, sum->p, n * sizeof(double));
    shewchuk_add(0.0, sum);     // remove partials overlap
    if (n == sum->last + 1)
      if (memcmp(prev, sum->p, n * sizeof(double)) == 0) break;
  }
  double x = sum->p[0];
  double lo = sum->p[1];
  if (sum->last > 1 && (lo < 0) == (sum->p[2] < 0)) {
    double hi = x + (lo *= 2);
    if (lo == (hi - x)) x = hi; // half-way case
  }
  return x;
}
