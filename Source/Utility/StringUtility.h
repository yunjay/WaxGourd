#pragma once
#include "MathDefines.h"
#include <string>
namespace WaxGourd {
	std::wstring ToWideString(std::string const&);
	std::string ToString(std::wstring const&);
	std::string ToString(float3 const&);
	std::string ToString(float4 const&);
}