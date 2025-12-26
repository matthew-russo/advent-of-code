#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *filePointer;
    char character;
    const char *filename = "../input.txt";

    filePointer = fopen(filename, "r");

    if (filePointer == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    uint64_t maximum_joltage = 0;

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), filePointer) != NULL) {
        size_t len = strlen(buffer);
        if (buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len -= 1;
        }
        uint8_t first_digit = 0;
        uint8_t second_digit = 0;
        for (int i = 0; i < len; i++) {
            uint8_t num = buffer[i] - '0';
            if (i != len - 1 && num > first_digit) {
                first_digit = num;
                second_digit = 0;
            } else if (num > second_digit) {
                second_digit = num;
            }
        }
        uint8_t joltage = first_digit * 10 + second_digit;
        maximum_joltage += joltage;
        printf("Line: %s, Joltage: %d, Maximum Joltage: %llu\n", buffer, joltage, maximum_joltage);
    }

    printf("Maximum Joltage: %llu\n", maximum_joltage);

    if (fclose(filePointer) == EOF) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    printf("\nFile closed successfully and program exiting.\n");

    return EXIT_SUCCESS;
}
