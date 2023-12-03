#pragma once
#include <fstream>
#include <cstdint>


namespace WaveFormat {

    class FormatChecker {
        private:
            std::ifstream iWaveFile;
        public:
            FormatChecker();
            bool checkFmtStart();
            uint32_t formatChunkSizeCalc();
            uint16_t audioFormatCalc(); 
            uint16_t noOfChannelCalc();
            uint32_t sampleRate();
            uint32_t byteRate();
            uint16_t blockAlign();
            uint16_t bitsPerSample();
    };
}