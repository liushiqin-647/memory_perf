#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <num_processes>"
    exit 1
fi

num_procs=$1

tmpdir=$(mktemp -d)
trap "rm -rf $tmpdir" EXIT

for i in $(seq 1 $num_procs); do
    core=$((i - 1))
    taskset -c $core /Users/liu/project/test/memcpy_bandwidth_st64b > $tmpdir/result_$i &
done

wait

total_bandwidth=0
for i in $(seq 1 $num_procs); do
    bw=$(cat $tmpdir/result_$i | awk '{print $2}')
    total_bandwidth=$(echo "$total_bandwidth + $bw" | bc)
done

echo "num_procs: $num_procs bandwidth: $total_bandwidth GB/s"
