#!/bin/bash
gcc -O2 -o /Users/liu/project/test/memcpy_bandwidth /Users/liu/project/test/memcpy_bandwidth_memcpy.c
gcc -O2 -o /Users/liu/project/test/memcpy_latency /Users/liu/project/test/memcpy_latency.c
gcc -O2 -march=armv8.2-a+stb -o /Users/liu/project/test/memcpy_bandwidth_st64b /Users/liu/project/test/memcpy_bandwidth_st64b.c