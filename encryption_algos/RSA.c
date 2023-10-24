#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rsa.h"

int isZero(BigInt *b);
BigInt* initBigInt(void);
void appendDigit(BigInt *b, int digit);
int compareBigInts(BigInt *a, BigInt *b);
void normalize(BigInt *b);
int isEven(BigInt *b);
void subtractDigitInPlace(BigInt *b, int digit);
void divideBy2InPlace(BigInt *b);
int isOne(BigInt *b);
BigInt* modBigInt(BigInt *a, BigInt *b);
BigInt* multiplyBigInts(BigInt *a, BigInt *b);
BigInt* cloneBigInt(BigInt *source);
BigInt* gcdBigInt(BigInt *a, BigInt *b);
BigInt* randomBigInt(int num_bits);
int isBigIntEqual(BigInt *a, BigInt *b);
BigInt* randomBigIntRange(BigInt *lower, BigInt *upper);
BigInt* createBigIntFromInt(int val);
int isBigIntEqualToInt(BigInt *b, int val);

extern BigInt *one;
extern BigInt *two;

BigInt* initBigInt(void) {
    BigInt* result = (BigInt*)malloc(sizeof(BigInt));
    if (result == NULL) {
        // Memory allocation failed, handle as appropriate for your application.
        exit(1);
    }
    
    result->chunks = NULL; // No chunks yet
    result->size = 0;      // No chunks, so size is 0
    result->sign = 1;      // Default to positive sign

    return result;
}

int isBigIntEqual(BigInt *a, BigInt *b) {
    if (a->size != b->size) {
        return 0; // Not equal if sizes differ
    }
    for (size_t i = 0; i < a->size; i++) {
        if (a->chunks[i] != b->chunks[i]) {
            return 0; // Not equal if any digit differs
        }
    }
    return 1; // If we've reached here, they are equal
}

int isZero(BigInt *b) {
    if (b->size == 0 || (b->size == 1 && b->chunks[0] == 0)) {
        return 1; // BigInt represents zero
    }
    return 0; // BigInt does not represent zero
}

BigInt* cloneBigInt(BigInt *source) {
    BigInt* result = initBigInt();
    result->chunks = (unsigned long*)malloc(result->size * sizeof(unsigned long));*)malloc(source->size * sizeof(int));
    if (result->chunks == NULL) {
        // Handle memory allocation failure
        exit(1);
    }
    memcpy(result->chunks, source->chunks, source->size * sizeof(int));
    result->size = source->size;
    result->sign = source->sign;
    return result;
}

int is_divisible_by_small_primes(BigInt *num) {
    int small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    int num_small_primes = sizeof(small_primes) / sizeof(small_primes[0]);

    for (int i = 0; i < num_small_primes; i++) {
        // If num is one of the small primes
        if (isBigIntEqualToInt(num, small_primes[i])) {
            return 1;
        }

        // If num is divisible by one of the small primes
        BigInt *divisor = createBigIntFromInt(small_primes[i]);
        BigInt *remainder = modBigInt(num, divisor);
        if (isZero(remainder)) {
            freeBigInt(divisor);
            freeBigInt(remainder);
            return 1;
        }
        freeBigInt(divisor);
        freeBigInt(remainder);
    }
    return 0;
}

BigInt* createBigIntFromInt(int val) {
    char buffer[12];  // To accommodate 10 chunks plus possible '-' sign and '\0' char
    sprintf(buffer, "%d", val);
    return createBigInt(buffer);
}


void appendDigit(BigInt *b, int digit) {
    b->chunks = realloc(b->chunks, (b->size + 1) * sizeof(int));
    if (b->chunks == NULL) {
        exit(1); // Memory allocation error
    }
    b->chunks[b->size] = digit;
    b->size++;
}

int compareBigInts(BigInt *a, BigInt *b) {
    if (a->size != b->size) {
        return a->size - b->size;
    }
    for (int i = 0; i < a->size; i++) {
        if (a->chunks[i] != b->chunks[i]) {
            return a->chunks[i] - b->chunks[i];
        }
    }
    return 0;  // Both BigInts are equal
}

int isBigIntEqualToInt(BigInt *b, int val) {
    if (b->size == 1) {
        return b->chunks[0] == val;
    }
    return 0;  // BigInt has more than one digit, so it's not equal to val
}



int isEven(BigInt *b) {
    if (b->size == 0) {
        return 1; // zero is even
    }
    return b->chunks[b->size - 1] % 2 == 0;
}

int isOne(BigInt *b) {
    return b->size == 1 && b->chunks[0] == 1;
}

void normalize(BigInt *b) {
    while (b->size > 1 && b->chunks[0] == 0) {
        memmove(b->chunks, b->chunks + 1, (b->size - 1) * sizeof(int));
        b->size--;
    }
}

void subtractDigitInPlace(BigInt *b, int digit) {
    if (b->size == 0) return;
    
    b->chunks[b->size - 1] -= digit;
    
    for (size_t i = b->size - 1; i > 0 && b->chunks[i] < 0; --i) {
        b->chunks[i] += 10;
        b->chunks[i - 1]--;
    }

    normalize(b);
}

void divideBy2InPlace(BigInt *b) {
    if (b->size == 0) return;
    
    for (size_t i = 0; i < b->size - 1; ++i) {
        if (b->chunks[i] % 2 != 0) {
            b->chunks[i + 1] += 10;
        }
        b->chunks[i] /= 2;
    }
    
    b->chunks[b->size - 1] /= 2;
    normalize(b);
}
// Create a BigInt from a string
BigInt* createBigInt(const char *str) {
    size_t len = strlen(str);
    int start = 0;
    int sign = 1;
    if (str[0] == '-') {
        sign = -1;
        start = 1;
    }

    BigInt *result = (BigInt*)malloc(sizeof(BigInt));
    if(result == NULL) {
        exit(1); // Allocation failed
    }
    result->size = len - start;
    result->chunks = (unsigned long*)malloc(result->size * sizeof(unsigned long));*)malloc(result->size * sizeof(int));
    if(result->chunks == NULL) {
        exit(1); // Allocation failed
    }
    result->sign = sign;

    for (size_t i = start; i < len; i++) {
        result->chunks[i - start] = str[i] - '0';
    }

    return result;
}

BigInt* randomBigInt(int num_bits) {
    int num_chunks = (num_bits + 3) / 4;  // Assuming each digit is 4 bits (hex representation)
    BigInt *result = initBigInt();
    result->size = num_chunks;
    result->chunks = (unsigned long*)malloc(result->size * sizeof(unsigned long)); *)malloc(num_chunks * sizeof(int));
    if (!result->chunks) {
        exit(1);  // Allocation failure
    }

    for (int i = 0; i < num_chunks; i++) {
        result->chunks[i] = rand() % 10;  // Random value between 0 and 9
    }

    // Ensure that the number is not a trivially small number
    if (result->chunks[num_chunks - 1] == 0) {
        result->chunks[num_chunks - 1] = 1;
    }

    return result;
}

BigInt* gcdBigInt(BigInt *a, BigInt *b) {
    if (isZero(b)) {
        return cloneBigInt(a);
    }
    
    BigInt *temp_a = cloneBigInt(a);
    BigInt *temp_b = cloneBigInt(b);
    while (!isZero(temp_b)) {
        BigInt *remainder = modBigInt(temp_a, temp_b);
        freeBigInt(temp_a);
        temp_a = temp_b;
        temp_b = remainder;
    }
    freeBigInt(temp_b);  // At this point, temp_b is zero

    return temp_a;  // GCD is stored in temp_a
}

// Free a BigInt
void freeBigInt(BigInt *b) {
    if(b) {
        if(b->chunks) {
            free(b->chunks);
        }
        free(b);
    }
}

// Print a BigInt
void printBigInt(BigInt *b) {
    if (b->sign == -1) {
        printf("-");
    }
    for (size_t i = 0; i < b->size; i++) {
        printf("%lu", b->chunks[i]);
    }
    printf("\n");
}

void ensureLarger(BigInt **a, BigInt **b) {
    if ((*b)->size > (*a)->size) {
        BigInt *temp = *a;
        *a = *b;
        *b = temp;
    }
}

// Addition for BigInts
BigInt* addBigInt(BigInt *a, BigInt *b) {
    // If signs differ, delegate to subtraction
    if (a->sign != b->sign) {
        if (b->sign == -1) {
            b->sign = 1;
            return subtractBigInt(a, b);
        } else {
            a->sign = 1;
            return subtractBigInt(b, a);
        }
    }

    ensureLarger(&a, &b);

    size_t resultSize = a->size + 1;
    BigInt *result = (BigInt*)malloc(sizeof(BigInt));
    result->chunks = (unsigned long*)malloc(result->size * sizeof(unsigned long));*)calloc(resultSize, sizeof(int));
    result->size = resultSize;
    result->sign = a->sign;

    int carry = 0;
    for (size_t i = 0; i < a->size; i++) {
        int sum = a->chunks[a->size - 1 - i] + carry;
        if (i < b->size) {
            sum += b->chunks[b->size - 1 - i];
        }
        result->chunks[resultSize - 1 - i] = sum % 10;
        carry = sum / 10;
    }
    result->chunks[0] = carry;

    return result;
}

// Subtraction for BigInts (assumes a > b)
BigInt* subtractBigInt(BigInt *a, BigInt *b) {
    // If signs differ, delegate to addition
    if (a->sign != b->sign) {
        b->sign = 1;
        return addBigInt(a, b);
    }

    ensureLarger(&a, &b);

    BigInt *result = (BigInt*)malloc(sizeof(BigInt));
    result->chunks = (unsigned long*)malloc(result->size * sizeof(unsigned long));*)calloc(a->size, sizeof(int));
    result->size = a->size;
    result->sign = 1;

    int borrow = 0;
    for (size_t i = 0; i < a->size; i++) {
        int diff = a->chunks[a->size - 1 - i] - borrow;
        if (i < b->size) {
            diff -= b->chunks[b->size - 1 - i];
        }
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result->chunks[result->size - 1 - i] = diff;
    }

    if (borrow) {
        result->sign = -1;
    }

    return result;
}

// Multiplication for BigInts
BigInt* multiplyBigInts(BigInt *a, BigInt *b) {
    BigInt *result = initBigInt();
    result->size = a->size + b->size;
    result->chunks = (unsigned long*)malloc(result->size * sizeof(unsigned long)); *)calloc(result->size, sizeof(int));
    
    for (size_t i = 0; i < a->size; i++) {
        int carry = 0;
        for (size_t j = 0; j < b->size; j++) {
            int product = a->chunks[i] * b->chunks[j] + carry;
            result->chunks[i + j] += product % 10;   // Add to current result digit
            carry = product / 10 + result->chunks[i + j] / 10;
            result->chunks[i + j] %= 10;
        }
        result->chunks[i + b->size] = carry;
    }
    
    // Normalize result (remove leading zeros)
    while (result->size > 1 && result->chunks[0] == 0) {
        memmove(result->chunks, result->chunks + 1, (result->size - 1) * sizeof(int));
        result->size--;
    }

    return result;
}

BigInt* divideBigInt(BigInt *a, BigInt *b) {
    if (isZero(b)) {
        // Division by zero is not allowed
        return NULL;
    }

    BigInt *quotient = initBigInt();
    BigInt *current = initBigInt();
    size_t position = 0;

    while (position < a->size) {
        appendDigit(current, a->chunks[position]);
        position++;

        if (compareBigInts(current, b) >= 0) {
            int count = 0;
            while (compareBigInts(current, b) >= 0) {
                current = subtractBigInt(current, b);
                count++;
            }
            appendDigit(quotient, count);
        } else {
            appendDigit(quotient, 0);
        }
    }
    normalize(quotient);
    freeBigInt(current);
    return quotient;
}

// Modulus for BigInts
BigInt* modulusBigInt(BigInt *a, BigInt *b) {
    BigInt *quotient = divideBigInt(a, b);
    BigInt *product = multiplyBigInts(quotient, b);
    BigInt *remainder = subtractBigInt(a, product);

    freeBigInt(quotient);
    freeBigInt(product);

    return remainder;
}

BigInt* modBigInt(BigInt *a, BigInt *b) {
    BigInt *divided = divideBigInt(a, b);
    BigInt *multiplied = multiplyBigInts(divided, b);
    BigInt *result = subtractBigInt(a, multiplied);
    
    freeBigInt(divided);
    freeBigInt(multiplied);
    
    return result;
}
void appendChunk(BigInt *b, unsigned long chunk) {
    if (!b) {
        return;
    }

    // Allocate space for the new chunk
    unsigned long *new_chunks = (unsigned long *)realloc(b->chunks, (b->size + 1) * sizeof(unsigned long));
    if (!new_chunks) {
        // Handle memory allocation failure
        exit(1);
    }

    b->chunks = new_chunks;
    b->chunks[b->size] = chunk;
    b->size++;
}
// Modular exponentiation using square-and-multiply
BigInt* powerModBigInt(BigInt *base, BigInt *exponent, BigInt *modulus) {
    BigInt *result = initBigInt();
    appendChunk(result, 1); 

    BigInt *tempBase = cloneBigInt(base);

    for (int i = exponent->size - 1; i >= 0; i--) {
        int currentExpChunk = exponent->chunks[i];
        for (int j = 0; j < sizeof(unsigned long) * 8; j++) { 
            if (currentExpChunk & 1) {
                BigInt *tempResult = multiplyBigInts(result, tempBase);
                BigInt *modResult = modBigInt(tempResult, modulus);
                freeBigInt(result);
                result = modResult;
                freeBigInt(tempResult);
            }
            currentExpChunk >>= 1;

            if (i > 0 || currentExpChunk) { 
                BigInt *squared = multiplyBigInts(tempBase, tempBase);
                freeBigInt(tempBase);
                tempBase = modBigInt(squared, modulus);
                freeBigInt(squared);
            }
        }
    }

    freeBigInt(tempBase);
    return result;
}



int miller_rabin(BigInt *n, int k) {
    if (isEven(n)) {
        return 0;
    }

    // Write n-1 as 2^r * d
    BigInt *d = cloneBigInt(n);
    subtractDigitInPlace(d, 1);
    int r = 0;
    while (isEven(d)) {
        r++;
        divideBy2InPlace(d);
    }

    while (k--) {
        BigInt *a = randomBigIntRange(two, n);
        BigInt *x = powerModBigInt(a, d, n);

        if (isOne(x) || isBigIntEqual(x, n)) {
            freeBigInt(a);
            freeBigInt(x);
            continue;
        }

        int cont = 0;
        for (int i = 0; i < r; i++) {
            BigInt *prev_x = x;
            x = powerModBigInt(x, two, n);
            
            // Free the old x after creating the new one
            freeBigInt(prev_x);

            if (isOne(x)) {
                freeBigInt(a);
                freeBigInt(d);
                freeBigInt(x);
                return 0;
            }
            if (isBigIntEqual(x, n)) {
                cont = 1;
                break;
            }
        }

        freeBigInt(a);
        freeBigInt(x);

        if (!cont) {
            freeBigInt(d);
            return 0;
        }
    }

    freeBigInt(d);
    return 1;
}

BigInt* generate_prime(int num_bits) {
    srand(time(NULL));

    while (1) {
        BigInt *candidate = randomBigInt(num_bits);
        
        // Ensure it's odd
        if (candidate->chunks[candidate->size - 1] % 2 == 0) {
            candidate->chunks[candidate->size - 1]++;
        }

        if (is_divisible_by_small_primes(candidate)) {
            freeBigInt(candidate);
            continue;
        }
        if (miller_rabin(candidate, 20)) {
            return candidate;
        }
        freeBigInt(candidate);
    }
}

BigInt* extended_gcd(BigInt *a, BigInt *b, BigInt **x, BigInt **y) {
    if (isZero(b)) {
        *x = initBigInt();
        appendDigit(*x, 1);
        *y = initBigInt();
        return cloneBigInt(a);
    }

    BigInt *x1 = NULL, *y1 = NULL;
    BigInt *gcd = extended_gcd(b, modBigInt(a, b), &x1, &y1);

    *x = y1;
    *y = subtractBigInt(x1, multiplyBigInts(divideBigInt(a, b), y1));

    freeBigInt(x1);
    return gcd;
}

BigInt* randomBigIntRange(BigInt *lower, BigInt *upper) {
    BigInt *range = subtractBigInt(upper, lower);
    BigInt *randomNum = randomBigInt(range->size * 4);  // Assuming each digit as 4 bits
    BigInt *result = modBigInt(randomNum, range);

    BigInt *finalResult = addBigInt(lower, result);
    freeBigInt(range);
    freeBigInt(randomNum);
    freeBigInt(result);
    
    return finalResult;
}

BigInt* mod_inverse(BigInt *a, BigInt *m) {
    BigInt *x = NULL, *y = NULL;
    BigInt *g = extended_gcd(a, m, &x, &y);
    if (!isOne(g)) {
        // a and m are not coprime
        freeBigInt(g);
        freeBigInt(x);
        freeBigInt(y);
        return NULL;
    }

    BigInt *result = modBigInt(x, m);
    freeBigInt(g);
    freeBigInt(x);
    freeBigInt(y);
    return result;
}

void generate_keypair(int num_bits, BigInt **public_key_n, BigInt **public_key_e, BigInt **private_key_n, BigInt **private_key_d) {
    BigInt *p = generate_prime(num_bits / 2);
    BigInt *q;

    do {
        q = generate_prime(num_bits / 2);
    } while (isBigIntEqual(p, q));  // Ensure p != q

    *public_key_n = multiplyBigInts(p, q);
    *private_key_n = cloneBigInt(*public_key_n);

    BigInt *p_1 = subtractBigInt(p, one);
    BigInt *q_1 = subtractBigInt(q, one);
    BigInt *phi_n = multiplyBigInts(p_1, q_1);

    // Choose e such that 1 < e < phi(n) and gcd(e, phi(n)) = 1
    BigInt* gcd_result;
    do {
        *public_key_e = randomBigIntRange(two, phi_n);
        gcd_result = gcdBigInt(*public_key_e, phi_n);

        if (!isOne(gcd_result)) {
            freeBigInt(*public_key_e);  // If the GCD isn't 1, free the e value and generate a new one in the next iteration.
        }

    } while (!isOne(gcd_result));

    freeBigInt(gcd_result);

    *private_key_d = mod_inverse(*public_key_e, phi_n);

    freeBigInt(p);
    freeBigInt(q);
    freeBigInt(p_1);
    freeBigInt(q_1);
    freeBigInt(phi_n);
    freeBigInt(gcd_result);
}

BigInt* rsa_encrypt(BigInt *message, BigInt *e, BigInt *n) {
    return powerModBigInt(message, e, n);
}

BigInt* rsa_decrypt(BigInt *ciphertext, BigInt *d, BigInt *n) {
    return powerModBigInt(ciphertext, d, n);
}