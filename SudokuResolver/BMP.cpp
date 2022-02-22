#include "BMP.h"
#include <string>


BMP::BMP() 
{

    this->path_ = "";
    this->width = 0;
    this->height = 0;
    this->fileSize = 0;
    this->bmpHeader = nullptr;
    this->bmpDIB = nullptr;
    this->imgSize = 0;
    this->DIBsize = 0;

}

BMP& BMP::operator=(BMP&& other)
{

    std::swap(this->bmpDIB, other.bmpDIB);
    std::swap(this->bmpHeader, other.bmpHeader);
    std::swap(this->img, other.img);

    this->bitDepth = other.bitDepth;
    this->DIBsize = other.DIBsize;
    this->height = other.height;
    this->width = other.width;
    this->fileSize = other.fileSize;
    this->imgSize = other.imgSize;
    this->path_ = other.path_;
    this->paddingAmount = other.paddingAmount;


    return *this;
}

BMP::BMP(const char* path):path_(path)
{
    
    std::ifstream input(this->path_, std::ios::in | std::ios::binary);

    if (!input.is_open()) { 
        
        std::string excp = "File \"" + std::string(this->path_) + "\" cannot be read";

     throw std::exception(excp.c_str());

    }
    else {

        this->bmpHeader = new byte[14];

        input.read(reinterpret_cast<char*>(this->bmpHeader), bmpHeaderSize);

        this->fileSize = (static_cast<int>(bmpHeader[5]) << 24) + (static_cast<int>(bmpHeader[4]) << 16) + (static_cast<int>(bmpHeader[3]) << 8) + static_cast<int>(bmpHeader[2]);

        this->bmpDIB = new byte[bmpDIBsize];

        input.read(reinterpret_cast<char*>(this->bmpDIB), bmpDIBsize);

        this->height = (static_cast<int>(bmpDIB[7]) << 24) + (static_cast<int>(bmpDIB[6]) << 16) + (static_cast<int>(bmpDIB[5]) << 8) + static_cast<int>(bmpDIB[4]);
        this->width = (static_cast<int>(bmpDIB[11]) << 24) + (static_cast<int>(bmpDIB[10]) << 16) + (static_cast<int>(bmpDIB[9]) << 8) + static_cast<int>(bmpDIB[8]);

        this->bitDepth = (static_cast<int>(bmpDIB[15]) << 8) + static_cast<int>(bmpDIB[14]);
        this->paddingAmount = (4 - (this->bitDepth / 8 * this->width) % 4) % 4;

        this->imgSize = (static_cast<int>(bmpDIB[23]) << 24) + (static_cast<int>(bmpDIB[22]) << 16) + (static_cast<int>(bmpDIB[21]) << 8) + static_cast<int>(bmpDIB[20]);

        this->img.resize(this->height);
        for (auto& row : this->img) { row = std::vector<uint32_t>(this->width); }

        uint32_t temp = 0;

        for (auto& row : this->img) {

            for (auto& col : row) {

                input.read(reinterpret_cast<char*>(&temp), this->bitDepth / 8);
                input.ignore(this->paddingAmount, -1);
                col = temp;
               

            }
            
        }

        this->img = image(this->img.rbegin(), this->img.rend());

    }

}

BMP::~BMP()
{

    if (this->bmpHeader != nullptr) { delete[] this->bmpHeader; }
    if (this->bmpDIB != nullptr) { delete[] this->bmpDIB; }

}

image BMP::getImage() const {

    if (this->img.size() != 0) {

        return this->img;

    }

    return image(0);

}

size_t BMP::getHeight() const
{
    return this->height;
}

size_t BMP::getWidth() const
{
    return this->width;
}
