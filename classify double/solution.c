#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>



/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64 (double number) {
    return *((uint64_t *)(&number));
}

bool getBit (const uint64_t number, const uint8_t index) {
    return (number >> index) & 1u;
}


bool checkRange(const uint64_t number, const uint8_t checkValue, const uint8_t from_index, const uint8_t to_index) {
    bool ret_val = true;
    uint8_t i = from_index;
    while (ret_val && i <= to_index) {
        ret_val = (((number >> i) & 1) == checkValue);
        i++;
    }
    return ret_val;
}


/**
 * Checkers here:
 */

bool checkForPlusZero (uint64_t number) {
    return number == 0x0;
}

bool checkForMinusZero (uint64_t number) {
    return number == 0x8000000000000000;
}

bool checkForPlusInf (uint64_t number) {
    return ( !getBit(number, 63)) &&//check for sign == 0 (plus)
        checkRange(number, 1, 52, 62) &&// check for exponent == 1  
        checkRange(number, 0, 0, 51);//check for #0 ..#51 is   zero 
}

bool checkForMinusInf (uint64_t number) {
    return checkRange(number, 1, 52, 63) &&// check for exponent == 1 and sign ==1
           checkRange(number, 0, 0, 51);//check for #0 ..#51 is   zero 
}

bool checkForPlusNormal (uint64_t number) {
    return  !getBit(number, 63) &&//check for sign == 0 (plus)
        !checkRange(number, 0, 52, 62) && !checkRange(number, 1, 52, 62);// check for the exponent (not all zeros and not all ones)


}

bool checkForMinusNormal (uint64_t number) {
    return  getBit(number, 63) &&//check for sign ==   1 (minus)
        !checkRange(number, 0, 52, 62) && !checkRange(number, 1, 52, 62);// check for the exponent (not all zeros and not all ones)
}

bool checkForPlusDenormal (uint64_t number) {
    return checkRange(number, 0, 52, 63) &&//check for sign == 0 and  exponent == 0
        (!checkRange(number, 0, 0, 51)); //check for fractional is not zero
}

bool checkForMinusDenormal (uint64_t number) {
    return getBit(number, 63) &&//check for sign == 1 (minus)
        checkRange(number, 0, 52, 62) &&// check for exponent == 0
        (!checkRange(number, 0, 0, 51));//check for fractional is not zero
}

bool checkForSignalingNan(uint64_t number) {
    return checkRange(number, 1, 52, 62) &&// check for exponent == 1
        (!getBit(number, 51)) &&//bit #51 is zero
        ( !checkRange(number, 0, 0, 50));//check for #0 ..#50 is not zero 
}
bool checkForQuietNan (uint64_t number) {
    return checkRange(number, 1, 51, 62);// check for exponent and  bit #51   == 1
   
 }        


void classify (double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus normal\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus normal\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}
void print_double_bits(double num) {
    uint64_t uint_number = convertToUint64(num);
    for (uint8_t i = 0; i < 64; i++) {
        printf("%d", getBit(uint_number, 63 - i));
    }
}
void print_double_inf(char * str) {
    double num;
    sscanf(str, "%lE", &num);
    printf("Input %s, num is %lE  . classify: ", str, num);
    //printf("\t");
    //print_double_bits( num);
 
    classify(num);
}

void print_err_double_inf(const uint64_t number) {
    double dnum = *((double*)(&number));
     
    printf("uint64 %" PRIx64 ", num is %lE . classify:  ", number, dnum);
    //print_double_bits(dnum);
    classify(dnum);
}
int main(void) {
    print_double_inf("+1");
    print_double_inf("-2");
    print_double_inf("+30E-311");
    print_double_inf("-40E-311");
    print_double_inf("5E-411");
    print_double_inf("-6E-411");
    print_double_inf("7E411");
    print_double_inf("-8E411");
    print_err_double_inf(0x7FF0000000000001);
    print_err_double_inf(0x7FF8000000000001);
  


    return 0;
}