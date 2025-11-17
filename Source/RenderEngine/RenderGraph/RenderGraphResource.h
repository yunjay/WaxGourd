#pragma once
#include "MathDefines.h"
#include "RenderGraphPass.h"
#include "ResourceEnum.h"

namespace WaxGourd {
struct RenderGraphTextureDesc {
    TextureDimension type = TextureDimension::Texture_2D;
    uint32 width = 0;
    uint32 height = 0;
    uint32 depth = 0;
    uint32 array_size = 1;
    uint32 mip_levels = 1;
    uint32 sample_count = 1;
    ResourceHeapType heap_type = ResourceHeapType::Default;
    ColorSpace color_space = ColorSpace::Linear;
    ClearValue clear_value{};
    ResourceFormat format = ResourceFormat::Unknown;
};
struct RenderGraphBufferDesc {
    uint64 size = 0;
    uint32 stride = 0;
    ResourceHeapType heap_type = ResourceHeapType::Default;
    GpuBufferInfoFlags misc_flags = GpuBufferInfoFlagsEnum::None;
    ResourceFormat format = ResourceFormat::Unknown;
};
struct RenderGraphResource {
	RenderGraphResource(uint64 m_id, bool imported, std::string name)
		: m_id(m_id), imported(imported), version(0), ref_count(0), name(name) {}
	uint64 m_id;
	bool imported;
	uint64 version;
	uint64 ref_count;
	RenderGraphPassBase* writer = nullptr;
	RenderGraphPassBase* most_recent_user = nullptr;
	std::string name = "";
};

#pragma region enums
enum class RenderGraphResourceType : uint8 {
	Buffer,
	Texture
};
enum class RenderGraphResourceHeapType : uint8 {
	CopySrc,
	CopyDst,
	IndirectArgs,
	Vertex,
	Index,
	Constant
};
#pragma endregion

#pragma region Resource m_id Types
struct RenderGraphResourceId {
	inline constexpr static uint32 invalid_id = uint32_MAX;

	RenderGraphResourceId() : m_id(invalid_id) {}
	RenderGraphResourceId(RenderGraphResourceId const&) = default; // default copy constructor
	explicit RenderGraphResourceId(uint32 _id) : m_id(_id) {}
	explicit RenderGraphResourceId(uint64 _id) : m_id(static_cast<uint32>(_id)) {}

	void Invalidate() { m_id = invalid_id; }
	bool IsValid() const { return m_id != invalid_id; }
	auto operator<=>(RenderGraphResourceId const&) const = default;

	uint32 m_id;
};

// Types to differentiate between buffers and textures
template<RenderGraphResourceType ResourceType> 
struct TypedRenderGraphResourceId : RenderGraphResourceId {
	using RenderGraphResourceId::RenderGraphResourceId; // Consturctor inheritance
};

// Final type aliases
using RenderGraphBufferId = TypedRenderGraphResourceId<RenderGraphResourceType::Buffer>;
using RenderGraphTextureId = TypedRenderGraphResourceId<RenderGraphResourceType::Texture>;

// Tie usage to resources
template<RenderGraphResourceHeapType Usage>
struct RenderGraphTextureWithUsageId : RenderGraphTextureId {
	using RenderGraphTextureId::RenderGraphTextureId;
private:
	friend class RenderGraphBuilder;
	friend class RenderGraph;

	RenderGraphTextureWithUsageId(RenderGraphTextureId const& m_id) : RenderGraphTextureId(m_id) {}
};	
template<RenderGraphResourceHeapType Usage>
struct RenderGraphBufferWithUsageId : RenderGraphBufferId {
	using RenderGraphBufferId::RenderGraphBufferId;
private:
	friend class RenderGraphBuilder;
	friend class RenderGraph;

	RenderGraphBufferWithUsageId(RenderGraphBufferId const& m_id) : RenderGraphBufferId(m_id) {}
};

// Final type aliases for usage
using RenderGraphTextureCopySrcId = RenderGraphTextureWithUsageId<RenderGraphResourceHeapType::CopySrc>;
using RenderGraphTextureCopyDstId = RenderGraphTextureWithUsageId<RenderGraphResourceHeapType::CopyDst>;

using RenderGraphBufferCopySrcId = RenderGraphBufferWithUsageId<RenderGraphResourceHeapType::CopySrc>;
using RenderGraphBufferCopyDstId = RenderGraphBufferWithUsageId<RenderGraphResourceHeapType::CopyDst>;
using RenderGraphBufferIndirectArgsId = RenderGraphBufferWithUsageId<RenderGraphResourceHeapType::IndirectArgs>;
using RenderGraphBufferVertexId = RenderGraphBufferWithUsageId<RenderGraphResourceHeapType::Vertex>;
using RenderGraphBufferIndexId = RenderGraphBufferWithUsageId<RenderGraphResourceHeapType::Index>;
using RenderGraphBufferConstantId = RenderGraphBufferWithUsageId<RenderGraphResourceHeapType::Constant>;

// Descriptors
struct RenderGraphResourceDescriptorId {
	inline constexpr static uint64 invalid_id = uint64_MAX;
	RenderGraphResourceDescriptorId() : m_id(invalid_id) {}
	RenderGraphResourceDescriptorId(uint64 view_id, RenderGraphResourceId resource_handle) 
		: m_id(invalid_id) {
		uint32 resource_id = resource_handle.m_id;
		m_id = (view_id << 32) | resource_id;
	}

	uint64 GetViewId() const { return (m_id >> 32); };
	uint64 GetResourceId() const {
		return (uint64)static_cast<uint32>(m_id);
	};

	RenderGraphResourceId operator*() const { // Pointer dereference for m_id
		return RenderGraphResourceId(GetResourceId());
	}

	void Invalidate() { m_id = invalid_id; }
	bool IsValid() const { return m_id != invalid_id; }
	auto operator<=>(RenderGraphResourceDescriptorId const&) const = default;

	uint64 m_id;
};

enum class RenderGraphDescriptorType : uint8
{
	ReadOnly,
	ReadWrite,
	RenderTarget,
	DepthStencil
};

template<RenderGraphResourceType ResourceType, RenderGraphDescriptorType DescriptorType>
struct TypedRenderGraphResourceDescriptorId : RenderGraphResourceDescriptorId {
	using RenderGraphResourceDescriptorId::RenderGraphResourceDescriptorId;
	using RenderGraphResourceDescriptorId::operator*;

	auto GetResourceId() const {
		if constexpr (ResourceType == RenderGraphResourceType::Buffer) return RenderGraphBufferId(RenderGraphResourceDescriptorId::GetResourceId());
		else if constexpr (ResourceType == RenderGraphResourceType::Texture) return RenderGraphTextureId(RenderGraphResourceDescriptorId::GetResourceId());
	}

	auto operator*() const { // Pointer dereference for m_id
		if constexpr (ResourceType == RenderGraphResourceType::Buffer) return RenderGraphBufferId(GetResourceId());
		else if constexpr (ResourceType == RenderGraphResourceType::Texture) return	RenderGraphTextureId(GetResourceId());
	}
};

// Final type aliases for descriptors
using RenderGraphRenderTargetId = TypedRenderGraphResourceDescriptorId<RenderGraphResourceType::Texture, RenderGraphDescriptorType::RenderTarget>;
using RenderGraphDepthStencilId = TypedRenderGraphResourceDescriptorId<RenderGraphResourceType::Texture, RenderGraphDescriptorType::DepthStencil>;
using RenderGraphTextureReadOnlyId = TypedRenderGraphResourceDescriptorId<RenderGraphResourceType::Texture, RenderGraphDescriptorType::ReadOnly>;
using RenderGraphTextureReadWriteId = TypedRenderGraphResourceDescriptorId<RenderGraphResourceType::Texture, RenderGraphDescriptorType::ReadWrite>;

using RenderGraphBufferReadOnlyId = TypedRenderGraphResourceDescriptorId<RenderGraphResourceType::Buffer, RenderGraphDescriptorType::ReadOnly>;
using RenderGraphBufferReadWriteId = TypedRenderGraphResourceDescriptorId<RenderGraphResourceType::Buffer, RenderGraphDescriptorType::ReadWrite>;

} // namespace WaxGourd

// Hash for RenderGraphResourceId - just maps to std::hash for the m_id.
namespace std {
	template <> struct hash<WaxGourd::RenderGraphTextureId> {
		uint64 operator()(WaxGourd::RenderGraphTextureId const& h) const
		{
			return hash<decltype(h.m_id)>()(h.m_id);
		}
	};
	template <> struct hash<WaxGourd::RenderGraphBufferId>
	{
		uint64 operator()(WaxGourd::RenderGraphBufferId const& h) const
		{
			return hash<decltype(h.m_id)>()(h.m_id);
		}
	};
	template <> struct hash<WaxGourd::RenderGraphTextureReadOnlyId>
	{
		uint64 operator()(WaxGourd::RenderGraphTextureReadOnlyId const& h) const
		{
			return hash<decltype(h.m_id)>()(h.m_id);
		}
	};
	template <> struct hash<WaxGourd::RenderGraphTextureReadWriteId>
	{
		uint64 operator()(WaxGourd::RenderGraphTextureReadWriteId const& h) const
		{
			return hash<decltype(h.m_id)>()(h.m_id);
		}
	};
	template <> struct hash<WaxGourd::RenderGraphRenderTargetId>
	{
		uint64 operator()(WaxGourd::RenderGraphRenderTargetId const& h) const
		{
			return hash<decltype(h.m_id)>()(h.m_id);
		}
	};
	template <> struct hash<WaxGourd::RenderGraphDepthStencilId>
	{
		uint64 operator()(WaxGourd::RenderGraphDepthStencilId const& h) const
		{
			return hash<decltype(h.m_id)>()(h.m_id);
		}
	};

	template <> struct hash<WaxGourd::RenderGraphBufferReadOnlyId>
	{
		uint64 operator()(WaxGourd::RenderGraphBufferReadOnlyId const& h) const
		{
			return hash<decltype(h.m_id)>()(h.m_id);
		}
	};

	template <> struct hash<WaxGourd::RenderGraphBufferReadWriteId>
	{
		uint64 operator()(WaxGourd::RenderGraphBufferReadWriteId const& h) const
		{
			return hash<decltype(h.m_id)>()(h.m_id);
		}
	};
	
#pragma endregion
} // namespace WaxGourd