#pragma once
#include "filters.h"

void printFixedBinary(sfint input);
void printIntBinary(int input);
void printBits(size_t const size, void const * const ptr);


float minFloat(float* buffer, int n);
float minFixed(sfint* buffer, int n);

float maxFloat(float* buffer, int n);
float maxFixed(sfint* buffer, int n);

float averageFloat(float* buffer, int n);
float averageFixed(sfint* buffer, int n);

void printCoeffs(Coeff* coeffs);