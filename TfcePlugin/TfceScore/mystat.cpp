#include "mystat.h"
#include "stat.h"
#include <cmath>

void ttest1sample(float data1[], unsigned long n1, float mu, float * t){
    float ave1, var1;
    avevar(data1, n1, &ave1, &var1);

    if(var1 == 0)
        *t = 0;
    else
        *t = (ave1 - mu)/(var1/sqrt(n1));
}
