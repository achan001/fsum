#include <math.h>
#define SC_STACK  48        // 2098 bit / 53 = min 40 doubles

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
  int i=0, n=last;
  double y, hi, lo;
  for(int j=0; j <= n; j++) {
    y = sum[j];
    hi = x + y;
#ifdef SC_BRANCH
    lo = (fabs(x) < fabs(y)) ? x - (hi - y) : y - (hi - x);
#else
    lo = hi - x;
    lo = (y - lo) + (x - (hi - lo));
#endif
    x = hi;
    if (lo) sum[i++] = lo;      // save partials
  }
  if (x - x != 0) {sum[ last = 0 ] = x; return;}
  sum[ last = i ] = x;

  if (i <= n && i != SC_STACK-1) return;
  for(n=i-1; n>0; ) {           // stack expanded or full
    x = sum[n];
    y = sum[n-1];
    sum[n--] = hi = x+y;
    sum[n--] = y - (hi-x);      // possibly 0
  }
  if (i == SC_STACK-1) *this += 0.0;
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
  if (i && (hi = x + (lo *= 2), lo == (hi - x))) {
    double z = i==1 ? sum[0] : sum[i-1] + sum[i-2];
    if ((lo < 0) == (z < 0) && z) return hi;
  }
  return x;
}
