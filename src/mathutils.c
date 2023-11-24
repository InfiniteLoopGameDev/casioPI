#include <stdint.h>
/* Trial Division Prime factorization algoritm 
   factors should be an array of all (non-repeated) prime factors of length max_factors with a maximum value of prime_limit
   Returns number of factors
   Some ideas taken from
   https://en.wikipedia.org/wiki/Trial_division#Method */
int prime_factorisation(int64_t integer, int64_t *factors , int max_factors, int64_t prime_limit) {
    int64_t original_integer = integer;
    int factor_count = 0;

    // Not needed as all numbers should be odd
   /* if (integer % 2 == 0) {
        factors[factor_count++] = 2;
    }
    while (integer % 2 == 0) {
        integer /= 2;
    } */

    //TODO: Fix this so we don't iterate until prime limit

    int64_t f = 3;
    int64_t p = 16;
    int64_t ac = 9;
    while (f <= prime_limit & factor_count < max_factors) { // Stop if factor is greater than the limit or exceeded lenght of array
        ac += p; // ac, p, and f are all witchcraft to me its based off wikipedia 
        // if (ac > integer) { factors[factor_count++] = f; break;};
        if (integer % f == 0) { // Check if is factor
            factors[factor_count++] = f; // Add factor to array
            while (integer % f == 0) { // No repeated factors needed
                integer /= f; // Divide all of them
                ac -= p; 
            }
        } else {
            f += 2;
            p += 8;
        }
    }
    if (integer > 1 & integer <= prime_limit) { // Add last digit and catch prime
        factors[factor_count++] = integer;
    }
    
    return factor_count;
}


/* Multiple of factor closest to n but not greater*/
int64_t closest_multiple(int64_t n, int64_t factor) {
    return factor * (n / factor);
}