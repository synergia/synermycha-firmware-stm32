#pragma once
#include <memory>
#include <array>
#include "Observer.hh"

namespace utils
{

constexpr int maxNrOfSignals = 10;

template<typename R, typename ...Args>
class FunctionPtr
{
public:
    FunctionPtr() : mPtr(nullptr), mObj(nullptr) 
    {}

    template<typename T, R(T::*ptr)(Args...)>
    static FunctionPtr bind(Observer* obj)
    {
        return {[](Observer* obj, Args... args)
            { return (static_cast<T*>(obj)->*ptr)(std::forward<Args>(args)...); }, obj};
    }

    template<typename... Aref>
    R operator()(Aref&&... args)
    {
        return (*mPtr)(mObj, std::forward<Aref>(args)...);
    }

    Observer* getObjectPtr()
    {
        return mObj;
    }

private:
    FunctionPtr(R(*_mPtr)(Observer*, Args...), Observer* _mObj)
    : mPtr(_mPtr), mObj(_mObj) {}

    R(*mPtr)(Observer*, Args...);
    Observer* mObj;
};

template<typename T>
class signal {};


template <typename R, typename ...Args>
class signal<R(Args...)>
{
public:
    template<typename T, R(T::*ptr)(Args...)>
    void connect(Observer& obj)
    {
        funs[actualNrOfElems++] = (FunctionPtr<R, Args... >::template bind<T, ptr>(&obj));
    }

    template <typename... ArgsRefs>
    void emit(ArgsRefs&&... args)
    {
        for(auto& f : funs)
        {
            if (f.getObjectPtr() != nullptr)
                f(std::forward<ArgsRefs>(args)...);
        }
    }

private:
    std::array<FunctionPtr<R, Args... >, maxNrOfSignals> funs;
    int actualNrOfElems = 0;
};

}