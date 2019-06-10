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
#include "xFile.h"
#include "xPlane.h"
#include "xPicture.h"
#include <mutex>
#include <string>
#include <vector>

namespace AVlib {

//=============================================================================================================================================================================

class xPackedTank;

//=============================================================================================================================================================================

class xPacked
{
public:
  typedef xPackedTank tCorespondingTank;

protected:
  int16       m_Width   [4]; //component width
  int16       m_Height  [4]; //component height
  int16       m_Stride  [4]; //component stride (to same position in next line)
  int8        m_Pitch   [4]; //component pitch (to next pixel in line)
  int8        m_BitDepth[4]; //component bit depth
  int32       m_NumComponents;
  int32       m_PackedSize;

  byte*       m_Buffer;
  int32       m_OriginOffset[4]; //offset to first pixel of component
  
  std::string m_FourCC;
  eImgTp      m_ImageType;
  ePckTp      m_PackedType;
  eCrF        m_ChromaFormat;  
  eCmpO       m_ComponentOrder;
  
  int64       m_POC;
  int64       m_Timestamp;

public:
  //Base functions
  xPacked                         ();
  void        create              (int32 Width, int32 Height, std::string FourCC);
  void        create              (int32 Width, int32 Height, int32 BitDepth, eImgTp ImageType, ePckTp PackedType, eCrF ChromaFormat, eCmpO ComponentOrder);
  void        destroy             ();
  void        clear               ();
  void        copy                (xPacked* Src); //perform copy operation
  
public:
  //Tool functions
  bool        isSameSize       (int32 Width, int32 Height) { return (m_Width[0]==Width && m_Height[0]==Height); }
  bool        isSameBitDepth   (int32 BitDepth) { return (m_BitDepth[0] == BitDepth); }
  bool        isSameTypeCF     (eImgTp ImageType, eCrF ChromaFormat) { return (m_ImageType==ImageType && m_ChromaFormat==ChromaFormat); }
  bool        isSamePckType    (ePckTp PackedType) { return (m_PackedType == PackedType); }
  bool        isSameCmpOrder   (eCmpO ComponentOrder) { return (m_ComponentOrder == ComponentOrder); }
  bool        isCompatible     (int32 Width, int32 Height, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat) { return (isSameSize(Width, Height) && isSameBitDepth(BitDepth) && isSameTypeCF(ImageType, ChromaFormat)); }
  bool        isCompatible     (int32 Width, int32 Height, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat, eClrSpc ColorSpace) { return (isSameSize(Width, Height) && isSameBitDepth(BitDepth) && isSameTypeCF(ImageType, ChromaFormat)); } 
  bool        isCompatible     (int32 Width, int32 Height, int32 BitDepth, eImgTp ImageType, ePckTp PackedType, eCrF ChromaFormat, eCmpO ComponentOrder) { return (isSameSize(Width, Height) && isSameBitDepth(BitDepth) && isSameTypeCF(ImageType, ChromaFormat) && isSamePckType(PackedType) && isSameCmpOrder(ComponentOrder)); } 
           
  bool        isSameSize       (xPacked* Packed) { xAssert(Packed!=nullptr); return isSameSize(Packed->m_Width[0], Packed->m_Height[0]); }
  bool        isSameBitDepth   (xPacked* Packed) { xAssert(Packed!=nullptr); return isSameBitDepth(Packed->m_BitDepth[0]); }
  bool        isSameTypeCF     (xPacked* Packed) { xAssert(Packed!=nullptr); return isSameTypeCF(Packed->m_ImageType, Packed->m_ChromaFormat); }
  bool        isSamePckType    (xPacked* Packed) { xAssert(Packed!=nullptr); return isSamePckType(Packed->m_PackedType); }
  bool        isSameCmpOrder   (xPacked* Packed) { xAssert(Packed!=nullptr); return isSameCmpOrder(Packed->m_ComponentOrder); }
  bool        isCompatible     (xPacked* Packed) { xAssert(Packed!=nullptr); return isCompatible(Packed->m_Width[0], Packed->m_Height[0], Packed->m_BitDepth[0], Packed->m_ImageType, Packed->m_PackedType, Packed->m_ChromaFormat, Packed->m_ComponentOrder); }

public:
  //Buffer operations
  bool        bindPackedBuffer (byte* Buffer)    { xAssert(Buffer!=nullptr); if(m_Buffer) { return false; } else { m_Buffer = Buffer; return true; } }
  byte*       unbindPackedBuffer()              { if(m_Buffer) { byte* Tmp = m_Buffer; m_Buffer = nullptr; return Tmp; } else { return nullptr; } }
  bool        swapPackedBuffer (byte*& Buffer)   { xAssert(Buffer!=nullptr); if(m_Buffer) { std::swap(m_Buffer, Buffer); return true; } else { return false; } }
  bool        swapPackedBuffer (xPacked* Packed) { xAssert(Packed!=nullptr); return swapPackedBuffer(Packed->m_Buffer); }

  //Pack/Unpack
  template <typename PelType> void pack             (xPic<PelType>* Src);
  template <typename PelType> void unpack           (xPic<PelType>* Dst);
  template <typename PelType> void packPlanar       (xPic<PelType>* Src);
  template <typename PelType> void unpackPlanar     (xPic<PelType>* Dst);
  template <typename PelType> void packSemiPlanar   (xPic<PelType>* Src) { xAssert(0); }
  template <typename PelType> void unpackSemiPlanar (xPic<PelType>* Dst) { xAssert(0); }
  template <typename PelType> void packInterleaved  (xPic<PelType>* Src); 
  template <typename PelType> void unpackInterleaved(xPic<PelType>* Dst);

  template <typename PelType> void pack             (xPlane<PelType>* Src) { xAssert(m_PackedType == ePckTp::Planar); packPlanar  (Src); }
  template <typename PelType> void unpack           (xPlane<PelType>* Dst) { xAssert(m_PackedType == ePckTp::Planar); unpackPlanar(Dst); }
  template <typename PelType> void packPlanar       (xPlane<PelType>* Src);
  template <typename PelType> void unpackPlanar     (xPlane<PelType>* Dst);

public:
  //convert
  void convert(xPacked* Src);

  //covnert packed type
  void convertPackedTypeYUV               (xPacked* Src);
  void convertPlanarYUV2SemiPlanarYUV     (xPacked* Src);
  void convertPlanarYUV2InterleavedYUV    (xPacked* Src);
  void convertSemiPlanarYUV2PlanarYUV     (xPacked* Src);
  void convertSemiPlanarYUV2InterleavedYUV(xPacked* Src);
  void convertInterleavedYUV2PlanarYUV    (xPacked* Src);
  void convertInterleavedYUV2SemiPlanarYUV(xPacked* Src);

  //Size calculation
  static int32 calkPlaneSize   (int32 Width, int32 Height, int32 BitDepth);
  static int32 calkPackedSize  (int32 Width, int32 Height, std::string FourCC);
  static int32 calkPackedSize  (int32 Width, int32 Height, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat);

  //Determine parameters or FourCC
  static std::string determineFourCC(int32  BitDepth, eImgTp  ImageType, ePckTp  PackedType, eCrF  ChromaFormat, eCmpO  ComponentOrder);
  static bool        determineParams(int32& BitDepth, eImgTp& ImageType, ePckTp& PackedType, eCrF& ChromaFormat, eCmpO& ComponentOrder, std::string FourCC);

  //Read from and write to file      
  void        readFromFile(xFile* File);
  void        writeToFile (xFile* File);

  //Interfaces
  int32       getWidth         () { return m_Width[0]; }
  int32       getHeight        () { return m_Height[0]; }
  int32       getBitDepth      () { return m_BitDepth[0]; }
  int32       getPackedSize    () { return m_PackedSize; }
  byte*       getPackedBuffer  () { return m_Buffer; }
  std::string getFourCC        () { return m_FourCC; }
  eImgTp      getImageType     () { return m_ImageType; }
  ePckTp      getPackedType    () { return m_PackedType; }
  eCrF        getChromaFormat  () { return m_ChromaFormat; }
  eCmpO       getComponentOrder() { return m_ComponentOrder; }
  void        setPOC           (int64 POC) { m_POC = POC; }
  int64       getPOC           () { return m_POC; }
  void        setTimestamp     (int64 Timestamp) { m_Timestamp = Timestamp; }
  int64       getTimestamp     (               ) { return m_Timestamp;      }


  //Planes access
  byte*    getAddr          (eCmp CmpId) { return m_Buffer + m_OriginOffset[CmpId];  }
  byte*    getAddrLm        (          ) { return m_Buffer + m_OriginOffset[CMP_LM]; }
  byte*    getAddrCb        (          ) { return m_Buffer + m_OriginOffset[CMP_CB]; }
  byte*    getAddrCr        (          ) { return m_Buffer + m_OriginOffset[CMP_CR]; }

  int32    getWidth         (eCmp CmpId) { return m_Width [CmpId ];  }
  int32    getWidthLm       (          ) { return m_Width [CMP_LM];  }
  int32    getWidthCb       (          ) { return m_Width [CMP_CB];  }
  int32    getWidthCr       (          ) { return m_Width [CMP_CR];  }

  int32    getHeight        (eCmp CmpId) { return m_Height[CmpId ]; }
  int32    getHeightLm      (          ) { return m_Height[CMP_LM]; }
  int32    getHeightCb      (          ) { return m_Height[CMP_CB]; }
  int32    getHeightCr      (          ) { return m_Height[CMP_CR]; }

  int32    getArea          (eCmp CmpId) { return m_Width [CmpId ] * m_Height[CmpId ]; }
  int32    getAreaLm        (          ) { return m_Width [CMP_LM] * m_Height[CMP_LM]; }
  int32    getAreaCb        (          ) { return m_Width [CMP_CB] * m_Height[CMP_CB]; }
  int32    getAreaCr        (          ) { return m_Width [CMP_CR] * m_Height[CMP_CR]; }

  int32    getStride        (eCmp CmpId) { return m_Stride[CmpId ]; }
  int32    getStrideLm      (          ) { return m_Stride[CMP_LM]; }
  int32    getStrideCb      (          ) { return m_Stride[CMP_CB]; }
  int32    getStrideCr      (          ) { return m_Stride[CMP_CR]; }
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename PelType> void xPacked::pack(xPic<PelType>* Src)
{ 
  xAssert(Src->isCompatible(m_Width[0], m_Height[0], m_BitDepth[0], m_ImageType, m_ChromaFormat)); 

  switch(m_PackedType)
  {
    case ePckTp::Planar:      packPlanar     (Src); break;
    case ePckTp::SemiPlanar:  packSemiPlanar (Src); break;
    case ePckTp::Interleaved: packInterleaved(Src); break;
    default: xAssert(0); break;
  }
}
template <typename PelType> void xPacked::unpack(xPic<PelType>* Dst)
{ 
  xAssert(Dst->isCompatible(m_Width[0], m_Height[0], m_BitDepth[0], m_ImageType, m_ChromaFormat));

  switch(m_PackedType)
  {
    case ePckTp::Planar:      unpackPlanar     (Dst); break;
    case ePckTp::SemiPlanar:  unpackSemiPlanar (Dst); break;
    case ePckTp::Interleaved: unpackInterleaved(Dst); break;
    default: xAssert(0); break;
  }
}
template <typename PelType> void xPacked::packPlanar(xPic<PelType>* Src)
{
  xAssert(Src->isCompatible(m_Width[0], m_Height[0], m_BitDepth[0], m_ImageType, m_ChromaFormat));

  byte*  DstPtr = m_Buffer;

  int32 NumComponents = Src->getNumComponents();
  for(int32 CmpIdx=0; CmpIdx<NumComponents; CmpIdx++)
  {    
    PelType* SrcPtr    = Src->getAddr  ((eCmp)CmpIdx);
    int32    SrcStride = Src->getStride((eCmp)CmpIdx);
    int32    Width     = Src->getWidth ((eCmp)CmpIdx);
    int32    Height    = Src->getHeight((eCmp)CmpIdx);
    int32    Margin    = Src->getMargin();
    int32    Area      = Src->getArea  ((eCmp)CmpIdx); 
    int32    BDM       = (Src->getBitDepth()<=8) ? 1 : 2; //BitDepthMultiplier

    switch(BDM)
    {
      case 1:
        if(Margin>0) { xPixelOps::Convert<uint8, PelType>((uint8*)DstPtr, SrcPtr, Width, SrcStride, Width, Height); }
        else         { xPixelOps::Convert<uint8, PelType>((uint8*)DstPtr, SrcPtr, Area); }
        break;
      case 2:
        if(Margin>0) { xPixelOps::Convert<uint16, PelType>((uint16*)DstPtr, SrcPtr, Width, SrcStride, Width, Height); }
        else         { xPixelOps::Convert<uint16, PelType>((uint16*)DstPtr, SrcPtr, Area); }
        break;
    }
    DstPtr += Area * BDM;
  }
}
template <typename PelType> void xPacked::unpackPlanar(xPic<PelType>* Dst)
{
  xAssert(Dst->isCompatible(m_Width[0], m_Height[0], m_BitDepth[0], m_ImageType, m_ChromaFormat));

  byte*  SrcPtr = m_Buffer;

  int32 NumComponents = Dst->getNumComponents();
  for(int32 CmpIdx=0; CmpIdx<NumComponents; CmpIdx++)
  {    
    PelType* DstPtr    = Dst->getAddr  ((eCmp)CmpIdx);
    int32    DstStride = Dst->getStride((eCmp)CmpIdx);
    int32    Width     = Dst->getWidth ((eCmp)CmpIdx);
    int32    Height    = Dst->getHeight((eCmp)CmpIdx);
    int32    Margin    = Dst->getMargin();
    int32    Area      = Dst->getArea  ((eCmp)CmpIdx); 
    int32    BDM       = (Dst->getBitDepth()<=8) ? 1 : 2; //BitDepthMultiplier

    switch(BDM)
    {
      case 1:
        if(Margin>0) { xPixelOps::Convert(DstPtr, (uint8*)SrcPtr, DstStride, Width, Width, Height); }
        else         { xPixelOps::Convert(DstPtr, (uint8*)SrcPtr, Area); }
        break;
      case 2:
        if(Margin>0) { xPixelOps::Convert(DstPtr, (uint16*)SrcPtr, DstStride, Width, Width, Height); }
        else         { xPixelOps::Convert(DstPtr, (uint16*)SrcPtr, Area); }
        break;
    }
    SrcPtr += Area * BDM;
  }
}
template <typename PelType> void xPacked::packInterleaved(xPic<PelType>* Src)
{
  xAssert(Src->isCompatible(m_Width[0], m_Height[0], m_BitDepth[0], m_ImageType, m_ChromaFormat));

  int32  SrcStride = Src->getStride(CMP_0);
  int32  Width     = Src->getWidth();
  int32  Height    = Src->getHeight();
  int32  Margin    = Src->getMargin();
  int32  BDM       = (Src->getBitDepth()<=8) ? 1 : 2; //BitDepthMultiplier

  eImgTp ImageType = Src->getImageType();
  switch(ImageType)
  {
    case eImgTp::YUV:
    case eImgTp::RGB:
    {
      PelType* SrcPtrA   = Src->getAddr(CMP_0);
      PelType* SrcPtrB   = Src->getAddr(CMP_1);
      PelType* SrcPtrC   = Src->getAddr(CMP_2);
      switch(BDM)
      {
        case 1:
          if(Margin>0) { xPixelOps::ConvertSOA3toAOS4((uint8*)m_Buffer, SrcPtrA, SrcPtrB, SrcPtrC, (PelType)0, Width, SrcStride, Width, Height); }
          else         { xPixelOps::ConvertSOA3toAOS4((uint8*)m_Buffer, SrcPtrA, SrcPtrB, SrcPtrC, (PelType)0, Height*Width); }
          break;
        case 2:
          if(Margin>0) { xPixelOps::ConvertSOA3toAOS4((uint16*)m_Buffer, SrcPtrA, SrcPtrB, SrcPtrC, (PelType)0, Width, SrcStride, Width, Height); }
          else         { xPixelOps::ConvertSOA3toAOS4((uint16*)m_Buffer, SrcPtrA, SrcPtrB, SrcPtrC, (PelType)0, Height*Width); }
          break;
      }
      break;
    }      
    case eImgTp::YUVA:
    case eImgTp::YUVD:
    case eImgTp::RGBA:
    case eImgTp::RGBD:
    {
      PelType* SrcPtrA   = Src->getAddr(CMP_0);
      PelType* SrcPtrB   = Src->getAddr(CMP_1);
      PelType* SrcPtrC   = Src->getAddr(CMP_2);
      PelType* SrcPtrD   = Src->getAddr(CMP_3);
      switch(BDM)
      {
        case 1:
          if(Margin>0) { xPixelOps::ConvertSOA4toAOS4((uint8*)m_Buffer, SrcPtrA, SrcPtrB, SrcPtrC, SrcPtrD, Width, SrcStride, Width, Height); }
          else         { xPixelOps::ConvertSOA4toAOS4((uint8*)m_Buffer, SrcPtrA, SrcPtrB, SrcPtrC, SrcPtrD, Height*Width); }
          break;
        case 2:
          if(Margin>0) { xPixelOps::ConvertSOA4toAOS4((uint16*)m_Buffer, SrcPtrA, SrcPtrB, SrcPtrC, SrcPtrD, Width, SrcStride, Width, Height); }
          else         { xPixelOps::ConvertSOA4toAOS4((uint16*)m_Buffer, SrcPtrA, SrcPtrB, SrcPtrC, SrcPtrD, Height*Width); }
          break;
      }
      break;
    }
    default:
      xAssert(0);
      break;
  }
}
template <typename PelType> void xPacked::unpackInterleaved(xPic<PelType>* Dst)
{
  xAssert(Dst->isCompatible(m_Width[0], m_Height[0], m_BitDepth[0], m_ImageType, m_ChromaFormat));

  int32  SrcStride = Dst->getStride(CMP_0);
  int32  Width     = Dst->getWidth();
  int32  Height    = Dst->getHeight();
  int32  Margin    = Dst->getMargin();
  int32  BDM       = (Dst->getBitDepth()<=8) ? 1 : 2; //BitDepthMultiplier

  eImgTp ImageType = Dst->getImageType();
  switch(ImageType)
  {
    case eImgTp::YUV:
    case eImgTp::RGB:
    {
      PelType* DstPtrA   = Dst->getAddr(CMP_0);
      PelType* DstPtrB   = Dst->getAddr(CMP_1);
      PelType* DstPtrC   = Dst->getAddr(CMP_2);
      switch(BDM)
      {
        case 1:
          if(Margin>0) { xPixelOps::ConvertAOS4toSOA3(DstPtrA, DstPtrB, DstPtrC, (uint8*)m_Buffer, Width, SrcStride, Width, Height); }
          else         { xPixelOps::ConvertAOS4toSOA3(DstPtrA, DstPtrB, DstPtrC, (uint8*)m_Buffer, Height*Width); }
          break;
        case 2:
          if(Margin>0) { xPixelOps::ConvertAOS4toSOA3(DstPtrA, DstPtrB, DstPtrC, (uint16*)m_Buffer, Width, SrcStride, Width, Height); }
          else         { xPixelOps::ConvertAOS4toSOA3(DstPtrA, DstPtrB, DstPtrC, (uint16*)m_Buffer, Height*Width); }
          break;
      }
      break;
    }      
    case eImgTp::YUVA:
    case eImgTp::YUVD:
    case eImgTp::RGBA:
    case eImgTp::RGBD:
    {
      PelType* DstPtrA   = Dst->getAddr(CMP_0);
      PelType* DstPtrB   = Dst->getAddr(CMP_1);
      PelType* DstPtrC   = Dst->getAddr(CMP_2);
      PelType* DstPtrD   = Dst->getAddr(CMP_3);
      switch(BDM)
      {
        case 1:
          if(Margin>0) { xPixelOps::ConvertAOS4toSOA4(DstPtrA, DstPtrB, DstPtrC, DstPtrD, (uint8*)m_Buffer, Width, SrcStride, Width, Height); }
          else         { xPixelOps::ConvertAOS4toSOA4(DstPtrA, DstPtrB, DstPtrC, DstPtrD, (uint8*)m_Buffer, Height*Width); }
          break;
        case 2:
          if(Margin>0) { xPixelOps::ConvertAOS4toSOA4(DstPtrA, DstPtrB, DstPtrC, DstPtrD, (uint16*)m_Buffer, Width, SrcStride, Width, Height); }
          else         { xPixelOps::ConvertAOS4toSOA4(DstPtrA, DstPtrB, DstPtrC, DstPtrD, (uint16*)m_Buffer, Height*Width); }
          break;
      }
      break;
    }
    default:
      xAssert(0);
      break;
  }
}

template <typename PelType> void xPacked::packPlanar(xPlane<PelType>* Src)
{
  xAssert(Src->isCompatible(m_Width[0], m_Height[0], m_BitDepth[0]));

  byte*    DstPtr    = m_Buffer;
  PelType* SrcPtr    = Src->getAddr  ();
  int32    SrcStride = Src->getStride();
  int32    Width     = Src->getWidth ();
  int32    Height    = Src->getHeight();
  int32    Margin    = Src->getMargin();
  int32    Area      = Src->getArea  (); 
  int32    BDM       = (Src->getBitDepth()<=8) ? 1 : 2; //BitDepthMultiplier

  switch(BDM)
  {
    case 1:
      if(Margin>0) { xPixelOps::Convert<uint8, PelType>((uint8*)DstPtr, SrcPtr, Width, SrcStride, Width, Height); }
      else         { xPixelOps::Convert<uint8, PelType>((uint8*)DstPtr, SrcPtr, Area); }
      break;
    case 2:
      if(Margin>0) { xPixelOps::Convert<uint16, PelType>((uint16*)DstPtr, SrcPtr, Width, SrcStride, Width, Height); }
      else         { xPixelOps::Convert<uint16, PelType>((uint16*)DstPtr, SrcPtr, Area); }
      break;
  }

  DstPtr += Area * BDM;

  if(m_NumComponents > 1)
  {
    for(int32 CmpIdx=1; CmpIdx < m_NumComponents; CmpIdx++)
    {     
      int32    Area = m_Width[CmpIdx] * m_Height[CmpIdx]; 
      int32    BDM  = (m_BitDepth[CmpIdx]<=8) ? 1 : 2; //BitDepthMultiplier
      switch(BDM)
      {
        case 1: xMemset8 ((uint8* )DstPtr, (uint8 )xBitDepth2MidValue((int32)m_BitDepth[CmpIdx]), Area); break;
        case 2: xMemset16((uint16*)DstPtr, (uint16)xBitDepth2MidValue((int32)m_BitDepth[CmpIdx]), Area); break;
      }
      DstPtr += Area * BDM;
    }
  }
}
template <typename PelType> void xPacked::unpackPlanar(xPlane<PelType>* Dst)
{
  xAssert(Dst->isCompatible(m_Width[0], m_Height[0], m_BitDepth[0]));

  byte*    SrcPtr    = m_Buffer;
  PelType* DstPtr    = Dst->getAddr  ();
  int32    DstStride = Dst->getStride();
  int32    Width     = Dst->getWidth ();
  int32    Height    = Dst->getHeight();
  int32    Margin    = Dst->getMargin();
  int32    Area      = Dst->getArea  (); 
  int32    BDM       = (Dst->getBitDepth()<=8) ? 1 : 2; //BitDepthMultiplier

  switch(BDM)
  {
    case 1:
      if(Margin>0) { xPixelOps::Convert<PelType, uint8>(DstPtr, (uint8*)SrcPtr, DstStride, Width, Width, Height); }
      else         { xPixelOps::Convert<PelType, uint8>(DstPtr, (uint8*)SrcPtr, Area); }
      break;
    case 2:
      if(Margin>0) { xPixelOps::Convert<PelType, uint16>(DstPtr, (uint16*)SrcPtr, DstStride, Width, Width, Height); }
      else         { xPixelOps::Convert<PelType, uint16>(DstPtr, (uint16*)SrcPtr, Area); }
      break;
  }
}

//=============================================================================================================================================================================

class xPackedTank
{
protected:
  std::vector<xPacked*> m_Buffer;
  std::mutex            m_Mutex;
  uintSize              m_CreatedUnits;
  uintSize              m_SizeLimit;

  //Unit creation parameters
  int32    m_Width;
  int32    m_Height;
  int32    m_BitDepth;
  eImgTp   m_ImageType;
  ePckTp   m_PackedType;
  eCrF     m_ChromaFormat;  
  eCmpO    m_ComponentOrder;

public:
  void     create         (int32 Width, int32 Height, int32 BitDepth, eImgTp ImageType, ePckTp PackedType, eCrF ChromaFormat, eCmpO ComponentOrder, uintSize InitSize);
  void     create         (xPacked* Packed, uintSize InitSize) { create(Packed->getWidth(), Packed->getHeight(), Packed->getBitDepth(), Packed->getImageType(), Packed->getPackedType(), Packed->getChromaFormat(), Packed->getComponentOrder(), InitSize); }
  void     recreate       (int32 Width, int32 Height, int32 BitDepth, eImgTp ImageType, ePckTp PackedType, eCrF ChromaFormat, eCmpO ComponentOrder, uintSize InitSize) { destroy(); create(Width, Height, BitDepth, ImageType, PackedType, ChromaFormat, ComponentOrder, InitSize); }
  void     recreate       (xPacked* Packed, uintSize InitSize) { destroy(); create(Packed, InitSize); }
  void     destroy        () { while(!m_Buffer.empty()) { xDestroyUnit(); } }
           
  void     setSizeLimit   (uintSize SizeLimit) { m_SizeLimit = SizeLimit; while(m_Buffer.size() > SizeLimit) { xDestroyUnit(); } }

  void     put            (xPacked* Packed);  
  xPacked* get            ();
                          
  bool     isCompatible   (int32 Width, int32 Height, int32 BitDepth, eImgTp ImageType, ePckTp PackedType, eCrF ChromaFormat, eCmpO ComponentOrder) { return (m_Width==Width && m_Height==Height && m_BitDepth == BitDepth && m_ImageType==ImageType && m_PackedType == PackedType && m_ChromaFormat==ChromaFormat && m_ComponentOrder == ComponentOrder); }
  bool     isCompatible   (xPacked* Packed) { xAssert(Packed != nullptr); return Packed->isCompatible(m_Width, m_Height, m_BitDepth, m_ImageType, m_PackedType, m_ChromaFormat, m_ComponentOrder); }
  
  uintSize getLoad        () { return m_Buffer.size(); }
  uintSize getCreatedUnits() { return m_CreatedUnits;  }

protected:
  void     xCreateNewUnit ();
  void     xDestroyUnit   ();
};

//=============================================================================================================================================================================

} //end of namespace AVLib