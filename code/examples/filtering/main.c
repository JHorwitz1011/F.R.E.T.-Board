#include <math.h>
#include <stdio.h>

#include "filters.h"
#include "util.h"

#define BUFFER_LEN 500000
#define SAMPLE_LEN 1 //sec
#define AMPLITUDE 9500

void dumpBuffer(int freq, float* wave, sfint* castedWave, sfint* filteredWave, int n) {
        printf("for a wave of frequency %d\n", freq);
        printf("the original float wave has an average of %f, max of %f, and min of %f.\n", averageFloat(wave, BUFFER_LEN), maxFloat(wave, BUFFER_LEN), minFloat(wave, BUFFER_LEN));
        printf("the casted wave has an average of %f, max of %f, and min of %f.\n", averageFixed(castedWave, BUFFER_LEN), maxFixed(castedWave, BUFFER_LEN), minFixed(castedWave, BUFFER_LEN));
        printf("the filtered wave has an average of %f, max of %f, and min of %f.\n", averageFixed(filteredWave, BUFFER_LEN), maxFixed(filteredWave, BUFFER_LEN), minFixed(filteredWave, BUFFER_LEN));

}

void dumpBufferToCSV(int freq, float* time, float* wave, sfint* castedWave, sfint* filteredWave, int n) {
    FILE *fpt;
    fpt = fopen("output.csv", "w+");
    fprintf(fpt,"time, wave, castedWave, filteredWave\n");
    for(int i = 0; i < n; i++) {
        fprintf(fpt, "%f, %f, %f, %f\n", time[i], wave[i], sfint_to_float(castedWave[i]), sfint_to_float(filteredWave[i]));
    }

    fclose(fpt);
}

int main() {
    Coeff* coeffs = initCoefficients();
    Gains* gains = initGains();

    // //cos buffer
    float time[BUFFER_LEN] = {0.0f};
    float wave[BUFFER_LEN] = {0.0f}; 
    sfint castedWave[BUFFER_LEN] = {0};
    sfint filteredWave[BUFFER_LEN] = {0};

    for(int i = 0; i < BUFFER_LEN; i++) {
        time[i] = ((float)i)/(BUFFER_LEN);
    }
    int freq = 10000;

    // for(int freq = 0; freq < 10000; freq += 100) { //hz
        for(int i = 0; i < BUFFER_LEN; i++) {
            wave[i] = AMPLITUDE*sin(2.0f*M_PI*freq*time[i]);
            castedWave[i] = float_to_sfint(wave[i]);
        }

        for(int i = BUFFER_LEN-6; i >= 0; i--) {
            filter(castedWave+i, filteredWave+i, coeffs, gains);
        }
        dumpBuffer(freq, wave, castedWave, filteredWave, BUFFER_LEN);
    // }
    dumpBufferToCSV(freq, time, wave, castedWave, filteredWave, BUFFER_LEN);
    
    deinitCoefficients(coeffs);
    deinitGains(gains);


}