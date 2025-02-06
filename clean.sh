#!/bin/bash

# Clean the sa directory
cd sa && make clean
cd ..

# Clean the sse directory
rm -r ./sse/bin
rm -r ./sse/obj

# Clean the main directory
rm -r bin
make clean
