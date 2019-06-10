#pragma once
//============================================================\\
//                         AVLib++                            \\
//============================================================\\
//           .----.                      .----.               \\
//          /      '.                  .'      \              \\
//       .------.    '.              .'    .-----.            \\
//      /        '-.   \            /   .-'        \          \\
//     |            '.  \          /  .'            |         \\
//      \             \  |        |  /             /          \\
//       '.            \ |   __   | /            .'           \\
//         '.           \|_-"__"-_|/           .'             \\
//           '.        ./ .\/ .\/ .\.        .'               \\
//             '-.    / \__/\__/\__/ \    .-'                 \\
//                '--|  / .\/ .\/ .\  |--'                    \\
//                   |  \__/\__/\__/  |                       \\
//                    \ / .\/ .\/ .\ /                        \\
//                   .-`\__/\__/\__/'-.                       \\
//                  /     `-....-'     \                      \\
//                 _\                  /_                     \\
//                __  __         _ Lider VIII                 \\
//               |  \/  |_  _ __| |_  __ _ ®                  \\
//               | |\/| | || / _| ' \/ _` |                   \\
//               |_|  |_|\_,_\__|_||_\__,_|                   \\
//                                                            \\
// "System rejrestacji i przetwarzania obrazu przestrzennego" \\
//   Project funded by The National Centre for Research and   \\
//           Development in the LIDER Programme               \\
//            (LIDER/34/0177/L-8/16/NCBR/2017)                \\
//============================================================\\

#include "xCommon.h"
#include "xByteBuffer.h"
#include "xPicture.h"

namespace AVlib {

//=============================================================================================================================================================================

class xBMP
{
public:
  class xBitmapFileHeader
  {
  public:
    static const uint32 c_HeaderLength = 12;

  protected:
    uint16  m_Type;
    uint32  m_FileSize;
    uint16  m_Reserved1;
    uint16  m_Reserved2;
    uint32  m_Offset;

  public:
    int32   Parse (xByteBuffer* Input ) { Absorb(Input); Input->modifyRead(-(int32)c_HeaderLength); return c_HeaderLength; }
    int32   Absorb(xByteBuffer* Input ); 
    int32   Format(xByteBuffer* Output) { Emit(Output); Output->modifyWritten(-(int32)c_HeaderLength); return c_HeaderLength; }
    int32   Emit  (xByteBuffer* Output); 

  public:
    void    setType           (uint16 Type           ){ m_Type = Type; }
    uint16  getType           (                      ){ return m_Type; }
    void    setFileSize       (uint32 FileSize       ){ m_FileSize = FileSize; }
    uint32  getFileSize       (                      ){ return m_FileSize; }
    void    setReserved1      (uint16 Reserved1      ){ m_Reserved1 = Reserved1; }
    uint16  getReserved1      (                      ){ return m_Reserved1; }
    void    setReserved2      (uint16 Reserved2      ){ m_Reserved2 = Reserved2; }
    uint16  getReserved2      (                      ){ return m_Reserved2; }
    void    setOffset         (uint32 Offset         ){ m_Offset = Offset; }
    uint32  getOffset         (                      ){ return m_Offset; }
  };

  class xBitmapInfoHeader
  {
  public:
    static const uint32 c_HeaderLength = 40;

  protected:
    uint32  m_HeaderSize;
    int32   m_Width;
    int32   m_Height;
    uint16  m_Planes;
    uint16  m_BitsPerPixel;
    uint32  m_Compression;
    uint32  m_ImageSize;
    int32   m_PelsPerMeterX;
    int32   m_PelsPerMeterY;
    uint32  m_ColorsUsed;
    uint32  m_ColorsImportant;

  public:
    int32   Parse (xByteBuffer* Input ) { Absorb(Input); Input->modifyRead(-(int32)c_HeaderLength); return c_HeaderLength; }
    int32   Absorb(xByteBuffer* Input ); 
    int32   Format(xByteBuffer* Output) { Emit(Output); Output->modifyWritten(-(int32)c_HeaderLength); return c_HeaderLength; }
    int32   Emit  (xByteBuffer* Output); 

  public:
    void    setHeaderSize     (uint32 HeaderSize     ){ m_HeaderSize = HeaderSize; }
    uint32  getHeaderSize     (                      ){ return m_HeaderSize; }
    void    setWidth          (int32  Width          ){ m_Width = Width; }
    int32   getWidth          (                      ){ return m_Width; }
    void    setHeight         (int32  Height         ){ m_Height = Height; }
    int32   getHeight         (                      ){ return m_Height; }
    void    setPlanes         (uint16 Planes         ){ m_Planes = Planes; }
    uint16  getPlanes         (                      ){ return m_Planes; }
    void    setBitsPerPixel   (uint16 BitsPerPixel   ){ m_BitsPerPixel = BitsPerPixel; }
    uint16  getBitsPerPixel   (                      ){ return m_BitsPerPixel; }
    void    setCompression    (uint32 Compression    ){ m_Compression = Compression; }
    uint32  getCompression    (                      ){ return m_Compression; }
    void    setImageSize      (uint32 ImageSize      ){ m_ImageSize = ImageSize; }
    uint32  getImageSize      (                      ){ return m_ImageSize; }
    void    setPelsPerMeterX  (int32  PelsPerMeterX  ){ m_PelsPerMeterX = PelsPerMeterX; }
    int32   getPelsPerMeterX  (                      ){ return m_PelsPerMeterX; }
    void    setPelsPerMeterY  (int32  PelsPerMeterY  ){ m_PelsPerMeterY = PelsPerMeterY; }
    int32   getPelsPerMeterY  (                      ){ return m_PelsPerMeterY; }
    void    setColorsUsed     (uint32 ColorsUsed     ){ m_ColorsUsed = ColorsUsed; }
    uint32  getColorsUsed     (                      ){ return m_ColorsUsed; }
    void    setColorsImportant(uint32 ColorsImportant){ m_ColorsImportant = ColorsImportant; }
    uint32  getColorsImportant(                      ){ return m_ColorsImportant; }
  };

public:
  template <typename PelType> static bool readFromFile(xPic<PelType>* DstPic, xFile* SrcFile);
  template <typename PelType> static bool writeToFile (xFile* DstFile, xPic<PelType>* SrcPic);
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename PelType> bool xBMP::readFromFile(xPic<PelType>* DstPic, xFile* SrcFile)
{
  xByteBuffer Buffer(128);

  xBitmapFileHeader BitmapFileHeader;
  Buffer.readFromFile(SrcFile, xBitmapFileHeader::c_HeaderLength);
  int32 FileHeadeReaded = BitmapFileHeader.Absorb(&Buffer);
  if(FileHeadeReaded != xBitmapFileHeader::c_HeaderLength || BitmapFileHeader.getType() != 0x4d42 || BitmapFileHeader.getOffset() < xBitmapFileHeader::c_HeaderLength + xBitmapInfoHeader::c_HeaderLength) { return false; }

  xBitmapInfoHeader BitmapInfoHeader;
  Buffer.readFromFile(SrcFile, xBitmapInfoHeader::c_HeaderLength);
  int32 InfoHeadeReaded = BitmapInfoHeader.Absorb(&Buffer);
  if(InfoHeadeReaded != xBitmapFileHeader::c_HeaderLength) { return false; }

  int32 v             = BitmapInfoHeader.getWidth();
  int32 h             = BitmapInfoHeader.getHeight();
  int32 Lev           = BitmapInfoHeader.getBitsPerPixel();
  int32 AbsV          = xAbs(v);
  int32 AbsH          = xAbs(h);
  int32 NumComponents = Lev==32 ? 4 : 3;
  eImgTp ImageType    = NumComponents == 3 ? eImgTp::RGB : eImgTp::RGBA;

  uint8* LineBuff = (uint8*)calloc(v*4, sizeof(uint8)); //line buffer
  uint8* PaletteBuff = nullptr;

  if(!DstPic->isCompatible(AbsV, AbsH, 8, ImageType, eCrF::CrF_UNKNOWN))
  {
    DstPic->destroy();
    DstPic->create(AbsV, AbsH, 0, 8, ImageType, eCrF::CrF_UNKNOWN);
  }

  //Load Paletee
  if((Lev==1)||(Lev==4)||(Lev==8))
  {
    int32 NumOfColors = 1<<Lev;
    PaletteBuff = (uint8*)xMalloc(4*NumOfColors*sizeof(uint8));
    SrcFile->read((void*)PaletteBuff, 4*NumOfColors*sizeof(uint8));
  }

  //Seek to the image beginning
  SrcFile->seek((uint64) BitmapFileHeader.getOffset(), xFile::seek_mode::beg);

  //Read image
  switch(Lev)
  {
    case 1: //Paletee 1bit bitmap 
    case 4: //Paletee 4bit bitmap
    case 8: //Paletee 8bit bitmap
      xAssert(0); break;

    case 24: //RGB 24bit bitmap
    {      
      int32 ls = (v*3+3)&0xFFFFFFFC; //4byte alignment
      if(h>0)
      {
        for(int32 j=(h-1); j>=0; j--)
        {
          SrcFile->read(LineBuff, ls);
          for(int32 i=0,l=0; i<v; i++,l+=3) 
          {
            DstPic->accessPel({ i, j }, (eCmp)2) = LineBuff[l+0];
            DstPic->accessPel({ i, j }, (eCmp)1) = LineBuff[l+1];
            DstPic->accessPel({ i, j }, (eCmp)0) = LineBuff[l+2];
          }
        }
      }
      else
      {
        for(int32 j=0; j<AbsH; j++)
        {
          SrcFile->read(LineBuff, ls);
          for(int32 i=0,l=0; i<v; i++,l+=3) 
          {
            DstPic->accessPel({ i, j }, (eCmp)2) = LineBuff[l+0];
            DstPic->accessPel({ i, j }, (eCmp)1) = LineBuff[l+1];
            DstPic->accessPel({ i, j }, (eCmp)0) = LineBuff[l+2];
          }
        }
      }
      break;
    }

    case 32: //RGBA 32bit bitmap 
    {
      if(h>0)
      {
        for(int32 j=(h-1); j>=0; j--)
        {
          SrcFile->read(LineBuff, v*4);
          for(int32 i=0,l=0; i<v; i++,l+=4) 
          {
            DstPic->accessPel({ i, j }, (eCmp)2) = LineBuff[l+0]; //B
            DstPic->accessPel({ i, j }, (eCmp)1) = LineBuff[l+1]; //G
            DstPic->accessPel({ i, j }, (eCmp)0) = LineBuff[l+2]; //R
            DstPic->accessPel({ i, j }, (eCmp)3) = LineBuff[l+3]; //A
          }
        }
      }
      else
      {
        for(int32 j=0; j<AbsH; j++)
        {
          SrcFile->read(LineBuff, v*4);
          for(int32 i=0,l=0; i<v; i++,l+=4) 
          {
            DstPic->accessPel({ i, j }, (eCmp)2) = LineBuff[l+0]; //B
            DstPic->accessPel({ i, j }, (eCmp)1) = LineBuff[l+1]; //G
            DstPic->accessPel({ i, j }, (eCmp)0) = LineBuff[l+2]; //R
            DstPic->accessPel({ i, j }, (eCmp)3) = LineBuff[l+3]; //A
          }
        }
      }
      break;
    }

    default:
      xAssert(0);
      break;
  }

  free(LineBuff); 
  if(PaletteBuff) xFree(PaletteBuff);
  return true;
}
template <typename PelType> bool xBMP::writeToFile(xFile* DstFile, xPic<PelType>* SrcPic)
{
  xByteBuffer Buffer(256);

  int32 h = SrcPic->getHeight();
  int32 v = SrcPic->getWidth ();

  int32 NumComponents = SrcPic->getImageType() == eImgTp::RGBA ? 4 : 3;
  int32 LineSize      = NumComponents==4 ? v*4 : (v*3+3)&0xFFFFFFFC;

  xBitmapFileHeader BitmapFileHeader;
  BitmapFileHeader.setType     (0x4d42);
  BitmapFileHeader.setFileSize (xBitmapFileHeader::c_HeaderLength + xBitmapInfoHeader::c_HeaderLength + LineSize*h);
  BitmapFileHeader.setReserved1(0);
  BitmapFileHeader.setReserved2(0);
  BitmapFileHeader.setOffset   (sizeof(xBitmapFileHeader)+sizeof(xBitmapInfoHeader));
  BitmapFileHeader.Emit(&Buffer);

  xBitmapInfoHeader BitmapInfoHeader;
  BitmapInfoHeader.setHeaderSize     (xBitmapInfoHeader::c_HeaderLength);
  BitmapInfoHeader.setWidth          (v);
  BitmapInfoHeader.setHeight         (h);
  BitmapInfoHeader.setPlanes         (1);
  BitmapInfoHeader.setBitsPerPixel   (8*NumComponents);
  BitmapInfoHeader.setCompression    (0); //RGB
  BitmapInfoHeader.setImageSize      (0);
  BitmapInfoHeader.setPelsPerMeterX  (0);
  BitmapInfoHeader.setPelsPerMeterY  (0);
  BitmapInfoHeader.setColorsUsed     (0);
  BitmapInfoHeader.setColorsImportant(0);  
  BitmapInfoHeader.Emit(&Buffer);

  Buffer.writeToFile(DstFile);

  uint8* LineBuff = (uint8*)xMalloc(LineSize * sizeof(uint8));

  if(NumComponents==3)
  {
    for(int32 j=(h-1); j>=0; j--)
    {
      for(int32 i=0,l=0; i<v; i++,l+=3)
      {
        LineBuff[l + 0] = (uint8)SrcPic->accessPel({ i, j }, (eCmp)2);
        LineBuff[l + 1] = (uint8)SrcPic->accessPel({ i, j }, (eCmp)1);
        LineBuff[l + 2] = (uint8)SrcPic->accessPel({ i, j }, (eCmp)0);
      }
      DstFile->write(LineBuff, LineSize);
    }
  }
  else
  {
    for(int32 j=(h-1); j>=0; j--)
    {
      for(int32 i=0,l=0; i<v; i++,l+=4)
      {
        LineBuff[l+0] = (uint8)SrcPic->accessPel({ i, j }, (eCmp)2);
        LineBuff[l+1] = (uint8)SrcPic->accessPel({ i, j }, (eCmp)1);
        LineBuff[l+2] = (uint8)SrcPic->accessPel({ i, j }, (eCmp)0);
        LineBuff[l+3] = (uint8)SrcPic->accessPel({ i, j }, (eCmp)3);
      }
      DstFile->write(LineBuff, LineSize);
    }
  }

  xFree(LineBuff);
  return true;
}

//=============================================================================================================================================================================

} //end of namespace AVLib
