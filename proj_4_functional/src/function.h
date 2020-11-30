//
// Created by asokolov on 21.11.2020.
//

#ifndef PROJECT_FUNCTION_H
#define PROJECT_FUNCTION_H
#include <memory>


/**************************************
 *   class FirstArgTypeGetter - get type of first argument
 *
 *
 ***************************************/
struct NoneType {};

// class FirstArgTypeGetter
template<typename... Args>
struct FirstArgTypeGetter {
    using FirstArgType = NoneType;
};

template<typename FirstArg, typename... RestArgs>
struct FirstArgTypeGetter<FirstArg, RestArgs...> {
    using FirstArgType = FirstArg;
};

template<>
struct FirstArgTypeGetter<> {
    using FirstArgType = NoneType;
};

/**************************************
 *   class Function - function wrapper
 *   like std::function
 *
 *
 ***************************************/

template<class T>
class Function;

template<class ReturnType, class... ArgsTypes>
class Function<ReturnType(ArgsTypes...)> {
    class FunctionStorageBase;

private:
    FunctionStorageBase *_caller_ptr;

public:
    using FunctionReturnType = ReturnType;
    using FunctionFirstArgumentType = typename FirstArgTypeGetter<ArgsTypes...>::FirstArgType ;
    static constexpr size_t arg_cnt = sizeof...(ArgsTypes);

    //default constructor
    Function() : _caller_ptr(nullptr){};

    //constructor from function
    Function(ReturnType (*func)(ArgsTypes... args)) {
        _caller_ptr = new FunctionStorage<ReturnType, ArgsTypes...>(func);
    };

    // constructor from member method
    template<typename MethodReturnType, typename ClassType, typename... MethodArguments>
    Function(MethodReturnType (ClassType::*func)(MethodArguments... otherArguments)) {
        _caller_ptr = new MemberMethodStorage<MethodReturnType, ClassType, MethodArguments...>(func);
    };

    // copy constructor
    Function(const Function &other) {
        _caller_ptr = other._caller_ptr->duplicate();
    };

    //move constructor
    Function(Function &&other) noexcept {
        _caller_ptr = other._caller_ptr;
        other._caller_ptr = nullptr;
    };

    // copy =
    Function &operator=(const Function &other) {
        if (this != &other) {
            _caller_ptr = _caller_ptr->duplicate();
        }
        return *this;
    };

    //move =
    Function &operator=(Function &&other) noexcept {
        if (this != &other) {
            delete _caller_ptr;
        }
        _caller_ptr = other._caller_ptr;
        other._caller_ptr = nullptr;
        return *this;
    };

    ~Function() {
        delete _caller_ptr;
    };

    void swap(Function &other) {
        auto tmp_ptr = _caller_ptr;
        _caller_ptr = other._caller_ptr;
        other._caller_ptr = tmp_ptr;
    };

    explicit operator bool() const noexcept {
        return _caller_ptr != nullptr;
    }

    ReturnType operator()(ArgsTypes... args) {
        return _caller_ptr->call(args...);
    }

    ReturnType operator()(ArgsTypes... args) const {
        return _caller_ptr->call(args...);
    }

private:
    class FunctionStorageBase {
    public:
        FunctionStorageBase() = default;
        ;
        virtual ~FunctionStorageBase() = default;
        ;
        virtual ReturnType call(ArgsTypes... args) = 0;
        virtual FunctionStorageBase *duplicate() = 0;
    };


    template<class FunctionReturnType, class... FunctionArgsTypes>
    class FunctionStorage : public FunctionStorageBase {
    public:
        explicit FunctionStorage(FunctionReturnType (*func)(FunctionArgsTypes...)) : FunctionStorageBase(),
                                                                                     _stored_function(func){};
        ~FunctionStorage() = default;
        virtual FunctionReturnType call(FunctionArgsTypes... args) {
            return _stored_function(args...);
        };

        virtual FunctionStorage *duplicate() {
            return new FunctionStorage<FunctionReturnType, FunctionArgsTypes...>(_stored_function);
        };

    private:
        FunctionReturnType (*_stored_function)(FunctionArgsTypes...);
    };


    template<typename MethodReturnType, typename ClassType, typename... MethodArguments>
    class MemberMethodStorage : public FunctionStorageBase {
    public:
        MemberMethodStorage(MethodReturnType (ClassType::*func)(MethodArguments...)) : _stored_function(func) {}

        virtual ReturnType call(ClassType obj, MethodArguments... method_args) {
            return (obj.*_stored_function)(method_args...);
        };

        virtual MemberMethodStorage *duplicate() {
            return new MemberMethodStorage<MethodReturnType, ClassType, MethodArguments...>(_stored_function);
        };

    private:
        MethodReturnType (ClassType::*_stored_function)(MethodArguments...);
    };
};

#endif//PROJECT_FUNCTION_H
