#pragma once

typedef signed int sfint;

#define mulsfint(a,b) ((sfint)(((( signed long long )(a))*(( signed long long )(b)))>>16)) //multiply two fixed 16:16
#define float_to_sfint(a) ((sfint)((a)*65536.0)) // 2^16
#define sfint_to_float(a) ((float)(a)/65536.0)
#define sfint_to_int(a)    ((int)((a)>>16))
#define int_to_sfint(a)    ((sfint)((a)<<16))
#define divsfint(a,b) ((sfint)((((signed long long)(a)<<16)/(b)))) 
#define abssfint(a) abs(a)


typedef struct Coeff{
    //high pass diff equation coefficients
    sfint l_a[3];
    sfint l_b[3];
    //lower mid diff equation coefficients
    sfint m1_a[3];
    sfint m1_b[3];
    //middle mid diff equation coefficients
    sfint m2_a[3];
    sfint m2_b[3];
    //high mid diff equation coefficients
    sfint m3_a[3];
    sfint m3_b[3];
    //high pass diff equation coefficients
    sfint h_a[3];
    sfint h_b[3];
} Coeff;

typedef struct Gains {
    sfint l;
    sfint m1;
    sfint m2;
    sfint m3;
    sfint h;
} Gains;

void lows(sfint* x, sfint* y, Coeff* coeffs);
void mid1s(sfint* x, sfint* y, Coeff* coeffs);
void mid2s(sfint* x, sfint* y, Coeff* coeffs);
void mid3s(sfint* x, sfint* y, Coeff* coeffs);
void highs(sfint* x, sfint* y, Coeff* coeffs);


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
void filter(sfint* x, sfint* y, Coeff* coeffs, Gains* gains);
