#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ITERATIONS 1000

int main(int argc, char *argv[]) {
    size_t length;
    char *src, *dst;
    double total_time, bandwidth;
    struct timespec start, end;

    if (argc != 2) {
        printf("Usage: %s <length>\n", argv[0]);
        return 1;
    }
    length = atol(argv[1]);
    if (length % 64 != 0) {
        fprintf(stderr, "Error: length must be a multiple of 64\n");
        return 1;
    }

    src = aligned_alloc(64, length);
    dst = aligned_alloc(64, length);
    if (!src || !dst) {
        return 1;
    }

    memset(src, 0xAA, length);

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < ITERATIONS; i++) {
        memcpy(dst, src, length);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    total_time = (end.tv_sec - start.tv_sec) + 
                 (end.tv_nsec - start.tv_nsec) / 1e9;
    
    bandwidth = (length * ITERATIONS) / total_time / 1e9;

    printf("length: %zu bandwidth: %.2f GB/s\n", length, bandwidth);

    free(src);
    free(dst);
    return 0;
}
