//
// Created by asokolov on 17.10.2020.
//

#ifndef MEM_ALLOCATOR_CUSTOMMEMALLOCATOR_H
#define MEM_ALLOCATOR_CUSTOMMEMALLOCATOR_H

#include <cstddef>
#include <cstdint>
#include <iostream>
#include "ChunkOwner.h"

const size_t MAX_CHUNK_MEM = 1024;

template<class T>
class CustomMemAllocator  {
private:
    ChunkOwner *chunkOwner_;

public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef T value_type;

    CustomMemAllocator() {
        chunkOwner_ = new ChunkOwner(MAX_CHUNK_MEM);
        std::cout << "CustomAllocator" << std::endl;

    }

    CustomMemAllocator(const CustomMemAllocator &other) {
        chunkOwner_  = other.chunkOwner_;
        chunkOwner_->references_++;
        std::cout << "CustomAllocator copy" << std::endl;
    }

    ~CustomMemAllocator() {
        chunkOwner_->references_--;
        if(0 == chunkOwner_->references_){
            delete chunkOwner_;
        }
        std::cout << "~CustomAllocator" << std::endl; }

    pointer address(reference x) const { return &x; }

    const_pointer address(const_reference x) const { return &x; }

     CustomMemAllocator & operator=(const CustomMemAllocator& other) {
        if ( &other ==  this) {
            return *this;
        }

        chunkOwner_->references_--;
        if(0 == chunkOwner_->references_){
            delete chunkOwner_;
        }
        chunkOwner_ = other.chunkOwner_;
        chunkOwner_->references_++;
        return *this;
    }



    template<class U, class... Args>
    void construct(U *p, Args &&...args) {
        new ((void *) p) U(std::forward<Args>(args)...);
    }

    void destroy(pointer p) { p->~T(); }

    size_t max_size() const { return MAX_CHUNK_MEM; }

    template<class U>
    struct rebind { typedef CustomMemAllocator<U> other; };

    pointer allocate(const size_type n) {
        size_type size = sizeof(value_type) * n;
        auto p = (pointer) chunkOwner_->allocMem(size);

        std::cout << "used CustomMemAllocator to allocate   at address " << p
                  << " (+)" << size << " bytes" << std::endl;
        return p;
    }

    void deallocate(void *p, size_type n) {
        size_type size = sizeof(value_type) * n;
        if (p) {
            std::cout << "used CustomMemAllocator to deallocate at address " << p
                      << " (-)" << size << " bytes" << std::endl;
        }
    }
};
#endif// MEM_ALLOCATOR_CUSTOMMEMALLOCATOR_H
