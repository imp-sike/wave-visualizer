#include "FormatChecker.h"
#include <iostream>
#include <cstring>
#include <cstdint>

WaveFormat::FormatChecker::FormatChecker()
{
  iWaveFile.open("s1.wav", std::ios::binary);
  if (!iWaveFile.is_open())
  {
    std::cerr << "Can not open the WAV file" << std::endl;
  }
}

bool WaveFormat::FormatChecker::checkFmtStart()
{
  iWaveFile.seekg(12, std::ios::beg);
  char format[4];
  iWaveFile.read(format, 4);
  return (std::strncmp(format, "fmt ", 4) == 0);
}

uint32_t WaveFormat::FormatChecker::formatChunkSizeCalc()
{
  uint32_t formatChunkSize;
  iWaveFile.seekg(16, std::ios::beg);
  iWaveFile.read(reinterpret_cast<char *>(&formatChunkSize), sizeof(formatChunkSize));
  return formatChunkSize;
}

uint16_t WaveFormat::FormatChecker::audioFormatCalc()
{
  uint16_t audioFormat;
  iWaveFile.seekg(20, std::ios::beg);
  iWaveFile.read(reinterpret_cast<char *>(&audioFormat), sizeof(audioFormat));
  return audioFormat;
}

uint16_t WaveFormat::FormatChecker::noOfChannelCalc()
{
  uint16_t noOfChannels;
  iWaveFile.seekg(22, std::ios::beg);
  iWaveFile.read(reinterpret_cast<char *>(&noOfChannels), sizeof(noOfChannels));
  return noOfChannels;
}

uint32_t WaveFormat::FormatChecker::sampleRate()
{
  uint32_t sampleRateValue;
  iWaveFile.seekg(24, std::ios::beg);
  iWaveFile.read(reinterpret_cast<char *>(&sampleRateValue), sizeof(sampleRateValue));
  return sampleRateValue;
}

uint32_t WaveFormat::FormatChecker::byteRate()
{
  uint32_t byteRateValue;
  iWaveFile.seekg(28, std::ios::beg);
  iWaveFile.read(reinterpret_cast<char *>(&byteRateValue), sizeof(byteRateValue));
  return byteRateValue;
}

uint16_t WaveFormat::FormatChecker::blockAlign()
{
  uint16_t blockAlighValue;
  iWaveFile.seekg(32, std::ios::beg);
  iWaveFile.read(reinterpret_cast<char *>(&blockAlighValue), sizeof(blockAlighValue));
  return blockAlighValue;
}

uint16_t WaveFormat::FormatChecker::bitsPerSample()
{
  uint16_t bitsPerSampleValue;
  iWaveFile.seekg(34, std::ios::beg);
  iWaveFile.read(reinterpret_cast<char *>(&bitsPerSampleValue), sizeof(bitsPerSampleValue));
  return bitsPerSampleValue;
}