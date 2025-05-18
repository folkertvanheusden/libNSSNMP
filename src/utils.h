// (C) 2022-2024 by folkert van heusden <mail@vanheusden.com>, released under Apache License v2.0
#include <cstdint>
#include <string>
#include <vector>


std::string myformat(const char *const fmt, ...);
std::vector<std::string> split(std::string in, const std::string & splitter);
uint64_t get_micros();
