
#define ASSERT_HRESULT(hr) if(FAILED(hr)) assert(false);

#define DELETE_COPY_OPERATORS(Class)\
        Class(Class const&)            = delete;\
        Class& operator=(Class const&) = delete;

#define DELETE_MOVE_OPERATORS(Class)\
        Class(Class&&) noexcept            = delete; \
        Class& operator=(Class&&) noexcept = delete;\

#define DEFAULT_COPY_OPERATORS(Class)             \
        Class(Class const&)            = default; \
        Class& operator=(Class const&) = default;

#define DEFAULT_MOVE_OPERATORS(Class)                  \
        Class(Class&&) noexcept            = default; \
        Class& operator=(Class&&) noexcept = default;

#define HasBit(flags, bit) ((flags & bit) == bit)
#define HasAnyBit(flags, bits) ((flags & bits) != 0)
#define BIT(x) (1 << x)