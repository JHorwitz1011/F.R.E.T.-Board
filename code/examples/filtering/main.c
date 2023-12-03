#include <math.h>
#include <stdio.h>
#include <string.h>

#include "filters.h"
#include "util.h"

#define BUFFER_LEN 44100
#define SAMPLE_LEN 1 //sec
#define AMPLITUDE 9500.0f
#define BANDWIDTH 15000

#define PRECISION 1

void dumpBuffer(int freq, float* wave, sfint* castedWave, sfint* filteredWave, int n) {
        printf("for a wave of frequency %d\n", freq);
        printf("the original float wave has an average of %f, max of %f, and min of %f.\n", averageFloat(wave, BUFFER_LEN), maxFloat(wave, BUFFER_LEN), minFloat(wave, BUFFER_LEN));
        printf("the casted wave has an average of %f, max of %f, and min of %f.\n", averageFixed(castedWave, BUFFER_LEN), maxFixed(castedWave, BUFFER_LEN), minFixed(castedWave, BUFFER_LEN));
        printf("the filtered wave has an average of %f, max of %f, and min of %f.\n", averageFixed(filteredWave, BUFFER_LEN), maxFixed(filteredWave, BUFFER_LEN), minFixed(filteredWave, BUFFER_LEN));

}

void dumpBufferToCSV(float* time, float* floatWave, sfint* sfintWave, sfint* sfintFilteredWave, float* floatFilteredWave, int n) {
    FILE *fpt;
    fpt = fopen("output.csv", "w+");
    fprintf(fpt,"time, floatWave, floatFilteredWave, sfintWave, sfintFilteredWave\n");
    for(int i = 0; i < n; i++) {
        fprintf(fpt, "%f, %f, %f, %f, %f\n", time[i], floatWave[i], floatFilteredWave[i], sfint_to_float(sfintWave[i]), sfint_to_float(sfintFilteredWave[i]));
    }

    fclose(fpt);
}

void dumpFreqToCSV(int* freq, float* pktopk, int n) {
    FILE *fpt;
    fpt = fopen("output.csv", "w+");
    fprintf(fpt,"freq, pktopk\n");
    for(int i = 0; i < n; i++) {
        fprintf(fpt, "%d, %f\n", freq[i], pktopk[i]);
    }

    fclose(fpt);
}

int main() {
    Coeff* coeffs = initCoefficients();
    Gains* gains = initGains();
    printCoeffs(coeffs);

    //fill buffers
    float time[BUFFER_LEN] = {0.0f};
    float floatWave[BUFFER_LEN] = {0.0f}; 
    float floatFilteredWave[BUFFER_LEN] = {0.0f};
    sfint sfintWave[BUFFER_LEN] = {0};
    sfint sfintFilteredWave[BUFFER_LEN] = {0};
    

    //fill time buffer
    for(int i = 0; i < BUFFER_LEN; i++) {
        time[i] = ((float)i)/(BUFFER_LEN);
    }


    //singular freq
    int freq = 10;
    //sweep frequencies
    // for(int freq = 0; freq <= BANDWIDTH; freq += 1) { //hz

        //first, calculate wave values
        for(int i = 0; i < BUFFER_LEN; i++) {
            floatWave[i] = AMPLITUDE*sin(2.0f*M_PI*freq*time[i]);
            sfintWave[i] = float_to_sfint(floatWave[i]);
        }

        //then, filter the values
        for(int i = 2; i < BUFFER_LEN; i++) {
            sfintFilteredWave[i] = lows(sfintWave+i, sfintFilteredWave+i, coeffs);
            floatFilteredWave[i] = lowFloat(floatWave+i, floatFilteredWave+i, coeffs);
            // floatFilteredWave[i] = (.0001)*(floatWave[i]) + (.0002)*(floatWave[i-1]) + (.0001)*(floatWave[i-2]) - (-1.9752)*(floatFilteredWave[i-1]) - (0.9755)*(floatFilteredWave[i-2]);
        }

        //log results
        printf("%d\n", freq);
        
        // //reset value
        // memset(sfintFilteredWave, 0, BUFFER_LEN*sizeof(sfint));
        // memset(sfintWave, 0, BUFFER_LEN*sizeof(sfint));
        // memset(floatFilteredWave, 0, BUFFER_LEN*sizeof(float));
        // memset(floatWave, 0, BUFFER_LEN*sizeof(float));
    // }
    dumpBufferToCSV(time, floatWave, sfintWave, sfintFilteredWave, floatFilteredWave, BUFFER_LEN);
    // dumpFreqToCSV(freqs, amplitudes, BANDWIDTH);
    
    deinitCoefficients(coeffs);
    deinitGains(gains);


}