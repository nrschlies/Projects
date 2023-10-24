#include "rc4.h"

static void swap(unsigned char *a, unsigned char *b) {
    unsigned char temp = *a;
    *a = *b;
    *b = temp;
}

static void KSA(unsigned char *key, unsigned char *S, size_t len_key) {
    int j = 0;
    for (int i = 0; i < 256; i++) {
        S[i] = i;
    }
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % len_key]) % 256;
        swap(&S[i], &S[j]);
    }
}

static void PRGA(unsigned char *S, unsigned char *plaintext, unsigned char *ciphertext, size_t len_text) {
    int i = 0;
    int j = 0;
    for (size_t n = 0; n < len_text; n++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        swap(&S[i], &S[j]);
        int rnd = S[(S[i] + S[j]) % 256];
        ciphertext[n] = rnd ^ plaintext[n];
    }
}

void RC4(unsigned char *key, size_t len_key, unsigned char *plaintext, size_t len_text, unsigned char *ciphertext) {
    unsigned char S[256];
    KSA(key, S, len_key);
    PRGA(S, plaintext, ciphertext, len_text);
}
