#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_invalid(uint64_t number) {
    char buf[20];
    sprintf(buf, "%lld", number);
    size_t len = strlen(buf);

    if (len % 2 != 0) {
        return false;
    }

    size_t half_len = len / 2;
    for (size_t i = 0; i < half_len; i++) {
        if (buf[i] != buf[i + half_len]) {
            return false;
        }
    }

    return true;
}

int main() {
    FILE *fp;
    char character;
    const char *filename = "../input.txt";

    fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buf[10000];

    size_t amount_read = fread(buf, sizeof(char), sizeof(buf) - sizeof(char), fp);
    if (ferror(fp) != 0) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }
    buf[amount_read + 1] = '\0';

    uint64_t invalid_sum = 0;

    char *start_str = strtok(buf, "-");
    while (start_str != NULL) {
        char *end_str = strtok(NULL, ",");

        printf("Range: '%s' - '%s'\n", start_str, end_str);

        uint64_t start = atoll(start_str);
        uint64_t end = atoll(end_str);

        for (uint64_t i = start; i <= end; i++) {
            if (is_invalid(i)) {
                printf("  Invalid ID found: %lld\n", i);
                invalid_sum += i;
            }
        }

        start_str = strtok(NULL, "-");
    }

    printf("Sum of invalid IDs: %lld\n", invalid_sum);

    if (fclose(fp) == EOF) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    printf("\nFile closed successfully and program exiting.\n");

    return EXIT_SUCCESS;
}
