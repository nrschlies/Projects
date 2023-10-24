#ifndef CAESAR_CIPHER_H
#define CAESAR_CIPHER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1024

char* caesar_cipher(char* plaintext, int shift);
char* caesar_decipher(char* ciphertext, int shift);

#endif