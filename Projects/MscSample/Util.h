#pragma ounce

#include <cstdio>
#include <cctype>

#define INFO_LOG(fmt, ...) std::printf("[info] " fmt, __VA_ARGS__)

namespace util {

void HexDump(const uint8_t* buffer, size_t size, const char* description) noexcept;

}
