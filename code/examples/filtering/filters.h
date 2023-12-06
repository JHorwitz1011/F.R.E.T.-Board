#pragma once
#include <stdint.h>

typedef signed int sfint;

//add/sub works as expected
#define mul_sfint(a,b) ((sfint)(((( signed long long )(a))*(( signed long long )(b)))>>16)) //multiply two fixed 16:16
// #define divsfint(a,b) ((sfint)((((signed long long)(a)<<15)/(b)))) 
// #define abssfint(a) abs(a)

#define float_to_sfint(a) (sfint)(a * 65536.0) // 2^15
#define sfint_to_float(a) (((float)(a)) / 65536.0)

// #define int2fix(a) (((fix))((a) << 15))
// #define fix2int(a) ((int))((a >> 15))
#define sfint_to_int(a)    ((int)((a)>>16))
#define int_to_sfint(a)    ((sfint)((a)<<16))

#define MIDPOINT 0x80000000 //65535/2
#define sfint_to_uint16(a) ((uint16_t)((a+MIDPOINT) >> 16))
#define uint16_to_sfint(a) ((sfint)((a<<16) + MIDPOINT))

#define HIGHB0 1.0
#define HIGHB1 -2.0
#define HIGHB2 1.0
#define HIGHA0 1.00
#define HIGHA1 -1.394137940850646
#define HIGHA2 0.5390287685792179

#define MID3B0 0.0504
#define MID3B1 0
#define MID3B2 -0.0504
#define MID3A0 1.00
#define MID3A1 -1.8527
#define MID3A2 0.8992

#define MID2B0 0.0205
#define MID2B1 0
#define MID2B2 -0.0205
#define MID2A0 1.00
#define MID2A1 -1.9516
#define MID2A2 0.9591

#define MID1B0 0.0082
#define MID1B1 0
#define MID1B2 -0.0082
#define MID1A0 1.00
#define MID1A1 -1.9824
#define MID1A2 0.9836

#define LOWB0 0.0001
#define LOWB1 0.0002
#define LOWB2 0.0001
#define LOWA0 1.00
#define LOWA1 -1.9752
#define LOWA2 0.9755

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

typedef struct Feedbacks{
    sfint l[3];
    sfint m1[3];
    sfint m2[3];
    sfint m3[3];
    sfint h[3];
} Feedbacks;



#define DEFAULT_LOW_GAIN 1.5f
#define DEFAULT_MID1_GAIN .00001f
#define DEFAULT_MID2_GAIN 1.0f
#define DEFAULT_MID3_GAIN .00001f
#define DEFAULT_HIGH_GAIN 1.5f

typedef struct Gains {
    sfint l;
    sfint m1;
    sfint m2;
    sfint m3;
    sfint h;
} Gains;

sfint lows(sfint* x, sfint* y, Coeff* coeffs);
sfint mid1s(sfint* x, sfint* y, Coeff* coeffs);
sfint mid2s(sfint* x, sfint* y, Coeff* coeffs);
sfint mid3s(sfint* x, sfint* y, Coeff* coeffs);
sfint highs(sfint* x, sfint* y, Coeff* coeffs);

float lowFloat(float* x, float* y);
float mid1Float(float* x, float* y);
float mid2Float(float* x, float* y);
float mid3Float(float* x, float* y);
float highFloat(float* x, float* y);

/***
 * 
 ***/
Coeff* initCoefficients();
void deinitCoefficients(Coeff* coeffs);

Gains* initGains();
void deinitGains(Gains* gains);

Feedbacks* initFeedbacks();
void deinitFeedbacks(Feedbacks* fb);

/***
 * Uses the input buffer to filter and construct the output buffer
 * It is assumed that x is updated with current values.
 * First, pop the output buffer leaving y[0] empty
 * Passes the x and y buffer y through all five filters and sums them to be constructs y[0]
 ***/
sfint filter(sfint* x, Feedbacks* fb, Coeff* coeffs, Gains* gains);
float filterFloat(float* x, float* y);

void stepFb(Feedbacks* fb);
void step(sfint* fbArray);

