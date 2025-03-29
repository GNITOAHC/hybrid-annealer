#!/bin/bash

mkdir -p bin

# Make sa binary and copy it to bin directory
cd digital-annealer && make
cd ..
mv ./digital-annealer/bin/main ./bin/sa

# Make sse binary and copy it to bin directory
cd sse-annealer && make
cd ..
mv ./sse-annealer/bin/main ./bin/sse

make
