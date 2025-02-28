#include "MathDefines.h"
#include <initializer_list>
#include <iterator>
#include <memory>
#include <type_traits>
#include <cstdlib>

template <typename Type, size_t Alignment = 64> // Cache lines are 64 bytes
class CpuBuffer {
	static_assert(std::is_trivially_copyable_v<Type>, "Type of CpuBuffer<Type> must be trivially copyable.");
private:
	Type* m_Data = nullptr;
	size_t m_Size = 0;
public:
	CpuBuffer() = default;
	
	size_t size() const {
		return m_size;
	}
	Type* data() {
		return m_data;
	}
	const Type* data() const {
		return m_data;
	}
private:
	template <class Type>
	Type* createAlignedArray(size_t _size, size_t _alignment) {
		size_t size = alignedSize(sizeof(Type) * _size, _alignment);
		Type* data = reinterpret_cast<Type*>(std::aligned_alloc(_alignment, size));
		if (data) {
			new (data)Type[_size];
		}
		return data;
	}
	constexpr Type alignedSize(Type _sizeBytes, Type _alignment) {
		return _sizeBytes == 0 ? 0 : ((_sizeBytes - 1) / _alignment + 1) * _alignment;
	}
};