#pragma once
#include "MathDefines.h"
#include "Macros.h"
#include <type_traits>

namespace WaxGourd {
// Data shared between passes in the render graph. Holds a map of POD structs by type. Immutable after creation.
class RenderGraphInterPassData {
public:
    RenderGraphInterPassData() = default;
    ~RenderGraphInterPassData() = default;
    DELETE_COPY_OPERATORS(RenderGraphInterPassData);

    template<typename T>
    T& Add(T&& data) {
        return Create<std::remove_cvref_t<T>>(std::forward<T>(data));
    }

    template<typename T, typename... Args> requires std::is_constructible_v<T, Args...>
    T& Create(Args&&... args) {
        static_assert(std::is_trivial_v<T> && std::is_standard_layout_v<T>); // POD types only
        auto [it, inserted] = m_inter_pass_data.try_emplace(typeid(T), std::make_unique<Uint8[]>(sizeof(T)));
        assert(inserted && "Can only create one instance of a type in RenderGraphInterPassData.");
        // placement new to construct
        T* data_entry = new (it->second.get()) T{ std::forward<Args>(args)... };
        return *data_entry;
    }

    template<typename T>
    const T* TryGet() const {
        if (auto it = m_inter_pass_data.find(typeid(T)); it != m_inter_pass_data.end()) {
            return reinterpret_cast<T const*>(it->second.get());
        }
        else return nullptr;
    }

    template<typename T>
    T const& Get() const {
        T const* p_data = TryGet<T>();
        assert(p_data != nullptr);
        return *p_data;
    }

    void Clear() {
        m_inter_pass_data.clear();
    }

private:
    TypeIndexedPodStorage m_inter_pass_data; // map of type (type_index) to POD data types.
};
}