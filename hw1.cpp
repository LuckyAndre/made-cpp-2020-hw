#include <stdio.h>
#include <stdint.h>

uint64_t convertToUint64(double number) {
    return *((uint64_t *)(&number));
}

bool getBit (const uint64_t number, const uint8_t index) {
    uint64_t mask = 1 << (index - 1);
    uint64_t temp = mask & number;
    temp = temp >> (index - 1);
    return temp;
}

bool checkForMinusZero (uint64_t number) {
    return number == 0x8000000000000000;
}

bool checkForPlusZero (uint64_t number) {
    return number == 0x0;
}

bool checkForMinusInf (uint64_t number) {
    return number == 0xFFF000000000;
}

bool checkForPlusInf (uint64_t number) {
    return number == 0x7FF000000000;
}


int checkForSign (uint64_t number) {
    return getBit(number, 64);
}

int checkForDenormal (uint64_t number) {
    int s = 0;

    for (uint8_t i = 63; i >= 53; --i)
        s += getBit(number, i); // all should be 0

    int last_bit = getBit(number, 1); // should be 1

    int result = 0;
    if ((s == 0) & (last_bit==1)) {
        result = 1;
    }
    return result==1;
}


bool checkForNormal (uint64_t number) {
    int s = 0;

    for (uint8_t i = 63; i >= 53; --i)
        s += getBit(number, i); // 0 < s < 11

    int result = 0;
    if ((s > 0) & (s < 11)) {
        result = 1;
    }
    return result == 1;
}

bool checkForPositiveDenormal (uint64_t number) {
    return (checkForSign(number)==0) & checkForDenormal(number);
}

bool checkForNegativeDenormal (uint64_t number) {
    return (checkForSign(number)==1) & checkForDenormal(number);
}

bool checkForPositiveNormal (uint64_t number) {
    return (checkForSign(number)==0) & checkForNormal(number);
}

bool checkForNegativeNormal (uint64_t number) {
    return (checkForSign(number)==1) & checkForDenormal(number);
}

bool checkForSNan (uint64_t number) {
    int s = 0;

    for (uint8_t i = 63; i >= 53; --i)
        s += getBit(number, i); // all should be 1

    int first_fraction_bit = getBit(number, 1); // should be 1
    int last_fraction_bit = getBit(number, 52); // should be 0

    int result = 0;
    if ((s == 11) & (first_fraction_bit==1) & (last_fraction_bit==0)) {
        result = 1;
    }
    return result==1;
}

bool checkForQNan (uint64_t number) {
    int s = 0;

    for (uint8_t i = 63; i >= 53; --i)
        s += getBit(number, i); // all should be 1

    int first_fraction_bit = getBit(number, 1); // should be 1

    int result = 0;
    if ((s == 11) & (first_fraction_bit==1)) {
        result = 1;
    }
    return result==1;
}


void classify (double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus infinity\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus  infinity\n");
    }

    else if (checkForPositiveDenormal(convertToUint64(number))) {
            printf("Positive Denormal\n");
    }

    else if (checkForNegativeDenormal(convertToUint64(number))) {
        printf("Negative Denormal\n");
    }

    else if (checkForPositiveNormal(convertToUint64(number))) {
        printf("Positive Normal\n");
    }

    else if (checkForNegativeNormal(convertToUint64(number))) {
        printf("Negative Normal\n");
    }

    else if (checkForQNan(convertToUint64(number))) {
        printf("QNan\n");
    }

    else if (checkForSNan(convertToUint64(number))) {
        printf("SNan\n");
    }
}


int main() {
    double x;
    scanf("%lf", &x);
    classify(x);
    return 0;
}
