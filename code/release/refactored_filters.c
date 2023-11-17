#include "filters.h"

//======================================================== 
// Second order Butterworth lowpass
// xx is the current input signal sample
// returns the current filtered output sample
// Simplification: b(1)*x(n)+b(2)*x(n-1)+b(3)*x(n-2) = 
// b(1)*x(n)+2*b(1)*x(n-1)+b(1)*x(n-2) =
// b(1)* (x(n)+(x(n-1)<<1)+x(n-2))
void lows(s7x16* x, s7x16* y, Coeff* coeffs){

  // b = 1.0e-03 * [0.0761    0.1523    0.0761]
  // a = [1.0000   -1.9752    0.9755]
  b1 = float_to_s7x16(0.0000761) ;
  a2 = float_to_s7x16(-1.9752) ;
  a3 = float_to_s7x16(0.9755);

  // sum the 5 terms: yy += xx*coeff 
  // and update the state variables
  // as soon as possible

  yy = muls7x16((xx + 2 * xn_1 + xn_2), b1);
  xn_2 = xn_1;
  xn_1 = xx;
  yy = yy - muls7x16(yn_2, a3);
  yn_2 = yn_1;
  yy = yy - muls7x16(yn_1, a2);
  yn_1 = yy;
  return yy;
} 

//======================================================== 
// Second order Butterworth bandpass
// xx is the current input signal sample
// b(1)*x(n)+b(2)*x(n-1)+b(3)*x(n-2) = 
// b(1)*x(n)+0*b(1)*x(n-1)-b(1)*x(n-2) =
// b(1)* (x(n)-x(n-2))
void mid1s(s7x16* x, s7x16* y, Coeff* coeffs)
{
  static s7x16 yy ;
  //IIR state variables
  static s7x16  b1,  a2, a3 ;
  static s7x16  xn, xn_1, xn_2 ;
  static s7x16  yn_1, yn_2  ;

  // b = 1.0e-03 * [0.0678   0   -0.1356    0    0.0678]
  // a = [1.0000   -3.9742    5.9252   -3.9279    0.9768]
  b1 = float_to_s7x16(0.0000678)  ;
  a2 = float_to_s7x16(-3.9742) ;  //s7x16() ;   
  a3 = float_to_s7x16(5.9252) ; // //s7x16() ;  

	yy = muls7x16((xx-xn_2), b1);
	xn_2 = xn_1;
	xn_1 = xx;
	yy = yy - muls7x16(yn_2,a3);
	yn_2 = yn_1;
	yy = yy - muls7x16(yn_1,a2);
    yn_1 = yy;
    return yy;
} 

//======================================================== 
// Second order Butterworth bandpass
// xx is the current input signal sample
// b(1)*x(n)+b(2)*x(n-1)+b(3)*x(n-2) = 
// b(1)*x(n)+0*b(1)*x(n-1)-b(1)*x(n-2) =
// b(1)* (x(n)-x(n-2))
void mid2s(s7x16* x, s7x16* y, Coeff* coeffs)
{
    static s7x16 yy;
    //IIR state variables
    static s7x16  b1, a2, a3;
    static s7x16  xn, xn_1, xn_2;
    static s7x16  yn_1, yn_2;

    // b = 1.0e-03 * [ 0.4232    0   -0.8464    0    0.4232]
    // a = [1.0000   -3.9260    5.7951   -3.8117    0.9427]
    b1 = float_to_s7x16(0.0004232);
    a2 = float_to_s7x16(-3.9260);  //s7x16() ;   
    a3 = float_to_s7x16(5.7951); // //s7x16() ;  

    yy = muls7x16((xx - xn_2), b1);
    xn_2 = xn_1;
    xn_1 = xx;
    yy = yy - muls7x16(yn_2, a3);
    yn_2 = yn_1;
    yy = yy - muls7x16(yn_1, a2);
    yn_1 = yy;
    return yy;
}

//======================================================== 
// Second order Butterworth bandpass
// xx is the current input signal sample
// b(1)*x(n)+b(2)*x(n-1)+b(3)*x(n-2) = 
// b(1)*x(n)+0*b(1)*x(n-1)-b(1)*x(n-2) =
// b(1)* (x(n)-x(n-2))
void mid3s(s7x16* x, s7x16* y, Coeff* coeffs) {
    //mid3s b = [0.0026     0   -0.0052     0    0.0026]
    //mid3s a = [1.0000   -3.7560    5.3817   -3.4843    0.8608]
    b1 = float_to_s7x16(0.0026);
    a2 = float_to_s7x16(-3.7560);  //s7x16() ;   
    a3 = float_to_s7x16(5.3817); // //s7x16() ;  

    yy = muls7x16((xx - xn_2), b1);
    xn_2 = xn_1;
    xn_1 = xx;
    yy = yy - muls7x16(yn_2, a3);
    yn_2 = yn_1;
    yy = yy - muls7x16(yn_1, a2);
    yn_1 = yy;
    return yy;
}

//======================================================== 
// Second order Butterworth highpass
// xx is the current input signal sample
// returns the current filtered output sample
// Simplification: b(1)*x(n)+b(2)*x(n-1)+b(3)*x(n-2) = 
// b(1)*x(n)+2*b(1)*x(n-1)+b(1)*x(n-2) =
// b(1)* (x(n)+2*x(n-1)+x(n-2))

void highs(s7x16* x, s7x16* y, Coeff* coeffs) {
    //high b = [0.7333   -1.4666    0.7333]
    //high a = [1.0000   -1.3942    0.5391]
    b1 = float_to_s7x16(0.0000761);
    a2 = float_to_s7x16(-1.9752);
    a3 = float_to_s7x16(0.9755);

    // sum the 5 terms: yy += xx*coeff 
    // and update the state variables
    // as soon as possible

    yy = muls7x24((xx + 2*xn_1 + xn_2), b1);
    xn_2 = xn_1;
    xn_1 = xx;
    yy = yy - muls7x16(yn_2, a3);
    yn_2 = yn_1;
    yy = yy - muls7x16(yn_1, a2);
    yn_1 = yy;
    return yy;
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