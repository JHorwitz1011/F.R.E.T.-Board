#include <stdlib.h>
#include "filters.h"


int main() {
    Coeff* coeffs = malloc(sizeof(Coeff));
    fillCoefficients(coeffs);
    free(coeffs);
}