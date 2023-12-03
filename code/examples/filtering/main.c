#include <math.h>
#include <stdio.h>
#include <string.h>

#include "filters.h"
#include "util.h"

#define BUFFER_LEN 44100
#define SAMPLE_LEN 1 //sec
#define AMPLITUDE 7000.0f
#define BANDWIDTH 15000

#define PRECISION 10

void dumpBuffer(int freq, float* wave, sfint* castedWave, sfint* filteredWave, int n) {
        printf("for a wave of frequency %d\n", freq);
        printf("the original float wave has an average of %f, max of %f, and min of %f.\n", averageFloat(wave, BUFFER_LEN), maxFloat(wave, BUFFER_LEN), minFloat(wave, BUFFER_LEN));
        printf("the casted wave has an average of %f, max of %f, and min of %f.\n", averageFixed(castedWave, BUFFER_LEN), maxFixed(castedWave, BUFFER_LEN), minFixed(castedWave, BUFFER_LEN));
        printf("the filtered wave has an average of %f, max of %f, and min of %f.\n", averageFixed(filteredWave, BUFFER_LEN), maxFixed(filteredWave, BUFFER_LEN), minFixed(filteredWave, BUFFER_LEN));

}

void dumpBufferToCSV(float* time, float* floatWave, sfint* sfintWave, sfint* sfintFilteredWave, float* floatFilteredWave, int n, const char* title) {
    FILE *fpt;
    fpt = fopen(title, "w+");
    fprintf(fpt,"time, floatWave, floatFilteredWave, sfintWave, sfintFilteredWave\n");
    for(int i = 0; i < n; i++) {
        fprintf(fpt, "%f, %f, %f, %f, %f\n", time[i], floatWave[i], floatFilteredWave[i], sfint_to_float(sfintWave[i]), sfint_to_float(sfintFilteredWave[i]));
    }

    fclose(fpt);
}

void dumpFreqToCSV(int* freq, 
                    float* lowpk, float* lowpkIdeal, 
                    float* mid1pk, float* mid1pkIdeal, 
                    float* mid2pk, float* mid2pkIdeal, 
                    float* mid3pk, float* mid3pkIdeal, 
                    float* highpk, float* highpkIdeal, 
                    float* fullpk, float* fullpkIdeal, 
                    int n, char* title) {
    FILE *fpt;
    fpt = fopen(title, "w+");
    fprintf(fpt,"freq, lows, lowsIdeal, mid1s, mid1sIdeal, mid2s, mid2sIdeal, mid3s, mid3sIdeal, highs, highsIdeal, full, fullIdeal\n");
    for(int i = 0; i < n; i++) {
        fprintf(fpt, "%d, ", freq[i]);
        fprintf(fpt, "%f, %f, ", sfint_to_float(lowpk[i]), lowpkIdeal[i]);
        fprintf(fpt, "%f, %f, ", sfint_to_float(mid1pk[i]), mid1pkIdeal[i]);
        fprintf(fpt, "%f, %f, ", sfint_to_float(mid2pk[i]), mid2pkIdeal[i]);
        fprintf(fpt, "%f, %f, ", sfint_to_float(mid3pk[i]), mid3pkIdeal[i]);
        fprintf(fpt, "%f, %f, ", sfint_to_float(highpk[i]), highpkIdeal[i]);
        fprintf(fpt, "%f, %f\n", sfint_to_float(fullpk[i]), fullpkIdeal[i]);
    }
    fclose(fpt);
}

float fixedPkPk(sfint* buffer, int n) {
    return maxFixed(buffer, BUFFER_LEN) - minFixed(buffer, BUFFER_LEN);
}

float floatPkPk(float* buffer, int n){
    return maxFloat(buffer, BUFFER_LEN) - minFloat(buffer, BUFFER_LEN);
}

int main() {
    Coeff* coeffs = initCoefficients();
    Gains* gains = initGains();
    printCoeffs(coeffs);

    //fill buffers
    float time[BUFFER_LEN] = {0.0f};
    float floatWave[BUFFER_LEN] = {0.0f}; 
    sfint sfintWave[BUFFER_LEN] = {0};

    float floatLowFilteredWave[BUFFER_LEN] = {0.0f};
    sfint sfintLowFilteredWave[BUFFER_LEN] = {0};

    float floatMid1FilteredWave[BUFFER_LEN] = {0.0f};
    sfint sfintMid1FilteredWave[BUFFER_LEN] = {0};

    float floatMid2FilteredWave[BUFFER_LEN] = {0.0f};
    sfint sfintMid2FilteredWave[BUFFER_LEN] = {0};

    float floatMid3FilteredWave[BUFFER_LEN] = {0.0f};
    sfint sfintMid3FilteredWave[BUFFER_LEN] = {0};

    float floatHighFilteredWave[BUFFER_LEN] = {0.0f};
    sfint sfintHighFilteredWave[BUFFER_LEN] = {0};
    
    float floatFullFilteredWave[BUFFER_LEN] = {0.0f};
    sfint sfintFullFilteredWave[BUFFER_LEN] = {0};
    

    //fill time buffer
    for(int i = 0; i < BUFFER_LEN; i++) {
        time[i] = ((float)i)/(BUFFER_LEN);
    }

    int freqs[BANDWIDTH] = {0};

    float lowpks[BANDWIDTH] = {0};
    float lowFloatPks[BANDWIDTH] = {0};

    float mid1pks[BANDWIDTH] = {0};
    float mid1FloatPks[BANDWIDTH] = {0};

    float mid2pks[BANDWIDTH] = {0};
    float mid2FloatPks[BANDWIDTH] = {0};

    float mid3pks[BANDWIDTH] = {0};
    float mid3FloatPks[BANDWIDTH] = {0};

    float highpks[BANDWIDTH] = {0};
    float highFloatPks[BANDWIDTH] = {0};

    float fullpks[BANDWIDTH] = {0};
    float fullFloatPks[BANDWIDTH] = {0};

    //sweep frequencies
    for(int freq = 0; freq <= BANDWIDTH; freq += PRECISION) { //hz

        //first, calculate wave values
        for(int i = 0; i < BUFFER_LEN; i++) {
            floatWave[i] = AMPLITUDE*sin(2.0f*M_PI*freq*time[i]);
            sfintWave[i] = float_to_sfint(floatWave[i]);
        }

        //then, filter the values
        for(int i = 2; i < BUFFER_LEN; i++) {
            sfintLowFilteredWave[i] = lows(sfintWave+i, sfintLowFilteredWave+i, coeffs);
            floatLowFilteredWave[i] = lowFloat(floatWave+i, floatLowFilteredWave+i);

            sfintMid1FilteredWave[i] = mid1s(sfintWave+i, sfintMid1FilteredWave+i, coeffs);
            floatMid1FilteredWave[i] = mid1Float(floatWave+i, floatMid1FilteredWave+i);

            sfintMid2FilteredWave[i] = mid2s(sfintWave+i, sfintMid2FilteredWave+i, coeffs);
            floatMid2FilteredWave[i] = mid2Float(floatWave+i, floatMid2FilteredWave+i);

            sfintMid3FilteredWave[i] = mid3s(sfintWave+i, sfintMid3FilteredWave+i, coeffs);
            floatMid3FilteredWave[i] = mid3Float(floatWave+i, floatMid3FilteredWave+i);

            sfintHighFilteredWave[i] = highs(sfintWave+i, sfintHighFilteredWave+i, coeffs);
            floatHighFilteredWave[i] = highFloat(floatWave+i, floatHighFilteredWave+i);

            sfintFullFilteredWave[i] = filter(sfintWave+i, sfintFullFilteredWave+i, coeffs, gains);
            floatFullFilteredWave[i] = filterFloat(floatWave+i, floatFullFilteredWave+i);
        }

        //log results
        freqs[freq] = freq;
        
        lowpks[freq] = fixedPkPk(sfintLowFilteredWave, BUFFER_LEN);
        mid1pks[freq] = fixedPkPk(sfintMid1FilteredWave, BUFFER_LEN);
        mid2pks[freq] = fixedPkPk(sfintMid2FilteredWave, BUFFER_LEN);
        mid3pks[freq] = fixedPkPk(sfintMid3FilteredWave, BUFFER_LEN);
        highpks[freq] = fixedPkPk(sfintHighFilteredWave, BUFFER_LEN);
        fullpks[freq] = fixedPkPk(sfintFullFilteredWave, BUFFER_LEN);
        
        lowFloatPks[freq] = floatPkPk(floatLowFilteredWave, BUFFER_LEN);
        mid1FloatPks[freq] = floatPkPk(floatMid1FilteredWave, BUFFER_LEN);
        mid2FloatPks[freq] = floatPkPk(floatMid2FilteredWave, BUFFER_LEN);
        mid3FloatPks[freq] = floatPkPk(floatMid3FilteredWave, BUFFER_LEN);
        highFloatPks[freq] = floatPkPk(floatHighFilteredWave, BUFFER_LEN);
        fullFloatPks[freq] = floatPkPk(floatFullFilteredWave, BUFFER_LEN);

        //reset value
        memset(sfintWave, 0, BUFFER_LEN*sizeof(sfint));
        memset(floatWave, 0, BUFFER_LEN*sizeof(float));
        
        memset(sfintLowFilteredWave, 0, BUFFER_LEN*sizeof(sfint));
        memset(floatLowFilteredWave, 0, BUFFER_LEN*sizeof(float));
    
        memset(sfintMid1FilteredWave, 0, BUFFER_LEN*sizeof(sfint));
        memset(floatMid1FilteredWave, 0, BUFFER_LEN*sizeof(float));
    
        memset(sfintMid2FilteredWave, 0, BUFFER_LEN*sizeof(sfint));
        memset(floatMid2FilteredWave, 0, BUFFER_LEN*sizeof(float));
    
        memset(sfintMid3FilteredWave, 0, BUFFER_LEN*sizeof(sfint));
        memset(floatMid3FilteredWave, 0, BUFFER_LEN*sizeof(float));
    
        memset(sfintHighFilteredWave, 0, BUFFER_LEN*sizeof(sfint));
        memset(floatHighFilteredWave, 0, BUFFER_LEN*sizeof(float));
        printf("iteration %d\n", freq);
    }

    //void dumpFreqToCSV(int* freq, sfint* lowpk, float* lowpkIdeal, sfint* mid1pk, float* mid1pkIdeal, sfint* mid2pk, float* mid2pkIdeal, sfint* mid3pk, float* mid3pkIdeal, sfint* highpk, float* highpkIdeal, int n, char* title) {

    dumpFreqToCSV(freqs, lowpks, lowFloatPks, mid1pks, mid1FloatPks, mid2pks, mid2FloatPks, mid3pks, mid3FloatPks, highpks, highFloatPks, fullpks, fullFloatPks, BANDWIDTH, "output.csv");
    
    deinitCoefficients(coeffs);
    deinitGains(gains);


}