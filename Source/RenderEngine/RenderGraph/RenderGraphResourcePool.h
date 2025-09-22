#pragma once
#include "Device.h"
#include "GpuBuffer.h"
#include "Texture.h"

namespace WaxGourd {
class RenderGraphResourcePool {
private:
    template<typename T>
    struct PooledResource {
        std::unique_ptr<T> resource;
        uint64 last_used_frame;
        bool is_active;
    };

    using PooledTexture = PooledResource<Texture>;
    using PooledBuffer = PooledResource<GpuBuffer>;

    Device* device = nullptr;
    uint64 frame_index = 0;
    std::vector<PooledTexture> m_texture_pool;
    std::vector<PooledBuffer>  m_buffer_pool;
public:
    explicit RenderGraphResourcePool(Device* device) : device(device) {}
    void Tick() {
        CleanPool(m_texture_pool);
        CleanPool(m_buffer_pool);
        ++frame_index;
    }
    Texture* AllocateTexture(TextureDesc const& desc) {
        for (auto& pooled_texture : m_texture_pool) {
            // TextureDesc has a custom IsCompatible function for more flexible reuse
            if (!pooled_texture.is_active && pooled_texture.resource->GetDesc().IsCompatible(desc)) {
                pooled_texture.last_used_frame = frame_index;
                pooled_texture.is_active = true;
                return pooled_texture.resource.get();
            }
        }
        auto& texture = m_texture_pool.emplace_back(PooledTexture{ std::make_unique<Texture>(device, desc), frame_index, true }).resource;
        return texture.get();
    }
    void ReleaseTexture(Texture* texture) {
        for (auto& pooled_texture : m_texture_pool) {
            // Could add a map with pointer keys to indices for faster lookup for faster release
            if (pooled_texture.is_active && pooled_texture.resource.get() == texture) {
                pooled_texture.is_active = false;
                return;
            }
        }
    }

    GpuBuffer* AllocateBuffer(GpuBufferDesc const& desc){
        for (auto& pooled_buffer : m_buffer_pool) {
            // GpuBufferDesc can be compared directly
            if (!pooled_buffer.is_active && pooled_buffer.resource->GetDesc() == desc) {
                pooled_buffer.last_used_frame = frame_index;
                pooled_buffer.is_active = true;
                return pooled_buffer.resource.get();
            }
        }
        auto& buffer = m_buffer_pool.emplace_back(PooledBuffer{ std::make_unique<GpuBuffer>(device, desc), frame_index, true }).resource;
        return buffer.get();
    }
    void ReleaseBuffer(GpuBuffer* buffer)
    {
        for (auto& pooled_buffer : m_buffer_pool) {
            if (pooled_buffer.is_active && pooled_buffer.resource.get() == buffer) {
                pooled_buffer.is_active = false;
                return;
            }
        }
    }

    Device* GetDevice() const { return device; }

private:
    template<typename Pool>
    void CleanPool(Pool& pool) {
        // How many frames to keep an inactive resource before deleting it.
        constexpr uint64 FRAME_RETENTION_COUNT = 4;

        // Delete inactive resources
        for (uint64 i = 0; i < pool.size();) {
            if (!pool[i].is_active && pool[i].last_used_frame + FRAME_RETENTION_COUNT < frame_index) {
                // Delete from back to avoid shifting elements.
                std::swap(pool[i], pool.back());
                pool.pop_back();
            }
            else {
                ++i;
            }
        }
    }
}; // class RenderGraphResourcePool
} // namespace WaxGourd