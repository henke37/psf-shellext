#ifndef STRING2INT_T_H
#define STRING2INT_T_H
#include <string>
#include <cstdint>


uint8_t str2uint8_t(const std::string &);
uint16_t str2uint16_t(const std::string &);
uint32_t str2uint32_t(const std::string &);
uint64_t str2uint64_t(const std::string &);

int8_t str2int8_t(const std::string &);
int16_t str2int16_t(const std::string &);
int32_t str2int32_t(const std::string &);
int64_t str2int64_t(const std::string &);

#endif