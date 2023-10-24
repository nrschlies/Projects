#include "playfair_cipher.h"
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MATRIX_SIZE 5
#define MAX_SIZE 1024

static void generate_key_table(char* key, char table[MATRIX_SIZE][MATRIX_SIZE]) {
    bool present[26] = {0};
    int i, j;
    char alphabet = 'A';

    // Process the key
    for (i = 0, j = 0; *key; key++) {
        char c = toupper(*key);
        if (c == 'J') c = 'I';  // Treating J as I
        if (!present[c - 'A']) {
            table[i][j] = c;
            present[c - 'A'] = true;
            j++;
            if (j == MATRIX_SIZE) {
                i++;
                j = 0;
            }
        }
    }

    // Process the remaining alphabet characters
    for (; alphabet <= 'Z'; alphabet++) {
        if (alphabet == 'J') continue;  // Skip J
        if (!present[alphabet - 'A']) {
            table[i][j] = alphabet;
            j++;
            if (j == MATRIX_SIZE) {
                i++;
                j = 0;
            }
        }
    }
}

// Find the position of a character in the key table
static void find_position(char table[MATRIX_SIZE][MATRIX_SIZE], char c, int *x, int *y) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (table[i][j] == c) {
                *x = i;
                *y = j;
                return;
            }
        }
    }
}

static void cipher_pair(char table[MATRIX_SIZE][MATRIX_SIZE], char *a, char *b) {
    int x1, y1, x2, y2;
    find_position(table, *a, &x1, &y1);
    find_position(table, *b, &x2, &y2);

    // Same row, wrap around
    if (x1 == x2) {
        y1 = (y1 + 1) % MATRIX_SIZE;
        y2 = (y2 + 1) % MATRIX_SIZE;
    }
    // Same column, wrap around
    else if (y1 == y2) {
        x1 = (x1 + 1) % MATRIX_SIZE;
        x2 = (x2 + 1) % MATRIX_SIZE;
    } 
    // Different row and column
    else {
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }
    *a = table[x1][y1];
    *b = table[x2][y2];
}

static void decipher_pair(char table[MATRIX_SIZE][MATRIX_SIZE], char *a, char *b) {
    int x1, y1, x2, y2;
    find_position(table, *a, &x1, &y1);
    find_position(table, *b, &x2, &y2);

    // Same row, wrap around
    if (x1 == x2) {
        y1 = (y1 - 1 + MATRIX_SIZE) % MATRIX_SIZE;
        y2 = (y2 - 1 + MATRIX_SIZE) % MATRIX_SIZE;
    }
    // Same column, wrap around
    else if (y1 == y2) {
        x1 = (x1 - 1 + MATRIX_SIZE) % MATRIX_SIZE;
        x2 = (x2 - 1 + MATRIX_SIZE) % MATRIX_SIZE;
    } 
    // Different row and column
    else {
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }
    *a = table[x1][y1];
    *b = table[x2][y2];
}

char* playfair_cipher(char* plaintext, char* key) {
    static char ciphertext[MAX_SIZE];
    char table[MATRIX_SIZE][MATRIX_SIZE];
    generate_key_table(key, table);

    int length = strlen(plaintext);
    int k = 0; // Index for ciphertext

    for (int i = 0; i < length; i += 2) {
        char a = toupper(plaintext[i]);
        char b = (i+1 < length) ? toupper(plaintext[i+1]) : 'X'; // If odd length, add padding
        if (a == 'J') a = 'I';
        if (b == 'J') b = 'I';
        cipher_pair(table, &a, &b);
        ciphertext[k++] = a;
        ciphertext[k++] = b;
    }
    ciphertext[k] = '\0';

    return ciphertext;
}

char* playfair_decipher(char* ciphertext, char* key) {
    static char plaintext[MAX_SIZE];
    char table[MATRIX_SIZE][MATRIX_SIZE];
    generate_key_table(key, table);

    int length = strlen(ciphertext);
    int k = 0;

    for (int i = 0; i < length; i += 2) {
        char a = toupper(ciphertext[i]);
        char b = toupper(ciphertext[i+1]);
        if (a == 'J') a = 'I';
        if (b == 'J') b = 'I';
        decipher_pair(table, &a, &b);
        plaintext[k++] = a;
        plaintext[k++] = b;
    }
    plaintext[k] = '\0';

    return plaintext;
}
