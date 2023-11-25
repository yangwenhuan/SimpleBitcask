#include "simple_bitcask.h"
#include <sstream>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

using namespace std;

Bitcask::Bitcask(const std::string &sDataDir, int iCapacity) : m_iCapacity(iCapacity)
{
    if(access(sDataDir.c_str(), F_OK) != 0)
    {
        if(mkdir(sDataDir.c_str(), 0755) != 0)
        {
            printf("err: mkdir fail!\n");
            return;
        }
    }
    m_iFileCnt = 0;
    DIR* dir = opendir(sDataDir.c_str());
    if(!dir)
    {
        printf("err: opendir fail!\n");
        return;
    }
    struct dirent* entry;
    while((entry = readdir(dir)) != NULL)
    {
        if(entry->d_type == DT_REG) m_iFileCnt++;
    }
    closedir(dir);

    if(m_iFileCnt == 0) m_iFileCnt = 1;
    m_sActiveFile = sDataDir + "/" + std::to_string(m_iFileCnt);

    GenCrc32Table(m_arrCrc32Table);
}

Bitcask::~Bitcask()
{
}

int Bitcask::Get(const std::string &sKey, std::string &sValue)
{
    if(m_mapKeyDir.count(sKey) == 0)
    {
        printf("err: sKey=%s not exists.\n", sKey.c_str());
        return 1;
    }
    else
    {
        MemEntry *pMemEntry = m_mapKeyDir[sKey];
        FILE *pFile = fopen(pMemEntry->sFileName.c_str(), "r");
        if(!pFile)
        {
            printf("err: open file=%s fail!\n", pMemEntry->sFileName.c_str());
            return -1;
        }
        fseek(pFile, pMemEntry->iValuePos/*offset*/, SEEK_SET/*文件开头*/); // value位置
        char *pValue = new char[pMemEntry->iValueSize + 1];
        pValue[pMemEntry->iValueSize] = '\0';
        fread(pValue, 1, pMemEntry->iValueSize, pFile);
        sValue = std::string(pValue);
        delete [] pValue;

        fclose(pFile);
    }
    return 0;
}

int Bitcask::Set(const std::string &sKey, const std::string &sValue)
{
    std::string sDiskEntry;
    CreateDiskEntry(sKey, sValue, sDiskEntry);

    FILE *pFile = fopen(m_sActiveFile.c_str(), "a+"); // 追加模式
    if(!pFile)
    {
        printf("err: open m_sActiveFile=%s fail!\n", m_sActiveFile.c_str());
        return -1;
    }
    fseek(pFile, 0, SEEK_END); // 文件末尾
    fwrite(sDiskEntry.c_str(), sizeof(sDiskEntry[0]), sDiskEntry.size(), pFile); // 追加写
    fclose(pFile); // 关闭文件

    m_iActiveFileSize += sDiskEntry.size();

    MemEntry *pMemEntry = new MemEntry;
    pMemEntry->sFileName = m_sActiveFile;
    pMemEntry->iTimeStamp = time(NULL);
    pMemEntry->iValuePos = m_iActiveFileSize - sValue.size();
    pMemEntry->iValueSize = sValue.size();

    m_mapKeyDir[sKey] = pMemEntry;

    return 0;
}

int Bitcask::Compact()
{
    return 0;
}

int Bitcask::CreateDiskEntry(const std::string &sKey, const std::string &sValue, std::string &sEntry)
{
    uint32_t iTimeStamp = time(NULL);

    std::ostringstream oStream1;
    oStream1 << iTimeStamp << sKey.size() << sValue.size() << sKey << sValue;

    uint32_t iCrc32 = CalCrc32(reinterpret_cast<const uint8_t*>(oStream1.str().data()), oStream1.str().size(), m_arrCrc32Table);

    std::ostringstream oStream2;
    oStream2 << iCrc32 << oStream1.str();

    sEntry = oStream2.str();

    return 0;
}

bool Bitcask::IsFull()
{
    return m_iActiveFileSize >= m_iCapacity;
}