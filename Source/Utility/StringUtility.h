#pragma once
#include "MathDefines.h"
#include <string>
#include <string_view>
namespace WaxGourd {
	std::wstring ToWideString(std::string const&);
	std::string ToString(std::wstring const&);
	std::string ToString(float3 const&);
	std::string ToString(float4 const&);

	std::string GetParentPath(std::string_view complete_path);
	std::string GetFilename(std::string_view complete_path);
	std::string GetFilenameWithoutExtension(std::string_view complete_path);
	bool FileExists(std::string_view file_path);
	std::string GetExtension(std::string_view path);
	std::string MakeAllLowercase(std::string const&);
	std::string MakeAllUppercase(std::string const&);
}