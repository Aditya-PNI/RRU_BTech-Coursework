/*Course: G4A19ITC: Introduction to Cryptography (LAB)
Semester: 4
Language: C*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to process the plaintext according to Playfair 
void preparePlaintext(char *input, char *output) {
    int len = strlen(input);
    int j = 0;
    
    for (int i = 0; i < len; i++) {
        char current = toupper(input[i]);
        if (current == 'J') current = 'I'; // Convert J to I 
        
        output[j++] = current;
        
        // Handle repeating letters in a pair
        if (i + 1 < len) {
            char next = toupper(input[i+1]);
            if (next == 'J') next = 'I';
            if (current == next) {
                output[j++] = 'X'; // Insert filler 'X'
            }
        }
    }
    

    if (j % 2 != 0) {
        output[j++] = 'X';
    }
    output[j] = '\0';
}

// Generate the 5x5 key matrix
void generateMatrix(char *key, char matrix[5][5]) {
    int alphabet[26] = {0};
    alphabet['J' - 'A'] = 1; // Treat J as I
    int x = 0, y = 0;
    

    for (int i = 0; key[i] != '\0'; i++) {
        char c = toupper(key[i]);
        if (c == 'J') c = 'I';
        if (!alphabet[c - 'A']) {
            matrix[x][y++] = c;
            alphabet[c - 'A'] = 1;
            if (y == 5) { x++; y = 0; }
        }
    }
    
    // Fill remaining alphabet
    for (int i = 0; i < 26; i++) {
        if (!alphabet[i]) {
            matrix[x][y++] = (char)(i + 'A');
            alphabet[i] = 1;
            if (y == 5) { x++; y = 0; }
        }
    }
}

// to find coordinates in matrix
void findPosition(char matrix[5][5], char c, int *row, int *col) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (matrix[i][j] == c) {
                *row = i; *col = j;
                return;
            }
        }
    }
}

// Playfair Encryption
void playfairEncrypt(char *text, char matrix[5][5], char *res) {
    for (int i = 0; i < strlen(text); i += 2) {
        int r1, c1, r2, c2;
        findPosition(matrix, text[i], &r1, &c1);
        findPosition(matrix, text[i+1], &r2, &c2);
        
        if (r1 == r2) {
            res[i] = matrix[r1][(c1 + 1) % 5];
            res[i+1] = matrix[r2][(c2 + 1) % 5];
        } else if (c1 == c2) {
            res[i] = matrix[(r1 + 1) % 5][c1];
            res[i+1] = matrix[(r2 + 1) % 5][c2];
        } else {
            res[i] = matrix[r1][c2];
            res[i+1] = matrix[r2][c1];
        }
    }
    res[strlen(text)] = '\0';
}

//Affine Encryption
void affineEncrypt(char *text, char *res) {
    for (int i = 0; text[i] != '\0'; i++) {
        res[i] = (char)((((11 * (text[i] - 'A')) + 15) % 26) + 'A');
    }
    res[strlen(text)] = '\0';
}

// Shift Encryption 
void shiftEncrypt(char *text, int k, char *res) {
    for (int i = 0; text[i] != '\0'; i++) {
        res[i] = (char)((((text[i] - 'A') + k) % 26) + 'A');
    }
    res[strlen(text)] = '\0';
}

// Inverse modular for Affine Decryption 
void affineDecrypt(char *text, char *res) {
    for (int i = 0; text[i] != '\0'; i++) {
        int val = (19 * ((text[i] - 'A' - 15 + 26) % 26)) % 26;
        res[i] = (char)(val + 'A');
    }
    res[strlen(text)] = '\0';
}

void playfairDecrypt(char *text, char matrix[5][5], char *res) {
    for (int i = 0; i < strlen(text); i += 2) {
        int r1, c1, r2, c2;
        findPosition(matrix, text[i], &r1, &c1);
        findPosition(matrix, text[i+1], &r2, &c2);
        
        if (r1 == r2) {
            res[i] = matrix[r1][(c1 + 4) % 5];
            res[i+1] = matrix[r2][(c2 + 4) % 5];
        } else if (c1 == c2) {
            res[i] = matrix[(r1 + 4) % 5][c1];
            res[i+1] = matrix[(r2 + 4) % 5][c2];
        } else {
            res[i] = matrix[r1][c2];
            res[i+1] = matrix[r2][c1];
        }
    }
    res[strlen(text)] = '\0';
}

int main() {
    char plaintext[100], delta[200], key1[100], c1[200], c2[200], c3[200];
    char matrix[5][5];
    int k3;


    printf("Enter plaintext: ");
    scanf("%s", plaintext);

    preparePlaintext(plaintext, delta);
    printf("Delta: %s\n", delta); 
 
    printf("Enter key K1: ");
    scanf("%s", key1);
 
    generateMatrix(key1, matrix);
    printf("Key Matrix:\n");
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++) printf("%c ", matrix[i][j]);
        printf("\n");
    }

    playfairEncrypt(delta, matrix, c1);
    printf("C1 (Playfair): %s\n", c1);


    affineEncrypt(c1, c2);
    printf("C2 (Affine): %s\n", c2);


    printf("Enter shift key K3: ");
    scanf("%d", &k3);
    shiftEncrypt(c2, k3, c3);
    printf("C3 (Shift): %s\n\n", c3);


    printf("Decryption Process\n");
    char d2[200], d1[200], final[200];
    
    // Reverse Shift
    shiftEncrypt(c3, 26 - k3, d2);
    printf("Decrypted from Shift: %s\n", d2);
    
    // Reverse Affine
    affineDecrypt(d2, d1);
    printf("Decrypted from Affine: %s\n", d1);
    
    // Reverse Playfair
    playfairDecrypt(d1, matrix, final);
    printf("Decrypted Playfair (Final Delta): %s\n", final);

    return 0;
}