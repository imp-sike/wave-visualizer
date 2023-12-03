#include "DataChunkHeader.h"
#include <iosfwd>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>

WaveFormat::DataChunkHeader::DataChunkHeader()
{
    iWaveFile.open("s1.wav", std::ios::binary);
    if (!iWaveFile.is_open())
    {
        std::cerr << "Can not open the WAV file" << std::endl;
    }
}

bool WaveFormat::DataChunkHeader::startOfDataChunkCheck()
{
    char data[4];
    // some of the wave file conains LIST, INFO and ISFT also
    // in case of that, 'data' is at 70 bytes away
    iWaveFile.seekg(36, std::ios::beg);

    iWaveFile.read(data, 4);

    if (std::strncmp(data, "data", 4) != 0)
    {
        is70 = true;
        iWaveFile.seekg(70, std::ios::beg);
        iWaveFile.read(data, 4);
    }

    // std::cout << data << std::endl;
    return (std::strncmp(data, "data", 4) == 0);
}

uint32_t WaveFormat::DataChunkHeader::sizeOfDataSectionCalc()
{
    uint offset = 40;
    
    if(is70) {
        offset = 73;
    } 

    uint32_t sizeOfDataSection;
    iWaveFile.seekg(offset, std::ios::beg);
    iWaveFile.read(reinterpret_cast<char *>(&sizeOfDataSection), sizeof(sizeOfDataSection));
    return sizeOfDataSection;
}