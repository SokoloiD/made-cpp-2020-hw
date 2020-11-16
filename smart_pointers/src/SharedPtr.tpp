#include "smart_pointers.h"
#pragma once
#ifndef SHAREPTR_TPP// include guard
#define SHAREPTR_TPP
using namespace task;

template<class T>
SharedPtr<T>::SharedPtr() {
    _control_block = new ControlBlock<T>();
    _control_block->shared_counter = 1;
}

template<class T>
SharedPtr<T>::SharedPtr(pointer ptr) {
    _control_block = new ControlBlock<T>();
    _control_block->ptr = ptr;
    _control_block->shared_counter = 1;
}

template<class T>
SharedPtr<T>::SharedPtr(const SharedPtr&r) : _control_block(r._control_block) {
    _control_block->shared_counter++;
}

template<class T>
SharedPtr<T>::SharedPtr(SharedPtr &&r) noexcept : _control_block(r._control_block) {
    _control_block->shared_counter++;
    r.reset();
}

template<class T>
SharedPtr<T>::SharedPtr(const WeakPtr<T> &r){
    _control_block = r.get_control_block();
    _control_block->shared_counter++;
}

template<class T>
void SharedPtr<T>::_clear_mem() {
    if(_control_block->shared_counter > 0) {
        _control_block->shared_counter--;
    }
    if (_control_block->shared_counter == 0){
            delete _control_block->ptr;
            _control_block->ptr = nullptr;
    }
    if(_control_block->shared_counter == 0 && _control_block->weak_counter == 0) {
        delete _control_block;
    }

    _control_block = nullptr;
}

template<class T>
SharedPtr<T>::~SharedPtr() {
    _clear_mem();
}

template<class T>
ControlBlock<T> * SharedPtr<T>::get_control_block() const { return _control_block;};

template<class T>
void SharedPtr<T>::reset() noexcept {
    _clear_mem();
    _control_block = new ControlBlock<T>();

}

template<class T>
void SharedPtr<T>::reset(pointer other_ptr) {
    _clear_mem();
    _control_block = new ControlBlock<T>();
    _control_block->ptr = other_ptr;
    _control_block->shared_counter = 1;
};

template<class T>
T &SharedPtr<T>::operator*() { return *_control_block->ptr; }

template<class T>
T *SharedPtr<T>::operator->() { return _control_block->ptr; }

template<class T>
void SharedPtr<T>::swap(SharedPtr &other) {// ugly swap  -> std::move
    std::swap(_control_block, other._control_block);
}

template<class T>
T *SharedPtr<T>::get() { return _control_block->ptr; };
template<class T>
size_t SharedPtr<T>::use_count() {
    return _control_block->shared_counter;
};

template<class T>
SharedPtr<T>::operator bool() {
    return nullptr == _control_block->ptr;
};

template<class T>
SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr &r) noexcept {
    if (_control_block != r._control_block) {
        _clear_mem();
        _control_block = r._control_block;
        _control_block->shared_counter++;
    }
    return *this;
}

template<class T>
SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr &&r) noexcept {
    if (_control_block != r._control_block) {
        _clear_mem();
        _control_block = r._control_block;
        _control_block->shared_counter++;
        r.reset();
    }
    return *this;
}

template<class T>
SharedPtr<T> &SharedPtr<T>::operator=(const WeakPtr<T> &r) noexcept {
    if (_control_block != r._control_block) {
        _clear_mem();
        _control_block = r._control_block;
        _control_block->shared_counter++;
    }
    return *this;
}

#endif /* SHAREPTR_TPP */