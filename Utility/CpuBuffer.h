#include "MathDefines.h"
#include <initializer_list>
#include <iterator>
#include <memory>
#include <type_traits>
#include <cstdlib>
#include <span>
namespace WaxGourd {
template <typename Type, size_t Alignment = 64> // Cache lines are 64 bytes
class CpuBuffer {
	static_assert(std::is_trivially_copyable_v<Type>, "Type of CpuBuffer<Type> must be trivially copyable.");
private:
	Type* m_Data = nullptr;
	size_t m_Size = 0;
	template <class Type>
	Type* createAlignedArray(size_t _size, size_t _alignment) {
		size_t size = alignedSize(sizeof(Type) * _size, _alignment);
		Type* data = reinterpret_cast<Type*>(std::aligned_alloc(_alignment, size));
		if (data) {
			new (data)Type[_size];
		}
		return data;
	}
	template <class Type>
	void deleteAlignedArray(Type*& data, size_t count) {
		for (size_t i = 0; i < count; i++) {
			data[i].~Type();
		}
		std::free(data);
		data = nullptr;
	}
	constexpr Type alignedSize(Type _sizeBytes, Type _alignment) {
		return _sizeBytes == 0 ? 0 : ((_sizeBytes - 1) / _alignment + 1) * _alignment;
	}
	void swap(Buffer& lhs, Buffer& rhs) {
		std::swap(lhs.m_Data, rhs.m_Data);
		std::swap(lhs.m_Size, rhs.m_Size);
	}
public:
	CpuBuffer() = default;
	
	// Determined size
	Buffer(size_t size) {
		m_Data = createAlignedArray<Type>(size, Alignment);
		memset(m_Data, 0, sizeof(Type) * size);
		m_Size = size;
	}

	// Copy from pointer
	Buffer(const Type* data, size_t size) {
		m_Data = createAlignedArray<Type>(size, Alignment);
		if (data) {
			memcpy(m_Data, data, sizeof(Type) * size);
		}
		m_Size = size;
	}

	// C++ initializer list
	Buffer(std::initializer_list<Type> list) {
		m_Size = (size_t)list.size();
		m_Data = createAlignedArray<Type>(m_Size, Alignment);
		memcpy(m_Data, std::data(list), sizeof(Type) * m_Size);
	}

	void Clear() {
		if (m_Data) {
			deleteAlignedArray(m_Data, m_Size);
			m_Size = 0;
		}
	}

	/// Reallocates the buffer *without* preserving old data.
	void Resize(size_t size) {
		// Reset the whole buffer
		Clear();
		m_Data = NewAlignedArray<Type>(size, Alignment);
		m_Size = size;
	}

	/// Reallocates the buffer while preserving old data.
	void ResizeWhilePreservingData(size_t size) {
#define MINIMUM(a,b) ((a)<(b) ? (a) : (b))
		if (m_Size == size) return;
		auto oldData = m_Data;
		auto oldSize = m_Size;
		m_Data = createAlignedArray<Type>(size, Alignment);
		if (oldData) {
			memcpy(m_Data, oldData, sizeof(Type) * MINIMUM(oldSize, size));
		}
		m_Size = size;
		deleteAlignedArray(oldData, oldSize);
#undef MINIMUM
	}
	Buffer(const Buffer& other)
		: Buffer(other.m_Data, other.m_Size) {}

	Buffer(Buffer&& other)
		: Buffer() {
		swap(*this, other);
	}
	Buffer& operator=(Buffer other) {
		swap(*this, other);
		return *this;
	}
	Buffer& operator=(Buffer&& other) noexcept {
		swap(*this, other);
		return *this;
	}
	size_t size() const {
		return m_size;
	}
	Type* data() {
		return m_data;
	}
	const Type* data() const {
		return m_data;
	}
	/// Create immutable Span type from the entire buffer range
	operator std::span<const Type>() const {
		return { m_Data, m_Size };
	}

	/// Create mutable Span type from the entire buffer range
	operator std::span<Type>() {
		return { m_Data, m_Size };
	}

};
}