#include <stdlib.h>

#include "pi.h"

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

    for(int j = 0; j < num; ++j) {
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

void pi_bellard(long num, char *buf) {
    
}