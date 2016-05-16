#ifndef S_CONV
#define S_CONV

#include <sstream>
#include <string>
#include <cmath>
using namespace std;

static inline float signf(float x){
    return (x > 0.f) ? 1.f : -1.f;
}
static inline int signi(float x){
    return (x > 0) ? 1 : (x==0) ? 0 : -1;
}

static const double TWO_PI = 6.28318530717958647692528676656;


static inline double mod(double x, double mod){
    return x - static_cast<int>(x/mod)*mod;
}

static inline double s_sin(double x)
{
    return sin(TWO_PI*x);
}

static inline double s_square(double x)
{
    return mod(static_cast<int>(x),2)-0.5;
}

static inline double s_tri(double x)
{
    return abs(2.*(mod((abs(x)),1.0)-0.5))-0.5;
}

static inline double s_saw(double x)
{
    return 2.*(mod((abs(x)),1.0)-0.5);
}

#endif
