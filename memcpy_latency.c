#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CHUNK_SIZE 64
#define ITERATIONS 1000

int main(int argc, char *argv[]) {
    size_t total_length;
    char *src, *dst;
    double total_time;
    struct timespec start, end;
    volatile char tmp;

    if (argc != 2) {
        printf("Usage: %s <length>\n", argv[0]);
        return 1;
    }
    total_length = atol(argv[1]);
    if (total_length % 64 != 0) {
        fprintf(stderr, "Error: length must be a multiple of 64\n");
        return 1;
    }

    src = aligned_alloc(64, total_length);
    dst = aligned_alloc(64, total_length);
    if (!src || !dst) {
        return 1;
    }

    memset((void*)src, 0xAA, total_length);

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int iter = 0; iter < ITERATIONS; iter++) {
        for (size_t i = 0; i < total_length / CHUNK_SIZE; i++) {
            memcpy(dst + i * CHUNK_SIZE, src + i * CHUNK_SIZE, CHUNK_SIZE);
            tmp = dst[i * CHUNK_SIZE];
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    total_time = (end.tv_sec - start.tv_sec) + 
                 (end.tv_nsec - start.tv_nsec) / 1e9;
    total_time /= ITERATIONS;

    printf("length: %zu latency: %.2f ns\n", total_length, total_time * 1e9);

    free(src);
    free(dst);
    return 0;
}
