#include "imageloader.h"
#include "tools.h"
#include "string.h"

using namespace Engine;

ImageLoader::ImageLoader()
: imageType( IMAGE_TYPE_UNKNOWN ), imageWidth( 0 ), imageHeight( 0 ), isTransparent( false ), isCompressed( false ) {
}

ImageLoader::~ImageLoader() {
}

bool ImageLoader::Load( const unsigned char *data, const size_t dataLength ) {
  ImageType format = this->GetImageType( data, dataLength );

  switch( format ) {
  case IMAGE_TYPE_BMP:
    LOGI( "ImageLoader::Load => BMP" );
    return this->LoadBMP( data, dataLength );
  case IMAGE_TYPE_TGA: 
    LOGI( "ImageLoader::Load => TGA" );
    return this->LoadTGA( data, dataLength );
  case IMAGE_TYPE_PNG: 
    LOGI( "ImageLoader::Load => PNG" );
    return this->LoadPNG( data, dataLength );
  case IMAGE_TYPE_JPG: 
    LOGI( "ImageLoader::Load => JPG" );
    return this->LoadJPG( data, dataLength );
  case IMAGE_TYPE_DXT1: 
    LOGW( "ImageLoader::Load => DXT1" );
    return this->LoadDXT1( data, dataLength );
  case IMAGE_TYPE_DXT3: 
    LOGW( "ImageLoader::Load => DXT3" );
    return this->LoadDXT3( data, dataLength );
  case IMAGE_TYPE_DXT5: 
    LOGW( "ImageLoader::Load => DXT5" );
    return this->LoadDXT5( data, dataLength );
  default:;
  }
  LOGE( "ImageLoader::Load => unknown format, data length[%d]", dataLength );

  return false;
}//Load

ImageType ImageLoader::GetImageType( const unsigned char *data, const size_t dataLength ) const {
  if( !data || !dataLength ) {
    return IMAGE_TYPE_UNKNOWN;
  }

  if( dataLength >= 2 ) {
    unsigned short *sign = ( unsigned short* ) data;
    if( *sign == 0x4D42 ) { //BM
      return IMAGE_TYPE_BMP;
    }
  }
  if( dataLength >= 3 ) {
    unsigned short *sign = ( unsigned short* ) data;
    if( *sign == 0 && (data[2] == 0x02 || data[2] == 0x0A) ) {
      return IMAGE_TYPE_TGA;
    }
  }
  if( dataLength >= 4 ) {
    uint32_t *sign = ( uint32_t* ) data;
    if( *sign == 0x474E5089 ) { //0x89 PNG
      return IMAGE_TYPE_PNG;
    }

    if( *sign == 0x31747864 ) { //DXT1
      return IMAGE_TYPE_DXT1;
    }

    if( *sign == 0x33747864 ) { //DXT3
      return IMAGE_TYPE_DXT3;
    }

    if( *sign == 0x35747864 ) { //DXT5
      return IMAGE_TYPE_DXT5;
    }
  }
  if( dataLength >= 10 ) {
    uint32_t *sign = ( uint32_t* ) ( data + 6 );
    if( *sign == 0x4649464A ) { //JFIF
      return IMAGE_TYPE_JPG;
    }
  }
  LOGE( "ImageLoader::GetImageType => unknown format" );

  return IMAGE_TYPE_UNKNOWN;
}//GetImageType

bool ImageLoader::LoadBMP( const unsigned char *data, const size_t dataLength ) {
  this->imageDataRGBA.Free();
  this->imageWidth = 0;
  this->imageHeight = 0;
  this->imageType = IMAGE_TYPE_BMP;
  this->isCompressed = false;

  ImageType_BMP_FileHeader *fileHeader;
  ImageType_BMP_InfoHeader *infoHeader;
  size_t x, y;
  size_t srcPos, destPos;

  if( dataLength < sizeof( ImageType_BMP_FileHeader ) + sizeof( ImageType_BMP_InfoHeader ) ) {
    LOGE( "ImageLoader::LoadBMP => data too short" );
    return false;
  }

  fileHeader = (ImageType_BMP_FileHeader*) data;
  infoHeader = (ImageType_BMP_InfoHeader*) ( data + sizeof( ImageType_BMP_FileHeader ) );
  LOGI( "sizeof(ImageType_BMP_FileHeader) = %d", ( int ) sizeof( ImageType_BMP_FileHeader ) );

  this->imageWidth = infoHeader->biWidth;
  this->imageHeight = infoHeader->biHeight;
  this->imageDataRGBA.Alloc( this->imageWidth * this->imageHeight * 4 );

  LOGI( "BMP: bpp[%d] size[%dx%d]", infoHeader->biBitCount, ( int ) this->imageWidth, ( int ) this->imageHeight );
  switch( infoHeader->biBitCount )
  {
    case 24: {
      this->isTransparent = false;
      for( y = 0; y < this->imageHeight; ++y ) {
        for( x = 0; x < this->imageWidth; ++x ) {
          destPos = ( this->imageHeight - y - 1 ) * this->imageWidth + x;
          srcPos  = fileHeader->bfOffBits + (y * this->imageWidth + x) * 3;
          ( ( uint32_t* ) this->imageDataRGBA.GetData() )[ destPos ] = COLOR_ARGB(
              0xFF,
              data[ srcPos + 0 ],
              data[ srcPos + 1 ],
              data[ srcPos + 2 ]
          );
        }//x
      }
    }
    break;
    case 32: {
      for( y = 0; y < this->imageHeight; ++y ) {
        for( x = 0; x < this->imageWidth; ++x ) {
          destPos = ( this->imageHeight - y - 1 ) * this->imageWidth + x;
          srcPos  = fileHeader->bfOffBits + (y * this->imageWidth + x) * 4;
          if( data[ srcPos + 4 ] != 0xFF ) {
            this->isTransparent = true;
          }
          ( ( uint32_t* ) this->imageDataRGBA.GetData() )[ destPos ] = COLOR_ARGB(
              data[ srcPos + 4 ], //255
              data[ srcPos + 0 ],
              data[ srcPos + 1 ],
              data[ srcPos + 2 ]
          );
        }
      }
    }
    break;
    default:
      LOGE( "BMP Failed: bpp %d", infoHeader->biBitCount );
      return false;
    break;
  }//switch

  return true;
}//LoadBMP

bool ImageLoader::LoadTGA( const unsigned char *data, const size_t dataLength ) {
  this->imageDataRGBA.Free();
  this->imageWidth = 0;
  this->imageHeight = 0;
  this->imageType = IMAGE_TYPE_TGA;
  this->isCompressed = false;

  if( !data || !dataLength ) {
    LOGE( "ImageLoader::LoadTGA => no data or too short" );
    return false;
  }

  unsigned char idLength = data[ 0 ];
  if( data[ 1 ] ) {
    LOGE( "ImageLoader::LoadTGA => idLength" );
    return false;
  }

  unsigned char compressed = data[ 2 ]; //RLE-compression
  if( !( compressed == 2 || compressed == 0x0A ) ) {
    LOGE( "ImageLoader::LoadTGA => unknown compression" );
    return false;
  }

  unsigned short
    *w = ( unsigned short* ) ( data + 12 ),
    *h = ( unsigned short* ) ( data + 14 );

  unsigned char bpp = data[ 16 ];
  if( !( bpp == 24 || bpp == 32 ) ) {
    LOGE( "ImageLoader::LoadTGA => unknown bpp[%d]", bpp );
    return false;
  }

  size_t src_pos = 18 + idLength;

  this->imageWidth = *w;
  this->imageHeight = *h;
  this->isTransparent = false;
  this->imageDataRGBA.Alloc( this->imageWidth * this->imageHeight * 4 );
  unsigned char *dest = this->imageDataRGBA.GetData();

  if( compressed == 2 ) { //not compressed
    size_t x, y;
    unsigned char mult = bpp >> 3;
    for( y = 0; y < this->imageHeight; y++) {
      for( x = 0; x < this->imageWidth; x++) {
        size_t dest_pos = ( x + y * this->imageWidth ) << 2;
        //dest_pos = ( x + ( this->imageHeight - y - 1 ) * this->imageWidth ) << 2;
        src_pos  = ( x + y * this->imageWidth ) * mult + 18;
        dest[ dest_pos     ] = data[ src_pos + 2 ];
        dest[ dest_pos + 1 ] = data[ src_pos + 1 ];
        dest[ dest_pos + 2 ] = data[ src_pos + 0 ];
        dest[ dest_pos + 3 ] = ( bpp == 32 ? data[ src_pos + 3 ] : 255);
        if( bpp == 32 && data[ src_pos + 3 ] != 0xFF ) {
          this->isTransparent = true;
        }
      }//x
    }//y
  } else { //RLE-compression 0x0A
    size_t src_pos = 18 + idLength, dest_pos = 0;
    unsigned char q;
    unsigned char r, g, b, a = 255;
    size_t x = 0, y = 0;
    while( y < this->imageHeight ) {
      unsigned char block = data[ src_pos++ ];
      unsigned char num = block & 127;

      if( block & 128 ) { //compressed block
        b = data[ src_pos ];
        g = data[ src_pos + 1 ];
        r = data[ src_pos + 2 ];
        if( bpp == 32 ) {
          a = data[ src_pos + 3 ];
          src_pos += 4;
        } else {
          src_pos += 3;
        }
        for( q = 0; q < num + 1; ++q ) {
          //dest_pos = ( x + y * this->imageWidth ) << 2;
          dest_pos = ( x + ( this->imageHeight - y - 1 ) * this->imageWidth ) << 2;
          dest[ dest_pos ] = r;
          dest[ dest_pos + 1 ] = g;
          dest[ dest_pos + 2 ] = b;
          dest[ dest_pos + 3 ] = a;
          if( a != 0xFF ) {
            this->isTransparent = true;
          }

          x = ( x + 1 ) % this->imageWidth;
          if( !x ) {
            ++y;
          }
        }
      } else { //not compressed block
        for( q = 0; q < num + 1; ++q ) {
          //dest_pos = ( x + y * this->imageWidth ) << 2;
          dest_pos = ( x + ( this->imageHeight - y - 1 ) * this->imageWidth ) << 2;
          b = data[ src_pos ];
          g = data[ src_pos + 1 ];
          r = data[ src_pos + 2 ];
          if( bpp == 32 ) {
            a = data[ src_pos + 3 ];
            src_pos += 4;
          } else {
            src_pos += 3;
          }

          dest[ dest_pos ] = r;
          dest[ dest_pos + 1 ] = g;
          dest[ dest_pos + 2 ] = b;
          dest[ dest_pos + 3 ] = a;
          if( a != 0xFF ) {
            this->isTransparent = true;
          }

          x = ( x + 1 ) % this->imageWidth;
          if( !x ) {
            ++y;
          }
        }//for q < num
      }//not compressed block
    }//while y < height
  }//RLE-compression

  return true;
}//LoadTGA

bool ImageLoader::LoadPNG( const unsigned char *data, const size_t dataLength ) {
  this->imageDataRGBA.Free();
  this->imageWidth = 0;
  this->imageHeight = 0;
  this->imageType = IMAGE_TYPE_PNG;
  this->isCompressed = false;

  return false;
}//LoadPNG

bool ImageLoader::LoadJPG( const unsigned char *data, const size_t dataLength ) {
  this->imageDataRGBA.Free();
  this->imageWidth = 0;
  this->imageHeight = 0;
  this->imageType = IMAGE_TYPE_JPG;
  this->isCompressed = false;

  return false;
}//LoadJPG

bool ImageLoader::LoadDXT1( const unsigned char *data, const size_t dataLength ) {
  uint32_t
    *width,
    *height;
  this->imageDataRGBA.Free();
  const size_t FOURCCSIZE = 4;
  const size_t MINIMAL_DXT1_FILE_SIZE = FOURCCSIZE + sizeof( *width ) * 2 + 8;
  this->imageType = IMAGE_TYPE_DXT1;
  this->isCompressed = true;
  if( dataLength < MINIMAL_DXT1_FILE_SIZE ) {
    LOGE( "ImageLoader::LoadDXT1 => dataLength[%d] minimal[%d]", dataLength, MINIMAL_DXT1_FILE_SIZE );
    return false;
  }

  width   = ( uint32_t* ) ( data + FOURCCSIZE );
  height  = ( uint32_t* ) ( data + FOURCCSIZE + sizeof( *width ) );
  this->imageWidth = *width;
  this->imageHeight = *height;

  size_t dataSize = ( this->imageWidth * this->imageHeight ) >> 1;
  this->imageDataRGBA.Alloc( dataSize );
  memcpy( this->imageDataRGBA.GetData(), data + FOURCCSIZE + sizeof( *width ) * 2, dataSize );

  return true;
}//LoadDXT1

bool ImageLoader::LoadDXT3( const unsigned char *data, const size_t dataLength ) {
  uint32_t
    *width,
    *height;
  this->imageDataRGBA.Free();
  const size_t FOURCCSIZE = 4;
  const size_t MINIMAL_DXT3_FILE_SIZE = FOURCCSIZE + sizeof( *width ) * 2 + 16;
  this->imageType = IMAGE_TYPE_DXT3;
  this->isCompressed = true;
  if( dataLength < MINIMAL_DXT3_FILE_SIZE ) {
    LOGE( "ImageLoader::LoadDXT3 => dataLength[%d] minimal[%d]", dataLength, MINIMAL_DXT3_FILE_SIZE );
    return false;
  }

  width   = ( uint32_t* ) ( data + FOURCCSIZE );
  height  = ( uint32_t* ) ( data + FOURCCSIZE + sizeof( *width ) );
  this->imageWidth = *width;
  this->imageHeight = *height;

  size_t dataSize = this->imageWidth * this->imageHeight;
  this->imageDataRGBA.Alloc( dataSize );
  memcpy( this->imageDataRGBA.GetData(), data + FOURCCSIZE + sizeof( *width ) * 2, dataSize );

  return true;
}//LoadDXT3

bool ImageLoader::LoadDXT5( const unsigned char *data, const size_t dataLength ) {
  uint32_t
    *width,
    *height;
  this->imageDataRGBA.Free();
  const size_t FOURCCSIZE = 4;
  const size_t MINIMAL_DXT5_FILE_SIZE = FOURCCSIZE + sizeof( *width ) * 2 + 16;
  this->imageType = IMAGE_TYPE_DXT5;
  this->isCompressed = true;
  if( dataLength < MINIMAL_DXT5_FILE_SIZE ) {
    LOGE( "ImageLoader::LoadDXT5 => dataLength[%d] minimal[%d]", dataLength, MINIMAL_DXT5_FILE_SIZE );
    return false;
  }

  width   = ( uint32_t* ) ( data + FOURCCSIZE );
  height  = ( uint32_t* ) ( data + FOURCCSIZE + sizeof( *width ) );
  this->imageWidth = *width;
  this->imageHeight = *height;

  size_t dataSize = this->imageWidth * this->imageHeight;
  this->imageDataRGBA.Alloc( dataSize );
  memcpy( this->imageDataRGBA.GetData(), data + FOURCCSIZE + sizeof( *width ) * 2, dataSize );

  return true;
}//LoadDXT5
