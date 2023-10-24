#ifndef _3DES_H_
#define _3DES_H_

#include <stdint.h>

// DES Encryption
uint64_t DES_encrypt(uint64_t plaintext, uint64_t key);
uint64_t DES_decrypt(uint64_t ciphertext, uint64_t key);
uint64_t DES3_encrypt(uint64_t plaintext, uint64_t key1, uint64_t key2, uint64_t key3);
uint64_t DES3_decrypt(uint64_t ciphertext, uint64_t key1, uint64_t key2, uint64_t key3);

#endif // _3DES_H_