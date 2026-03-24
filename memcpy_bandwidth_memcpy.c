#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>

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

    src = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    dst = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (src == MAP_FAILED || dst == MAP_FAILED) {
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

    munmap(src, length);
    munmap(dst, length);
    return 0;
}
