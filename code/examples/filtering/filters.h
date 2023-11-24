#pragma once

typedef signed int sfint32_t;

#define mulsfint32_t(a,b) ((sfint32_t)(((( signed long long )(a))*(( signed long long )(b)))>>16)) //multiply two fixed 16:16
#define float_to_sfint32_t(a) ((sfint32_t)((a)*65536.0)) // 2^16
#define sfint32_t_to_float(a) ((float)(a)/65536.0)
#define sfint32_t_to_int(a)    ((int)((a)>>16))
#define int_to_sfint32_t(a)    ((sfint32_t)((a)<<16))
#define divsfint32_t(a,b) ((sfint32_t)((((signed long long)(a)<<16)/(b)))) 
#define abssfint32_t(a) abs(a)


typedef struct Coeff{
    //high pass diff equation coefficients
    sfint32_t l_a[3];
    sfint32_t l_b[3];
    //lower mid diff equation coefficients
    sfint32_t m1_a[3];
    sfint32_t m1_b[3];
    //middle mid diff equation coefficients
    sfint32_t m2_a[3];
    sfint32_t m2_b[3];
    //high mid diff equation coefficients
    sfint32_t m3_a[3];
    sfint32_t m3_b[3];
    //high pass diff equation coefficients
    sfint32_t h_a[3];
    sfint32_t h_b[3];
} Coeff;

typedef struct Gains {
    sfint32_t l;
    sfint32_t m1;
    sfint32_t m2;
    sfint32_t m3;
    sfint32_t h;
} Gains;

void lows(sfint32_t* x, sfint32_t* y, Coeff* coeffs);
void mid1s(sfint32_t* x, sfint32_t* y, Coeff* coeffs);
void mid2s(sfint32_t* x, sfint32_t* y, Coeff* coeffs);
void mid3s(sfint32_t* x, sfint32_t* y, Coeff* coeffs);
void highs(sfint32_t* x, sfint32_t* y, Coeff* coeffs);


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
void filter(sfint32_t* x, sfint32_t* y, Coeff* coeffs, Gains* gains);
