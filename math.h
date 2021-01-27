#include <stdlib.h>
#include <math.h>

int ptb1(float a, float b,float* x){
    if (a==0) {
        return 0;
    }
    *x=-b/a;
    return 1;
}

void swapNum(float* a, float* b){
    float temp=*a;
    *a=*b;
    *b=temp;
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

void pathFunction (float x1,float y1,float x2,float y2,float *a,float *b){
    *a=(y2-y1)/(x2-x1);
    *b=y1-(*a)*x1;
}

void perpendicularFunction (float x1,float y1,float a1,float b1, float *a2, float *b2){
    *a2=-1/a1;
    *b2=y1-(*a2)*x1;
}

// tim diem thoa khoang cach diem cho truoc - chi tra ve bien x
void distancePt(float x1,float y1,int distance,float a,float b,float *x2_1,float *x2_2){
    float C=x1*x1+pow(y1-b,2)-distance*distance;
    float B=-(2*x1+2*a*y1-2*a*b);
    float A=a*a+1;
    ptb2(A,B,C,x2_1,x2_2);
    if (*x2_1<*x2_2) swapNum(x2_1,x2_2); // x1>=x2
}

float tinhY(float x,float a, float b){
    return a*x+b;
}

float tinhX(float y,float a, float b){
    return (y-b)/a;
}

