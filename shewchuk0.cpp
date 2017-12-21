#include <stdlib.h>
#include <math.h>
#define SHEWCHUK_STACK  64      // 2098 bit / 53 = min 40 doubles

class partials {                // shewchuk algorithm
  public:
    partials() {last=-1;}
    void operator+=(double x);
    void operator-=(double x) {return operator+=(-x);}
    void operator=(double x)  {sum[last = 0] = x;}
    operator double() const;

    int last;
    int capacity;
    double sum[SHEWCHUK_STACK];
};

void partials::operator+=(double x)
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
  if (!i || !isfinite(x)) { sum[ last = 0 ] = x; return; }
  sum[ last = i ] = x;
  if (i == SHEWCHUK_STACK - 1) {x = 0.0; goto COMPRESS_STACK;}
}

partials::operator double() const
{
  int i = last;
  if (i <= 0) return i==0 ? sum[0] : 0.0;
  double y, hi, lo, x = sum[i];
  do {
    y = sum[--i];               // sum in reverse
    hi = x + y;
    if (i == 0) return hi;
    lo = y - (hi - x);
    x = hi;
  } while (lo == 0);
  if ((lo < 0) == (sum[i-1] < 0))
    if ((hi = x + (lo *= 2)), (lo == (hi - x)))
      x = hi;                   // half-way case
  return x;
}
