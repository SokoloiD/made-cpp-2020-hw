#pragma once
#ifndef SMARTPOINTERS_H// include guard
#define SMARTPOINTERS_H

namespace task {

    template<class T>
    class UniquePtr {
    public:
        typedef T *pointer;
        typedef T &reference;

        UniquePtr(const UniquePtr &other) = delete;
        UniquePtr &operator=(const UniquePtr &other) = delete;

        UniquePtr(UniquePtr &&other) noexcept;

        explicit UniquePtr(pointer ptr);

        UniquePtr();
        ~UniquePtr();
        UniquePtr &operator=(UniquePtr &&other) noexcept;
        reference operator*();
        pointer operator->();
        pointer get();

    private:
        pointer _obj_ptr;
    };

    template<class T>
    struct ControlBlock {
        size_t shared_counter;
        size_t weak_counter;
        T* ptr;
        ControlBlock() : shared_counter(0), weak_counter(0), ptr(nullptr){};
        ControlBlock &operator=(const ControlBlock &other) = delete;
    };

    template<class T>
    class WeakPtr;

    template<class T>
    class SharedPtr {
    public:
        typedef T *pointer;
        typedef T &reference;

        SharedPtr();
        SharedPtr(pointer ptr);
        SharedPtr(const SharedPtr &r);
        SharedPtr(SharedPtr &&r) noexcept;
        SharedPtr(const WeakPtr<T> &r);
        ~SharedPtr();

        reference operator*();
        pointer operator->();

        void swap(SharedPtr &other);
        void reset() noexcept;
        void reset(pointer other_ptr);

        pointer get();
        size_t use_count();
        operator bool();

        SharedPtr &operator=(const SharedPtr &r) noexcept;
        SharedPtr &operator=(SharedPtr &&r) noexcept;
        SharedPtr &operator=(const WeakPtr<T> &r) noexcept;
        ControlBlock<T> * get_control_block() const;
    private:
        ControlBlock<T> *_control_block;
        void _clear_mem();
    };

    template<class T>
    class WeakPtr {
    public:
        WeakPtr() noexcept;
        WeakPtr(const WeakPtr &r) noexcept;
        WeakPtr(WeakPtr &&r) noexcept;
        WeakPtr(const SharedPtr<T> &r) noexcept;

        ~WeakPtr();

        void reset() noexcept;
        size_t use_count();
        bool expired();

        SharedPtr<T> lock();

        WeakPtr &operator=(const SharedPtr<T> &r) noexcept;
        WeakPtr &operator=(const WeakPtr &r) noexcept;
        WeakPtr &operator=(const WeakPtr &&r) noexcept;
        ControlBlock<T> * get_control_block() const ;
    private:
        ControlBlock<T> *_control_block;
        void _clear_mem();
    };

}// namespace task

#include "SharedPtr.tpp"
#include "Uniqptr.tpp"
#include "WeakPtr.tpp"

#endif /*SMARTPOINTERS_H*/