#include "enigma_machine.h"
#include <string.h>
#include <ctype.h>

// Define 3 sample rotor configurations
static const char* rotor_configurations[] = {
    "EKMFLGDQVZNTOWYHXUSPAIBRCJ",
    "AJDKSIRUXBLHWTMCQGZNPYFVOE",
    "BDFHJLCPRTXVZNYEIWGAKMUSQO"
};

#define ALPHABET_LENGTH 26

static char forward_transform(char input, const char* rotor_config, int offset) {
    char plain = toupper(input) - 'A';
    char cipher = (plain + offset) % ALPHABET_LENGTH;
    return rotor_config[cipher];
}

static char reverse_transform(char input, const char* rotor_config, int offset) {
    for (int i = 0; i < ALPHABET_LENGTH; i++) {
        if (rotor_config[i] == input) {
            int plain = (i - offset + ALPHABET_LENGTH) % ALPHABET_LENGTH;
            return 'A' + plain;
        }
    }
    return '?';  // This should not happen with a valid input
}

void enigma_encrypt(const char* plaintext, char* output, int rotor1, int rotor2, int rotor3) {
    int rotor_position[3] = {0};
    const char* rotor[3];
    rotor[0] = rotor_configurations[rotor1 % 3];
    rotor[1] = rotor_configurations[rotor2 % 3];
    rotor[2] = rotor_configurations[rotor3 % 3];

    int length = strlen(plaintext);
    for (int i = 0; i < length; i++) {
        char c = toupper(plaintext[i]);

        // Ignore non-alphabet characters
        if (c < 'A' || c > 'Z') {
            output[i] = c;
            continue;
        }

        // Forward through rotors
        c = forward_transform(c, rotor[0], rotor_position[0]);
        c = forward_transform(c, rotor[1], rotor_position[1]);
        c = forward_transform(c, rotor[2], rotor_position[2]);

        // No reflector for simplicity; in real Enigma, the signal would now be reflected and sent back.

        // Reverse through rotors
        c = reverse_transform(c, rotor[2], rotor_position[2]);
        c = reverse_transform(c, rotor[1], rotor_position[1]);
        c = reverse_transform(c, rotor[0], rotor_position[0]);

        output[i] = c;

        // Advance rotors
        rotor_position[0] = (rotor_position[0] + 1) % ALPHABET_LENGTH;
        if (rotor_position[0] == 0) {
            rotor_position[1] = (rotor_position[1] + 1) % ALPHABET_LENGTH;
            if (rotor_position[1] == 0) {
                rotor_position[2] = (rotor_position[2] + 1) % ALPHABET_LENGTH;
            }
        }
    }

    output[length] = '\0';
}