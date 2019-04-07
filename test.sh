#!/usr/bin/env bash

EXEC=$1
FACTOR=$2

function count {
	echo "$(wc -l $1 | cut -f1 -d' ')*$FACTOR+1" | bc -l |  awk '{printf("%d\n",$1 + 0.5)}'
}

function test_exec {
	$EXEC $1 $(count $1)
}

echo "Brands $(test_exec examples/brands.txt)"
echo "C++ $(test_exec examples/cpp_keywords.txt)"
echo "JavaScript $(test_exec examples/javascript_keywords.txt)"
echo "Python $(test_exec examples/python_keywords.txt)"
