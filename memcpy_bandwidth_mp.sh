#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <length> <num_processes>"
    exit 1
fi

length=$1
num_procs=$2

if [ $((length % 64)) -ne 0 ]; then
    echo "Error: length must be a multiple of 64"
    exit 1
fi

tmpdir=$(mktemp -d)
trap "rm -rf $tmpdir" EXIT

for i in $(seq 1 $num_procs); do
    core=$((i - 1))
    taskset -c $core /Users/liu/project/test/memcpy_bandwidth $length > $tmpdir/result_$i &
done

wait

total_bandwidth=0
for i in $(seq 1 $num_procs); do
    bw=$(cat $tmpdir/result_$i | awk '{print $4}')
    total_bandwidth=$(echo "$total_bandwidth + $bw" | bc)
done

avg_bandwidth=$(echo "scale=2; $total_bandwidth / $num_procs" | bc)

echo "length: $length num_procs: $num_procs bandwidth: $avg_bandwidth GB/s"
