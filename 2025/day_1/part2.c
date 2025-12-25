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

    int current = 50;
    int rotations = 0;

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), filePointer) != NULL) {
        size_t len = strlen(buffer);

        // assume the input is well formed -- our first char is either 'L' or 'R'
        int num = atoi(buffer + 1);

        // 1. first we calculate the number of full rotations. full rotations
        // pass 0 exactly 1 time and direction doesn't matter since we end up
        // back at the same number
        int full_rotations = num / 100;
        int remainder = num % 100;

        rotations += full_rotations;

        // 2. calculate whether the partial rotation moves us past 0, dependent
        // on the direction of rotation

        // left rotates *lower* (subtraction)
        if (buffer[0] == 'L') {
          if (remainder == current) {
            // if we're rotating left exactly the amount, we'll be at 0
            rotations += 1;
            current = 0;
          } else if (remainder > current) {
            // if we're rotating left more than current, we need to rotate around
            int remainder_remainder = remainder - current;
            if (current != 0) rotations += 1;
            current = 100 - remainder_remainder;
          } else {
            // otherwise we won't cycle and won't rotate
            current = current - remainder;
          }
        // right rotates *higher* (addition)
        } else if (buffer[0] == 'R') {
          int distance_to_zero = 100 - current;
          if (remainder == distance_to_zero) {
            // if we're rotating right by exactly the amount to get to 100, we'll roll around to 0
            current = 0;
            rotations += 1;
          } else if (remainder > distance_to_zero) {
            // if we're rotating right more than the distance to 100, we need to rotate past 0
            current = remainder - distance_to_zero;
            rotations += 1;
          } else {
            // otherwise we won't cycle and won't rotate
            current = current + remainder;
          }
        } else {
          printf("impossible first char: '%c'\n", buffer[0]);
          exit(1);
        }
    }

    printf("rotations: '%d'\n", rotations);

    if (fclose(filePointer) == EOF) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    printf("\nFile closed successfully and program exiting.\n");

    return EXIT_SUCCESS;
}
