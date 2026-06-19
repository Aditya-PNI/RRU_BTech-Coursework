/*Program: Longest Common Subsequence (LCS)
Course: G4AD18DAA: Design Analysis of Algorithm
Semester: 4
Language: C

Description:
Finds the Longest Common Subsequence between two strings using Dynamic Programming.

Time Complexity: O(m*n)
Space Complexity: O(m*n)*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>

void main(){
    char x[100] = "abcb";
    char y[100]="abaab";
    int i,j,m,n;
    m=strlen(x);
    n=strlen(y);
    int L[m+1][n+1];
    for(i=0;i<=m;i++){
        for(j=0;j<=n;j++){
            if(i==0 || j==0){
                L[i][j]=0;
            }
            else if(x[i-1]==y[j-1]){
                L[i][j]=L[i-1][j-1]+1;
            }
            else{
                L[i][j]=(L[i-1][j]>L[i][j-1])?L[i-1][j]:L[i][j-1];
            }
        }
    }

    int index=L[m][n];
    char lcs[index+1];
    lcs[index]='\0';
    i=m;
    j=n;
    while(i>0 && j>0){
        if(x[i-1]==y[j-1]){
            lcs[index-1]=x[i-1];
            i--;
            j--;
            index--;
        }
        else if(L[i-1][j]>L[i][j-1]){
            i--;
        }
        else{
            j--;
        }
    }
    printf("LCS is %s\n",lcs);
    printf("Length of LCS is %d\n",L[m][n]);

}