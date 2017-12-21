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
COMPRESS_STACK:;
  int i=0;
  double y, hi, lo;
  for(int j=0; j <= sum->last; j++) {
    y = sum->p[j];
    hi = x + y;
    lo = (fabs(x) < fabs(y)) ? x - (hi - y) : y - (hi - x);
    x = hi;
    if (lo) sum->p[i++] = lo;   // save partials
  }
  if (!i || !isfinite(x)) { sum->p[ sum->last = 0 ] = x; return; }
  sum->p[ sum->last = i ] = x;
  if (i == SHEWCHUK_STACK - 1) {x = 0.0; goto COMPRESS_STACK;}
}

double sum_partials(partials *sum)
{
  int i = sum->last;
  if (i <= 0) return i==0 ? sum->p[0] : 0.0;
  double y, hi, lo, x = sum->p[i];
  do {
    y = sum->p[--i];            // sum in reverse
    hi = x + y;
    if (i == 0) return hi;
    lo = y - (hi - x);
    x = hi;
  } while (lo == 0);
  if ((lo < 0) == (sum->p[i-1] < 0))
    if ((hi = x + (lo *= 2)), (lo == (hi - x)))
      x = hi;                   // half-way case
  return x;
}
