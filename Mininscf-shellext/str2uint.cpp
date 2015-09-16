#include "string2int_t.h"
#include <string>
#include <sstream>
#include <memory>

#define str2T(T, N) T N (const std::string &str) { \
	std::stringstream strm; \
	T val; \
	strm<<str; \
	strm.seekg(0); \
	strm>>val; \
	return val; \
}

str2T(uint16_t, str2uint16_t)
str2T(uint32_t, str2uint32_t)
str2T(uint64_t, str2uint64_t)
str2T(int16_t, str2int16_t)
str2T(int32_t, str2int32_t)
str2T(int64_t, str2int64_t)
