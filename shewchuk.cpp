#include <string.h>
#include <stdlib.h>
#include <math.h>
#define SC_STACK  40        // 2098 bit / 53 <= 40 doubles

class sc_partials {         // modified shewchuk algorithm
  public:
    sc_partials() {*this = 0.0;}
    void operator+=(double x);
    void operator-=(double x) {return operator+=(-x);}
    void operator=(double x)  {sum[last = 0] = x;}
    operator double();

    int last;
    double sum[SC_STACK];
};

void sc_partials::operator+=(double x)
{
  int i=0;
  double y, hi, lo;
  for(int j=0; j <= last; j++) {
    y = sum[j];
    hi = x + y;
    lo = (fabs(x) < fabs(y)) ? x - (hi - y) : y - (hi - x);
    x = hi;
    if (lo) sum[i++] = x, x = lo;
  }
  if (isnan(x)) { last = 0; return; }
  sum[ last = i ] = x;
}

sc_partials::operator double()
{
  if (last == 0) return sum[0];
  double x = sum[0] + sum[1];
  if (last == 1) return x;
  double y = sum[1] - (x - sum[0]);
  double z = sum[2];
  if ((y < 0) == (z < 0) && z != 0) {
    z = x + (y *= 2);
    if (y == z - x) return z;   // half-way case
  }
  return x;
}
