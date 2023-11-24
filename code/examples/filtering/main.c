#include <math.h>
#include <stdio.h>

#include "filters.h"

#define BUFFER_LEN 10000


void dumpBuffer(int freq, float* wave, sfint castedWave, sfint filteredWave, int n) {
        printf("for a wave of frequency %d\n", freq);
        printf("the original float wave has an average of %f, max of %f, and min of %f.\n", averageFloat(wave, BUFFER_LEN), maxFloat(wave, BUFFER_LEN), minFloat(wave, BUFFER_LEN));

}

void main() {


    // //cos buffer
    // float time[BUFFER_LEN];
    // float wave[BUFFER_LEN]; 
    // sfint castedWave[BUFFER_LEN];
    // sfint filteredWave[BUFFER_LEN];

    // for(int i = 0; i < BUFFER_LEN; i++) {
    //     time[i] = ((float)i)*.0001;
    // }

    // for(int freq = 1; freq < 10000; freq+= 100) { //hz
        
    // }

    
    

}