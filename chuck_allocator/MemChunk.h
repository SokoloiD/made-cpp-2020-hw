//
// Created by asokolov on 17.10.2020.
//

#ifndef MEM_ALLOCATOR_MEMCHUNK_H
#define MEM_ALLOCATOR_MEMCHUNK_H


#include <cstddef>
#include <cstdint>

class MemChunk {
public:
    MemChunk(const size_t size, MemChunk *prev);
    ~MemChunk();
    MemChunk *getPrev();
    size_t getFreeMem();
    uint8_t *allocMem(size_t size);

private:
    size_t chunkSize_;
    size_t allocated_;
    uint8_t *buffer_;
    MemChunk *prevChunk_;
};


#endif//MEM_ALLOCATOR_MEMCHUNK_H
