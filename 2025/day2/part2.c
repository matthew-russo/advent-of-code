#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oasis/collections/hash_map.h>

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
//
// --- Part Two ---
// 
// The clerk quickly discovers that there are still invalid IDs in the ranges in your list. Maybe the young Elf was doing other silly patterns as well?
// 
// Now, an ID is invalid if it is made only of some sequence of digits repeated at least twice. So, 12341234 (1234 two times), 123123123 (123 three times), 1212121212 (12 five times), and 1111111 (1 seven times) are all invalid IDs.
// 
// From the same example as before:
// 
//     11-22 still has two invalid IDs, 11 and 22.
//     95-115 now has two invalid IDs, 99 and 111.
//     998-1012 now has two invalid IDs, 999 and 1010.
//     1188511880-1188511890 still has one invalid ID, 1188511885.
//     222220-222224 still has one invalid ID, 222222.
//     1698522-1698528 still contains no invalid IDs.
//     446443-446449 still has one invalid ID, 446446.
//     38593856-38593862 still has one invalid ID, 38593859.
//     565653-565659 now has one invalid ID, 565656.
//     824824821-824824827 now has one invalid ID, 824824824.
//     2121212118-2121212124 now has one invalid ID, 2121212121.
// 
// Adding up all the invalid IDs in this example produces 4174379265.
// 
// What do you get if you add up all of the invalid IDs using these new rules?

size_t num_digits(uint64_t n) {
    if (n < 10) return 1;
    return 1 + num_digits (n / 10);
}

void repeat(char* buf, uint64_t to_repeat, size_t desired_len) {
    sprintf(buf, "%lld", to_repeat);
    size_t repeater_digit_count = num_digits(to_repeat);
    size_t i = 0;
    for(size_t i = repeater_digit_count; i < desired_len; i++){
        buf[i] = buf[i % repeater_digit_count];
    }
    buf[desired_len] = '\0';
}

uint64_t identity_hash_fn(void *key) {
    return (*(uint64_t *)key) % UINT64_MAX;
}

uint64_t* check_i(hash_map* seen_ids, uint64_t i, size_t desired_len, uint64_t start, uint64_t end) {
    char buf[20] = {0};
    repeat(buf, i, desired_len);
    uint64_t* invalid_id = (uint64_t*)malloc(sizeof(uint64_t));
    *invalid_id = atoll(buf);

    if (*invalid_id < start) {
        free(invalid_id);
        return NULL;
    }

    if (*invalid_id > end) {
        free(invalid_id);
        return NULL;
    }

    if (hm_contains(seen_ids, invalid_id)) {
        free(invalid_id);
        return NULL;
    }

    hm_insert(seen_ids, invalid_id, NULL);
    return invalid_id;
}

uint64_t sum_invalid_ids_with_digits(hash_map* seen_ids, uint64_t start, uint64_t end, size_t target_digits) {
    uint64_t invalid_sum = 0;
    for (size_t num_digits = 1; num_digits <= (target_digits / 2); num_digits++) {
        if (target_digits % num_digits == 0) {
            size_t lower = (size_t)pow((double)10, (double)(num_digits - 1));
            size_t upper = (size_t)pow((double)10, (double)num_digits);
            for (uint64_t i = lower; i < upper; i++) {
                uint64_t* to_add = check_i(seen_ids, i, target_digits, start, end);
                if (to_add != NULL) {
                    invalid_sum += *to_add;
                }
            }
        }
    }
    return invalid_sum;
}

uint64_t sum_invalid_ids_in_range(hash_map* seen_ids, uint64_t start, uint64_t end) {
    size_t start_len = num_digits(start);
    size_t end_len = num_digits(end);

    if (start_len == end_len) {
        return sum_invalid_ids_with_digits(seen_ids, start, end, start_len);
    } else {
        assert(start_len < end_len);
        uint64_t invalid_sum = 0;
        for (size_t len = start_len; len <= end_len; len++) {
            invalid_sum += sum_invalid_ids_with_digits(seen_ids, start, end, len);
        }
        return invalid_sum;
    }
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

    hash_map* seen_ids = hm_create(identity_hash_fn, 0.8);

    char *start_str = strtok(buf, "-");
    while (start_str != NULL) {
        char *end_str = strtok(NULL, ",");
        printf("Range: '%s' - '%s'\n", start_str, end_str);
        uint64_t start = atoll(start_str);
        uint64_t end = atoll(end_str);
        uint64_t sum_of_range = sum_invalid_ids_in_range(seen_ids, start, end);
        invalid_sum += sum_of_range;
        start_str = strtok(NULL, "-");
    }

    hm_destroy(seen_ids);

    printf("Sum of invalid IDs: %llu\n", invalid_sum);

    if (fclose(fp) == EOF) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    printf("\nFile closed successfully and program exiting.\n");

    return EXIT_SUCCESS;
}
