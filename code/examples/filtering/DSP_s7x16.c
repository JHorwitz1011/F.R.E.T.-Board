

// ========================================
// === dds setup 
// =======================================
// s7x16 fixed point macros ==
typedef signed int s7x16;
#define muls7x16(a,b) ((s7x16)(((( signed long long )(a))*(( signed long long )(b)))>>16)) //multiply two fixed 16:16
#define float_to_s7x16(a) ((s7x16)((a)*65536.0)) // 2^16
#define s7x16_to_float(a) ((float)(a)/65536.0)
#define s7x16_to_int(a)    ((int)((a)>>16))
#define int_to_s7x16(a)    ((s7x16)((a)<<16))
#define divs7x16(a,b) ((s7x16)((((signed long long)(a)<<16)/(b)))) 
#define abss7x16(a) abs(a)

// =======================================
// 1/Fs in microseconds for ISR
volatile int alarm_period = 25 ;

// DDS variables 
unsigned int main_inc ;
unsigned int main_accum ;
s7x24 sine_table_out, cos_table_out ;
s7x24 dsp_input, filter_out ;
s7x24 sine_table[256], cos_table[256] ;
float Fs=44100, Fout, F_low, F_high, F_inc;
 
//======================================================== 
// Second order Butterworth lowpass
// xx is the current input signal sample
// returns the current filtered output sample
// Simplification: b(1)*x(n)+b(2)*x(n-1)+b(3)*x(n-2) = 
// b(1)*x(n)+2*b(1)*x(n-1)+b(1)*x(n-2) =
// b(1)* (x(n)+(x(n-1)<<1)+x(n-2))
s7x16 lows(s7x16 xx){
  static s7x16 yy  ;
  //IIR state variables
  static s7x16  b1,  a2, a3 ;
  static s7x16  xn, xn_1, xn_2 ;
  static s7x16  yn_1, yn_2  ;

  // b = 1.0e-03 * [0.0761    0.1523    0.0761]
  // a = [1.0000   -1.9752    0.9755]
  b1 = float_to_s7x16(0.0000761) ;
  a2 = float_to_s7x16(-1.9752) ;
  a3 = float_to_s7x16(0.9755);

  // sum the 5 terms: yy += xx*coeff 
  // and update the state variables
  // as soon as possible

  yy = muls7x16((xx+(xn_1<<1)+xn_2), b1);
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
s7x16 mids1(s7x16 xx)
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
s7x16 mids2(s7x16 xx)
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
s7x16 mids3(s7x16 xx)
{
    static s7x16 yy;
    //IIR state variables
    static s7x16  b1, a2, a3;
    static s7x16  xn, xn_1, xn_2;
    static s7x16  yn_1, yn_2;

    // b = [0.0026     0   -0.0052     0    0.0026]
    // a = [1.0000   -3.7560    5.3817   -3.4843    0.8608]
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

s7x16 highs(s7x16 xx) {
    static s7x16 yy;
    //IIR state variables
    static s7x16  b1, a2, a3;
    static s7x16  xn, xn_1, xn_2;
    static s7x16  yn_1, yn_2;

    // b = [0.7333   -1.4666    0.7333]
    // a = [1.0000   -1.3942    0.5391]
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

s7x16 signedFilter(s7x16* inputs, ) {

}
