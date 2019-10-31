#include <math.h>
#define SC_STACK  48
// max bits = 1023 - (-1074) + 1 = 2098
// SC_STACK > ceil(2098/53) = 40 doubles

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
  int i=0, n=sum->last;
  double y, hi, lo;
  for(int j=0; j <= n; j++) {
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

  if (i <= n && i != SC_STACK-1) return;
  for(n=i-1; n>0; ) {           // stack expanded or full
    x = sum->p[n];
    y = sum->p[n-1];
    sum->p[n--] = hi = x+y;
    sum->p[n--] = y - (hi-x);   // possibly 0
  }
  if (i == SC_STACK-1) sc_iadd(sum, 0.0);
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
  if (i && (hi = x + (lo *= 2), lo == (hi - x))) {
    double z = i==1 ? sum->p[0] : sum->p[i-1] + sum->p[i-2];
    if ((lo < 0) == (z < 0) && z) return hi;
  }
  return x;
}
