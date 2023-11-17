#include "filters.h"

//======================================================== 
// Second order Butterworth lowpass
// x is the input signal
// y is the output signal

void lows(s7x16* x, s7x16* y, Coeff* coeffs){
    y[0] = muls7x16((x[0] + 2 * x[1] + x[2]), l_b[0]);
    x[2] = x[1];
    x[1] = x[0];
    y[0] = y[0] - muls7x16(y[2], l_a[2]);
    y[2] = y[1];
    y[0] = y[0] - muls7x16(y[1], la_[1]);
    y[1] = y[0];
    return y;
} 

//======================================================== 
// Second order Butterworth bandpass
// x is the input signal
// y is the output signal

void mid1s(s7x16* x, s7x16* y, Coeff* coeffs)
{
	y[0] = muls7x16((x[0] - x[2]), m1_b[0]);
	x[2] = x[1];
	x[1] = x[0];
	y[0] = y[0] - muls7x16(y[2], m1_a[2]);
	y[2] = y[1];
	y[0] = y[0] - muls7x16(y[1], m1_a[1]);
    y[1] = y[0];
    return y;
} 

//======================================================== 
// Second order Butterworth bandpass
// x is the input signal
// y is the output signal

void mid2s(s7x16* x, s7x16* y, Coeff* coeffs)
{
    y[0] = muls7x16((x[0] - x[2]), m2_b[0]);
    x[2] = x[1];
    x[1] = x[0];
    y[0] = y[0] - muls7x16(y[2], m2_a[2]);
    y[2] = y[1];
    y[0] = y[0] - muls7x16(y[1], m2_a[1]);
    y[1] = y[0];
    return y;
}

//======================================================== 
// Second order Butterworth bandpass
// x is the input signal
// y is the output signal

void mid3s(s7x16* x, s7x16* y, Coeff* coeffs) {
    y[0] = muls7x16((x[0] - x[2]), m3_b[0]);
    x[2] = x[1];
    x[1] = x[0];
    y[0] = y[0] - muls7x16(y[2], m3_a[2]);
    y[2] = y[1];
    y[0] = y[0] - muls7x16(y[1], m3_a[1]);
    y[1] = y[0];
    return y;
}

//======================================================== 
// Second order Butterworth highpass
// x is the input signal
// y is the output signal

void highs(s7x16* x, s7x16* y, Coeff* coeffs) {
    y[0] = muls7x16((x[0] + 2*x[1] + x[2]), h_b[0]);
    x[2] = x[1];
    x[1] = x[0];
    y[0] = y[0] - muls7x16(y[2], h_a[2]);
    y[2] = y[1];
    y[0] = y[0] - muls7x16(y[1], ha_[1]);
    y[1] = y[0];
    return y[0];
}

void fillCoefficients(Coeff* coeff) {
    //high b = [0.7333   -1.4666]
    coeff->h_b[0] = float_to_s7x16(0.7333);

    //high a = [1.0000   -1.3942    0.5391]
    coeff->h_a[0] = float_to_s7x16(1.00);
    coeff->h_a[1] = float_to_s7x16(-1.9752);
    coeff->h_a[2] = float_to_s7x16(0.9755);

    //mid3s b = [0.0026     0   -0.0052]
    coeff->m3_b[0] = float_to_s7x16(.0026);

    //mid3s a = [1.0000   -3.7560    5.3817]
    coeff->m3_a[0] = float_to_s7x16(1.00);
    coeff->m3_a[1] = float_to_s7x16(-3.7560);
    coeff->m3_a[2] = float_to_s7x16(5.3817);
    
    //mid2s b = 1.0e-03 * [ 0.4232    0   -0.8464]
    coeff->m2_b[0] = float_to_s7x16(.0004232);

    //mid2s a = [1.0000   -3.9260    5.7951]
    coeff->m2_a[0] = float_to_s7x16(1.00);
    coeff->m2_a[1] = float_to_s7x16(-3.9260);
    coeff->m2_a[2] = float_to_s7x16(5.7951);

    //mid1s b = b = 1.0e-03 * [0.0678   0   -0.1356]
    coeff->m1_b[0] = float_to_s7x16(0.0000678);

    //mid1s a = [1.0000   -3.9742    5.9252]
    coeff->m1_a[0] = float_to_s7x16(1.00);
    coeff->m1_a[1] = float_to_s7x16(-3.9742);
    coeff->m1_a[2] = float_to_s7x16(5.9252);

    //lows b = 1.0e-03 * [0.0761    0.1523]
    coeff->l_b[0] = float_to_s7x16(0.0000761);

    //lows a = [1.0000   -1.9752    0.9755]
    coeff->l_a[0] = float_to_s7x16(1.00);
    coeff->l_a[1] = float_to_s7x16(-1.9752);
    coeff->l_a[2] = float_to_s7x16(0.9755);
}

void filter(s7x16* x, s7x16* y, Coeff* coeffs, Gains gains) {

}