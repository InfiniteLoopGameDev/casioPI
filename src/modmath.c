#include <stdlib.h>
#include "mathutils.h"

const int MAX_FACTORS = 20;

/* Modular Multiplication
   a*b mod m
   Based of code found on Wikipedia
   https://en.wikipedia.org/wiki/Modular_arithmetic#Example_implementations */
int64_t mul_mod(int64_t a, int64_t b, int64_t m) {
    int64_t d = 0, mp2 = m >> 1;
    int64_t i;
    if (a >= m) a %= m;
    if (b >= m) b %= m;
    for (i = 0; i < 64; ++i) {
        d = (d > mp2) ? (d << 1) - m : d << 1;
        if (a & 0x8000000000000000LL) d += b;
        if (d >= m) d -= m;
        a <<= 1;
    }
    return d;
}

/* Sum of Modular Multiplication
   (a*b + c*d) mod m
   = (a*b mod m + c*d mod m) mod m */
int64_t sum_mul_mod(int64_t a, int64_t b, int64_t c, int64_t d, int64_t m) {
    int64_t ab = mul_mod(a, b, m);
    int64_t cd = mul_mod(c, d, m);
    return (ab + cd) % m;
}

/* Modular Multiplicative Inverse
   such that a*x mod m = 1 
   Returns 0 on error */
int64_t inv_mod(int64_t a, int64_t m) {
    /* Extended Euclidean Algorithm
    Returns GCD from *gcd and a coefficient from *coeff
    Such that a*coeff = gcd mod m*/
    int64_t oldrem = a, rem = m;
    int64_t olds = 1, s = 0;
    int64_t temp;
    while (rem != 0) {
        int64_t quot = oldrem / rem;
        temp = rem;
        rem = oldrem - quot * rem;
        oldrem = temp;
        temp = s;
        s = olds - quot * s;
        olds = temp;
    }
    if (oldrem =! 1) {
        return 0;
    }
    return olds;
}

/* Modular Exponentiation
   a^b mod m
   Based of code found on Wikipedia
   https://en.wikipedia.org/wiki/Modular_exponentiation#Right-to-left_binary_method */
int64_t pow_mod(int64_t a, int64_t b, int64_t m) {
    int64_t r = 1;
    while (b > 0) {
        if (b & 1) {
            r = mul_mod(r, a, m);
        }
        b = b >> 1;
        a = mul_mod(a, a, m);
    }
    return r;
}


/* Sum of binomials modulo an integer
    ∑ ^k ̌ j (binom(n, j) mod m)
    Based of the original algoritm 
    http://numbers.computation.free.fr/Constants/Algorithms/nthdecimaldigit.pdf
    and its official implementation
    http://numbers.computation.free.fr/Constants/Algorithms/pidec.cpp */
int64_t sum_binomial_mod(int64_t k, int64_t n, int64_t m) {
    int64_t prime_factors[MAX_FACTORS] = {0};
    int factor_count = prime_factorisation(m, prime_factors, MAX_FACTORS, k);

    int64_t A = 1, B = 1, C = 1;
    int64_t R[MAX_FACTORS];
    for (int i = 0; i < factor_count; i++) { R[i] = 1; }
    

    for (int64_t j = 1; j <= k; j++) {
        int64_t a = n - j + 1;
        int64_t b = j;
    }
}