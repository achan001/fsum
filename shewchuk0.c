#include <math.h>
#define SC_STACK  48        // 2098 bit / 53 = min 40 doubles

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
    if (lo) sum->p[i++] = lo;   // save partials
  }
  if (x - x != 0) {sum->p[ sum->last = 0 ] = x; return;}
  sum->p[ sum->last = i ] = x;
  if (i == SC_STACK - 1) sc_iadd(sum, 0.0);
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
  } while (i && lo == 0);
  if (i && (hi = x + (lo *= 2), lo == (hi - x)))
    if ((lo < 0) == (sum->p[i-1] < 0))
      return hi;                // half-way case
  return x;
}
