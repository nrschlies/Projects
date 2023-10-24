#ifndef RC4_H
#define RC4_H

#include <stddef.h>

// RC4 encryption/decryption function
void RC4(unsigned char *key, size_t len_key, unsigned char *plaintext, size_t len_text, unsigned char *ciphertext);

#endif /* RC4_H */
