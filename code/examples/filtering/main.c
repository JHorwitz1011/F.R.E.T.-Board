#include <math.h>
#include <stdio.h>

#include "filters.h"

#define BUFFER_LEN 10000

float minFloat(float* buffer, int n) {
    float min = 2;
    for(int i = 0; i < n; i++) {
        if(buffer[i] < min)
            min = buffer[i];
    }
    return min;
}

s7x16 minFixed(s7x16* buffer, int n) {
    s7x16 min = 65535;
    for(int i = 0; i < n; i++) {
        if(buffer[i] < min)
            min = buffer[i];
    }
    return min;
}

float maxFloat(float* buffer, int n) {
    float max = -2;
    for(int i = 0; i < n; i++) {
        if(buffer[i] > max)
            max = buffer[i];
    }
    return max;
}

s7x16 maxFixed(s7x16* buffer, int n) {
    s7x16 max = 0;
    for(int i = 0; i < n; i++) {
        if(buffer[i] > max)
            max = buffer[i];
    }
    return max;
}


void averageFloat(float* buffer, int n) {
    float sum = 0;
    for(int i = 0; i< n; i++) {
        sum += buffer[i]/((float)n)
    }
}

void averageFixed(s7x16* buffer, int n) {
    
}

void dumpBuffer(int freq, float* wave, s7x16 castedWave, s7x16 filteredWave, int n) {
        printf("For frequency %f\n 
                The original wave has average %f min %f and max %f\n 
                The casted wave has average %f min %f and max %f\n 
                The filtered wave has average %f min %f and max %f\n\n", 
                freq, 
                averageFloat(wave, BUFFER_LEN), minFloat(wave, BUFFER_LEN), maxFloat(wave, BUFFER_LEN),
                averageFixed(castedWave, BUFFER_LEN), minFixed(castedWave, BUFFER_LEN), maxFixed(BUFFER_LEN),
                averageFixed(castedWave, BUFFER_LEN), minFixed(castedWave, BUFFER_LEN), maxFixed(BUFFER_LEN));
}

void main() {


    //cos buffer
    float time[BUFFER_LEN];
    float wave[BUFFER_LEN]; 
    s7x16 castedWave[BUFFER_LEN];
    s7x16 filteredWave[BUFFER_LEN];

    for(int i = 0; i < BUFFER_LEN; i++) {
        time[i] = ((float)i)*.0001;
    }

    for(int freq = 1; freq < 10000; freq+= 100) { //hz
        
    }
    
}