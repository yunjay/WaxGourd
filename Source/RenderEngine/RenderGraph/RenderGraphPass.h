#pragma once
#include "CommandList.h"
#include "EnumFlags.h"
#include "MathDefines.h"
#include "RenderGraphPassExecutionContext.h"
#include "RenderGraphResource.h"
#include "ResourceEnum.h"

#include <functional>
#include <optional>

namespace WaxGourd {

#pragma region RenderGraphPass enums

enum class RenderGraphPassType : uint8 {
	Graphics,
	Compute,
	AsyncCompute,
	Copy
};
enum class RenderGraphPassFlagsEnum : uint32 {
	None = 0x00,
	ForceNoCull = 0x01 // Pass will not be culled by Render Graph, useful for debugging
};
using RenderGraphPassFlags = EnumFlags<RenderGraphPassFlagsEnum>;

enum RenderGraphReadAccess : uint8 {
	ReadAccess_PixelShader,
	ReadAccess_NonPixelShader,
	ReadAccess_AllShader
};
enum class RenderGraphLoadAccessOp : uint8 {
	Discard,
	Preserve,
	Clear,
	NoAccess
};
enum class RenderGraphStoreAccessOp : uint8 {
	Discard,
	Preserve,
	Resolve,
	NoAccess
};

inline constexpr uint8 CombineAccessOps(RenderGraphLoadAccessOp load_op, RenderGraphStoreAccessOp store_op) {
	return (uint8)load_op << 2 | (uint8)store_op;
}
enum class RenderGraphLoadStoreAccessOp : uint8 {
	Discard_Discard = CombineAccessOps(RenderGraphLoadAccessOp::Discard, RenderGraphStoreAccessOp::Discard),
	Discard_Preserve = CombineAccessOps(RenderGraphLoadAccessOp::Discard, RenderGraphStoreAccessOp::Preserve),
	Clear_Preserve = CombineAccessOps(RenderGraphLoadAccessOp::Clear, RenderGraphStoreAccessOp::Preserve),
	Preserve_Preserve = CombineAccessOps(RenderGraphLoadAccessOp::Preserve, RenderGraphStoreAccessOp::Preserve),
	Clear_Discard = CombineAccessOps(RenderGraphLoadAccessOp::Clear, RenderGraphStoreAccessOp::Discard),
	Preserve_Discard = CombineAccessOps(RenderGraphLoadAccessOp::Preserve, RenderGraphStoreAccessOp::Discard),
	Clear_Resolve = CombineAccessOps(RenderGraphLoadAccessOp::Clear, RenderGraphStoreAccessOp::Resolve),
	Preserve_Resolve = CombineAccessOps(RenderGraphLoadAccessOp::Preserve, RenderGraphStoreAccessOp::Resolve),
	Discard_Resolve = CombineAccessOps(RenderGraphLoadAccessOp::Discard, RenderGraphStoreAccessOp::Resolve),
	NoAccess_NoAccess = CombineAccessOps(RenderGraphLoadAccessOp::NoAccess, RenderGraphStoreAccessOp::NoAccess),
};
inline constexpr void SplitAccessOp(RenderGraphLoadStoreAccessOp load_store_op, RenderGraphLoadAccessOp& load_op, RenderGraphStoreAccessOp& store_op) {
	store_op = static_cast<RenderGraphStoreAccessOp>((uint8)load_store_op & 0b11);
	load_op = static_cast<RenderGraphLoadAccessOp>(((uint8)load_store_op >> 2) & 0b11);
}

#pragma endregion

class RenderGraph;
class RenderGraphBuilder;
class Device;

class RenderGraphPassBase {
	friend RenderGraph;
	friend RenderGraphBuilder;

	struct RenderTargetInfo {
		RenderGraphRenderTargetId render_target_handle;
		RenderGraphLoadStoreAccessOp render_target_access;
	};
	struct DepthStencilInfo {
		RenderGraphDepthStencilId depth_stencil_handle;
		RenderGraphLoadStoreAccessOp depth_access;
		RenderGraphLoadStoreAccessOp stencil_access;
		bool depth_read_only;
	};

public:
	explicit RenderGraphPassBase(char const* name, RenderGraphPassType type = RenderGraphPassType::Graphics, RenderGraphPassFlags flags = RenderGraphPassFlags::None)
		: m_name(name), m_type(type), m_flags(flags), m_id(0)  {}
	virtual ~RenderGraphPassBase() = default;

protected:
	virtual void Setup(RenderGraphBuilder&) = 0;
	virtual void Execute(RenderGraphPassExecutionContext&, CommandList*) const = 0;

	bool IsCulled() const { return CanBeCulled() && m_ref_count == 0; }
	bool CanBeCulled() const { return !m_flags.HasFlag(RenderGraphPassFlagsEnum::ForceNoCull); }
private:
	std::string const m_name;
	uint64 m_ref_count = 0ull;
	RenderGraphPassType m_type;
	RenderGraphPassFlags m_flags = RenderGraphPassFlagsEnum::None;
	uint64 m_id;

	std::unordered_set<RenderGraphTextureId> m_texture_creates;
	std::unordered_set<RenderGraphTextureId> m_texture_reads;
	std::unordered_set<RenderGraphTextureId> m_texture_writes;
	std::unordered_set<RenderGraphTextureId> m_texture_destroys;
	std::unordered_map<RenderGraphTextureId, ResourceState> m_texture_state_map;

	std::unordered_set<RenderGraphBufferId> m_buffer_creates;
	std::unordered_set<RenderGraphBufferId> m_buffer_reads;
	std::unordered_set<RenderGraphBufferId> m_buffer_writes;
	std::unordered_set<RenderGraphBufferId> m_buffer_destroys;
	std::unordered_map<RenderGraphBufferId, ResourceState> m_buffer_state_map;

	std::vector<RenderTargetInfo> m_render_targets_info;
	std::optional<DepthStencilInfo> m_depth_stencil = std::nullopt;
	uint32 m_viewport_width = 0;
	uint32 m_viewport_height = 0;

	std::vector<uint32> m_events_to_start;
	uint32 m_num_events_to_end = 0;

	uint64 m_wait_graphics_pass_id	= uint64_MAX;
	uint64 m_signal_graphics_pass_id	= uint64_MAX;
	uint64 m_signal_value				= uint64_MAX;
	uint64 m_wait_value				= uint64_MAX;
};

template<typename PassData>
class RenderGraphPass final : public RenderGraphPassBase {
public:
	using SetupFunc = std::function<void(PassData&, RenderGraphBuilder&)>;
	using ExecuteFunc = std::function<void(PassData const&, RenderGraphPassExecutionContext&, CommandList*)>;

public:
	RenderGraphPass(char const* name, SetupFunc&& setup, ExecuteFunc&& execute, RenderGraphPassType type = RenderGraphPassType::Graphics, RenderGraphPassFlags flags = RenderGraphPassFlagsEnum::None)
		: RenderGraphPassBase(name, type, flags), m_setup(std::move(setup)), m_execute(std::move(execute))
	{}

	PassData const& GetPassData() const {
		return m_data;
	}

private:
	PassData m_data;
	SetupFunc m_setup;
	ExecuteFunc m_execute;

private:
	void Setup(RenderGraphBuilder& builder) override {
		assert(m_setup != nullptr && "setup function is null!");
		m_setup(m_data, builder);
	}

	void Execute(RenderGraphPassExecutionContext& context, CommandList* ctx) const override {
		assert(m_execute != nullptr && "execute function is null!");
		m_execute(m_data, context, ctx);
	}
};

#pragma region void specialization
template<>
class RenderGraphPass<void> final : public RenderGraphPassBase {
public:
	using SetupFunc = std::function<void(RenderGraphBuilder&)>;
	using ExecuteFunc = std::function<void(RenderGraphPassExecutionContext&, CommandList*)>;

public:
	RenderGraphPass(char const* name, SetupFunc&& setup, ExecuteFunc&& execute, RenderGraphPassType type = RenderGraphPassType::Graphics, RenderGraphPassFlags flags = RenderGraphPassFlagsEnum::None)
		: RenderGraphPassBase(name, type, flags), m_setup(std::move(setup)), m_execute(std::move(execute))
	{}

	void GetPassData() const {
		return;
	}

private:
	SetupFunc m_setup;
	ExecuteFunc m_execute;

private:
	void Setup(RenderGraphBuilder& builder) override {
		assert(m_setup != nullptr && "setup function is null!");
		m_setup(builder);
	}

	void Execute(RenderGraphPassExecutionContext& context, CommandList* ctx) const override {
		assert(m_execute != nullptr && "execute function is null!");
		m_execute(context, ctx);
	}
};

inline std::string RenderGraphPassTypeToString(RenderGraphPassType type) {
	switch (type) {
		case RenderGraphPassType::Graphics: 
			return "Graphics";
		case RenderGraphPassType::Compute: 
			return "Compute";
		case RenderGraphPassType::AsyncCompute: 
			return "ComputeAsync";
		case RenderGraphPassType::Copy: 
			return "Copy";
	}
	return "Invalid";
}
#pragma endregion
};