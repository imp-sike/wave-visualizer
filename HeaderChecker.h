#pragma once
#include<fstream>

namespace WaveFormat {
  class WaveHeaderInfo {
    private:
      std::ifstream iWaveFile;

       
      // in bytes, excludes "RIFF" header
    
    public:
      WaveHeaderInfo();
      bool headerRiffExamine();
      uint32_t fileSizeCalc(); 
      bool headerWaveExamine();
      std::ifstream& getIwaveFile();
      ~WaveHeaderInfo();
  };
}
