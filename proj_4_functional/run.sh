#!/bin/bash

set -e

g++ -std=c++17 -I./ src/test.cpp -o project_4_test
./project_4_test

echo All tests passed!
