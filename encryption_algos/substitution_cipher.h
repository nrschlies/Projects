#ifndef SUBSTITUTION_CIPHER_H
#define SUBSTITUTION_CIPHER_H

char* substitution_cipher(char* plaintext, char** keys, int num_keys);
char* substitution_decipher(char* ciphertext, char** keys, int num_keys);

#endif
