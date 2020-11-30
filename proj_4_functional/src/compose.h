
//
// Created by asokolov on 21.11.2020.
//

#ifndef PROJECT_COMPOSE_H
#define PROJECT_COMPOSE_H


#include "function.h"
#include <iostream>


template<class FuncType, class... AgrType>
class Composer {
private:
    FuncType _function_ptr;
    Composer<AgrType...> _rest_args;

public:
    Composer(FuncType func_ptr, AgrType... rest_args) : _function_ptr(func_ptr), _rest_args(rest_args...) {
    }

    template<typename InputType>
    auto operator()(const InputType &x) const {
        static_assert(std::is_same<typename decltype(_function_ptr)::FunctionFirstArgumentType,
                                   decltype(_rest_args(x))>::value,
                      "Pipeline argument type  mismatch in compose function");
        return _function_ptr(_rest_args(x));
        ;
    }
};

template<class FuncType>
class Composer<FuncType> {
    FuncType _function_ptr;

public:
    Composer(FuncType func_ptr) : _function_ptr(func_ptr) {}

    template<typename InputType>
    auto operator()(const InputType &x) const {
        static_assert(std::is_same<typename decltype(_function_ptr)::FunctionFirstArgumentType,
                                   InputType>::value,
                      "Argument type  mismatch (arg of Compose(x))");
        return _function_ptr(x);
    }
};

template<typename... FuncList>
Composer<FuncList...> compose(FuncList... func) {
    return Composer<FuncList...>(func...);
}

#endif// PROJECT_COMPOSE_H
