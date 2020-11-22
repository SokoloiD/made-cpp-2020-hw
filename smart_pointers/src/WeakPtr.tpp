#include "smart_pointers.h"
#pragma once
#ifndef WEAKPTR_TPP// include guard
#define WEAKPTR_TPP
using namespace task;

template<typename T>
WeakPtr<T>::WeakPtr() noexcept {
    _control_block = new ControlBlock<T>();
    _control_block->weak_counter = 1;

}

template<typename T>
WeakPtr<T>::WeakPtr(const WeakPtr &r) noexcept : _control_block(r._control_block) {
    _control_block->weak_counter++;
}

template<typename T>
WeakPtr<T>::WeakPtr(WeakPtr &&r) noexcept : _control_block(r._control_block) {
    _control_block = r._control_block;
    _control_block->weak_counter++;
    r.reset();
}

template<typename T>
WeakPtr<T>::WeakPtr(const SharedPtr<T> &r) noexcept  {
    _control_block = r.get_control_block();
    _control_block->weak_counter++;
}
template<typename T>
WeakPtr<T>::~WeakPtr() { _clear_mem(); }

template<typename T>
void WeakPtr<T>::_clear_mem() {
    if(_control_block->weak_counter > 0){
        _control_block->weak_counter--;
    }

    if (_control_block->shared_counter == 0 && _control_block->weak_counter == 0) {
        delete _control_block;
    }
    _control_block = nullptr;
}

template<typename T>
ControlBlock<T> * WeakPtr<T>::get_control_block() const { return _control_block;};

template<typename T>
void WeakPtr<T>::reset() noexcept {
    _clear_mem();
    _control_block = new ControlBlock<T>();
}

template<typename T>
size_t WeakPtr<T>::use_count() {
    return _control_block->shared_counter;
};

template<typename T>
bool WeakPtr<T>::expired() {
    return 0 == _control_block->shared_counter;
};

template<typename T>
SharedPtr<T> WeakPtr<T>::lock() {
    return (expired()) ? SharedPtr<T>() : SharedPtr<T>(*this);//
}

template<typename T>
WeakPtr<T> &WeakPtr<T>::operator=(const SharedPtr<T> &r) noexcept {
    if (_control_block != r.get_control_block()) {
        _clear_mem();
        _control_block = r.get_control_block();
        _control_block->weak_counter++;
    }
    return *this;
}

template<typename T>
WeakPtr<T> &WeakPtr<T>::operator=(const WeakPtr<T> &r) noexcept {
    if (_control_block != r._control_block) {
        _clear_mem();
        _control_block = r._control_block;
        _control_block->weak_counter++;
    }
    return *this;
}

template<typename T>
WeakPtr<T> &WeakPtr<T>::operator=(const WeakPtr<T> &&r) noexcept {
    if (_control_block != r._control_block) {
        _clear_mem();
        _control_block = r._control_block;
        _control_block->weak_counter++;
        r.reset();
    }
    return *this;
}

#endif /* WEAKPTR_TPP */