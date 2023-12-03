#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "HeaderChecker.h"
#include "FormatChecker.h"
#include "DataChunkHeader.h"

const int windowWidth = 800;
const int windowHeight = 600;
const int pointCount = 1000;

void generateRandomPoints(std::vector<std::pair<int, int>> &points)
{
    std::ifstream is("s1.wav", std::ios::binary);

    // Adjust the header size based on the provided information
    int headerSize = 77; // Assuming 77 bytes, adjust as needed
    is.seekg(headerSize, std::ios::beg);

    int x = 0;
    char byte1, byte2;
    int16_t sample;
    int m = 0;
    int prevY = 0;

    while (is.get(byte1) && is.get(byte2) && m < pointCount)
    {
        // Combine two bytes into a 16-bit signed integer
        sample = static_cast<int16_t>((static_cast<unsigned char>(byte2) << 8) | static_cast<unsigned char>(byte1));

        // Scale the sample value based on your needs
        int yt = static_cast<int>(sample) + 32768; // Assuming 16-bit signed audio, adjust as needed

        int y = yt % windowHeight;

        if (std::abs(prevY - y) >= 50)
        {
            prevY = y;
            points.push_back(std::make_pair(x, y));
            m++;
            x += 10; // Adjust the increment based on your rendering area width
        }
    }

    is.close();
}

void drawSmoothCurve(SDL_Renderer *renderer, const std::vector<std::pair<int, int>> &points)
{
    if (points.size() < 3)
    {
        return; // Not enough points to form a curve
    }

    // Draw the curve using Bezier interpolation
    for (size_t i = 1; i < points.size() - 2; ++i)
    {
        float x0 = (float)points[i - 1].first;
        float y0 = (float)points[i - 1].second;
        float x1 = (float)points[i].first;
        float y1 = (float)points[i].second;
        float x2 = (float)points[i + 1].first;
        float y2 = (float)points[i + 1].second;
        float x3 = (float)points[i + 2].first;
        float y3 = (float)points[i + 2].second;

        for (float t = 0.0; t <= 1.0; t += 0.01)
        {
            float tx = 0.5 * ((2 * x1) + (-x0 + x2) * t + (2 * x0 - 5 * x1 + 4 * x2 - x3) * t * t + (-x0 + 3 * x1 - 3 * x2 + x3) * t * t * t);
            float ty = 0.5 * ((2 * y1) + (-y0 + y2) * t + (2 * y0 - 5 * y1 + 4 * y2 - y3) * t * t + (-y0 + 3 * y1 - 3 * y2 + y3) * t * t * t);

            SDL_RenderDrawPoint(renderer, static_cast<int>(tx), static_cast<int>(ty));
        }
    }
}

void drawText()
{
    // TODO make this inside the SDL window
    // examine the header of the sampleWavFile
    WaveFormat::WaveHeaderInfo *headerInfo = new WaveFormat::WaveHeaderInfo();

    std::cout << "Riff Check: bytes [1-4]: " << headerInfo->headerRiffExamine() << std::endl;

    std::cout << "File Size in KB (excluding 'RIFF'): bytes [5-8]: " << ((float)headerInfo->fileSizeCalc() / (1024 * 1024)) << std::endl;

    std::cout << "Wave check: bytes [9-12]: " << headerInfo->headerWaveExamine() << std::endl;

    // format checking and data gathering
    WaveFormat::FormatChecker *formatChecker = new WaveFormat::FormatChecker();
    std::cout << "FMT presence: bytes [13-16]: " << formatChecker->checkFmtStart() << std::endl;
    std::cout << "Format Chunk : bytes [17-20]: " << formatChecker->formatChunkSizeCalc() << std::endl;
    std::cout << "Audio Format : bytes [21-22]: " << formatChecker->audioFormatCalc() << std::endl;
    std::cout << "No. of channel : bytes [23-24]: " << formatChecker->noOfChannelCalc() << std::endl;
    std::cout << "Sample Rate Value : bytes [25-28]: " << formatChecker->sampleRate() << std::endl;
    std::cout << "Byte Rate Value : bytes [29-32]: " << formatChecker->byteRate() << std::endl;
    std::cout << "Block Align Value : bytes [33-34]: " << formatChecker->blockAlign() << std::endl;
    std::cout << "Bits Per Sample : bytes [35-36]: " << formatChecker->bitsPerSample() << std::endl;

    // Data Chunk Header Examine
    WaveFormat::DataChunkHeader *dataChunkHeaderCheck = new WaveFormat::DataChunkHeader();
    std::cout << "contains 'data': bytes [37-40]: " << dataChunkHeaderCheck->startOfDataChunkCheck() << std::endl;
    std::cout << "size of data section: bytes [41-44]: " << dataChunkHeaderCheck->sizeOfDataSectionCalc() << std::endl;
}

int main()
{
    drawText();

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    SDL_Window *window = SDL_CreateWindow("WAV Format Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::vector<std::pair<int, int>> points;
    generateRandomPoints(points);

    SDL_Event event;
    bool quit = false;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw points
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
        drawSmoothCurve(renderer, points);

        SDL_RenderPresent(renderer);

        // Shift all x-coordinates to the left
        for (auto &point : points)
        {
            point.first -= 2; // Adjust the decrement based on your desired speed
        }

        SDL_Delay(20); // Add a small delay to control the speed of the visualizer
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
