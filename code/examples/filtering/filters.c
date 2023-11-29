#include "filters.h"

#include <string.h>
#include <stdlib.h>

#define FIXED2 int_to_sfint(2)

//======================================================== 
// Second order Butterworth lowpass
// x is the input signal
// y is the output signal

sfint lows(sfint* x, sfint* y, Coeff* coeffs){
    sfint output = mul_sfint((x[0] - mul_sfint(FIXED2, x[1]) + x[2]), coeffs->l_b[0]);
    output -= mul_sfint(y[2], coeffs->l_a[2]);
    output -= mul_sfint(y[1], coeffs->l_a[1]);
    return output;
} 

//======================================================== 
// Second order Butterworth bandpass
// x is the input signal
// y is the output signal

sfint mid1s(sfint* x, sfint* y, Coeff* coeffs) {
	sfint output = mul_sfint((x[0] - x[2]), coeffs->m1_b[0]);
	output -= mul_sfint(y[2], coeffs->m1_a[2]);
	output -= mul_sfint(y[1], coeffs->m1_a[1]);
    return output;
} 

//======================================================== 
// Second order Butterworth bandpass
// x is the input signal
// y is the output signal

sfint mid2s(sfint* x, sfint* y, Coeff* coeffs)
{
    sfint output = mul_sfint((x[0] - x[2]), coeffs->m2_b[0]);
    output -= mul_sfint(y[2], coeffs->m2_a[2]);
    output -= mul_sfint(y[1], coeffs->m2_a[1]);
    return output;
}

//======================================================== 
// Second order Butterworth bandpass
// x is the input signal
// y is the output signal

sfint mid3s(sfint* x, sfint* y, Coeff* coeffs) {
    sfint output = mul_sfint((x[0] - x[2]), coeffs->m3_b[0]);
    output -= mul_sfint(y[2], coeffs->m3_a[2]);
    output -= mul_sfint(y[1], coeffs->m3_a[1]);
    return output;
}

//======================================================== 
// Second order Butterworth highpass
// x is the input signal
// y is the output signal

sfint highs(sfint* x, sfint* y, Coeff* coeffs) {
    sfint output = mul_sfint(((x[0] - mul_sfint(FIXED2, x[1])) + x[2]), coeffs->h_b[0]);
    output -= mul_sfint(y[2], coeffs->h_a[2]);
    output -= mul_sfint(y[1], coeffs->h_a[1]);
    return output;
}

Coeff* initCoefficients() {
    Coeff* coeff = malloc(sizeof(Coeff));
    memset(coeff, 0x00, sizeof(Coeff));

    //highs b
    coeff->h_b[0] = float_to_sfint(0.7333);
    //highs a
    coeff->h_a[0] = float_to_sfint(1.00);
    coeff->h_a[1] = float_to_sfint(-1.3942);
    coeff->h_a[2] = float_to_sfint(0.5391);

    //mid3s b
    coeff->m3_b[0] = float_to_sfint(0.0504);
    //mid3s a
    coeff->m3_a[0] = float_to_sfint(1.00);
    coeff->m3_a[1] = float_to_sfint(-1.8527);
    coeff->m3_a[2] = float_to_sfint(0.8992);
    
    //mid2s b
    coeff->m2_b[0] = float_to_sfint(0.0205);
    //mid2s a
    coeff->m2_a[0] = float_to_sfint(1.00);
    coeff->m2_a[1] = float_to_sfint(-1.9516);
    coeff->m2_a[2] = float_to_sfint(0.9591);

    //mid1s b
    coeff->m1_b[0] = float_to_sfint(0.0082);
    //mid1s a 
    coeff->m1_a[0] = float_to_sfint(1.00);
    coeff->m1_a[1] = float_to_sfint(-1.9824);
    coeff->m1_a[2] = float_to_sfint(0.9836);

    //lows b
    coeff->l_b[0] = float_to_sfint(0.0761);
    //lows a 
    coeff->l_a[0] = float_to_sfint(1.00);
    coeff->l_a[1] = float_to_sfint(-1.9752);
    coeff->l_a[2] = float_to_sfint(0.9755);

    return coeff;
}

void deinitCoefficients(Coeff* coeff) {
    free(coeff);
}

Gains* initGains() {
    Gains* gains = malloc(sizeof(Gains));
    memset(gains, 0x00, sizeof(Gains));
    
    gains->h = 1;
    gains->m1 = 1;
    gains->m2 = 1;
    gains->m3 = 1;
    gains->l = 1;

    return gains;
}

void deinitGains(Gains* gains) {
    free(gains);
}

sfint filter(sfint* x, sfint* y, Coeff* coeffs, Gains* gains) {

    //filter 
    // lows(x,y, coeffs);  
    // y[0] = mul_sfint(y[0], float_to_sfint(1));
    // mid1s(x,y, coeffs);
    // mid2s(x,y, coeffs);
    // mid3s(x,y, coeffs);
    return mid1s(x,y, coeffs);

    //ignore gains for now
}