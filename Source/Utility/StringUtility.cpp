#pragma once
#include "StringUtility.h"
#include <sstream>
#include <codecvt>
#include <locale>
namespace WaxGourd {
std::wstring ToWideString(std::string const& str) {
	int num_chars = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), NULL, 0);
	std::wstring wstr;
	if (num_chars) {
		wstr.resize(num_chars);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), &wstr[0], num_chars);
	}
	return wstr;
}
std::string ToString(std::wstring const&) {
	int num_chars = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
	std::string str;
	if (num_chars > 0) {
		str.resize(num_chars);
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), &str[0], num_chars, NULL, NULL);
	}
	return str;
}
std::string ToString(float3 const& vec) {
	return "(" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "," + std::to_string(vec.z) + ")";
}
std::string ToString(float4 const& vec) {
	return "(" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "," + std::to_string(vec.z) + "," + std::to_string(vec.w) + ")";
}
}