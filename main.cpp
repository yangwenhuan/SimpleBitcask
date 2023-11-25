#include "simple_bitcask.h"
#include <iostream>

int main(int argc, char *argv[])
{
    int iCapacity = 1024;
    std::string sDataDir = "/Users/wenhuanyang/bc";
    Bitcask oBitcask(sDataDir, iCapacity);
    oBitcask.Set("k1", "v1");
    oBitcask.Set("k2", "v2");

    std::string sValue;
    oBitcask.Get("k2", sValue);
    std::cout << sValue << std::endl;

    return 0;
}