#!/bin/bash

# Clean the sa directory
cd digital-annealer && make clean
cd ..

# Clean the sse directory
cd sse-annealer && make clean
cd ..

# Clean the main directory
rm -r bin
make clean
