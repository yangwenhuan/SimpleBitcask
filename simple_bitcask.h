#pragma once

// 论文：https://riak.com/assets/bitcask-intro.pdf

#include <string>
#include "crc32.h"
#include <unordered_map>

using namespace std;

struct MemEntry // 内存entry
{
    std::string sFileName;
    int iValueSize;
    int iValuePos;
    int iTimeStamp;
};

class Bitcask
{
public:
    Bitcask(const std::string &sDataDir, int iCapacity);
    ~Bitcask();

    int Get(const std::string &sKey, std::string &sValue);
    int Set(const std::string &sKey, const std::string &sValue);

private:
    std::string m_sDataDir;
    std::string m_sActiveFile;
    std::unordered_map<std::string/*key*/, MemEntry *> m_mapKeyDir;

    int m_iActiveFileSize;
    int m_iCapacity;
    int m_iFileCnt;
    uint32_t m_arrCrc32Table[256];

private:
    int Compact(); // 压缩
    int CreateDiskEntry(const std::string &sKey, const std::string &sValue, std::string &sEntry); // 构造磁盘存储entry
    bool IsFull(); // active file 是否满
};