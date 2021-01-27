#include <stdlib.h>
#include <math.h>

int ptb1(float a, float b,float* x){
    if (a==0) {
        return 0;
    }
    *x=-b/a;
    return 1;
}

int ptb2(float a, float b, float c,float* x1,float* x2){
    if (a==0) return ptb1(b,c,x1);
    float delta=b*b-4*a*c;
    if (delta>0){
        *x1=(-b+sqrt(delta))/(2*a);
        *x2=(-b-sqrt(delta))/(2*a);
        return 2;
    }
    else if(delta==0){
        *x1=(-sqrt(delta))/(2*a);
        *x2=*x1;
        return 3;
    }
    return 4;
}
