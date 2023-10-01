#include <stdint.h>
#include <stdio.h>
#include "mathutils.h"

const int MAX_FACTORS = 20;

/* Modular Multiplication (for unsigned integers)
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
    ∑ ^k ̌ j (binom(N, j) mod m)
    Based of the original algorithm 
    http://numbers.computation.free.fr/Constants/Algorithms/nthdecimaldigit.pdf
    and its official implementation
    http://numbers.computation.free.fr/Constants/Algorithms/pidec.cpp */
int64_t sum_binomial_mod(int64_t k, int64_t N, int64_t m) {
    // TODO: malloc some this sh*t
    // TODO: Optimisation when k>n/2 ->  2^n - sum_{j=0}^{n-k-1} binomial(n,j)

    if (k > N / 2) {
        return pow_mod(2, N, m) - sum_binomial_mod(N, N - k - 1, m);
    }

    int64_t prime_factors[MAX_FACTORS]; // List of prime factors (non repeating) of m
    int factor_count = prime_factorisation(m, prime_factors, MAX_FACTORS, k);

    int64_t A = 1, B = 1, C = 1;
    int64_t R[MAX_FACTORS];

    // TODO: Try without this silliness and just using a % prime == 0
    int64_t closest_multiple_a[MAX_FACTORS]; // Arrays that stores closest multiple of p[i] for a
    int64_t closest_multiple_b[MAX_FACTORS]; // Array that stores closest multiple of p[i] for b
    for (int i = 0; i < factor_count; i++) {
        R[i] = 1;
        closest_multiple_a[i] = closest_multiple(N, prime_factors[i]); // Starting with multiple of p[i] to N as a starts as N
        // TODO: memcpy?
        closest_multiple_b[i] = prime_factors[i]; // Starting with p[i], b starts at 1, so won't be won't be factor until equal 
    }

    int64_t R_product = 1; // Stores the product of entire array R with respect to m

    for (int64_t j = 1; j <= k; j++) {
        int R_array_updated = 0;

        int64_t a = N - j + 1;
        int64_t a_complement = a;
        int64_t b = j;
        int64_t b_complement = b;

        for (int i = 0; i < factor_count; i++) {
            int64_t prime = prime_factors[i];

            if (closest_multiple_a[i] == a) { // If a is equal to nearest multliple, prime is a factor of a
                a_complement /= prime; // Factor out prime
                R[i] *= prime; // Add power of prime to corresponding R 
                while (a_complement % prime == 0) { // Repeat for all instances of the prime
                    R[i] *= prime;
                    a_complement /= prime;
                }
                closest_multiple_a[i] -= prime; // Next closest multiple will be less as a is decreasing
                R_array_updated = 1;
            }

            if (closest_multiple_b[i] == b) {
                b_complement /= prime;
                R[i] /= prime; // Add negative power of prime to corresponding R
                while (b_complement % prime == 0) {
                    R[i] /= prime;
                    b_complement /= prime; 
                }
                closest_multiple_b[i] += prime; // Next closest prime will be more as b is increasing
                R_array_updated = 1;
            }
        }

        if (R_array_updated == 1) {
            R_product = R[0];
            for (int i = 0; i < factor_count; i++) {
                R_product = mul_mod(R_product, R[i], m);
            }
        }

        A = mul_mod(A, a_complement, m);
        B = mul_mod(B, b_complement, m);

        if (R_product == 1) {
            C = mul_mod(C, b_complement, m) + A;
        } else {
            C = sum_mul_mod(C, b_complement, A, R_product, m);
        }
    }
    int64_t sum = (C * inv_mod(B, m)) % m;
    return sum;
}