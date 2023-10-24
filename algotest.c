#include "caesar_cipher.h"
#include "substitution_cipher.h"
#include "vigenere_cipher.h"
#include "playfair_cipher.h"
#include "transposition_cipher.h"
#include "enigma_machine.h"
#include "3des.h"
#include "rc4.h"
#include "rsa.h"
#include <stdio.h>
#include <time.h>

BigInt *one;
BigInt *two;

void test_caesar_cipher() {
    char plaintext[] = "This is a Caesar cipher test!";
    int shift = 7; // Sample shift

    int iterations = 1000000; // You may want to test with a larger iteration count since Caesar cipher is faster

    clock_t start, end;
    double cpu_time_used;

    // Test encryption speed
    start = clock();
    for (int i = 0; i < iterations; i++) {
        caesar_cipher(plaintext, shift);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Caesar Encryption took %f seconds for %d iterations.\n", cpu_time_used, iterations);

    char* ciphertext = caesar_cipher(plaintext, shift);

    // Test decryption speed
    start = clock();
    for (int i = 0; i < iterations; i++) {
        caesar_decipher(ciphertext, shift);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Caesar Decryption took %f seconds for %d iterations.\n", cpu_time_used, iterations);
}

void test_substitution_cipher() {
    char plaintext[MAX_SIZE] = "HELLO WORLD"; 
    char* keys[] = { "qwertyuiopasdfghjklzxcvbnm" };  // Array of keys
    int num_keys = 1;  // Number of keys in the array

    clock_t start, end;
    double cpu_time_used;

    // Test encryption speed
    start = clock();
    char* encrypted = substitution_cipher(plaintext, keys, num_keys);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Substitution Cipher Encryption took %f seconds.\n", cpu_time_used);

    // Test decryption speed
    start = clock();
    char* decrypted = substitution_decipher(encrypted, keys, num_keys);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Substitution Cipher Decryption took %f seconds.\n", cpu_time_used);
}

void test_vigenere_cipher() {
    char plaintext[MAX_SIZE] = "HELLO WORLD"; 
    char key[] = "KEY"; // Sample key

    clock_t start, end;
    double cpu_time_used;

    // Test encryption speed
    start = clock();
    char* encrypted = vigenere_cipher(plaintext, key);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Vigenère Cipher Encryption took %f seconds.\n", cpu_time_used);

    // Test decryption speed
    start = clock();
    char* decrypted = vigenere_decipher(encrypted, key);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Vigenère Cipher Decryption took %f seconds.\n", cpu_time_used);
}

void test_playfair() {
    char plaintext[MAX_SIZE] = "HELLO";
    char key[MAX_SIZE] = "KEYWORD";  // Sample key

    clock_t start, end;
    double cpu_time_used;

    // Test encryption speed
    start = clock();
    char* encrypted = playfair_cipher(plaintext, key);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Playfair Cipher Encryption took %f seconds.\n", cpu_time_used);

    // Test decryption speed
    start = clock();
    char* decrypted = playfair_decipher(encrypted, key);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Playfair Cipher Decryption took %f seconds.\n", cpu_time_used);
}

void test_transposition_cipher() {
    char plaintext[MAX_SIZE] = "HELLO WORLD";
    char key[] = "KEY"; // Sample key

    clock_t start, end;
    double cpu_time_used;

    // Test encryption speed
    start = clock();
    char* encrypted = transposition_encrypt(plaintext, key);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Transposition Cipher Encryption took %f seconds.\n", cpu_time_used);

    // Test decryption speed
    start = clock();
    char* decrypted = transposition_decrypt(encrypted, key);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Transposition Cipher Decryption took %f seconds.\n", cpu_time_used);
}

void test_enigma_machine() {
    char plaintext[MAX_SIZE] = "HELLO";
    char output[MAX_SIZE];

    clock_t start, end;
    double cpu_time_used;

    // Test encryption speed
    start = clock();
    enigma_encrypt(plaintext, output, 0, 1, 2);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Enigma Encryption took %f seconds.\n", cpu_time_used);

    printf("Original: %s\nEncrypted: %s\n", plaintext, output);
}

void test_3DES() {
    uint64_t plaintext = 0x0123456789ABCDEF;
    uint64_t key1 = 0x0123456789ABCDEFULL; // Sample key1
    uint64_t key2 = 0xFEDCBA9876543210ULL; // Sample key2
    uint64_t key3 = 0x0123456789ABCDEFULL; // Sample key3

    int iterations = 100000; // Number of iterations to check speed

    clock_t start, end;
    double cpu_time_used;

    // Test encryption speed
    start = clock();
    for (int i = 0; i < iterations; i++) {
        DES3_encrypt(plaintext, key1, key2, key3);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("3DES Encryption took %f seconds for %d iterations.\n", cpu_time_used, iterations);

    uint64_t ciphertext = DES3_encrypt(plaintext, key1, key2, key3);

    // Test decryption speed
    start = clock();
    for (int i = 0; i < iterations; i++) {
        DES3_decrypt(ciphertext, key1, key2, key3);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("3DES Decryption took %f seconds for %d iterations.\n", cpu_time_used, iterations);
}

void test_rc4() {
    unsigned char key[] = "sample_key";
    unsigned char plaintext[] = "RC4 encryption test!";
    unsigned char ciphertext[sizeof(plaintext)];
    unsigned char decrypted[sizeof(plaintext)];
    
    // Zero out the memory
    memset(ciphertext, 0, sizeof(ciphertext));
    memset(decrypted, 0, sizeof(decrypted));

    clock_t start, end;
    double cpu_time_used;

    // Test encryption speed
    start = clock();
    RC4(key, strlen((char*)key), plaintext, strlen((char*)plaintext), ciphertext);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("RC4 Encryption took %f seconds.\n", cpu_time_used);

    // Test decryption speed (RC4 is symmetric so the function is the same)
    start = clock();
    RC4(key, strlen((char*)key), ciphertext, strlen((char*)plaintext), decrypted);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("RC4 Decryption took %f seconds.\n", cpu_time_used);

    // Optional: Check if decryption matches original plaintext
    if (memcmp(plaintext, decrypted, sizeof(plaintext)) == 0) {
        printf("RC4 Decryption was successful.\n");
    } else {
        printf("RC4 Decryption failed.\n");
    }
}

void test_rsa() {
    printf("Testing RSA...\n");
    
    BigInt *public_key_n, *public_key_e, *private_key_n, *private_key_d;

    // Generate RSA key pair
    generate_keypair(512, &public_key_n, &public_key_e, &private_key_n, &private_key_d); // For demonstration, using 512-bit key

    // Sample message
    BigInt *message = createBigInt("1234567890123456789012345678901234567890123456789012345678901234"); // Ensure it's smaller than n
    printf("Original Message: ");
    printBigInt(message);

    // Encrypt the message
    BigInt *encrypted_message = rsa_encrypt(message, public_key_e, public_key_n);
    printf("Encrypted Message: ");
    printBigInt(encrypted_message);

    // Decrypt the message
    BigInt *decrypted_message = rsa_decrypt(encrypted_message, private_key_d, private_key_n);
    printf("Decrypted Message: ");
    printBigInt(decrypted_message);

    // Clean up
    freeBigInt(message);
    freeBigInt(encrypted_message);
    freeBigInt(decrypted_message);
    freeBigInt(public_key_n);
    freeBigInt(public_key_e);
    freeBigInt(private_key_n);
    freeBigInt(private_key_d);
}

int main() {
    one = createBigInt("1");
    two = createBigInt("2");
    test_caesar_cipher();
    test_substitution_cipher();
    test_vigenere_cipher();
    test_playfair();
    test_transposition_cipher();
    test_enigma_machine();
    test_3DES();
    test_rc4();
    test_rsa();
    return 0;
}