#pragma once
#include "GpuBuffer.h"
#include "Texture.h"
#include "CommandList.h"
#include "RenderGraphResource.h"
#include "RenderGraphInterPassDataTypes.h"

namespace WaxGourd {
class RenderGraphPassBase;
class RenderGraph;
struct RenderGraphTextureDesc;
struct RenderGraphBufferDesc;

#pragma region RenderGraphResource Traits and Temaplate Specialization for RenderGraphTexture and RenderGraphBuffer
// Type traits to avoid run time branching
template<RenderGraphResourceType ResourceType>
struct RenderGraphResourceTraits;

template<>
struct RenderGraphResourceTraits<RenderGraphResourceType::Texture> {
	using Resource = Texture;
	using ResourceDesc = TextureDesc;
};
template<>
struct RenderGraphResourceTraits<RenderGraphResourceType::Buffer> {
	using Resource = GpuBuffer;
	using ResourceDesc = GpuBufferDesc;
};

template<ResourceType ResourceType>
struct TypedRenderGraphResource : RenderGraphResource {
	using Resource = RenderGraphResourceTraits<ResourceType>::Resource;
	using ResourceDesc = RenderGraphResourceTraits<ResourceType>::ResourceDesc;

	TypedRenderGraphResource(uint64 id, Resource* resource, Char const* name = "")
		: RenderGraphResource(id, true, name), resource(resource), desc(resource->GetDesc()) {}

	TypedRenderGraphResource(uint64 id, ResourceDesc const& desc, Char const* name = "")
		: RenderGraphResource(id, false, name), resource(nullptr), desc(desc) {}

	void SetName() {
#ifndef NDEBUG
		assert(resource != nullptr && "Resource is null");
		resource->SetName(name);
#endif
	}
	Resource* resource;
	ResourceDesc desc;
};

using RenderGraphTexture = TypedRenderGraphResource<RenderGraphResourceType::Texture>;
using RenderGraphBuffer = TypedRenderGraphResource<RenderGraphResourceType::Buffer>;
#pragma endregion

class RenderGraphExecutionContext {
	friend RenderGraph;
public:
	RenderGraphContext() = delete;
	DELETE_COPY_OPERATORS(RenderGraphContext);

	RGBlackboard& GetBlackboard();

	Texture& GetTexture(RGTextureId res_id) const;
	GpuBuffer& GetBuffer(RGBufferId res_id) const;

	Texture const& GetCopySrcTexture(RenderGraphTextureCopySrcId res_id) const;
	Texture& GetCopyDstTexture(RenderGraphTextureCopyDstId res_id) const;
	GpuBuffer const& GetCopySrcBuffer(RenderGraphBufferCopySrcId res_id) const;
	GpuBuffer& GetCopyDstBuffer(RenderGraphBufferCopyDstId res_id) const;
	GpuBuffer const& GetIndirectArgsBuffer(RenderGraphBufferIndirectArgsId res_id) const;
	GpuBuffer const& GetVertexBuffer(RenderGraphBufferVertexId res_id) const;
	GpuBuffer const& GetIndexBuffer(RenderGraphBufferIndexId res_id) const;
	GpuBuffer const& GetConstantBuffer(RenderGraphBufferConstantId res_id) const;

	GfxDescriptor GetRenderTarget(RenderGraphRenderTargetId res_id) const;
	GfxDescriptor GetDepthStencil(RenderGraphDepthStencilId res_id) const;
	GfxDescriptor GetReadOnlyTexture(RenderGraphTextureReadOnlyId res_id) const;
	GfxDescriptor GetReadWriteTexture(RenderGraphTextureReadWriteId res_id) const;

	GfxDescriptor GetReadOnlyBuffer(RenderGraphBufferReadOnlyId res_id) const;
	GfxDescriptor GetReadWriteBuffer(RenderGraphBufferReadWriteId res_id) const;
private:
	RenderGraph& m_render_graph;
	RenderGraphPassBase& m_render_graph_pass;

private:
	RenderGraphExecutionContext(RenderGraph& rg, RenderGraphPassBase& rg_pass);
};
}