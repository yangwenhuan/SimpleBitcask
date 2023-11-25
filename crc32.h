#pragma once

#include <cstdint>
#include <cstddef>

using namespace std;

void GenCrc32Table(uint32_t arrCrc32Table[256]);

uint32_t CalCrc32(const uint8_t* data, size_t iLen, uint32_t arrCrc32Table[256]);