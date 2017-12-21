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

void sum_shewchuk(double x, partials *sum)
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
  if (i == SHEWCHUK_STACK - 1) sum_shewchuk(0.0, sum);
}

double sum_partials(partials *sum)
{
  for(;;) {
    int n = sum->last + 1;      // number of partials
    double prev[n];
    memcpy(prev, sum->p, n * sizeof(double));
    sum_shewchuk(0.0, sum);     // remove partials overlap
    if (n <= 2) return sum->p[0];
    if (n == sum->last + 1)     // prev == sum->p imply no overlap
      if (memcmp(prev, sum->p, n * sizeof(double)) == 0) break;
  }
  double x = sum->p[0];
  double lo = sum->p[1];
  double err = sum->p[2];
  if ((lo < 0) == (err < 0)) {
    double hi = x + (lo *= 2);
    if (lo == (hi - x)) x = hi; // half-way case
  }
  return x;
}
