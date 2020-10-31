#!/bin/bash

g++ -std=c++17 ChunkOwner.cpp   MemChunk.cpp main.cpp -o mem_allocator
 
./mem_allocator
