#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "modmath.h"

const int NUMBER_OF_DIGITS = 14;

void append_digit(int digit, char *buf, int *pos) {
    buf[*pos] = (digit + '0');
    (*pos)++;
}

// Pi calculated using the Rabinowitz/Wagon pi spigot algorithm
// Based of the Pascal code by Simeon Simeonov
// Both can be found at: 
// https://www.maa.org/sites/default/files/pdf/pubs/amm_supplements/Monthly_Reference_12.pdf

void pi_rabinowitz(int num, char *buf){
    int len = ((10 * num/3) + 1);
    int* A = (int*) malloc(len * sizeof(int));
    for (int i = 0; i < len; i++) { A[i] = 2; } // Initialize array to 2s
    int current_digit = 0;

    int nines = 0;
    int predigit = 0;

    while (current_digit < num) {
        int q = 0;

        for(int i = len; i > 0; --i) {
            int x  = 10 * A[i-1] + q*i;
            A[i-1] = x % (2*i - 1);
            q = x / (2*i - 1);
        }

        A[0] = q % 10;
        q = q / 10;

        if (q == 9) {
            ++nines;
        } else if (q == 10) {
            append_digit(predigit + 1, buf, &current_digit);

            for (int k = 0; k < nines; ++k) { 
                append_digit(0, buf, &current_digit);
            }
            predigit = 0;
            nines = 0;
        } else {
            append_digit(predigit, buf, &current_digit);
            predigit = q;

            if (nines != 0) {    
                for (int k = 0; k < nines; ++k) {
                    append_digit(9, buf, &current_digit);
                }
                nines = 0;
            }
        }
    }
    append_digit(predigit, buf, &current_digit);
    free(A);
}

float pi_bellard(long num, char *buf) {
    long M = 2 * ceil((float) num / pow(log(num), 3));
    long N = ceil((num + NUMBER_OF_DIGITS + 1) * log(10) / (1 + log(2 * M))); // should be labelled ln on the paper
    N += N%2; // Make N next even number

    float b = 0;
    //TODO: Investigate 2k+1 - 2k+3 in k+2 increments
    // Could bring performance improvments
    for (long k = 0; k < ((M + 1) * N); k++) {
        long exponent = pow_mod(10, num, 2*k+1);
        long x = mul_mod(4, exponent, 2*k+1);
        float operation = k % 2 == 0 ? 1. : -1.; // Subtract if odd, add if even
        b += operation * ((float) x / (float) (2*k+1));
        b -= floor(b); // Only keep fractional part
    }
    printf("b: %f\n", b);

    float c = 0;
    for (long k = 0; k < N; k++) {
        long modulus = 2 * M * N + 2 * k + 1;
        long x = sum_binomial_mod(k, N, modulus);
        printf("k: %d, N: %d, mod: %d, x: %d\n", k, N, modulus, x);
        long lhs = pow_mod(5, N - 2, modulus);
        long rhs = pow_mod(10, num - N + 2, modulus);
        long y = mul_mod(lhs, rhs, modulus);
        float operation = k % 2 == 0 ? 1. : -1.; // Subtract if odd, add if even
        c += operation * (float) y / (float) modulus;
        c -= floor(c); // Only keep fractional part
    }

    return b - c;
}

int main() {
    char empty[64];
    float pi = pi_bellard(542, empty);
    printf("Pi: %f\n", pi);
    return 0;
}