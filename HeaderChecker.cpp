#include "HeaderChecker.h"
#include<iostream>
#include<fstream>
#include <cstring>
#include <cstdint>
#include<algorithm>

WaveFormat::WaveHeaderInfo::WaveHeaderInfo() {
  // current constructor does nothing
  std::cout<<"Examining the header of WAV file"<<std::endl;
  iWaveFile.open("s1.wav", std::ios::binary);
  if(!iWaveFile.is_open()) {
    std::cerr<<"Can not open the WAV file"<<std::endl;
  }
}

// bytes [1-4] is "RIFF" checker
bool WaveFormat::WaveHeaderInfo::headerRiffExamine() {
  // checks if the byte 1-4 contains "RIFF"
  char riff[4];
  iWaveFile.read(riff,4); 
  // check for RIFF signature
  return (std::strncmp(riff,"RIFF",4) == 0);
}

// bytes [5-8] has the size of the file 
// in little endian format. (excludes [0-4] bytes)
uint32_t WaveFormat::WaveHeaderInfo::fileSizeCalc() {
  // take out the bytes 5-8
  uint32_t fileSize;
  iWaveFile.seekg(4, std::ios::beg);
  iWaveFile.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));

  // convet from little endian to host format
  std::reverse(reinterpret_cast<char*>(&fileSize), reinterpret_cast<char*>(&fileSize));

  return fileSize;
}

// bytes 9-12 contains "WAVE"
bool WaveFormat::WaveHeaderInfo::headerWaveExamine() {
  // seek to correct place
  iWaveFile.seekg(8, std::ios::beg);

  char wave[4];
  iWaveFile.read(wave, 4);
  
  // check "WAVE"
  return (std::strncmp(wave, "WAVE", 4) == 0);
}


std::ifstream& WaveFormat::WaveHeaderInfo::getIwaveFile() {
  return iWaveFile;
}

WaveFormat::WaveHeaderInfo::~WaveHeaderInfo() {
  iWaveFile.close();
}
// endl
