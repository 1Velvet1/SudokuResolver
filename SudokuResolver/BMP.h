#ifndef BMP___H
#define BMP___H


#include <cstdint>
#include <vector>
#include <fstream>

using image = std::vector<std::vector<uint32_t>>;
using byte = unsigned char;
constexpr uint16_t bmpHeaderSize = 14;
constexpr uint16_t bmpDIBsize = 40;



class BMP
{

public:
    
    BMP();
    BMP& operator=(BMP&& other);
    explicit BMP(const char* path);
    ~BMP();
    image getImage() const;
    size_t getHeight() const;
    size_t getWidth() const;

private:

    const char* path_;
    byte* bmpHeader;
    byte* bmpDIB;
    size_t fileSize;
    size_t DIBsize;
    size_t width;
    size_t height;
    size_t imgSize;
    uint8_t paddingAmount;
    uint8_t bitDepth;
    image img;

};


#endif // !BMP___H