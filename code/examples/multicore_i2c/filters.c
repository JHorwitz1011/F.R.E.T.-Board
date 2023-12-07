#include "filters.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define FIXED2 int_to_sfint(2)

//======================================================== 
// Second order Butterworth lowpass
// x is the input signal
// y is the output signal

void printTest(sfint a, sfint b) {
    printf("printing..., %08x, %08x, %08x\n", a, b, mul_sfint(a,b));
}

// sfint mulx(sfint a, sfint b) {
//     // printf("a: %f, b: %f, axb %f, FLOAT: a: %f, b: %f, axb: %f\n", a/65536.0, b/65536.0, mul_sfint(a,b)/65536.0, sfint_to_float(a), sfint_to_float(b), sfint_to_float(a) * sfint_to_float(b));
//     if(sfint_to_float(a) * sfint_to_float(b) > 32767 || sfint_to_float(a) * sfint_to_float(b) < -32768 ) {
//         printf("., %f,  %f\n", sfint_to_float(a), sfint_to_float(b));
//         // while(1);
//     }
//     return mul_sfint(a,b);
// }

sfint lows(sfint* x, sfint* y, Coeff* coeffs){
    return mul_sfint(coeffs->l_b[0], x[-0]) + mul_sfint(coeffs->l_b[1], x[-1]) + mul_sfint(coeffs->l_b[2],x[-2]) - mul_sfint(coeffs->l_a[1],y[-1]) - mul_sfint(coeffs->l_a[2], y[-2]);
} 

float lowFloat(float* x, float* y){
    return (LOWB0)*(x[0]) + (LOWB1)*(x[-1]) + (LOWB2)*(x[-2]) - (LOWA1)*(y[-1]) - (LOWA2)*(y[-2]);
} 

sfint mid1s(sfint* x, sfint* y, Coeff* coeffs){
    return mul_sfint(coeffs->m1_b[0], x[-0]) + mul_sfint(coeffs->m1_b[2],x[-2]) - mul_sfint(coeffs->m1_a[1],y[-1]) - mul_sfint(coeffs->m1_a[2], y[-2]);
} 


float mid1Float(float* x, float* y){
    return (MID1B0)*(x[0]) + (MID1B1)*(x[-1]) + (MID1B2)*(x[-2]) - (MID1A1)*(y[-1]) - (MID1A2)*(y[-2]);
} 

sfint mid2s(sfint* x, sfint* y, Coeff* coeffs){
    return mul_sfint(coeffs->m2_b[0], x[-0]) + mul_sfint(coeffs->m2_b[2],x[-2]) - mul_sfint(coeffs->m2_a[1],y[-1]) - mul_sfint(coeffs->m2_a[2], y[-2]);
} 


float mid2Float(float* x, float* y){
    return (MID2B0)*(x[0]) + (MID2B1)*(x[-1]) + (MID2B2)*(x[-2]) - (MID2A1)*(y[-1]) - (MID2A2)*(y[-2]);
} 

sfint mid3s(sfint* x, sfint* y, Coeff* coeffs){
    return mul_sfint(coeffs->m3_b[0], x[0]) + mul_sfint(coeffs->m3_b[2],x[-2]) - mul_sfint(coeffs->m3_a[1],y[-1]) - mul_sfint(coeffs->m3_a[2], y[-2]);
} 


float mid3Float(float* x, float* y) {
    return (MID3B0)*(x[0]) + (MID3B1)*(x[-1]) + (MID3B2)*(x[-2]) - (MID3A1)*(y[-1]) - (MID3A2)*(y[-2]);
} 

// sfint highs(sfint* x, sfint* y, Coeff* coeffs){
//     return mul_sfint(coeffs->h_b[0], x[-0]) + mul_sfint(coeffs->h_b[1], x[-1]) + mul_sfint(coeffs->h_b[2],x[-2]) - mul_sfint(coeffs->h_a[1],y[-1]) - mul_sfint(coeffs->h_a[2], y[-2]);
// } 
sfint highs(sfint* x, sfint* y, Coeff* coeffs){
    return x[0] + mul_sfint(coeffs->h_b[1], x[-1]) + x[-2] - y[-1] - mul_sfint(coeffs->h_a[2], y[-2]);
} 

float highFloat(float* x, float* y){
    return (HIGHB0)*(x[0]) + (HIGHB1)*(x[-1]) + (HIGHB2)*(x[-2]) - (HIGHA1)*(y[-1]) - (HIGHA2)*(y[-2]);
} 

Coeff* initCoefficients() {
    Coeff* coeff = malloc(sizeof(Coeff));
    memset(coeff, 0x00, sizeof(Coeff));

    //highs b
    coeff->h_b[0] = float_to_sfint(HIGHB0);
    coeff->h_b[1] = float_to_sfint(HIGHB1);
    coeff->h_b[2] = float_to_sfint(HIGHB2);
    //highs a
    coeff->h_a[0] = float_to_sfint(HIGHA0);
    coeff->h_a[1] = float_to_sfint(HIGHA1);
    coeff->h_a[2] = float_to_sfint(HIGHA2);

    //mid3s b
    coeff->m3_b[0] = float_to_sfint(MID3B0);
    coeff->m3_b[1] = float_to_sfint(MID3B1);
    coeff->m3_b[2] = float_to_sfint(MID3B2);
    //mid3s a
    coeff->m3_a[0] = float_to_sfint(MID3A0);
    coeff->m3_a[1] = float_to_sfint(MID3A1);
    coeff->m3_a[2] = float_to_sfint(MID3A2);
    
    //mid2s b
    coeff->m2_b[0] = float_to_sfint(MID2B0);
    coeff->m2_b[1] = float_to_sfint(MID2B1);
    coeff->m2_b[2] = float_to_sfint(MID2B2);
    //mid2s a
    coeff->m2_a[0] = float_to_sfint(MID2A0);
    coeff->m2_a[1] = float_to_sfint(MID2A1);
    coeff->m2_a[2] = float_to_sfint(MID2A2);

    //mid1s b
    coeff->m1_b[0] = float_to_sfint(MID1B0);
    coeff->m1_b[1] = float_to_sfint(MID1B1);
    coeff->m1_b[2] = float_to_sfint(MID1B2);
    //mid1s a 
    coeff->m1_a[0] = float_to_sfint(MID1A0);
    coeff->m1_a[1] = float_to_sfint(MID1A1);
    coeff->m1_a[2] = float_to_sfint(MID1A2);

    //lows b
    coeff->l_b[0] = float_to_sfint(LOWB0);
    coeff->l_b[1] = float_to_sfint(LOWB1);
    coeff->l_b[2] = float_to_sfint(LOWB2); 
    //lows a 
    coeff->l_a[0] = float_to_sfint(LOWA0);
    coeff->l_a[1] = float_to_sfint(LOWA1);
    coeff->l_a[2] = float_to_sfint(LOWA2);

    return coeff;
}

void deinitCoefficients(Coeff* coeff) {
    free(coeff);
}

Gains* initGains() {
    Gains* gains = malloc(sizeof(Gains));
    memset(gains, 0x00, sizeof(Gains));
    
    gains->h = float_to_sfint(DEFAULT_HIGH_GAIN);
    gains->m1 = float_to_sfint(DEFAULT_MID1_GAIN);
    gains->m2 = float_to_sfint(DEFAULT_MID2_GAIN);
    gains->m3 = float_to_sfint(DEFAULT_MID3_GAIN);
    gains->l = float_to_sfint(DEFAULT_LOW_GAIN);

    return gains;
}

void deinitGains(Gains* gains) {
    free(gains);
}

Feedbacks* initFeedbacks() {
    Feedbacks* fb = malloc(sizeof(Feedbacks));
    memset(fb, 0x00, sizeof(Feedbacks));
    return fb;
}

void deinitFeedbacks(Feedbacks* fb) {
    free(fb);
}

sfint filter(sfint* x, Feedbacks* fb, Coeff* coeffs, Gains* gains) {
    fb->l[2] = lows(x, fb->l+2, coeffs);
    fb->m1[2] = mid1s(x, fb->m1+2, coeffs);
    fb->m2[2] = mid2s(x, fb->m2+2, coeffs);
    fb->m3[2] = mid3s(x, fb->m3+2, coeffs);
    fb->h[2] = highs(x, fb->h+2, coeffs);
    return mul_sfint(gains->l, fb->l[2]) + mul_sfint(gains->m1, fb->m1[2]) + mul_sfint(gains->m2, fb->m2[2]) + mul_sfint(gains->m3, fb->m3[2]) + mul_sfint(gains->h, fb->h[2]);  
}

void step(sfint* fbArray) {
    fbArray[0] = fbArray[1];
    fbArray[1] = fbArray[2];
}

void stepFb(Feedbacks* fb) {
    step(fb->l);
    step(fb->m1);
    step(fb->m2);
    step(fb->m3);
    step(fb->h);
}

float filterFloat(float* x, float* y) {
    return 1.0f;
    // return DEFAULT_LOW_GAIN*lowFloat(x,y) + DEFAULT_LOW_GAIN*mid1Float(x,y) + DEFAULT_LOW_GAIN*mid2Float(x,y) + DEFAULT_LOW_GAIN*mid3Float(x,y) + DEFAULT_LOW_GAIN*highFloat(x,y);  
}