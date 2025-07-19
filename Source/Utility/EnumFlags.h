#pragma once
#include <type_traits>
#include <concepts>

namespace WaxGourd {
    // Enable bit operations on enum class types
    template<typename E>
    concept EnumType = std::is_enum_v<E>;

    // Type-safe bitmask class for enum values
    template<EnumType E>
    class EnumFlags {
    private:
        using UnderlyingType = std::underlying_type_t<E>;
        UnderlyingType m_value;

    public:
        constexpr EnumFlags() noexcept : m_value(0) {}

        constexpr EnumFlags(E value) noexcept
            : m_value(static_cast<UnderlyingType>(value)) {
        }

        constexpr EnumFlags(const EnumFlags& other) noexcept = default;

        // Allow implicit conversion from values of the underlying enum type
        template<EnumType U> requires std::same_as<E, U>
        constexpr EnumFlags(std::initializer_list<U> values) noexcept
            : m_value(0) {
            for (auto value : values) {
                m_value |= static_cast<UnderlyingType>(value);
            }
        }

        // Enable bitwise operations
        constexpr EnumFlags operator|(E value) const noexcept {
            return EnumFlags(m_value | static_cast<UnderlyingType>(value));
        }

        constexpr EnumFlags operator&(E value) const noexcept {
            return EnumFlags(m_value & static_cast<UnderlyingType>(value));
        }

        constexpr EnumFlags operator^(E value) const noexcept {
            return EnumFlags(m_value ^ static_cast<UnderlyingType>(value));
        }

        constexpr EnumFlags operator|(const EnumFlags& other) const noexcept {
            return EnumFlags(m_value | other.m_value);
        }

        constexpr EnumFlags operator&(const EnumFlags& other) const noexcept {
            return EnumFlags(m_value & other.m_value);
        }

        constexpr EnumFlags operator^(const EnumFlags& other) const noexcept {
            return EnumFlags(m_value ^ other.m_value);
        }

        constexpr EnumFlags operator~() const noexcept {
            return EnumFlags(~m_value);
        }

        // Assignment operators
        constexpr EnumFlags& operator|=(E value) noexcept {
            m_value |= static_cast<UnderlyingType>(value);
            return *this;
        }

        constexpr EnumFlags& operator&=(E value) noexcept {
            m_value &= static_cast<UnderlyingType>(value);
            return *this;
        }

        constexpr EnumFlags& operator^=(E value) noexcept {
            m_value ^= static_cast<UnderlyingType>(value);
            return *this;
        }

        constexpr EnumFlags& operator|=(const EnumFlags& other) noexcept {
            m_value |= other.m_value;
            return *this;
        }

        constexpr EnumFlags& operator&=(const EnumFlags& other) noexcept {
            m_value &= other.m_value;
            return *this;
        }

        constexpr EnumFlags& operator^=(const EnumFlags& other) noexcept {
            m_value ^= other.m_value;
            return *this;
        }

        // Comparison operators
        constexpr bool operator==(const EnumFlags& other) const noexcept {
            return m_value == other.m_value;
        }

        constexpr bool operator!=(const EnumFlags& other) const noexcept {
            return m_value != other.m_value;
        }

        // Test if a flag is set (compatible with your HasBit/HasAnyBit macros)
        constexpr bool HasFlag(E value) const noexcept {
            return (m_value & static_cast<UnderlyingType>(value)) == static_cast<UnderlyingType>(value);
        }
		constexpr bool HasFlag(const EnumFlags& flags) const noexcept {
			return (m_value & flags.m_value) == flags.m_value;
		}
        constexpr bool HasAnyFlag(const EnumFlags& flags) const noexcept {
            return (m_value & flags.m_value) != 0;
        }

        // Clear all flags
        constexpr void Clear() noexcept {
            m_value = 0;
        }

        // Get the underlying value
        constexpr UnderlyingType Value() const noexcept {
            return m_value;
        }

        // Helper constructor (private)
        constexpr explicit EnumFlags(UnderlyingType value) noexcept : m_value(value) {}
    };

    // Free functions for creating flags from enum values
    template<EnumType E>
    constexpr EnumFlags<E> MakeFlags(E value) noexcept {
        return EnumFlags<E>(value);
    }

    template<EnumType E>
    constexpr EnumFlags<E> MakeFlags(std::initializer_list<E> values) noexcept {
        return EnumFlags<E>(values);
    }

    // Define binary operators between enum and EnumFlags
    template<EnumType E>
    constexpr EnumFlags<E> operator|(E lhs, const EnumFlags<E>& rhs) noexcept {
        return rhs | lhs;
    }

    template<EnumType E>
    constexpr EnumFlags<E> operator&(E lhs, const EnumFlags<E>& rhs) noexcept {
        return rhs & lhs;
    }

    template<EnumType E>
    constexpr EnumFlags<E> operator^(E lhs, const EnumFlags<E>& rhs) noexcept {
        return rhs ^ lhs;
    }

    template<EnumType E>
    constexpr EnumFlags<E> operator|(E lhs, E rhs) noexcept {
        return EnumFlags<E>(lhs) | rhs;
    }

    template<EnumType E>
    constexpr EnumFlags<E> operator&(E lhs, E rhs) noexcept {
        return EnumFlags<E>(lhs) & rhs;
    }

    template<EnumType E>
    constexpr EnumFlags<E> operator^(E lhs, E rhs) noexcept {
        return EnumFlags<E>(lhs) ^ rhs;
    }

} // namespace WaxGourd