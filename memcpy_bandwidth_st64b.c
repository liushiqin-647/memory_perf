#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>

#define ITERATIONS 10000000

static inline void st64b_copy(void *dst, void *src) {
    __asm__ __volatile__ (
        "ldp x0, x1, [%[src], #0]\n"
        "ldp x2, x3, [%[src], #16]\n"
        "ldp x4, x5, [%[src], #32]\n"
        "ldp x6, x7, [%[src], #48]\n"
        "st64b x0, x1, x2, x3, x4, x5, x6, x7, [%[dst]]\n"
        :
        : [dst]"r"(dst), [src]"r"(src)
        : "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "memory"
    );
}

int main() {
    char *src, *dst;
    double total_time, bandwidth;
    struct timespec start, end;
    const size_t length = 64;

    src = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    dst = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (src == MAP_FAILED || dst == MAP_FAILED) {
        return 1;
    }

    memset(src, 0xAA, length);

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < ITERATIONS; i++) {
        st64b_copy(dst, src);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    total_time = (end.tv_sec - start.tv_sec) + 
                 (end.tv_nsec - start.tv_nsec) / 1e9;
    
    bandwidth = (length * ITERATIONS) / total_time / 1e9;

    printf("bandwidth: %.2f GB/s\n", bandwidth);

    munmap(src, length);
    munmap(dst, length);
    return 0;
}
