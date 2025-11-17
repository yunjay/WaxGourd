#pragma once
#include "Utility/CompileTimeStringHash.h"
#if 1 //defined(_DEBUG) || defined(_PROFILE)
#define RENDER_GRAPH_DEBUG 1
#else
#define RENDER_GRAPH_DEBUG 0
#endif

#if RENDER_GRAPH_DEBUG
#define RENDER_GRAPH_NAME(name) RenderGraphResourceName(#name, WaxGourd::stringHash(#name)) 
#define RENDER_GRAPH_NAME_IDX(name, idx) RenderGraphResourceName(#name, WaxGourd::stringHash(#name) + idx)
#else 
#define RENDER_GRAPH_NAME(name) RenderGraphResourceName(WaxGourd::stringHash(#name)) 
#define RENDER_GRAPH_NAME_IDX(name, idx) RenderGraphResourceName(WaxGourd::stringHash(#name) + idx)
#endif


namespace WaxGourd {
struct RenderGraphResourceName {
    static constexpr uint64 INVALID_HASH = uint64(-1);

    // Conserve original string in debug mode
#if RENDER_GRAPH_DEBUG
    RenderGraphResourceName() : hashed_name(INVALID_HASH), name{"uninitialized"}{}
    template<uint64 N>
    constexpr explicit RenderGraphResourceName(char const (&_name)[N], uint64 hash) : hashed_name(hash), name(_name) {}

    operator char const*() const {
        return name;
    }
#else
    RenderGraphResourceName() : hashed_name(INVALID_HASH) {}
    constexpr explicit RenderGraphResourceName(uint64 hash) : hashed_name(hash)
    {}
    operator char const*() const {
        return "";
    }
#endif

    bool IsValidName() const {
        return hashed_name != INVALID_HASH;
    }

    uint64 hashed_name;

#if RENDER_GRAPH_DEBUG
    char const* name;
#endif
};
inline bool operator==(RenderGraphResourceName const& name1, RenderGraphResourceName const& name2) {
    return name1.hashed_name == name2.hashed_name;
}
}
namespace std {
template <> struct hash<WaxGourd::RenderGraphResourceName> {
    WaxGourd::uint64 operator()(WaxGourd::RenderGraphResourceName const& res_name) const {
        return hash<decltype(res_name.hashed_name)>()(res_name.hashed_name);
    }
};
}