#include "crc32.h"

void GenCrc32Table(uint32_t arrCrc32Table[256])
{
    for(uint32_t i = 0; i < 256; i++) 
    {
        uint32_t iCrc = i;
        for(uint32_t j = 0; j < 8; j++) 
        {
            if (iCrc & 1) iCrc = (iCrc >> 1) ^ 0xEDB88320;
            else iCrc >>= 1;
        }
        arrCrc32Table[i] = iCrc;
    }
}

uint32_t CalCrc32(const uint8_t* data, size_t iLen, uint32_t arrCrc32Table[256]) 
{
    uint32_t iCrc = 0xFFFFFFFF;
    for(size_t i = 0; i < iLen; i++)
        iCrc = (iCrc >> 8) ^ arrCrc32Table[(iCrc ^ data[i]) & 0xFF];
    
    return iCrc ^ 0xFFFFFFFF;
}