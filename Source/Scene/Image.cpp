#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "StringUtility.h"
/*
* Cpu side image loading and container.
*/
namespace WaxGourd {
#pragma region ImageFormat
namespace {
enum class ImageFormat {
	DDS = 0,
	BMP,
	JPG,
	PNG,
	TIFF,
	GIF,
	TGA,
	HDR,
	// WEBP ...
	Not_Supported
};
ImageFormat GetImageFormat(std::string_view path) {
	std::string extension = GetExtension(path);
	MakeAllLowercase(extension);
	if (extension == ".dds")
		return ImageFormat::DDS;
	else if (extension == ".bmp")
		return ImageFormat::BMP;
	else if (extension == ".jpg" || extension == ".jpeg")
		return ImageFormat::JPG;
	else if (extension == ".png")
		return ImageFormat::PNG;
	else if (extension == ".tiff" || extension == ".tif")
		return ImageFormat::TIFF;
	else if (extension == ".gif")
		return ImageFormat::GIF;
	else if (extension == ".tga")
		return ImageFormat::TGA;
	else if (extension == ".hdr")
		return ImageFormat::HDR;
	else
		return ImageFormat::Not_Supported;
}
ImageFormat GetImageFormat(std::wstring_view path) {
	return GetImageFormat(ToString(std::wstring(path)));
}

void ConvertDX10Format(DXGI_FORMAT format, ResourceFormat& outFormat, bool outSRGB) {
		if (format == DXGI_FORMAT_BC1_UNORM) { outFormat = ResourceFormat::BC1_UNORM;			 outSRGB = false;	return; }
		if (format == DXGI_FORMAT_BC1_UNORM_SRGB) { outFormat = ResourceFormat::BC1_UNORM;		 outSRGB = true;	return; }
		if (format == DXGI_FORMAT_BC2_UNORM) { outFormat = ResourceFormat::BC2_UNORM;			 outSRGB = false;	return; }
		if (format == DXGI_FORMAT_BC2_UNORM_SRGB) { outFormat = ResourceFormat::BC2_UNORM;		 outSRGB = true;	return; }
		if (format == DXGI_FORMAT_BC3_UNORM) { outFormat = ResourceFormat::BC3_UNORM;			 outSRGB = false;	return; }
		if (format == DXGI_FORMAT_BC4_UNORM) { outFormat = ResourceFormat::BC4_UNORM;			 outSRGB = false;	return; }
		if (format == DXGI_FORMAT_BC5_UNORM) { outFormat = ResourceFormat::BC5_UNORM;			 outSRGB = false;	return; }
		if (format == DXGI_FORMAT_BC6H_UF16) { outFormat = ResourceFormat::BC6H_UF16;			 outSRGB = false;	return; }
		if (format == DXGI_FORMAT_BC7_UNORM) { outFormat = ResourceFormat::BC7_UNORM;			 outSRGB = false;	return; }
		if (format == DXGI_FORMAT_BC7_UNORM_SRGB) { outFormat = ResourceFormat::BC7_UNORM;		 outSRGB = true;	return; }
		if (format == DXGI_FORMAT_R32G32B32A32_FLOAT) { outFormat = ResourceFormat::R32G32B32A32_FLOAT; outSRGB = false;	return; }
		if (format == DXGI_FORMAT_R32G32_FLOAT) { outFormat = ResourceFormat::R32G32_FLOAT;		 outSRGB = false;	return; }
		if (format == DXGI_FORMAT_R9G9B9E5_SHAREDEXP) { outFormat = ResourceFormat::R9G9B9E5_SHAREDEXP;	outSRGB = false;	return; }
	};
} // translation unit
#pragma endregion

uint32 Image::Width() const { return m_width; }
uint32 Image::Height() const { return m_height; }
uint32 Image::Depth() const { return m_depth; }
uint32 Image::MipLevels() const { return m_mip_levels; }
ResourceFormat Image::Format() const { return m_format; }
bool Image::IsHDR() const { return m_is_hdr; }
bool Image::IsCubemap() const { return m_is_cubemap; }
Image const* Image::NextImage() const { return m_next_image.get(); }

Image::Image(std::string_view file_path) {
	ImageFormat format = GetImageFormat(file_path);
	if(format == ImageFormat::Not_Supported) {
		assert(false && "Unsupported Texture Format!");
	}
	bool result;
	if(format == ImageFormat::DDS) {
		result = loadDDS(file_path);
	} else {
		result = loadSTB(file_path);
	}
}

uint64 Image::setData(uint32 _width, uint32 _height, uint32 _depth, uint32 _mip_levels, void const* _data) {
	m_width = std::max(_width, 1u);
	m_height = std::max(_height, 1u);
	m_depth = std::max(_depth, 1u);
	m_mip_levels = std::max(_mip_levels, 1u);
	
	uint64 texture_byte_size = GetResourceFormatByteSize(m_format, m_width, m_height, m_depth, m_mip_levels);
	m_pixels.resize(texture_byte_size);
	memcpy(m_pixels.data(), _data, m_pixels.size());
	return texture_byte_size;
}

bool Image::loadDDS(std::string_view texture_path) {
	// https://github.com/simco50/D3D12_Research/blob/master/D3D12/Content/Image.cpp
	FILE* file = nullptr;
	fopen_s(&file, texture_path.data(), "rb");
	if (!file)
		return false;

	fseek(file, 0, SEEK_END);
	std::vector<char> data((uint64)ftell(file));
	fseek(file, 0, SEEK_SET);
	fread(data.data(), data.size(), 1, file);

	char* bytes = data.data();
#pragma pack(push,1)
	struct PixelFormatHeader {
		uint32 dwSize;
		uint32 dwFlags;
		uint32 dwFourCC;
		uint32 dwRGBBitCount;
		uint32 dwRBitMask;
		uint32 dwGBitMask;
		uint32 dwBBitMask;
		uint32 dwABitMask;
	};
#pragma pack(pop)

	// .DDS header.
#pragma pack(push,1)
	struct FileHeader {
		uint32 dwSize;
		uint32 dwFlags;
		uint32 dwHeight;
		uint32 dwWidth;
		uint32 dwLinearSize;
		uint32 dwDepth;
		uint32 dwMipMapCount;
		uint32 dwReserved1[11];
		PixelFormatHeader ddpf;
		uint32 dwCaps;
		uint32 dwCaps2;
		uint32 dwCaps3;
		uint32 dwCaps4;
		uint32 dwReserved2;
	};
#pragma pack(pop)

	// .DDS 10 header.
#pragma pack(push,1)
	struct DX10FileHeader {
		uint32 dxgiFormat;
		uint32 resourceDimension;
		uint32 miscFlag;
		uint32 arraySize;
		uint32 reserved;
	};
#pragma pack(pop)
	enum DDS_CAP_ATTRIBUTE {
		DDSCAPS_COMPLEX = 0x00000008U,
		DDSCAPS_TEXTURE = 0x00001000U,
		DDSCAPS_MIPMAP = 0x00400000U,
		DDSCAPS2_VOLUME = 0x00200000U,
		DDSCAPS2_CUBEMAP = 0x00000200U,
	};

	auto MakeFourCC = [](uint32 a, uint32 b, uint32 c, uint32 d) { return a | (b << 8u) | (c << 16u) | (d << 24u); };

	constexpr const char magic[] = "DDS ";
	if (memcmp(magic, bytes, 4) != 0) return false;
	bytes += 4;

	const FileHeader* dds_header = (FileHeader*)bytes;
	bytes += sizeof(FileHeader);

	if (dds_header->dwSize == sizeof(FileHeader) &&
		dds_header->ddpf.dwSize == sizeof(PixelFormatHeader)) {
		m_color_space = ColorSpace::Linear;
		uint32 bpp = dds_header->ddpf.dwRGBBitCount;

		uint32 four_cc = dds_header->ddpf.dwFourCC;
		bool has_dxgi = four_cc == MakeFourCC('D', 'X', '1', '0');
		const DX10FileHeader* pDx10Header = nullptr;

		if (has_dxgi) {
			pDx10Header = (DX10FileHeader*)bytes;
			bytes += sizeof(DX10FileHeader);

			ConvertDX10Format((DXGI_FORMAT)pDx10Header->dxgiFormat, m_format, (m_color_space == ColorSpace::sRGB));
		}
		else {
			static constexpr uint32 FOURCC_R5G6B5 = 23;			// B5G6R5_UNORM  
			static constexpr uint32 FOURCC_RGB5A1 = 25;			// B5G5R5A1_UNORM
			static constexpr uint32 FOURCC_RGBA4 = 26;			// B4G4R4A4_UNORM
			static constexpr uint32 FOURCC_RGBA16U = 36;
			static constexpr uint32 FOURCC_RGBA16S = 110;
			static constexpr uint32 FOURCC_R16F = 111;
			static constexpr uint32 FOURCC_RG16F = 112;
			static constexpr uint32 FOURCC_RGBA16F = 113;
			static constexpr uint32 FOURCC_R32F = 114;
			static constexpr uint32 FOURCC_RG32F = 115;
			static constexpr uint32 FOURCC_RGBA32F = 116;
			switch (four_cc)
			{
			case MakeFourCC('B', 'C', '4', 'U'):	m_format = ResourceFormat::BC4_UNORM;			break;
			case MakeFourCC('D', 'X', 'T', '1'):	m_format = ResourceFormat::BC1_UNORM;			break;
			case MakeFourCC('D', 'X', 'T', '3'):	m_format = ResourceFormat::BC2_UNORM;			break;
			case MakeFourCC('D', 'X', 'T', '5'):	m_format = ResourceFormat::BC3_UNORM;			break;
			case MakeFourCC('B', 'C', '5', 'U'):	m_format = ResourceFormat::BC5_UNORM;			break;
			case MakeFourCC('A', 'T', 'I', '2'):	m_format = ResourceFormat::BC5_UNORM;			break;
			case FOURCC_RGBA16U:					m_format = ResourceFormat::R16G16B16A16_UNORM;	break;
			case FOURCC_RGBA16S:					m_format = ResourceFormat::R16G16B16A16_SNORM;	break;
			case FOURCC_R16F:						m_format = ResourceFormat::R16_FLOAT;			break;
			case FOURCC_RG16F:						m_format = ResourceFormat::R16G16_FLOAT;		break;
			case FOURCC_RGBA16F:					m_format = ResourceFormat::R16G16B16A16_FLOAT;	break;
			case FOURCC_R32F:						m_format = ResourceFormat::R32_FLOAT;			break;
			case FOURCC_RG32F:						m_format = ResourceFormat::R32G32_FLOAT;		break;
			case FOURCC_RGBA32F:					m_format = ResourceFormat::R32G32B32A32_FLOAT;	break;
			case 0:
				if (bpp == 32)
				{
					auto TestMask = [=](uint32 r, uint32 g, uint32 b, uint32 a)
						{
							return dds_header->ddpf.dwRBitMask == r &&
								dds_header->ddpf.dwGBitMask == g &&
								dds_header->ddpf.dwBBitMask == b &&
								dds_header->ddpf.dwABitMask == a;
						};

					if (TestMask(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000)) m_format = ResourceFormat::R8G8B8A8_UNORM;
					else if (TestMask(0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000)) m_format = ResourceFormat::B8G8R8A8_UNORM;
					else return false;
				}
				break;
			default:
				return false;
			}
		}

		bool _is_cubemap = (dds_header->dwCaps2 & 0x0000FC00U) != 0 || (has_dxgi && (pDx10Header->miscFlag & 0x4) != 0);
		uint32 image_chain_count = 1;
		if (_is_cubemap)
		{
			image_chain_count = 6;
			m_is_cubemap = true;
		}
		else if (has_dxgi && pDx10Header->arraySize > 1)
		{
			image_chain_count = pDx10Header->arraySize;
		}

		Image* current_image = this;
		for (uint32 image_idx = 0; image_idx < image_chain_count; ++image_idx)
		{
			uint64 offset = current_image->setData(dds_header->dwWidth, dds_header->dwHeight, dds_header->dwDepth, dds_header->dwMipMapCount, bytes);
			bytes += offset;
			if (image_idx < image_chain_count - 1)
			{
				current_image->m_next_image = std::make_unique<Image>(m_format);
				current_image = current_image->m_next_image.get();
			}
		}
	}
	else {
		return false;
	}
	return true;
}

bool Image::loadSTB(std::string_view texture_path) {
	int32 components = 0;
	m_is_hdr = stbi_is_hdr(texture_path.data());
	if (m_is_hdr) {
		int32 _width, _height;
		float* stbi_loaded_pixels = stbi_loadf(texture_path.data(), &_width, &_height, &components, 4);
		if (stbi_loaded_pixels == nullptr) return false;
		m_width = (uint32)_width;
		m_height = (uint32)_height;
		m_depth = 1;
		m_mip_levels = 1;
		m_format = ResourceFormat::R32G32B32A32_FLOAT;
		m_pixels.resize((size_t)m_width * (size_t)m_height * 4 * sizeof(float));
		memcpy(m_pixels.data(), stbi_loaded_pixels, m_pixels.size());
		stbi_image_free(stbi_loaded_pixels);
		return true;
	}
	else {
		int _width, _height;
		stbi_uc* stbi_loaded_pixels = stbi_load(texture_path.data(), &_width, &_height, &components, 4);
		if (stbi_loaded_pixels == nullptr) return false;
		m_width = (uint32)_width;
		m_height = (uint32)_height;
		m_depth = 1;
		m_mip_levels = 1;
		m_format = ResourceFormat::R8G8B8A8_UNORM;
		m_pixels.resize((size_t)m_width * (size_t)m_height * 4);
		memcpy(m_pixels.data(), stbi_loaded_pixels, m_pixels.size());
		stbi_image_free(stbi_loaded_pixels);
		return true;
	}
}
}