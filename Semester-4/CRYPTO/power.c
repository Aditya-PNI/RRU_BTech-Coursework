/*Program: Power Calculation
Course: G4A19ITC: Introduction to Cryptography (LAB)
Semester: 4
Language: C

Description:
Calculates 3^32 using iterative multiplication.*/
#include<stdio.h>
#define ull unsigned long long int

int main(){
    ull result=1;

    for(int i=1;i<33;i++){
        result*=3;
    }
    printf("%llu", result);

    return 0;
}