#pragma once

#include "memory.h"

namespace Engine {

enum ImageType {
  IMAGE_TYPE_UNKNOWN = 0,
  IMAGE_TYPE_BMP     = 1,
  IMAGE_TYPE_TGA     = 2,
  IMAGE_TYPE_PNG     = 3,
  IMAGE_TYPE_JPG     = 4,
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
  ImageType GetImageType( const unsigned char *data, const size_t dataLength );

  Memory imageDataRGBA;
  ImageType imageType;
  size_t imageWidth;
  size_t imageHeight;

private:
  ImageLoader( ImageLoader& );
  ImageLoader& operator=( ImageLoader& );
};

#pragma pack(2)

struct ImageType_BMP_FileHeader {
  unsigned short  bfType;
  unsigned long   bfSize;
  unsigned short  bfReserved1;
  unsigned short  bfReserved2;
  unsigned long   bfOffBits;
};

struct ImageType_BMP_InfoHeader {
  unsigned long   biSize;
  long            biWidth;
  long            biHeight;
  unsigned short  biPlanes;
  unsigned short  biBitCount;
  unsigned long   biCompression;
  unsigned long   biSizeImage;
  long            biXPelsPerMeter;
  long            biYPelsPerMeter;
  unsigned long   biClrUsed;
  unsigned long   biClrImportant;
};

#pragma pack()

};
