#include "caesar_cipher.h"

char* caesar_cipher(char* plaintext, int shift) {
    static char ciphertext[MAX_SIZE];
    int length = strlen(plaintext);

    for(int i = 0; i < length; i++) {
        char c = plaintext[i];

        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            ciphertext[i] = (c - base + shift) % 26 + base;
        } else {
            ciphertext[i] = c;
        }
    }
    ciphertext[length] = '\0';
    return ciphertext;
}

char* caesar_decipher(char* ciphertext, int shift) {
    return caesar_cipher(ciphertext, 26 - shift);  // Since there are 26 letters in the English alphabet
}