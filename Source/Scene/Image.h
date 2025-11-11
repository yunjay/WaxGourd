#pragma once
#pragma once
#include "ResourceEnum.h"
#include "MathDefines.h"
#include <memory>
#include <string>
#include <string_view>
namespace WaxGourd {
class Image {
public:
	explicit Image(ResourceFormat format) : m_format(format) {}
	explicit Image(std::string_view file_path);

	uint32 Width() const;
	uint32 Height() const;
	uint32 Depth() const;
	uint32 MipLevels() const;
	ResourceFormat Format() const;
	bool IsHDR() const;
	bool IsCubemap() const;
	Image const* NextImage() const;

	template<typename T = uint8>
	T const* Data() const;
	template<typename T = uint8>
	T const* MipData(uint32 mip_level) const;

private:
	uint32 m_width = 0u;
	uint32 m_height = 0u;
	uint32 m_depth = 0u;
	uint32 m_mip_levels = 0u;
	std::vector<uint8> m_pixels;
	bool m_is_hdr = false;
	bool m_is_cubemap = false;
	ColorSpace m_color_space = ColorSpace::Linear;
	ResourceFormat m_format = ResourceFormat::Unknown;
	std::unique_ptr<Image> m_next_image = nullptr;

private:
	uint64 setData(uint32 width, uint32 height, uint32 depth, uint32 mip_levels, void const* data);

	bool loadDDS(std::string_view texture_path);
	bool loadSTB(std::string_view texture_path);
};

template<typename T>
T const* Image::Data() const {
	return reinterpret_cast<T const*>(pixels.data());
}

template<typename T>
T const* Image::MipData(uint32 mip_level) const {
	uint64 offset = 0;
	for (uint32 mip = 0; mip < mip_level; ++mip)
	{
		offset += GetResourceFormatMipByteSize(m_format, m_width, m_height, m_depth, mip);
	}
	return reinterpret_cast<T const*>(m_pixels.data() + offset);
}
}