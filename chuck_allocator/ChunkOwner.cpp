//
// Created by asokolov on 24.10.2020.
//

#include "ChunkOwner.h"


ChunkOwner::ChunkOwner(size_t size) : references_(1),
                                      maxChunkSize(size) {
    memChunk_ = new MemChunk(size, nullptr);
}

ChunkOwner::~ChunkOwner() {
    MemChunk *currChunk = memChunk_;
    while (currChunk) {
        MemChunk *prevChunk = currChunk->getPrev();
        delete currChunk;
        currChunk = prevChunk;
    }
}

uint8_t *ChunkOwner::allocMem(size_t size) {
    if (size > maxChunkSize) {
        // too big to allocate
        return nullptr;
    }

    auto currMemChunk = memChunk_;
    // try to find in existing chunks
    while (currMemChunk && currMemChunk->getFreeMem() < size) {
        currMemChunk = currMemChunk->getPrev();
    }

    if (!currMemChunk) {
        // need to allocate a new chunk
        currMemChunk = new MemChunk(maxChunkSize, memChunk_);
        memChunk_ = currMemChunk;
    }
    return currMemChunk->allocMem(size);
}

void ChunkOwner::freeMem(uint8_t *p) {
}