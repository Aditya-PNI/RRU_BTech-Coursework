/*Program: DES Encryption and Decryption
Course: G4A19ITC: Introduction to Cryptography (LAB)
Semester: 4
Language: C

Description:
Implements a DES-style Feistel cipher using expansion permutation, S-box substitution and multiple rounds.*/
#include <stdio.h>
#include <stdlib.h>

#define ull unsigned long long
#define ul unsigned long
#define MASK32 (((ull)1<<32)-1)
#define ROUNDS 16

int main(){

    ull plaintext, ciphertext, key, decrypted;
    ull roundKeys[ROUNDS];
    ull expandedR = 0;

    ul L0, R0, temp, swap1, swap2;

    int bits[32];
    int mapped[48];
    int block[8][6];
    int row[8], col[8];
    int value;

    int rowBit1 = 0;
    int rowBit2 = 5;

    int expansionTable[48]={
        32,1,2,3,4,5,
        4,5,6,7,8,9,
        8,9,10,11,12,13,
        12,13,14,15,16,17,
        16,17,18,19,20,21,
        20,21,22,23,24,25,
        24,25,26,27,28,29,
        28,29,30,31,32,1
    };

    int sbox[8][4][16]={
        {
            {0,3,6,10,13,5,4,12,1,7,11,2,9,15,8,14},
            {11,2,6,4,10,8,15,7,1,14,9,0,3,12,5,13},
            {8,3,10,15,0,5,1,13,7,11,4,6,2,9,14,12},
            {15,8,5,0,14,11,3,9,10,2,7,12,4,13,6,1}
        },
        {
            {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
            {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
            {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
            {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
        },
        {
            {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
            {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
            {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
            {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
        },
        {
            {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
            {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
            {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
            {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
        },
        {
            {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
            {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
            {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
            {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
        },
        {
            {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
            {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
            {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
            {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
        },
        {
            {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
            {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
            {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
            {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
        },
        {
            {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
            {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
            {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
            {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
        }
    };

    printf("Enter plain text : ");
    scanf("%llu",&plaintext);

    printf("Enter key : ");
    scanf("%llu",&key);

    L0 = (ul)(plaintext >> 32);
    R0 = (ul)(plaintext & MASK32);

    srand((unsigned)key);

    for(int i=0;i<ROUNDS;i++)
        roundKeys[i] = ((ull)rand()<<32) | rand();

    for(int i=0;i<ROUNDS;i++){

        temp = L0;
        L0 = R0;

        for(int j=0;j<32;j++)
            bits[j] = (R0>>(31-j)) & 1;

        for(int j=0;j<48;j++)
            mapped[j] = bits[expansionTable[j]-1];

        expandedR = 0;

        for(int j=0;j<48;j++)
            expandedR = (expandedR<<1) | mapped[j];

        expandedR ^= roundKeys[i];

        int k=0;

        for(int r=0;r<8;r++)
            for(int c=0;c<6;c++)
                block[r][c] = (expandedR>>(47-k++)) & 1;

        for(int j=0;j<8;j++){

            row[j] = block[j][rowBit1];
            row[j] = (row[j]<<1) | block[j][rowBit2];

            col[j] = block[j][1];
            col[j] = (col[j]<<1) | block[j][2];
            col[j] = (col[j]<<1) | block[j][3];
            col[j] = (col[j]<<1) | block[j][4];
        }

        R0 = 0;

        for(int j=0;j<8;j++){
            value = sbox[j][row[j]][col[j]];
            R0 = (R0<<4) | (ul)value;
        }

        R0 ^= temp;
    }

    swap1 = L0;
    L0 = R0;
    R0 = swap1;

    ciphertext = ((ull)L0<<32) | R0;

    printf("Encryption : %llu",ciphertext);

    //Decrypt

    L0 = (ul)(ciphertext >> 32);
    R0 = (ul)(ciphertext & MASK32);

    for(int i=ROUNDS-1;i>=0;i--){

        temp = L0;
        L0 = R0;

        for(int j=0;j<32;j++)
            bits[j] = (R0>>(31-j)) & 1;

        for(int j=0;j<48;j++)
            mapped[j] = bits[expansionTable[j]-1];

        expandedR = 0;

        for(int j=0;j<48;j++)
            expandedR = (expandedR<<1) | mapped[j];

        expandedR ^= roundKeys[i];

        int k=0;

        for(int r=0;r<8;r++)
            for(int c=0;c<6;c++)
                block[r][c] = (expandedR>>(47-k++)) & 1;

        for(int j=0;j<8;j++){

            row[j] = block[j][rowBit1];
            row[j] = (row[j]<<1) | block[j][rowBit2];

            col[j] = block[j][1];
            col[j] = (col[j]<<1) | block[j][2];
            col[j] = (col[j]<<1) | block[j][3];
            col[j] = (col[j]<<1) | block[j][4];
        }

        R0 = 0;

        for(int j=0;j<8;j++){
            value = sbox[j][row[j]][col[j]];
            R0 = (R0<<4) | (ul)value;
        }

        R0 ^= temp;
    }

    swap2 = L0;
    L0 = R0;
    R0 = swap2;

    decrypted = ((ull)L0<<32) | R0;

    printf("\nDecryption: %llu",decrypted);

    return 0;
}