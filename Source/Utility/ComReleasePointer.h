#pragma once
#include <memory>
namespace WaxGourd {
    template<typename T>
concept Releasable = requires (T t) {
    { t.Release() };
};

template<typename T>
auto deleter = [](T* allocation) { allocation->Release(); };

template<typename T> requires Releasable<T>
using ComReleasePtr = std::unique_ptr<T, decltype(deleter<T>)>;
}