#pragma once
#include <type_traits>
// Code to replace ComPtr (Reference counted pointer similar to shared_ptr)
namespace WaxGourd {
template <typename T>
class ComPointer {
public:
using InterfaceType = T;
	ComPointer() : ptr_(nullptr) {}

	ComPointer(decltype(nullptr)) : ptr_(nullptr) {}

	T* Get() const {
		return ptr_;
	}
	
	void Swap(ComPointer&& r) {
		T* tmp = ptr_;
		ptr_ = r.ptr_;
		r.ptr_ = tmp;
	}
	
	void Swap(ComPointer& r) {
		T* tmp = ptr_;
		ptr_ = r.ptr_;
		r.ptr_ = tmp;
	}

	template<typename U>
	ComPointer(U* other) : ptr_(other) {
		internalAddReference();
	}

	ComPointer(ComPointer const& other) : ptr_(other.ptr_) {
		internalAddReference();
	}

	// copy constructor that allows to instantiate class when U* is convertible to T*
	template<typename U>
	ComPointer(const ComPointer<U>& other, typename std::enable_if_t<std::is_convertible_v<U*, T*>, void*>* = 0) :
		ptr_(other.ptr_) {
		internalAddReference();
	}

	ComPointer(ComPointer&& other) : ptr_(nullptr) {
		if (this != reinterpret_cast<ComPointer*>(&reinterpret_cast<Uchar&>(other)))
		{
			Swap(other);
		}
	}

	// Move constructor that allows instantiation of a class when U* is convertible to T*
	template<typename U>
	ComPointer(ComPointer<U>&& other, typename std::enable_if_t<std::is_convertible_v<U*, T*>, void*>* = 0) :
		ptr_(other.ptr_) {
		other.ptr_ = nullptr;
	}
	~ComPointer() noexcept {
		internalRelease();
	}
	ComPointer& operator=(decltype(nullptr)) {
		internalRelease();
		return *this;
	}

	ComPointer& operator=(T* other) {
		if (ptr_ != other) {
			ComPointer(other).Swap(*this);
		}
		return *this;
	}

	template <typename U>
	ComPointer& operator=(U* other) {
		ComPointer(other).Swap(*this);
		return *this;
	}

	ComPointer& operator=(const ComPointer& other) {
		if (ptr_ != other.ptr_)
		{
			ComPointer(other).Swap(*this);
		}
		return *this;
	}

	template<typename U>
	ComPointer& operator=(const ComPointer<U>& other) {
		ComPointer(other).Swap(*this);
		return *this;
	}

	ComPointer& operator=(ComPointer&& other) {
		ComPointer(static_cast<ComPointer&&>(other)).Swap(*this);
		return *this;
	}

	template<typename U>
	ComPointer& operator=(_Inout_ ComPointer<U>&& other) {
		ComPointer(static_cast<ComPointer<U>&&>(other)).Swap(*this);
		return *this;
	}

	operator bool() const {
		return Get() != nullptr;
	}

	operator T* () const {
		return Get();
	}

	InterfaceType* operator->() const {
		return ptr_;
	}

	T* const* GetAddressOf() const {
		return &ptr_;
	}

	T** GetAddressOf() {
		return &ptr_;
	}

	T** ReleaseAndGetAddressOf() {
		internalRelease();
		return &ptr_;
	}

	T* Detach() {
		T* ptr = ptr_;
		ptr_ = nullptr;
		return ptr;
	}

	void Attach(InterfaceType* other) {
		if (ptr_ != nullptr)
		{
			auto ref = ptr_->Release();
			(void)ref;
			assert(ref != 0 || ptr_ != other);
		}
		ptr_ = other;
	}

	unsigned long Reset() {
		return internalRelease();
	}

	// query interface
	template<typename U>
	HRESULT As(ComPointer<U>* p) const {
		return ptr_->QueryInterface(__uuidof(U), reinterpret_cast<void**>(p->ReleaseAndGetAddressOf()));
	}

protected:
	InterfaceType* ptr_;
	template<class U> friend class ComPointer;

	void internalAddReference() const {
		if (ptr_ != nullptr)
		{
			ptr_->AddRef(); // COM
		}
	}
	unsigned long internalRelease() {
		unsigned long ref = 0;
		T* temp = ptr_;

		if (temp != nullptr)
		{
			ptr_ = nullptr;
			ref = temp->Release(); // COM
		}

		return ref;
	}
};

}