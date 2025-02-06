#!/bin/bash

mkdir -p bin

# Make sa binary and copy it to bin directory
cd sa && make
cd ..
mv ./sa/main_exe ./bin/sa

# Make sse binary and copy it to bin directory
cd sse && /bin/bash compile.sh
cd ..
mv ./sse/bin/main ./bin/sse

make
