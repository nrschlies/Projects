#include "vigenere_cipher.h"
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1024

static char vigenere_transform(char c, char key, int direction) {
    if (!isalpha(c)) return c;

    char offset = islower(c) ? 'a' : 'A';
    char keyOffset = islower(key) ? 'a' : 'A';

    return offset + (c - offset + direction * (key - keyOffset) + 26) % 26;
}

char* vigenere_cipher(char* plaintext, char* key) {
    static char ciphertext[MAX_SIZE];
    int key_len = strlen(key);
    int length = strlen(plaintext);

    for (int i = 0; i < length; i++) {
        ciphertext[i] = vigenere_transform(plaintext[i], key[i % key_len], 1);
    }
    ciphertext[length] = '\0';

    return ciphertext;
}

char* vigenere_decipher(char* ciphertext, char* key) {
    static char plaintext[MAX_SIZE];
    int key_len = strlen(key);
    int length = strlen(ciphertext);

    for (int i = 0; i < length; i++) {
        plaintext[i] = vigenere_transform(ciphertext[i], key[i % key_len], -1);
    }
    plaintext[length] = '\0';

    return plaintext;
}
