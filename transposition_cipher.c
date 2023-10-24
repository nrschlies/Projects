#include "transposition_cipher.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 1024

static int* get_column_order(const char* key, int key_len) {
    int* order = (int*)malloc(key_len * sizeof(int));
    bool* taken = (bool*)malloc(key_len * sizeof(bool));
    memset(taken, false, key_len);

    for (int i = 0; i < key_len; i++) {
        int min_index = -1;
        for (int j = 0; j < key_len; j++) {
            if (!taken[j] && (min_index == -1 || key[j] < key[min_index])) {
                min_index = j;
            }
        }
        order[i] = min_index;
        taken[min_index] = true;
    }

    free(taken);
    return order;
}

char* transposition_encrypt(const char* plaintext, const char* key) {
    static char ciphertext[MAX_SIZE];
    int text_len = strlen(plaintext);
    int key_len = strlen(key);

    int* column_order = get_column_order(key, key_len);

    int idx = 0;
    for (int i = 0; i < key_len; i++) {
        int col = column_order[i];
        for (int j = col; j < text_len; j += key_len) {
            ciphertext[idx++] = plaintext[j];
        }
    }

    ciphertext[idx] = '\0';
    free(column_order);
    return ciphertext;
}

char* transposition_decrypt(const char* ciphertext, const char* key) {
    static char plaintext[MAX_SIZE];
    int text_len = strlen(ciphertext);
    int key_len = strlen(key);

    int* column_order = get_column_order(key, key_len);

    int idx = 0;
    int* offset = (int*)malloc(key_len * sizeof(int));
    offset[0] = 0;
    for (int i = 1; i < key_len; i++) {
        offset[i] = offset[i-1] + (text_len + i) / key_len;
    }

    for (int j = 0; j < (text_len + key_len - 1) / key_len; j++) {
        for (int i = 0; i < key_len; i++) {
            if (j + offset[column_order[i]] < text_len) {
                plaintext[idx++] = ciphertext[j + offset[column_order[i]]];
            }
        }
    }

    plaintext[idx] = '\0';
    free(column_order);
    free(offset);
    return plaintext;
}
