#include <stdio.h>

#include "filters.h"
#include "util.h"

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
}

void printFixedBinary(sfint input) {
    for(int i = 0; i < sizeof(sfint)*8; i++) {
        printf("%d", (input & (0x01 << i)) >> i );
    }
}

void printIntBinary(int input) {
    int copy = input;
    for(int i = 0; i < sizeof(int)*8; i++) {
        // printf("test");
        printf("%d", (copy & (0b1 << i)) >> i );
        copy = input;
    }
}

float minFloat(float* buffer, int n) {
    float min = 2;
    for(int i = 0; i < n; i++) {
        if(buffer[i] < min)
            min = buffer[i];
    }
    return min;
}

float minFixed(sfint* buffer, int n) {
    float min = 65535;
    for(int i = 0; i < n; i++) {
        float conversion = sfint_to_float(buffer[i]);
        if(conversion < min)
            min = conversion;
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

float maxFixed(sfint* buffer, int n) {
    float min = 0;
    for(int i = 0; i < n; i++) {
        float conversion = sfint_to_float(buffer[i]);
        if(conversion > min)
            min = conversion;
    }
    return min;
}


float averageFloat(float* buffer, int n) {
    float sum = 0;
    for(int i = 0; i< n; i++) {
        sum += buffer[i]/((float)n);
    }
    return sum;
}

float averageFixed(sfint* buffer, int n) {
    float sum = 0;
    for(int i = 0; i< n; i++) {
        sum += sfint_to_float(buffer[i])/((float)n);
    }
    return sum;
}

