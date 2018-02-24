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
  if (!i || !isfinite(x)) {sum->p[ sum->last = 0 ] = x; return;}
  sum->p[ sum->last = i ] = x;
  if (i == SC_STACK - 1) {x = 0.0; goto COMPRESS_STACK;}
}

double sc_total(sc_partials *sum)
{
  int i = sum->last;
  if (i == 0) return sum->p[0];
  double lo, hi, x = sum->p[i];
  do {
    lo = sum->p[--i];           // sum in reverse
    hi = x + lo;
    lo -= (hi - x);
    x = hi;
  } while (lo == 0 && i);
  if (i && (lo < 0) == (sum->p[i-1] < 0))
    if ((hi = x + (lo *= 2)), (lo == (hi - x)))
      x = hi;                   // half-way case
  return x;
}
