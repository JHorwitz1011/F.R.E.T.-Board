#include <stdio.h>
#include <stdint.h>

#include "util.h"
#include "filters.h"


void testIntToFixed(int target) {
    printf("from int ");
    printBits(sizeof(target), &target);
    
    printf(" to fixed ");
    sfint a = int_to_sfint(target);
    printBits(sizeof(a), &a);

    printf("\n");
}

void testFixedToInt(sfint target) {
    printf("from fixed ");
    printBits(sizeof(target), &target);
    
    printf(" to int ");
    int a = sfint_to_int(target);
    printBits(sizeof(a), &a);
    
    printf("\n");
}

void testFloatToFixed(float target) {
    printf("from float ");
    printBits(sizeof(target), &target);
    
    printf(" to fixed ");
    sfint a = float_to_sfint(target);
    printBits(sizeof(a), &a);

    printf("\n");
}

void testFixedToFloat(sfint target) {
    printf("from fixed ");
    printBits(sizeof(target), &target);
    
    printf(" to float ");
    float a = sfint_to_float(target);
    printBits(sizeof(a), &a);
    
    printf(" %f \n", a);
}

void testUintToFixed(uint16_t target) {
    printf("from uint16 ");
    printBits(sizeof(target), &target);
    
    printf(" to fixed ");
    sfint a = uint16_to_sfint(target);
    printBits(sizeof(a), &a);

    printf("\n");
}

void testFixedToUint(sfint target) {
    printf("from fixed ");
    printBits(sizeof(target), &target);
    
    printf(" to uint16 ");
    uint16_t a = sfint_to_uint16(target);
    printBits(sizeof(a), &a);
    
    printf(" %d \n", a);
}

void testMultiplication(sfint a, sfint b) {
    printf("a ");
    printBits(sizeof(a), &a);
    
    printf(" b ");
    printBits(sizeof(b), &b);
    
    printf(" a*b ");
    sfint ab = mul_sfint(a,b);
    printBits(sizeof(ab), &ab);

    printf("\n");
}

void main() {
    printf("\n~~~~~~~INT TO FIXED~~~~~~~\n");
    printf("last valid -32768, breaks at -32769 (overflow to positive limit).\n");
    testIntToFixed(-32768);
    testIntToFixed(-32769);
    printf("last valid 32767, breaks at 32768 (overflow to negative limit).\n");
    testIntToFixed(32767);
    testIntToFixed(32768);

    printf("\n~~~~~~~FIXED TO INT~~~~~~~\n");
    printf("no limit; observe min and max values\n");
    testFixedToInt(0b10000000000000000000000000000000);
    testFixedToInt(0b01111111111111111111111111111111);

    printf("\n~~~~~~~FLOAT TO FIXED~~~~~~~\n");
    printf("no lower bound; just caps at -32768\n");
    testFloatToFixed(-32767.0f);
    testFloatToFixed(-32768.0f);
    testFloatToFixed(-32769.0f);
    testFloatToFixed(-9999999999999999.0f);

    printf("upper bound somehere between 32767.99 and 32768 (overflow to pos)\n");
    testFloatToFixed(32767.9972544f);
    testFloatToFixed(32768);

    printf("\n~~~~~~~FIXED TO FLOAT~~~~~~~\n");
    printf("no limits. observe min and max values\n");
    testFixedToFloat(0b10000000000000000000000000000000);
    testFixedToFloat(0b00000000000000000000000000001000);
    testFixedToFloat(0b01111111111111111111111111111111);

    // test uint -> fixed range
    printf("\n~~~~~~~FIXED TO UINT16~~~~~~~\n");
    printf("no limits, precision matches directly\n");
    testFixedToUint(0b10000000000000000000000000000000);
    testFixedToUint(0b10000000100000000000000000000000);
    testFixedToUint(int_to_sfint(-30423));
    testFixedToUint(int_to_sfint(-5101));
    testFixedToUint(int_to_sfint(-5));
    testFixedToUint(int_to_sfint(0));
    testFixedToUint(int_to_sfint(5));
    testFixedToUint(0b01111111111111111111111111111111);
    
    // test fixed -> uint range
    printf("\n~~~~~~~UINT16 TO FIXED~~~~~~~\n");
    printf("no limits. observe min and max values\n");
    testUintToFixed(0x0000);
    testUintToFixed(0x00FF);
    testUintToFixed(0x8000);
    testUintToFixed(0xFF00);
    testUintToFixed(0xFFFF);

    printf("\n~~~~~~~~ TEST MULTIPLICATION ~~~~~~~~~~~~\n");
    printf("limit occurs when product of two input exceeds 32767\n");
    testMultiplication(int_to_sfint(0),int_to_sfint(0));
    testMultiplication(int_to_sfint(1),int_to_sfint(0));
    testMultiplication(int_to_sfint(1),int_to_sfint(1));
    testMultiplication(int_to_sfint(10),int_to_sfint(10));
    testMultiplication(int_to_sfint(-10),int_to_sfint(-10));
    testMultiplication(int_to_sfint(10),int_to_sfint(-10));
    testMultiplication(int_to_sfint(10000), float_to_sfint(3.2));

}