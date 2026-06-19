/*Program: Square and Multiply Algorithm
Course: G4A19ITC: Introduction to Cryptography (LAB)
Semester: 4
Language: C

Description:
Implements fast modular exponentiation using the Square and Multiply algorithm.*/
#include <stdio.h>

long long square_and_multiply(long long base, long long exp, long long mod) {
    if (mod == 1) return 0; 
    
    long long res = 1;
    base%= mod;
    

    if (base < 0) base += mod; //for -ve

    while (exp > 0) {
        
        if (exp & 1) {
            //overflow if (res * base) > 2^63 - 1
            res = (res * base) % mod;
        }

        // Potential overflow here if (base * base) > 2^63 - 1
        base = (base * base) % mod;
        
        // Bitwise right shift is equivalent to exp / 2
        exp >>= 1;
    }
    return res;
}

int main() {
    long long base, exp, mod;

    printf("Enter base, exponent, and modulus: ");
    if (scanf("%lld %lld %lld", &base, &exp, &mod) != 3) {
        printf("Invalid input.\n");
        return 1;
    }

    if (mod <= 0) {
        printf("Modulus must be positive.\n");
        return 1;
    }

    long long result = square_and_multiply(base, exp, mod);
    printf("Result: %lld\n", result);

    return 0;
}