#pragma once
#include <vector>
#include <optional>

#include "EnumFlags.h"
#include "Descriptor.h"
#include "Macros.h"
#include "MathDefines.h"
#include "ResourceEnum.h"
namespace WaxGourd {
using WaxGourd::Descriptor;
using WaxGourd::ClearValue;
enum class LoadAccess : uint8 {
    Discard,
    Preserve,
    Clear,
    No_Access
};
enum class StoreAccess : uint8 {
    Discard,
    Preserve,
    Resolve,
    No_Access
};
enum class RenderPassFlagBits : uint32 {
    None = 0,
    Read_Only_Depth = BIT(0),
    Read_Only_Stencil = BIT(1),
    Allow_UAV_Writes = BIT(2),
    Suspending_Pass = BIT(3),
    Resuming_Pass = BIT(4),
};
using RenderPassFlags = EnumFlags<RenderPassFlagBits>;

struct ColorAttachmentInfo {
    Descriptor cpu_handle;
    LoadAccess beginning_access;
    StoreAccess ending_access;
    ClearValue clear_value;
};

struct DepthAttachmentInfo {
    Descriptor cpu_handle;
    LoadAccess depth_beginning_access;
    StoreAccess depth_ending_access;
    LoadAccess stencil_beginning_access = GfxLoadAccessOp::NoAccess;
    StoreAccess stencil_ending_access = GfxStoreAccessOp::NoAccess;
    ClearValue clear_value;
};

struct RenderPassInfo {
    std::vector<ColorAttachmentInfo> rtv_attachments{}; // render targets
    std::optional<DepthAttachmentInfo> dsv_attachment = std::nullopt; // depth stencil views
    RenderPassFlags flags = RenderPassFlagBits::None;
    uint32 width = 0;
    uint32 height = 0;
    bool legacy_render_pass = false; // legacy barriers
};

}