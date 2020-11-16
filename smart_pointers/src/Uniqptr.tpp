#include "smart_pointers.h"
#pragma once
#ifndef UNIQPTR_TPP// include guard
#define UNIQPTR_TPP
using namespace task;

template<typename T>
UniquePtr<T>::UniquePtr(UniquePtr &&other) noexcept {
    _obj_ptr = other._obj_ptr;
    other._obj_ptr = nullptr;
}

template<typename T>
UniquePtr<T>::UniquePtr(pointer ptr) { _obj_ptr = ptr; }

template<typename T>
UniquePtr<T>::UniquePtr() { _obj_ptr = nullptr; }

template<typename T>
UniquePtr<T>::~UniquePtr() {
    if (nullptr != _obj_ptr) {
        delete _obj_ptr;
    }
}

template<typename T>
UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr<T> &&other) noexcept {
    _obj_ptr = other._obj_ptr;
    other._obj_ptr = nullptr;
    return *this;
}

template<typename T>
T &task::UniquePtr<T>::operator*() { return *_obj_ptr; }

template<class T>
T *UniquePtr<T>::operator->() { return _obj_ptr; }

template<class T>
T *UniquePtr<T>::get() { return _obj_ptr; }

#endif /* UNIQPTR_TPP */