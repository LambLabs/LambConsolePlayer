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

#include "xPacked.h"
#include "xPixelOps.h"
#include "xPicture.h"
#include "xPlane.h"

namespace AVlib {

//=============================================================================================================================================================================

xPacked::xPacked()
{
  for(int32 CmpIdx=0; CmpIdx<3; CmpIdx++)
  {
    m_Width       [CmpIdx] = NOT_VALID;
    m_Height      [CmpIdx] = NOT_VALID;
    m_Stride      [CmpIdx] = NOT_VALID;
    m_Pitch       [CmpIdx] = NOT_VALID;
    m_BitDepth    [CmpIdx] = NOT_VALID;
    m_OriginOffset[CmpIdx] = NOT_VALID;
  }
  
  m_NumComponents  = NOT_VALID;
  m_PackedSize     = NOT_VALID;

  m_Buffer         = nullptr;

  m_FourCC.clear();
  m_ImageType      = eImgTp::UNKNOWN;
  m_PackedType     = ePckTp::INVALID;
  m_ChromaFormat   = CrF_UNKNOWN;
  m_ComponentOrder = eCmpO::INVALID;

  m_POC            = NOT_VALID;  
}
void xPacked::create(int32 Width, int32 Height, std::string FourCC)
{
  int32   BitDepth;
  eImgTp  ImageType;
  ePckTp  PackedType;
  eCrF    ChromaFormat;
  eCmpO   ComponentOrder;
  if(determineParams(BitDepth, ImageType, PackedType, ChromaFormat, ComponentOrder, FourCC))
  {
    m_FourCC = FourCC;
    create(Width, Height, BitDepth, ImageType, PackedType, ChromaFormat, ComponentOrder);
  }
  else
  {
    xAssert(0); 
  }
}
void xPacked::create(int32 Width, int32 Height, int32 BitDepth, eImgTp ImageType, ePckTp PackedType, eCrF ChromaFormat, eCmpO ComponentOrder)
{
  if(m_FourCC=="") { m_FourCC = determineFourCC(BitDepth, ImageType, PackedType, ChromaFormat, ComponentOrder); }

  //componets sizes
  ::memset(m_Width,    0, sizeof(m_Width   ));
  ::memset(m_Height,   0, sizeof(m_Height  ));
  ::memset(m_BitDepth, 0, sizeof(m_BitDepth));

  switch(ImageType)
  {
    case eImgTp::YUV:  
    case eImgTp::YUVA:  
    case eImgTp::YUVD:  
    {
      m_NumComponents = 3;
      //luma
      m_Width   [CMP_LM] = Width;
      m_Height  [CMP_LM] = Height;
      m_BitDepth[CMP_LM] = BitDepth;
      //chroma
      switch(ChromaFormat)
      {
        case CrF_444:    
          m_Width   [CMP_CB] = m_Width   [CMP_CR] = Width;
          m_Height  [CMP_CB] = m_Height  [CMP_CR] = Height;
          m_BitDepth[CMP_CB] = m_BitDepth[CMP_CR] = BitDepth;
          break;
        case CrF_422:    
          m_Width   [CMP_CB] = m_Width   [CMP_CR] = Width;
          m_Height  [CMP_CB] = m_Height  [CMP_CR] = Height>>1;
          m_BitDepth[CMP_CB] = m_BitDepth[CMP_CR] = BitDepth;
          break;
        case CrF_420:    
          m_Width   [CMP_CB] = m_Width   [CMP_CR] = Width>>1;
          m_Height  [CMP_CB] = m_Height  [CMP_CR] = Height>>1;
          m_BitDepth[CMP_CB] = m_BitDepth[CMP_CR] = BitDepth;
          break;
        case CrF_400:   
          m_NumComponents = 1;
          break;
        default:
          xAssert(0);
          break;
      }
      break;
    }
    case eImgTp::RGB:
    case eImgTp::RGBA:
    case eImgTp::RGBD:
    {
      m_NumComponents = 3;
      m_Width [CMP_R]   = m_Width [CMP_G]   = m_Width [CMP_B]   = Width;
      m_Height[CMP_R]   = m_Height[CMP_G]   = m_Height[CMP_B]   = Height;
      m_BitDepth[CMP_R] = m_BitDepth[CMP_G] = m_BitDepth[CMP_B] = BitDepth;
      break;
    }
    case eImgTp::Bayer:
    {
      m_NumComponents = 1;
      m_Width   [0] = Width;
      m_Height  [0] = Height;
      m_BitDepth[0] = BitDepth;
      break;
    }    
    default:
      xAssert(0);
      break;
  }
  if(ImageType == eImgTp::YUVA || ImageType == eImgTp::RGBA || ImageType == eImgTp::YUVD || ImageType == eImgTp::RGBD)
  {
    m_NumComponents = 4;
    // alpha / depth
    m_Width   [3] = Width;
    m_Height  [3] = Height;
    m_BitDepth[3] = BitDepth;
  }
  if(((ImageType == eImgTp::YUV && ChromaFormat == CrF_444)  || ImageType == eImgTp::RGB) && (PackedType == ePckTp::Interleaved))
  {
    m_NumComponents = 4;
    // empty filler
    m_Width   [3] = Width;
    m_Height  [3] = Height;
    m_BitDepth[3] = BitDepth;
  }
  
  m_ImageType      = ImageType;
  m_PackedType     = PackedType;
  m_ChromaFormat   = ChromaFormat;
  m_ComponentOrder = ComponentOrder;

  m_PackedSize = 0;
  for(int32 CmpIdx=0; CmpIdx < m_NumComponents; CmpIdx++) { m_PackedSize += calkPlaneSize(m_Width[CmpIdx], m_Height[CmpIdx], m_BitDepth[CmpIdx]); }
  m_Buffer = (byte*)xAlignedMalloc(m_PackedSize, X_AlignmentDef);

  //stride, pitch and origin offset
  xMemsetX(m_Stride,       (int16)NOT_VALID, 4);
  xMemsetX(m_Pitch,        (int8 )NOT_VALID, 4);
  xMemsetX(m_OriginOffset, (int32)NOT_VALID, 4);

  if(ImageType == eImgTp::Bayer && ImageType == eImgTp::UNKNOWN)
  {
    m_Stride      [0] = Width;
    m_Pitch       [0] = 1;
    m_OriginOffset[0] = 0;
  }
  else
  {
    switch(PackedType)
    {
      case ePckTp::Planar:
      {
        for(int32 CmpIdx=0, OriginOffset=0; CmpIdx<m_NumComponents; CmpIdx++)
        {
          int32 BitDepthMultiplier = (m_BitDepth[CmpIdx] <= 8) ? 1 : 2;
          m_Stride      [CmpIdx] = m_Width[CmpIdx] * BitDepthMultiplier;
          m_Pitch       [CmpIdx] = 1 * BitDepthMultiplier;
          m_OriginOffset[CmpIdx] = OriginOffset;
          OriginOffset += calkPlaneSize(m_Width[CmpIdx], m_Height[CmpIdx], BitDepth);    
        }
        break;
      }
      case ePckTp::SemiPlanar:
      {
        xAssert(ImageType == eImgTp::YUV);
        int32 BitDepthMultiplier = (m_BitDepth[CMP_LM] <= 8) ? 1 : 2;
        m_Stride      [CMP_LM] = m_Width[CMP_LM] * BitDepthMultiplier;
        m_Pitch       [CMP_LM] = 1 * BitDepthMultiplier;
        m_OriginOffset[CMP_LM] = 0;
        int32 LumaSize = calkPlaneSize(m_Width[CMP_LM], m_Height[CMP_LM], BitDepth); 
        for(int32 CmpIdx=1; CmpIdx<m_NumComponents; CmpIdx++)
        {
          int32 BitDepthMultiplier = (m_BitDepth[CmpIdx] <= 8) ? 1 : 2;
          m_Stride      [CmpIdx] = (m_Width[CmpIdx]<<1) * BitDepthMultiplier;
          m_Pitch       [CmpIdx] = 2 * BitDepthMultiplier;
          m_OriginOffset[CmpIdx] = LumaSize + (CmpIdx-1) * BitDepthMultiplier;
        }
        break;
      }
      case ePckTp::Interleaved:
      {
        for(int32 CmpIdx=0; CmpIdx<m_NumComponents; CmpIdx++)
        {
          int32 BitDepthMultiplier = (m_BitDepth[CmpIdx] <= 8) ? 1 : 2;
          m_Stride      [CmpIdx] = (m_Width[CmpIdx]<<2) * BitDepthMultiplier;
          m_Pitch       [CmpIdx] = 4 * BitDepthMultiplier;
          m_OriginOffset[CmpIdx] = CmpIdx * BitDepthMultiplier;
        }
        break;
      }
      default:
      {
        xAssert(0); break;
      }      
    }
  }
}
void xPacked::destroy()
{
  xAlignedFree(m_Buffer); m_Buffer = nullptr;
}
void xPacked::clear()
{
  ::memset(m_Buffer, 0, m_PackedSize);
}
void xPacked::copy(xPacked* Src)
{
  xAssert(isCompatible(Src));
  ::memcpy(m_Buffer, Src->m_Buffer, m_PackedSize);
  m_POC = Src->m_POC;
}
void xPacked::convert(xPacked* Src)
{
  xAssert(Src->isCompatible(m_Width[CMP_LM], m_Height[CMP_LM], m_BitDepth[CMP_LM], m_ImageType, m_ChromaFormat));
  switch(m_ImageType)
  {
    case eImgTp::YUV: convertPackedTypeYUV(Src); break;
    default: xAssert(0); break;
  }
}
void xPacked::convertPackedTypeYUV(xPacked* Src)
{
  switch(Src->m_PackedType)
  {
    case ePckTp::Planar:
    {
      switch(m_PackedType)
      {
        case ePckTp::Planar:      copy(Src);                            break; //Planar2Planar - copy
        case ePckTp::SemiPlanar:  convertPlanarYUV2SemiPlanarYUV(Src);  break; //Planar2SemiPlanar
        case ePckTp::Interleaved: convertPlanarYUV2InterleavedYUV(Src); break; //Planar2Interleaved
        default:                xAssert(0);                           break;
      }
      break;
    }
    case ePckTp::SemiPlanar:
    {
      switch(m_PackedType)
      {
      case ePckTp::Planar:      convertSemiPlanarYUV2PlanarYUV(Src);      break; //SemiPlanar2Planar
      case ePckTp::SemiPlanar:  copy(Src);                                break; //SemiPlanar2SemiPlanar - copy
      case ePckTp::Interleaved: convertSemiPlanarYUV2InterleavedYUV(Src); break; //SemiPlanar2Interleaved
      default:                xAssert(0);                               break;
      }
      break;
    }
    case ePckTp::Interleaved:
    {
      switch(m_PackedType)
      {
      case ePckTp::Planar:      convertInterleavedYUV2PlanarYUV(Src);     break; //Interleaved2Planar
      case ePckTp::SemiPlanar:  convertInterleavedYUV2SemiPlanarYUV(Src); break; //Interleaved2SemiPlanar
      case ePckTp::Interleaved: copy(Src);                                break; //Interleaved2Interleaved - copy
      default:                xAssert(0);                               break;
      }
      break;
    }
    default: xAssert(0); break;
  }
}
void xPacked::convertPlanarYUV2SemiPlanarYUV(xPacked* Src)
{
  xAssert(Src->isCompatible(m_Width[CMP_LM], m_Height[CMP_LM], m_BitDepth[CMP_LM], m_ImageType, m_ChromaFormat) && Src->isSamePckType(ePckTp::Planar) && m_PackedType==ePckTp::SemiPlanar );
  int32  BitDepthMultiplier = (m_BitDepth[CMP_LM]<=8) ? 1 : 2;

  //copy luma
  int32 PointsL = m_Height[CMP_LM]*m_Width[CMP_LM];  
  memcpy(getAddrLm(), Src->getAddrLm(), PointsL*BitDepthMultiplier); 

  //interleave chroma
  int32 PointsC  = m_Height[CMP_CB]*m_Width[CMP_CB];
  byte* SrcPtrCb = Src->getAddrCb();
  byte* SrcPtrCr = Src->getAddrCr();
  if(!Src->isSameCmpOrder(m_ComponentOrder)) { std::swap(SrcPtrCb, SrcPtrCr); }
  byte* DstPtr   = getAddrLm() + PointsL*BitDepthMultiplier;

  switch(BitDepthMultiplier)
  {
    case 1: xPixelOps::SOA2toAOS2((uint8*)DstPtr, (uint8*)SrcPtrCb, (uint8*)SrcPtrCr, PointsC); break;
    case 2: xPixelOps::SOA2toAOS2((int16*)DstPtr, (int16*)SrcPtrCb, (int16*)SrcPtrCr, PointsC); break;
  }
}
void xPacked::convertPlanarYUV2InterleavedYUV(xPacked* Src)
{
  xAssert(Src->isCompatible(m_Width[CMP_LM], m_Height[CMP_LM], m_BitDepth[CMP_LM], m_ImageType, m_ChromaFormat) && Src->isSamePckType(ePckTp::Planar) && m_PackedType == ePckTp::Interleaved);
  xAssert(m_ChromaFormat == CrF_422);
  int32  BitDepthMultiplier = (m_BitDepth[CMP_LM]<=8) ? 1 : 2;

  int32 PointsC  = m_Height[CMP_CB] * m_Width[CMP_CB];
  byte* SrcPtrLm = Src->getAddrLm();
  byte* SrcPtrCb = Src->getAddrCb();
  byte* SrcPtrCr = Src->getAddrCr();
  byte* DstPtr   = getPackedBuffer();

  if(Src->getComponentOrder() == eCmpO::YVU ) { std::swap(SrcPtrCb, SrcPtrCr); }
  if(m_ComponentOrder         == eCmpO::YVYU) { std::swap(SrcPtrCb, SrcPtrCr); }
  
  switch(m_ComponentOrder)
  {
    case eCmpO::YUYV: 
    case eCmpO::YVYU:
    {
      switch(BitDepthMultiplier)
      {
        case 1: xPixelOps::SOA3toAOS4_ABAC((uint8*)DstPtr, (uint8*)SrcPtrLm, (uint8*)SrcPtrCb, (uint8*)SrcPtrCr, PointsC); break;
        case 2: xPixelOps::SOA3toAOS4_ABAC((int16*)DstPtr, (int16*)SrcPtrLm, (int16*)SrcPtrCb, (int16*)SrcPtrCr, PointsC); break;
      }
      break;
    }    
    case eCmpO::UYVY:
    {
      switch(BitDepthMultiplier)
      {
        case 1: xPixelOps::SOA3toAOS4_BACA((uint8*)DstPtr, (uint8*)SrcPtrLm, (uint8*)SrcPtrCb, (uint8*)SrcPtrCr, PointsC); break;
        case 2: xPixelOps::SOA3toAOS4_BACA((int16*)DstPtr, (int16*)SrcPtrLm, (int16*)SrcPtrCb, (int16*)SrcPtrCr, PointsC); break;
      }
      break;
    }
    default: xAssert(0);  break;
  }
}
void xPacked::convertSemiPlanarYUV2PlanarYUV(xPacked* Src)
{
  xAssert(Src->isCompatible(m_Width[CMP_LM], m_Height[CMP_LM], m_BitDepth[CMP_LM], m_ImageType, m_ChromaFormat) && Src->isSamePckType(ePckTp::SemiPlanar) && m_PackedType==ePckTp::Planar );
  int32 BitDepthMultiplier = (m_BitDepth[CMP_LM]<=8) ? 1 : 2;

  //copy luma
  int32 PointsL = m_Height[CMP_LM]*m_Width[CMP_LM];  
  memcpy(getAddrLm(), Src->getAddrLm(), PointsL*BitDepthMultiplier); 

  //deinterleave chroma
  int32 PointsC  = m_Height[CMP_CB]*m_Width[CMP_CB];
  byte* SrcPtr   = Src->getAddrLm() + PointsL*BitDepthMultiplier;
  byte* DstPtrCb = getAddrCb();
  byte* DstPtrCr = getAddrCr();
  if(!Src->isSameCmpOrder(m_ComponentOrder)) { std::swap(DstPtrCb, DstPtrCr); }

  switch(BitDepthMultiplier)
  {
    case 1: xPixelOps::AOS2toSOA2((uint8*)DstPtrCb, (uint8*)DstPtrCr, (uint8*)SrcPtr, PointsC); break;
    case 2: xPixelOps::AOS2toSOA2((int16*)DstPtrCb, (int16*)DstPtrCr, (int16*)SrcPtr, PointsC); break;
  }
}
void xPacked::convertSemiPlanarYUV2InterleavedYUV(xPacked* Src)
{
  xAssert(Src->isCompatible(m_Width[CMP_LM], m_Height[CMP_LM], m_BitDepth[CMP_LM], m_ImageType, m_ChromaFormat) && Src->isSamePckType(ePckTp::SemiPlanar) && m_PackedType == ePckTp::Interleaved);
  xAssert(m_ChromaFormat == CrF_422 && (m_ComponentOrder == eCmpO::YUYV || m_ComponentOrder == eCmpO::UYVY) && Src->isSameCmpOrder(eCmpO::YUV));
  int32 BitDepthMultiplier = (m_BitDepth[CMP_LM]<=8) ? 1 : 2;

  int32 PointsL = m_Height[CMP_LM] * m_Width[CMP_LM];
  byte* SrcPtrL = Src->getAddrLm(); //YYYY
  byte* SrcPtrC = Src->getAddrLm() + PointsL*BitDepthMultiplier;//UVUV
  byte* DstPtr  = getPackedBuffer(); //YUYV or UYVY

  if(m_ComponentOrder == eCmpO::UYVY) { std::swap(SrcPtrL, SrcPtrC); }

  switch(BitDepthMultiplier)
  {
    case 1: xPixelOps::SOA2toAOS2((uint8*)DstPtr, (uint8*)SrcPtrL, (uint8*)SrcPtrC, PointsL); break;
    case 2: xPixelOps::SOA2toAOS2((int16*)DstPtr, (int16*)SrcPtrL, (int16*)SrcPtrC, PointsL); break;
  }
}
void xPacked::convertInterleavedYUV2PlanarYUV(xPacked* Src)
{
  xAssert(Src->isCompatible(m_Width[CMP_LM], m_Height[CMP_LM], m_BitDepth[CMP_LM], m_ImageType, m_ChromaFormat) && Src->isSamePckType(ePckTp::Interleaved) && m_PackedType == ePckTp::Planar);
  xAssert(m_ChromaFormat == CrF_422);
  int32  BitDepthMultiplier = (m_BitDepth[CMP_LM]<=8) ? 1 : 2;

  int32 PointsC  = m_Height[CMP_CB] * m_Width[CMP_CB];
  byte* SrcPtr   = Src->getPackedBuffer();
  byte* DstPtrLm = getAddrLm();
  byte* DstPtrCb = getAddrCb();
  byte* DstPtrCr = getAddrCr();

  if(Src->getComponentOrder() == eCmpO::YVYU) { std::swap(DstPtrCb, DstPtrCr); }
  if(m_ComponentOrder         == eCmpO::YVU ) { std::swap(DstPtrCb, DstPtrCr); }

  switch(m_ComponentOrder)
  {
    case eCmpO::YUYV: 
    case eCmpO::YVYU:
    {
      switch(BitDepthMultiplier)
      {
        case 1: xPixelOps::SOA3toAOS4_ABAC((uint8*)DstPtrLm, (uint8*)DstPtrCb, (uint8*)DstPtrCr, (uint8*)SrcPtr, PointsC); break;
        case 2: xPixelOps::SOA3toAOS4_ABAC((int16*)DstPtrLm, (int16*)DstPtrCb, (int16*)DstPtrCr, (int16*)SrcPtr, PointsC); break;
      }
      break;
    }      
    case eCmpO::UYVY:
    {
      switch(BitDepthMultiplier)
      {
        case 1: xPixelOps::SOA3toAOS4_BACA((uint8*)DstPtrLm, (uint8*)DstPtrCb, (uint8*)DstPtrCr, (uint8*)SrcPtr, PointsC); break;
        case 2: xPixelOps::SOA3toAOS4_BACA((int16*)DstPtrLm, (int16*)DstPtrCb, (int16*)DstPtrCr, (int16*)SrcPtr, PointsC); break;
      }
      break;
    }
    default: xAssert(0);  break;
  }
}
void xPacked::convertInterleavedYUV2SemiPlanarYUV(xPacked* Src)
{
  xAssert(Src->isCompatible(m_Width[CMP_LM], m_Height[CMP_LM], m_BitDepth[CMP_LM], m_ImageType, m_ChromaFormat) && Src->isSamePckType(ePckTp::Interleaved) && m_PackedType == ePckTp::SemiPlanar);
  xAssert(m_ChromaFormat == CrF_422 && (Src->isSameCmpOrder(eCmpO::YUYV) || Src->isSameCmpOrder(eCmpO::UYVY)) && m_ComponentOrder == eCmpO::YUV);
  int32 BitDepthMultiplier = (m_BitDepth[CMP_LM]<=8) ? 1 : 2;

  int32 PointsL = m_Height[CMP_LM] * m_Width[CMP_LM];
  byte* SrcPtr  = Src->getPackedBuffer(); //YUYV or UYVY
  byte* DstPtrL = getAddrLm();//YYYY
  byte* DstPtrC = getAddrLm() + PointsL*BitDepthMultiplier;//UVUV

  if(Src->isSameCmpOrder(eCmpO::UYVY)) { std::swap(DstPtrL, DstPtrC); }

  switch(BitDepthMultiplier)
  {
    case 1: xPixelOps::AOS2toSOA2((uint8*)DstPtrL, (uint8*)DstPtrC, (uint8*)SrcPtr, PointsL); break;
    case 2: xPixelOps::AOS2toSOA2((int16*)DstPtrL, (int16*)DstPtrC, (int16*)SrcPtr, PointsL); break;
  }
}
int32 xPacked::calkPlaneSize(int32 Width, int32 Height, int32 BitDepth)
{
  int32 BDM       = (BitDepth<=8) ? 1 : 2; //BitDepthMultiplier
  int32 PlaneSize = Width*Height*BDM;
  return PlaneSize;
}
int32 xPacked::calkPackedSize(int32 Width, int32 Height, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat)
{
  int32 PackedSize = NOT_VALID;
  int32 BitDepthMultiplier = (BitDepth<=8) ? 1 : 2;

  switch(ImageType)
  {
    case eImgTp::YUV:
    {
      int32 SamplingReference     = (int32)ChromaFormat/100;
      int32 ChromaSamplesIn1stRow = (ChromaFormat-SamplingReference*100)/10;
      int32 ChromaSamplesIn2ndRow = ChromaFormat%10;
      int32 LumaSamples           = 2*SamplingReference;
      int32 ChromaSamples         = ChromaSamplesIn1stRow + ChromaSamplesIn2ndRow;        
      PackedSize = (Width*Height*BitDepthMultiplier*(LumaSamples+(2*ChromaSamples)))/LumaSamples;
    }
    break;
    case eImgTp::RGB:
      PackedSize = (Width*Height*BitDepthMultiplier*3);
      break;
    case eImgTp::YUVA:
    case eImgTp::RGBA:
    case eImgTp::YUVD:
    case eImgTp::RGBD:
      PackedSize = (Width*Height*BitDepthMultiplier*4);
      break;
    case eImgTp::XYZ:
      PackedSize = (Width*Height*BitDepthMultiplier*3);
      break;
    case eImgTp::Bayer:
    case eImgTp::UNKNOWN:
      PackedSize = (Width*Height*BitDepthMultiplier);
      break;
    default:
      xAssert(0);
      break;
  }

  return PackedSize;
}
int32 xPacked::calkPackedSize(int32 Width, int32 Height, std::string FourCC)
{
  int32  BitDepth;
  eImgTp ImageType;
  ePckTp PackedType;
  eCrF   ChromaFormat;
  eCmpO  ComponentOrder;
  if(determineParams(BitDepth, ImageType, PackedType, ChromaFormat, ComponentOrder, FourCC)) { return calkPackedSize(Width, Height, BitDepth, ImageType, ChromaFormat); }
  else                                                                             { xAssert(0); return NOT_VALID; }
}
std::string xPacked::determineFourCC(int32 BitDepth, eImgTp ImageType, ePckTp PackedType, eCrF ChromaFormat, eCmpO ComponentOrder)
{
  std::string FourCC = "";
  switch (ImageType)
  {
    case eImgTp::YUV:
    {
      switch(PackedType)
      {
        case ePckTp::Planar:
        {
          switch(ChromaFormat)
          {
            case CrF_444: FourCC = "I444"; break;
            case CrF_422: FourCC = ComponentOrder==eCmpO::YUV ? "I422" : "YV16"; break;
            case CrF_420: FourCC = ComponentOrder==eCmpO::YUV ? "I420" : "YV12"; break;
            case CrF_400: FourCC = "Y800"; break;
            default: xAssert(0);  break;
          }
          break;
        }          
        case ePckTp::SemiPlanar:
        {
          switch(ChromaFormat)
          {
            case CrF_444: FourCC = ComponentOrder==eCmpO::YUV ? "NV24" : "NV42"; break;
            case CrF_422: FourCC = ComponentOrder==eCmpO::YUV ? "NV16" : "NV61"; break;
            case CrF_420: FourCC = ComponentOrder==eCmpO::YUV ? "NV12" : "NV21"; break;
            default: xAssert(0);  break;
          }
          break;
        }
        case ePckTp::Interleaved:
        {
          switch(ChromaFormat)
          {
            case CrF_444:
            {
              switch(ComponentOrder)
              {
                case eCmpO::YUV : FourCC = "YUVx"; break;
                case eCmpO::YUVA: FourCC = "YUVA"; break;
                case eCmpO::AYUV: FourCC = "AYUV"; break;
                case eCmpO::YUVD: FourCC = "YUVD"; break;
                case eCmpO::DYUV: FourCC = "DYUV"; break;
                default: xAssert(0);  break;
              }
              break;
            }
            case CrF_422: 
            {
              switch(ComponentOrder)
              {
                case eCmpO::YUYV: FourCC = "YUYV"; break;
                case eCmpO::UYVY: FourCC = "UYVY"; break;
                case eCmpO::YVYU: FourCC = "YVYU"; break;
                default: xAssert(0);  break;
              }
              break;
            }
            default: xAssert(0);  break;
          }
          break;
        }         
        default: 
        {
          xAssert(0); break;
        }
      }      
    }
    case eImgTp::RGB:
    {
      break;
    }
    case eImgTp::Bayer:
    {
      FourCC = BitDepth<=8 ? "BA81" : "BA82";
      break;
    }
    default: break;
  }
  return FourCC;
}
bool xPacked::determineParams(int32& BitDepth, eImgTp& ImageType, ePckTp& PackedType, eCrF& ChromaFormat, eCmpO& ComponentOrder, std::string FourCC)
{
  //Planar 8 bit YUV formats
  if     (FourCC=="I444" || FourCC=="J444")                                     { BitDepth=8; ImageType=eImgTp::YUV; PackedType=ePckTp::Planar; ChromaFormat=CrF_444; ComponentOrder=eCmpO::YUV; return true; }
  else if(FourCC=="I422" || FourCC=="J422")                                     { BitDepth=8; ImageType=eImgTp::YUV; PackedType=ePckTp::Planar; ChromaFormat=CrF_422; ComponentOrder=eCmpO::YUV; return true; }
  else if(FourCC=="YV16")                                                       { BitDepth=8; ImageType=eImgTp::YUV; PackedType=ePckTp::Planar; ChromaFormat=CrF_422; ComponentOrder=eCmpO::YVU; return true; }
  else if(FourCC=="I420" || FourCC=="J420" || FourCC=="IYUV" || FourCC=="YU12") { BitDepth=8; ImageType=eImgTp::YUV; PackedType=ePckTp::Planar; ChromaFormat=CrF_420; ComponentOrder=eCmpO::YUV; return true; }
  else if(FourCC=="YV12")                                                       { BitDepth=8; ImageType=eImgTp::YUV; PackedType=ePckTp::Planar; ChromaFormat=CrF_420; ComponentOrder=eCmpO::YVU; return true; }
  else if(FourCC=="GREY" || FourCC=="Y800" || FourCC=="Y8"  )                   { BitDepth=8; ImageType=eImgTp::YUV; PackedType=ePckTp::Planar; ChromaFormat=CrF_420; ComponentOrder=eCmpO::INVALID; return true; }
  //Semi-planar 8 bit YUV formats
  else if(FourCC=="NV24") { BitDepth=8; ImageType=eImgTp::YUV; PackedType=ePckTp::SemiPlanar; ChromaFormat=CrF_444; ComponentOrder=eCmpO::YUV; return true; }
  else if(FourCC=="NV42") { BitDepth=8; ImageType=eImgTp::YUV; PackedType=ePckTp::SemiPlanar; ChromaFormat=CrF_444; ComponentOrder=eCmpO::YVU; return true; }
  else if(FourCC=="NV16") { BitDepth=8; ImageType=eImgTp::YUV; PackedType=ePckTp::SemiPlanar; ChromaFormat=CrF_422; ComponentOrder=eCmpO::YUV; return true; }
  else if(FourCC=="NV61") { BitDepth=8; ImageType=eImgTp::YUV; PackedType=ePckTp::SemiPlanar; ChromaFormat=CrF_422; ComponentOrder=eCmpO::YVU; return true; }
  else if(FourCC=="NV12") { BitDepth=8; ImageType=eImgTp::YUV; PackedType=ePckTp::SemiPlanar; ChromaFormat=CrF_420; ComponentOrder=eCmpO::YUV; return true; }
  else if(FourCC=="NV21") { BitDepth=8; ImageType=eImgTp::YUV; PackedType=ePckTp::SemiPlanar; ChromaFormat=CrF_420; ComponentOrder=eCmpO::YVU; return true; }
  //Semi-planar 10 and 16 bit YUV formats
  else if(FourCC=="P410") { BitDepth=10; ImageType=eImgTp::YUV; PackedType=ePckTp::SemiPlanar; ChromaFormat=CrF_444; ComponentOrder=eCmpO::YUV; return true; }
  else if(FourCC=="P416") { BitDepth=16; ImageType=eImgTp::YUV; PackedType=ePckTp::SemiPlanar; ChromaFormat=CrF_444; ComponentOrder=eCmpO::YUV; return true; }
  else if(FourCC=="P210") { BitDepth=10; ImageType=eImgTp::YUV; PackedType=ePckTp::SemiPlanar; ChromaFormat=CrF_422; ComponentOrder=eCmpO::YUV; return true; }
  else if(FourCC=="P216") { BitDepth=16; ImageType=eImgTp::YUV; PackedType=ePckTp::SemiPlanar; ChromaFormat=CrF_422; ComponentOrder=eCmpO::YUV; return true; }
  else if(FourCC=="P010") { BitDepth=10; ImageType=eImgTp::YUV; PackedType=ePckTp::SemiPlanar; ChromaFormat=CrF_420; ComponentOrder=eCmpO::YUV; return true; }
  else if(FourCC=="P016") { BitDepth=16; ImageType=eImgTp::YUV; PackedType=ePckTp::SemiPlanar; ChromaFormat=CrF_420; ComponentOrder=eCmpO::YUV; return true; }
  //Interleaved 8 bit YUV formats
  else if(FourCC=="YUVA")                                     { BitDepth=8; ImageType=eImgTp::YUVA; PackedType=ePckTp::Interleaved; ChromaFormat=CrF_444; ComponentOrder=eCmpO::YUVA; return true; }
  else if(FourCC=="AYUV")                                     { BitDepth=8; ImageType=eImgTp::YUVA; PackedType=ePckTp::Interleaved; ChromaFormat=CrF_444; ComponentOrder=eCmpO::AYUV; return true; }
  else if(FourCC=="YUVD")                                     { BitDepth=8; ImageType=eImgTp::YUVD; PackedType=ePckTp::Interleaved; ChromaFormat=CrF_444; ComponentOrder=eCmpO::YUVD; return true; }
  else if(FourCC=="DYUV")                                     { BitDepth=8; ImageType=eImgTp::YUVD; PackedType=ePckTp::Interleaved; ChromaFormat=CrF_444; ComponentOrder=eCmpO::DYUV; return true; }
  else if(FourCC=="YUYV" || FourCC=="YUY2" || FourCC=="YUNV") { BitDepth=8; ImageType=eImgTp::YUV;  PackedType=ePckTp::Interleaved; ChromaFormat=CrF_422; ComponentOrder=eCmpO::YUYV; return true; }
  else if(FourCC=="UYVY" || FourCC=="Y422" || FourCC=="UYNV") { BitDepth=8; ImageType=eImgTp::YUV;  PackedType=ePckTp::Interleaved; ChromaFormat=CrF_422; ComponentOrder=eCmpO::UYVY; return true; }  
  else if(FourCC=="YVYU")                                     { BitDepth=8; ImageType=eImgTp::YUV;  PackedType=ePckTp::Interleaved; ChromaFormat=CrF_422; ComponentOrder=eCmpO::YVYU; return true; }
  //RGB formats
  //else if(FourCC=="RGB" ) { ImageType=eImgTp::RGB; PackedType=ePckTp::Interleaved; ChromaFormat=CrF_INVALID; ComponentOrder=eCmpO::RGB; }
  //RGB3 GRB3 RGBA RGB4
  //Bayer formats
  else if(FourCC=="BA81" || FourCC=="BYR1") { BitDepth= 8; ImageType=eImgTp::Bayer ; PackedType=ePckTp::INVALID; ChromaFormat=CrF_INVALID; ComponentOrder=eCmpO::INVALID; return true; }
  else if(FourCC=="BA82" || FourCC=="BYR2") { BitDepth=12; ImageType=eImgTp::Bayer ; PackedType=ePckTp::INVALID; ChromaFormat=CrF_INVALID; ComponentOrder=eCmpO::INVALID; return true; }
  //not supported
  else { BitDepth=NOT_VALID; ImageType=eImgTp::INVALID; PackedType=ePckTp::INVALID; ChromaFormat=CrF_INVALID; ComponentOrder=eCmpO::INVALID; return false; }
}
void xPacked::readFromFile(xFile* File)
{
  [[maybe_unused]]uint32 Read = File->read(m_Buffer, m_PackedSize);
  xAssert(Read == (uint32)m_PackedSize);
}
void xPacked::writeToFile(xFile* File)
{
  [[maybe_unused]]uint32 Writen = File->write(m_Buffer, m_PackedSize);
  xAssert(Writen == (uint32)m_PackedSize);
}

//=============================================================================================================================================================================
// xPackedTank
//=============================================================================================================================================================================
void xPackedTank::create(int32 Width, int32 Height, int32 BitDepth, eImgTp ImageType, ePckTp PackedType, eCrF ChromaFormat, eCmpO ComponentOrder, uintSize InitSize)
{
  m_Mutex.lock();
  m_Width          = Width;    
  m_Height         = Height;  
  m_BitDepth       = BitDepth;
  m_ImageType      = ImageType;
  m_PackedType     = PackedType;
  m_ChromaFormat   = ChromaFormat;
  m_ComponentOrder = ComponentOrder;
  m_CreatedUnits   = 0;
  m_SizeLimit      = uintSize_max;
  
  for(uintSize i=0; i<InitSize; i++) { xCreateNewUnit(); }
  m_Mutex.unlock();
}
void xPackedTank::put(xPacked* Packed)
{
  m_Mutex.lock();
  xAssert(Packed != nullptr);
  Packed->setPOC      (NOT_VALID);
  Packed->setTimestamp(NOT_VALID);
  m_Buffer.push_back(Packed);
  if(m_Buffer.size() > m_SizeLimit) { xDestroyUnit(); }
  m_Mutex.unlock();
}
xPacked* xPackedTank::get()
{
  m_Mutex.lock();
  if(m_Buffer.empty()) { xCreateNewUnit(); }
  xPacked* Packed = m_Buffer.back();
  m_Buffer.pop_back();
  m_Mutex.unlock();
  return Packed;
}
void xPackedTank::xCreateNewUnit()
{
  xPacked* Tmp = new xPacked;
  Tmp->create(m_Width, m_Height, m_BitDepth, m_ImageType, m_PackedType, m_ChromaFormat, m_ComponentOrder); 
  m_Buffer.push_back(Tmp);
  m_CreatedUnits++;
}
void xPackedTank::xDestroyUnit()
{
  if(!m_Buffer.empty())
  {
    xPacked* Tmp = m_Buffer.back();
    m_Buffer.pop_back();
    Tmp->destroy();
    delete Tmp;
  }
}

//=============================================================================================================================================================================

} //end of namespace AVLib