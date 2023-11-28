#include "filters.h"

#include <string.h>
#include <stdlib.h>

//======================================================== 
// Second order Butterworth lowpass
// x is the input signal
// y is the output signal

void lows(sfint* x, sfint* y, Coeff* coeffs){
    y[0] = mul_sfint((x[0] - 2*x[1] + x[2]), coeffs->l_b[0]);
    y[0] = y[0] - mul_sfint(y[2], coeffs->l_a[2]);
    y[0] = y[0] - mul_sfint(y[1], coeffs->l_a[1]);
} 

//======================================================== 
// Second order Butterworth bandpass
// x is the input signal
// y is the output signal

void mid1s(sfint* x, sfint* y, Coeff* coeffs) {
	y[0] = mul_sfint((x[0] - x[2]), coeffs->m1_b[0]);
	y[0] = y[0] - mul_sfint(y[2], coeffs->m1_a[2]);
	y[0] = y[0] - mul_sfint(y[1], coeffs->m1_a[1]);
} 

//======================================================== 
// Second order Butterworth bandpass
// x is the input signal
// y is the output signal

void mid2s(sfint* x, sfint* y, Coeff* coeffs)
{
    y[0] = mul_sfint((x[0] - x[2]), coeffs->m2_b[0]);
    y[0] = y[0] - mul_sfint(y[2], coeffs->m2_a[2]);
    y[0] = y[0] - mul_sfint(y[1], coeffs->m2_a[1]);
}

//======================================================== 
// Second order Butterworth bandpass
// x is the input signal
// y is the output signal

void mid3s(sfint* x, sfint* y, Coeff* coeffs) {
    y[0] = mul_sfint((x[0] - x[2]), coeffs->m3_b[0]);
    y[0] = y[0] - mul_sfint(y[2], coeffs->m3_a[2]);
    y[0] = y[0] - mul_sfint(y[1], coeffs->m3_a[1]);
}

//======================================================== 
// Second order Butterworth highpass
// x is the input signal
// y is the output signal

void highs(sfint* x, sfint* y, Coeff* coeffs) {
    y[0] = mul_sfint((x[0] - 2*x[1]) + x[2]), coeffs->h_b[0]);
    y[0] = y[0] - mul_sfint(y[2], coeffs->h_a[2]);
    y[0] = y[0] - mul_sfint(y[1], coeffs->h_a[1]);
}

Coeff* initCoefficients() {
    Coeff* coeff = malloc(sizeof(Coeff));
    memset(coeff, 0x00, sizeof(Coeff));

    //high b = [0.7333   -1.4666]
    coeff->h_b[0] = float_to_sfint(0.7333);

    //high a = [1.0000   -1.3942    0.5391]
    coeff->h_a[0] = float_to_sfint(1.00);
    coeff->h_a[1] = float_to_sfint(-1.3942);
    coeff->h_a[2] = float_to_sfint(0.5391);

    //mid3s b = [0.0026     0   -0.0052]
    coeff->m3_b[0] = float_to_sfint(.0026);

    //mid3s a = [1.0000   -3.7560    5.3817]
    coeff->m3_a[0] = float_to_sfint(1.00);
    coeff->m3_a[1] = float_to_sfint(-3.7560);
    coeff->m3_a[2] = float_to_sfint(5.3817);
    
    //mid2s b = 1.0e-03 * [ 0.4232    0   -0.8464]
    coeff->m2_b[0] = float_to_sfint(.0004232);

    //mid2s a = [1.0000   -3.9260    5.7951]
    coeff->m2_a[0] = float_to_sfint(1.00);
    coeff->m2_a[1] = float_to_sfint(-3.9260);
    coeff->m2_a[2] = float_to_sfint(5.7951);

    //mid1s b = b = 1.0e-03 * [0.0678   0   -0.1356]
    coeff->m1_b[0] = float_to_sfint(0.0000678);

    //mid1s a = [1.0000   -3.9742    5.9252]
    coeff->m1_a[0] = float_to_sfint(1.00);
    coeff->m1_a[1] = float_to_sfint(-3.9742);
    coeff->m1_a[2] = float_to_sfint(5.9252);

    //lows b = 1.0e-03 * [0.0761    0.1523]
    coeff->l_b[0] = float_to_sfint(0.0000761);

    //lows a = [1.0000   -1.9752    0.9755]
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

void filter(sfint* x, sfint* y, Coeff* coeffs, Gains* gains) {
    //bump output
    y[2] = y[1];
    y[1] = y[0];
    y[0] = 0;
 
    //filter 
    lows(x,y, coeffs);  
    // y[0] = mul_sfint(y[0], float_to_sfint(1));
    // mid1s(x,y, coeffs);
    // mid2s(x,y, coeffs);
    // mid3s(x,y, coeffs);
    // highs(x,y, coeffs);

    //ignore gains for now
}