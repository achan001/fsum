#include <stdlib.h>
#include <math.h>
#define SC_STACK  64        // 2098 bit / 53 = min 40 doubles

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
  if (i > 0 && isnan(x)) { last = 0; return; }
  sum[ last = i ] = x;
  if (i == SC_STACK - 1) *this += 0.0;
}

sc_partials::operator double()
{
  for(;;) {
    int n = last + 1;           // number of partials
    double prev[SC_STACK];
    memcpy(prev, sum, n * sizeof(double));
    *this += 0.0;               // remove partials overlap
    if (n == last + 1)
      if (memcmp(prev, sum, n * sizeof(double)) == 0) break;
  }
  double x = sum[0], lo = sum[1];
  if (last > 1 && (lo < 0) == (sum[2] < 0)) {
    double hi = x + (lo *= 2);
    if (lo == (hi - x)) x = hi; // half-way case
  }
  return x;
}
