//
// Created by asokolov on 24.10.2020.
//

#ifndef MEM_ALLOCATOR_CHUNKOWNER_H
#define MEM_ALLOCATOR_CHUNKOWNER_H
#include "MemChunk.h"

class ChunkOwner {
public:
    ChunkOwner(size_t size);
    ~ChunkOwner();
    uint8_t *allocMem(size_t size);
    void freeMem(uint8_t *p);
    size_t references_;

private:
    size_t maxChunkSize;
    MemChunk *memChunk_;
};


#endif//MEM_ALLOCATOR_CHUNKOWNER_H
