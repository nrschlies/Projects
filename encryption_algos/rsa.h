// rsa.h
#ifndef RSA_H
#define RSA_H

typedef struct {
    unsigned long *chunks;
    size_t size;
    int sign;
} BigInt;

BigInt* initBigInt(void);
int isZero(BigInt *b);
void appendDigit(BigInt *b, int digit);
BigInt* createBigInt(const char *str);
BigInt* cloneBigInt(BigInt *source);
void freeBigInt(BigInt *b);
void printBigInt(BigInt *b);
BigInt* addBigInt(BigInt *a, BigInt *b);
BigInt* subtractBigInt(BigInt *a, BigInt *b);
BigInt* multiplyBigInts(BigInt *a, BigInt *b);
BigInt* divideBigInt(BigInt *a, BigInt *b);
BigInt* modulusBigInt(BigInt *a, BigInt *b);
BigInt* powerModBigInt(BigInt *base, BigInt *exponent, BigInt *modulus);
int miller_rabin(BigInt *n, int k);
BigInt* generate_prime(int num_bits);
BigInt* extended_gcd(BigInt *a, BigInt *b, BigInt **x, BigInt **y);
BigInt* randomBigIntRange(BigInt *lower, BigInt *upper);
BigInt* mod_inverse(BigInt *a, BigInt *m);
void generate_keypair(int num_bits, BigInt **public_key_n, BigInt **public_key_e, BigInt **private_key_n, BigInt **private_key_d);
BigInt* rsa_encrypt(BigInt *message, BigInt *e, BigInt *n);
BigInt* rsa_decrypt(BigInt *ciphertext, BigInt *d, BigInt *n);

#endif // RSA_H