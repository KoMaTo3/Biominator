#pragma once

#include "memory.h"
#include "tools.h"

namespace Engine {

enum ImageType {
  IMAGE_TYPE_UNKNOWN = 0,
  IMAGE_TYPE_BMP     = 1,
  IMAGE_TYPE_TGA     = 2,
  IMAGE_TYPE_PNG     = 3,
  IMAGE_TYPE_JPG     = 4,
  IMAGE_TYPE_DXT1    = 5,
  IMAGE_TYPE_DXT3    = 6,
  IMAGE_TYPE_DXT5    = 7,
};

class ImageLoader {
public:
  ImageLoader();
  virtual ~ImageLoader();
  bool Load( const unsigned char *data, const size_t dataLength );
  bool LoadBMP( const unsigned char *data, const size_t dataLength );
  bool LoadTGA( const unsigned char *data, const size_t dataLength );
  bool LoadPNG( const unsigned char *data, const size_t dataLength );
  bool LoadJPG( const unsigned char *data, const size_t dataLength );
  bool LoadDXT1( const unsigned char *data, const size_t dataLength );
  bool LoadDXT3( const unsigned char *data, const size_t dataLength );
  bool LoadDXT5( const unsigned char *data, const size_t dataLength );
  ImageType GetImageType( const unsigned char *data, const size_t dataLength ) const;
  inline bool IsCompressed() const { return this->isCompressed; }

  Memory imageDataRGBA;
  ImageType imageType;
  size_t imageWidth;
  size_t imageHeight;
  bool isTransparent;
  bool isCompressed;

private:
  ImageLoader( ImageLoader& );
  ImageLoader& operator=( ImageLoader& );
};

#pragma pack(2)

struct ImageType_BMP_FileHeader {
  uint16_t        bfType;
  uint32_t        bfSize;
  uint16_t        bfReserved1;
  uint16_t        bfReserved2;
  uint32_t        bfOffBits;
};

struct ImageType_BMP_InfoHeader {
  uint32_t        biSize;
  int32_t         biWidth;
  int32_t         biHeight;
  uint16_t        biPlanes;
  uint16_t        biBitCount;
  uint32_t        biCompression;
  uint32_t        biSizeImage;
  int32_t         biXPelsPerMeter;
  int32_t         biYPelsPerMeter;
  uint32_t        biClrUsed;
  uint32_t        biClrImportant;
};

#pragma pack()

};
