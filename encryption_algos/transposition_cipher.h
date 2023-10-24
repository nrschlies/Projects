#ifndef TRANSPOSITION_CIPHER_H
#define TRANSPOSITION_CIPHER_H

char* transposition_encrypt(const char* plaintext, const char* key);
char* transposition_decrypt(const char* ciphertext, const char* key);

#endif
