#include <math.h>
#define SC_STACK  64        // 2098 bit / 53 = min 40 doubles

class sc_partials {         // shewchuk algorithm
  public:
    sc_partials() {*this = 0.0;}
    void operator+=(double x);
    void operator-=(double x) {return operator+=(-x);}
    void operator=(double x)  {sum[last = 0] = x;}
    operator double() const;

    int last;
    double sum[SC_STACK];
};

void sc_partials::operator+=(double x)
{
COMPRESS_STACK:;
  int i=0;
  double y, hi, lo;
  for(int j=0; j <= last; j++) {
    y = sum[j];
    hi = x + y;
    lo = (fabs(x) < fabs(y)) ? x - (hi - y) : y - (hi - x);
    x = hi;
    if (lo) sum[i++] = lo;      // save partials
  }
  if (!i || !isfinite(x)) {sum[ last = 0 ] = x; return;}
  sum[ last = i ] = x;
  if (i == SC_STACK - 1) {x = 0.0; goto COMPRESS_STACK;}
}

sc_partials::operator double() const
{
  int i = last;
  if (i == 0) return sum[0];
  double lo, hi, x = sum[i];
  do {
    lo = sum[--i];              // sum in reverse
    hi = x + lo;
    lo -= (hi - x);
    x = hi;
  } while (i && lo == 0);
  if (i && (lo < 0) == (sum[i-1] < 0))
    if ((hi = x + (lo *= 2)), (lo == (hi - x)))
      x = hi;                   // half-way case
  return x;
}
