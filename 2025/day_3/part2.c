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

        char joltage_str[13] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '\0'};
        for (int b_idx = 0; b_idx < len; b_idx++) {
            uint8_t b_num = buffer[b_idx] - '0';
            bool assigned = false;
            for (int j_idx = 0; j_idx < 12; j_idx++) {
                uint8_t j_num = joltage_str[j_idx] - '0';
                if (assigned) {
                    joltage_str[j_idx] = '0';
                    continue;
                }
                if (b_num > j_num && len - b_idx >= 12 - j_idx) {
                    joltage_str[j_idx] = buffer[b_idx];
                    assigned = true;
                }
            }
        }
        uint64_t joltage = atoll(joltage_str);
        maximum_joltage += joltage;
        printf("Line: %s, JoltageStr: %s, Joltage: %llu, Maximum Joltage: %llu\n", buffer, joltage_str, joltage, maximum_joltage);
    }

    printf("Maximum Joltage: %llu\n", maximum_joltage);

    if (fclose(filePointer) == EOF) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    printf("\nFile closed successfully and program exiting.\n");

    return EXIT_SUCCESS;
}

