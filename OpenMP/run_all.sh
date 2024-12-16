#!/bin/bash

# Compile the programs
gcc -o bin/pi src/pi.c -fopenmp
gcc -o bin/matrix src/matrix.c -fopenmp
gcc -o bin/heat src/heat.c -fopenmp

# Run the programs and measure their execution time
echo "Running pi program..."
./bin/pi
python3 src/pi.py

echo "Running matrix program..."
./bin/matrix
python3 src/matrix.py

echo "Running heat program..."
./bin/heat
python3 src/heat.py

echo "All programs have been executed."