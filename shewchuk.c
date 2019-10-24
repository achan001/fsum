#include <string.h>
#include <stdlib.h>
#include <math.h>
#define SC_STACK  40        // 2098 bit / 53 <= 40 doubles

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
}

double sc_total(sc_partials *sum)
{
  if (sum->last == 0) return sum->p[0];
  double x = sum->p[0] + sum->p[1];
  double z = sum->p[2];
  if (sum->last == 1 || z == 0) return x;
  double u = 2*(sum->p[1] - (x - sum->p[0]));
  return u==u+x-x && (u<0)==(z<0) ? x+u : x;
}
