#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oasis/collections/hash_map.h>

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
                    // printf("adding invalid_id: %llu\n", *to_add);
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
