#pragma once
#include <fstream>
#include<cstdint>

namespace WaveFormat
{
    class DataChunkHeader {
        private:
            std::ifstream iWaveFile;
            bool is70 = false;
        public:
           DataChunkHeader();
           bool startOfDataChunkCheck();
           uint32_t sizeOfDataSectionCalc();
    };
} // namespace WaveFormat
