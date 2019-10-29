#include <math.h>
#define SC_STACK  42        // 2098 bit / 53 = min 40 doubles

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
#ifdef SC_BRANCH
    lo = (fabs(x) < fabs(y)) ? x - (hi - y) : y - (hi - x);
#else
    lo = hi - x;
    lo = (y - lo) + (x - (hi - lo));
#endif    
    x = hi;
    if (lo) sum[i++] = x, x = lo;
  }
  if (x != x) { last = 0; return; }
  sum[ last = i ] = x;
  if (i == SC_STACK-1) *this += 0.0;
}

sc_partials::operator double()
{
  for(;; *this += 0.0) {
    int n = last;
    if (n == 0) return sum[0];
    if (n == 1) return sum[0] + sum[1];
    double y, *x = &sum[n];
    while(y=0.5 * *x--, --n && *x == *x + y) ;
    if (*x == *x + y) {         // partials no overlap
      double r = *x + (y += y); // *x + 2y => r + y/2
      y = 2*(y - (r - *x));
      if (y != y+r-r) return r; // 0 < |y| < 1 ULP
      return (y<0) == (x[2]<0) && x[2] ? r+y : r;
    }
  }
}
