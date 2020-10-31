//
// Created by asokolov on 17.10.2020.
//
#include "MemChunk.h"
#include <stdlib.h>

MemChunk::MemChunk(const size_t size, MemChunk *prev) : prevChunk_(prev),
                                                        allocated_(0),
                                                        buffer_(nullptr),
                                                        chunkSize_(size) {
}

MemChunk::~MemChunk() {
    if (buffer_) {
        free(buffer_);
    }
}

MemChunk *MemChunk::getPrev() {
    return prevChunk_;
}

size_t MemChunk::getFreeMem() {
    return chunkSize_ - allocated_;
}

uint8_t *MemChunk::allocMem(size_t size) {

    if (!buffer_) {
        // try to allocate first time
        buffer_ = (uint8_t *) malloc(chunkSize_);
    }
    uint8_t *newBuffer = buffer_ + allocated_;
    allocated_ += size;
    return newBuffer;
}
