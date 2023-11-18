#pragma once

typedef signed int s7x16;

#define muls7x16(a,b) ((s7x16)(((( signed long long )(a))*(( signed long long )(b)))>>16)) //multiply two fixed 16:16
#define float_to_s7x16(a) ((s7x16)((a)*65536.0)) // 2^16
#define s7x16_to_float(a) ((float)(a)/65536.0)
#define s7x16_to_int(a)    ((int)((a)>>16))
#define int_to_s7x16(a)    ((s7x16)((a)<<16))
#define divs7x16(a,b) ((s7x16)((((signed long long)(a)<<16)/(b)))) 
#define abss7x16(a) abs(a)


typedef struct Coeff{
    //high pass diff equation coefficients
    s7x16 l_a[3];
    s7x16 l_b[3];
    //lower mid diff equation coefficients
    s7x16 m1_a[3];
    s7x16 m1_b[3];
    //middle mid diff equation coefficients
    s7x16 m2_a[3];
    s7x16 m2_b[3];
    //high mid diff equation coefficients
    s7x16 m3_a[3];
    s7x16 m3_b[3];
    //high pass diff equation coefficients
    s7x16 h_a[3];
    s7x16 h_b[3];
} Coeff;

typedef struct Gains {
    s7x16 l;
    s7x16 m1;
    s7x16 m2;
    s7x16 m3;
    s7x16 h;
} Gains;

void lows(s7x16* x, s7x16* y, Coeff* coeffs);
void mid1s(s7x16* x, s7x16* y, Coeff* coeffs);
void mid2s(s7x16* x, s7x16* y, Coeff* coeffs);
void mid3s(s7x16* x, s7x16* y, Coeff* coeffs);
void highs(s7x16* x, s7x16* y, Coeff* coeffs);


/***
 * 
 ***/
Coeff* initCoefficients();
void deinitCoefficients(Coeff* coeffs);

Gains* initGains();
void deinitGains(Gains* gains);
/***
 * Uses the input buffer to filter and construct the output buffer
 * It is assumed that x is updated with current values.
 * First, pop the output buffer leaving y[0] empty
 * Passes the x and y buffer y through all five filters and sums them to be constructs y[0]
 ***/
void filter(s7x16* x, s7x16* y, Coeff* coeffs, Gains* gains);
