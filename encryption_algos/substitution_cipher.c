#include "substitution_cipher.h"
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1024

static char* single_substitution_cipher(char* plaintext, char* key) {
    static char buffer[MAX_SIZE];
    int length = strlen(plaintext);

    for (int i = 0; i < length; i++) {
        char c = plaintext[i];

        if (isalpha(c)) {
            if (islower(c)) {
                buffer[i] = key[c - 'a'];
            } else {
                buffer[i] = toupper(key[c - 'A']);
            }
        } else {
            buffer[i] = c;
        }
    }
    buffer[length] = '\0';
    return buffer;
}

static char* single_substitution_decipher(char* ciphertext, char* key) {
    static char buffer[MAX_SIZE];
    int length = strlen(ciphertext);
    char reverse_key[26];

    for (int i = 0; i < 26; i++) {
        char c = key[i];
        reverse_key[c - 'a'] = 'a' + i;
    }

    for (int i = 0; i < length; i++) {
        char c = ciphertext[i];

        if (isalpha(c)) {
            if (islower(c)) {
                buffer[i] = reverse_key[c - 'a'];
            } else {
                buffer[i] = toupper(reverse_key[c - 'A']);
            }
        } else {
            buffer[i] = c;
        }
    }
    buffer[length] = '\0';
    return buffer;
}

char* substitution_cipher(char* plaintext, char** keys, int num_keys) {
    char* temp = plaintext;
    for (int i = 0; i < num_keys; i++) {
        temp = single_substitution_cipher(temp, keys[i]);
    }
    return temp;
}

char* substitution_decipher(char* ciphertext, char** keys, int num_keys) {
    char* temp = ciphertext;
    for (int i = num_keys - 1; i >= 0; i--) {
        temp = single_substitution_decipher(temp, keys[i]);
    }
    return temp;
}