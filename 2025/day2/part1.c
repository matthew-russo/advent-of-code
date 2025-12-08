#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Day 2: Gift Shop ---
// 
// You get inside and take the elevator to its only other stop: the gift shop. "Thank you for visiting the North Pole!" gleefully exclaims a nearby sign. You aren't sure who is even allowed to visit the North Pole, but you know you can access the lobby through here, and from there you can access the rest of the North Pole base.
// 
// As you make your way through the surprisingly extensive selection, one of the clerks recognizes you and asks for your help.
// 
// As it turns out, one of the younger Elves was playing on a gift shop computer and managed to add a whole bunch of invalid product IDs to their gift shop database! Surely, it would be no trouble for you to identify the invalid product IDs for them, right?
// 
// They've even checked most of the product ID ranges already; they only have a few product ID ranges (your puzzle input) that you'll need to check. For example:
// 
// 11-22,95-115,998-1012,1188511880-1188511890,222220-222224,
// 1698522-1698528,446443-446449,38593856-38593862,565653-565659,
// 824824821-824824827,2121212118-2121212124
// 
// (The ID ranges are wrapped here for legibility; in your input, they appear on a single long line.)
// 
// The ranges are separated by commas (,); each range gives its first ID and last ID separated by a dash (-).
// 
// Since the young Elf was just doing silly patterns, you can find the invalid IDs by looking for any ID which is made only of some sequence of digits repeated twice. So, 55 (5 twice), 6464 (64 twice), and 123123 (123 twice) would all be invalid IDs.
// 
// None of the numbers have leading zeroes; 0101 isn't an ID at all. (101 is a valid ID that you would ignore.)
// 
// Your job is to find all of the invalid IDs that appear in the given ranges. In the above example:
// 
//     11-22 has two invalid IDs, 11 and 22.
//     95-115 has one invalid ID, 99.
//     998-1012 has one invalid ID, 1010.
//     1188511880-1188511890 has one invalid ID, 1188511885.
//     222220-222224 has one invalid ID, 222222.
//     1698522-1698528 contains no invalid IDs.
//     446443-446449 has one invalid ID, 446446.
//     38593856-38593862 has one invalid ID, 38593859.
//     The rest of the ranges contain no invalid IDs.
// 
// Adding up all the invalid IDs in this example produces 1227775554.
// 
// What do you get if you add up all of the invalid IDs?

bool is_invalid(long long number) {
    char buf[200];
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

    long long invalid_sum = 0;

    char *start_str = strtok(buf, "-");
    while (start_str != NULL) {
        char *end_str = strtok(NULL, ",");

        printf("Range: '%s' - '%s'\n", start_str, end_str);

        long long start = atoll(start_str);
        long long end = atoll(end_str);

        for (long long i = start; i <= end; i++) {
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
