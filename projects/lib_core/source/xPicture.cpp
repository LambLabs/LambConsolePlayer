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

#define AVlib_xPicture_IMPLEMENTATION
#include "xPicture.h"
#undef AVlib_xPicture_IMPLEMENTATION
#include "xPacked.h"
#include "xBMP.h"
#include "xResample.h"
#include "xColorSpace.h"
//#include "xDistortion.h"
#include <functional>

namespace AVlib {

//=============================================================================================================================================================================
// Base functions
//=============================================================================================================================================================================
template <typename PelType> xPic<PelType>::xPic()
{
  m_Width         = NOT_VALID;
  m_Height        = NOT_VALID;
  m_Margin        = NOT_VALID;
  m_BitDepth      = NOT_VALID;
  m_ImageType     = eImgTp::INVALID;
  m_ChromaFormat  = CrF_INVALID;
  m_NumComponents = 0;

  for(uint32 CmpIdx=0; CmpIdx < c_NumberOfComponents; CmpIdx++) { m_CmpShift[CmpIdx] = (int8)0; }

  for(uint32 CmpIdx = 0; CmpIdx < c_NumberOfComponents; CmpIdx++)
  {
    m_CmpNumPels[CmpIdx] = NOT_VALID;
    m_CmpPelStride[CmpIdx] = NOT_VALID;
    m_CmpPelBuffer[CmpIdx] = nullptr;
    m_CmpPelOrigin[CmpIdx] = nullptr;
  }

  m_POC       = NOT_VALID;
  m_Timestamp = NOT_VALID;
  m_Name.clear();

  m_IsMarginExtended = false;
}
template <typename PelType> void xPic<PelType>::create(int32 Width, int32 Height, int32 Margin, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat)
{
  m_Width           = Width;
  m_Height          = Height;
  m_Margin          = Margin;
  m_BitDepth        = BitDepth;
  m_ImageType       = ImageType;
  m_ChromaFormat    = ChromaFormat;

  for(uint32 CmpIdx = 0; CmpIdx < c_NumberOfComponents; CmpIdx++)
  {
    m_CmpShift[CmpIdx] ={ 31, 31 };
    m_CmpNumPels[CmpIdx] = 0;
    m_CmpPelStride[CmpIdx] = 0;
    m_CmpPelBuffer[CmpIdx] = nullptr;
    m_CmpPelOrigin[CmpIdx] = nullptr;
  }

  if(ImageType == eImgTp::YUV || ImageType == eImgTp::YUVA || ImageType == eImgTp::YUVD)
  {
    m_NumComponents = 3;

    //luma
    if(ChromaFormat >= 400) { m_CmpShift[CMP_LM] ={ 0, 0 }; }
    else { ChromaFormat = (eCrF)((int32)ChromaFormat + 400); }

    //chroma
    switch(ChromaFormat)
    {
      case CrF_444: m_CmpShift[CMP_CB] = m_CmpShift[CMP_CR] ={ 0,  0 }; break;
      case CrF_422: m_CmpShift[CMP_CB] = m_CmpShift[CMP_CR] ={ 1,  0 }; break;
      case CrF_420: m_CmpShift[CMP_CB] = m_CmpShift[CMP_CR] ={ 1,  1 }; break;
      case CrF_400: m_CmpShift[CMP_CB] = m_CmpShift[CMP_CR] ={ 31, 31 }; break;
      default: xAssert(0); break;
    }

    if(ImageType == eImgTp::YUVA || ImageType == eImgTp::YUVD) { m_CmpShift[CMP_3] ={ 0, 0 }; m_NumComponents = 4; }
  }
  else
  {
    switch(ImageType)
    {
      case eImgTp::RGB  : case eImgTp::XYZ    : m_NumComponents = 3; break;
      case eImgTp::RGBA : case eImgTp::RGBD   : m_NumComponents = 4; break;
      case eImgTp::Bayer: case eImgTp::UNKNOWN: m_NumComponents = 1; break;
      default:                                  m_NumComponents = 0; break;
    }
    for(int32 CmpIdx = 0; CmpIdx < m_NumComponents; CmpIdx++) { m_CmpShift[CmpIdx] ={ 0, 0 }; }
  }

  for(int32 CmpId=0; CmpId < m_NumComponents; CmpId++)
  {
    m_CmpNumPels  [CmpId] = (getWidth((eCmp)CmpId) + (m_Margin << 1)) * (getHeight((eCmp)CmpId) + (m_Margin << 1));
    m_CmpPelStride[CmpId] = getWidth((eCmp)CmpId) + (m_Margin << 1);
    m_CmpPelBuffer[CmpId] = (PelType*)xAlignedMalloc(m_CmpNumPels[CmpId] * sizeof(PelType), X_AlignmentPel);
    m_CmpPelOrigin[CmpId] = m_CmpPelBuffer[CmpId] + (m_Margin * m_CmpPelStride[CmpId]) + m_Margin;
  }

  m_POC              = NOT_VALID;
  m_Timestamp        = NOT_VALID;
  m_Name.clear();

  m_IsMarginExtended = false;
}
template <typename PelType> void xPic<PelType>::destroy()
{
  for(uint32 CmpIdx = 0; CmpIdx < c_NumberOfComponents; CmpIdx++)
  {
    if(m_CmpPelBuffer[CmpIdx] != nullptr) { xAlignedFree(m_CmpPelBuffer[CmpIdx]); m_CmpPelBuffer[CmpIdx] = nullptr; }
    m_CmpPelOrigin[CmpIdx] = nullptr;
  }
}
template <typename PelType> void xPic<PelType>::duplicate(xPic* Pic)
{
  create(Pic);
  copy(Pic);
  m_POC           = Pic->m_POC;
  m_Timestamp     = Pic->m_Timestamp;
  m_Name          = Pic->m_Name;
}
template <typename PelType> void xPic<PelType>::clear()
{
  for(int32 CmpId=0; CmpId < m_NumComponents; CmpId++) { memset(m_CmpPelBuffer[CmpId], 0, m_CmpNumPels[CmpId] * sizeof(PelType)); }
  m_POC              = NOT_VALID;  
  m_Timestamp        = NOT_VALID;  
  m_Name.clear();
  m_IsMarginExtended = false;
}
template <typename PelType> void xPic<PelType>::copy(xPic* Src)
{
  xAssert(Src!=nullptr);
  xAssert(isSameBitDepth(Src) && isSameSize(Src));
  if(m_Margin == Src->m_Margin)
  {  
    for(int32 CmpIdx=0; CmpIdx < m_NumComponents; CmpIdx++)
    {
      xPixelOps::Copy<PelType>(m_CmpPelBuffer[CmpIdx], Src->m_CmpPelBuffer[CmpIdx], m_CmpNumPels[CmpIdx]);
    }
    m_IsMarginExtended = Src->m_IsMarginExtended;;
  }
  else     
  { 
    for(int32 CmpIdx=0; CmpIdx < m_NumComponents; CmpIdx++)
    {
      eCmp CmpId = (eCmp)CmpIdx;
      xPixelOps::Copy<PelType>(getAddr(CmpId), Src->getAddr(CmpId), getStride(CmpId), Src->getStride(CmpId), getWidth(CmpId), getHeight(CmpId));
    }
    m_IsMarginExtended = false;
  }  
}
template <typename PelType> void xPic<PelType>::copyEx(xPic* Src, int32 DstX, int32 DstY, int32 SrcSX, int32 SrcSY, int32 SrcEX, int32 SrcEY)
{
  xAssert(Src!=nullptr && m_ChromaFormat != CrF_444);

  for(int32 CmpIdx=0; CmpIdx < m_NumComponents; CmpIdx++)
  {
    eCmp CmpId = (eCmp)CmpIdx;
    xPixelOps::CopyPart<PelType>(getAddr(CmpId), Src->getAddr(CmpId), getStride(CmpId), Src->getStride(CmpId), { DstX, DstY }, { SrcSX, SrcSY }, { SrcEX - SrcSX + 1, SrcEY - SrcSY + 1 });
  }
}
template <typename PelType> void xPic<PelType>::set(PelType Value)
{
  for(int32 CmpIdx=0; CmpIdx < m_NumComponents; CmpIdx++) { set(Value, (eCmp)CmpIdx); }
  m_IsMarginExtended = true;
}
template <typename PelType> void xPic<PelType>::set(PelType Value, eCmp CmpId)
{
  xMemsetX<PelType>(m_CmpPelBuffer[CmpId], (PelType)Value, m_CmpNumPels[CmpId]);
}

//=============================================================================================================================================================================
// Buffer modification
//=============================================================================================================================================================================
template <typename PelType> bool xPic<PelType>::bindBuffer(PelType* Buffer, eCmp CmpId)
{
  xAssert(Buffer!=nullptr); 
  if(m_CmpPelBuffer[CmpId]) { return false; }

  m_CmpPelBuffer[CmpId] = Buffer;
  m_CmpPelOrigin[CmpId] = m_CmpPelBuffer[CmpId] + (m_Margin * m_CmpPelStride[CMP_LM]) + m_Margin;

  return true;
}
template <typename PelType> PelType* xPic<PelType>::unbindBuffer(eCmp CmpId)
{
  if(m_CmpPelBuffer[CmpId]==nullptr) { return nullptr; }
  PelType* Tmp = m_CmpPelBuffer[CmpId];
  m_CmpPelBuffer[CmpId] = nullptr;
  m_CmpPelOrigin[CmpId] = nullptr;

  //offsets
  m_CmpPelOrigin[CMP_LM] = nullptr;
  m_CmpPelOrigin[CMP_CB] = nullptr;
  m_CmpPelOrigin[CMP_CR] = nullptr;

  return Tmp;
}
template <typename PelType> bool xPic<PelType>::swapBuffer(PelType*& Buffer, eCmp CmpId)
{
  xAssert(Buffer!=nullptr);
  if(m_CmpPelBuffer[CmpId]==nullptr) { return false; }
  std::swap(m_CmpPelBuffer[CmpId], Buffer); 
  m_CmpPelOrigin[CmpId] = m_CmpPelBuffer[CmpId] + (m_Margin * m_CmpPelStride[CMP_LM]) + m_Margin;

  return true;
}

//=============================================================================================================================================================================
// Resize and rescale
//=============================================================================================================================================================================
template <typename PelType> void xPic<PelType>::rescaleFast(xPic* Src, xThreadPoolInterface* ThreadPoolIntf)
{
  //same size of destination and source component
  if(isSameSize(Src)) { copy(Src); }
  //2x downsampling
  else if((getSize()*2) == (Src->getSize())) { rescaleD2Avg(Src); }

  //2x upsampling
  else if((getSize()) == (Src->getSize()*2)) { rescaleU2Bilinear(Src); }

  //more than 2x downsampling
  else if((2*m_Width<=Src->m_Width)&&(2*m_Height<=Src->m_Height))
  {
    xPic* Tmp = new xPic(Src->m_Width>>1, Src->m_Height>>1, m_Margin, m_BitDepth, m_ImageType, m_ChromaFormat);
    Tmp->rescaleD2Avg(Src);
    rescaleFast(Tmp);
    delete Tmp;
  }

  //more than 2x upsampling
  else if((m_Width>2*Src->m_Width)&&(m_Height>2*Src->m_Height))
  {
    xPic* Tmp = new xPic(Src->m_Width>>1, Src->m_Height>>1, m_Margin, m_BitDepth, m_ImageType, m_ChromaFormat);
    Tmp->rescaleFast(Src);
    rescaleU2Bilinear(Tmp);
    delete Tmp;
  }

  //no diadic resampling
  else rescaleBilinear(Src, ThreadPoolIntf);
}
template <typename PelType> void xPic<PelType>::rescaleChroma(xPic* Src, bool AllowBuffersSwap)
{  
  xAssert(m_ImageType==eImgTp::YUV && m_ChromaFormat!=CrF_400);
  //copy luma
  xPixelOps::Copy<PelType>(getAddr(CMP_LM), Src->getAddr(CMP_LM), getStride(CMP_LM), Src->getStride(CMP_LM), getWidth(CMP_LM), getHeight(CMP_LM));
  //rescale chroma
  for(int32 CmpIdx=1; CmpIdx < m_NumComponents; CmpIdx++)
  {
    eCmp CmpId = (eCmp)CmpIdx;
    int32 DstWidth  = getWidth (CmpId);
    int32 DstHeight = getHeight(CmpId);
    int32 SrcWidth  = Src->getWidth (CmpId);
    int32 SrcHeight = Src->getHeight(CmpId);

    if     ((  DstWidth==  SrcWidth)&&(  DstHeight==  SrcHeight)) xPixelOps::Copy<PelType>      (getAddr(CmpId), Src->getAddr(CmpId), getStride(CmpId), Src->getStride(CmpId), getWidth(CmpId), getHeight(CmpId)); //same size of destination and source component  
    else if((2*DstWidth==  SrcWidth)&&(2*DstHeight==  SrcHeight)) xResample::ResampleD2HVAverage(getAddr(CmpId), Src->getAddr(CmpId), getStride(CmpId), Src->getStride(CmpId), getWidth(CmpId), getHeight(CmpId)); //2x downsampling - Horizontal and Vertival - 4:4:4 --> 4:2:0  
    else if((  DstWidth==2*SrcWidth)&&(  DstHeight==2*SrcHeight)) xResample::ResampleU2HVNearest(getAddr(CmpId), Src->getAddr(CmpId), getStride(CmpId), Src->getStride(CmpId), getWidth(CmpId), getHeight(CmpId)); //2x upsampling   - Horizontal and Vertival - 4:2:0 --> 4:4:4
    else if((2*DstWidth==  SrcWidth)&&(  DstHeight==  SrcHeight)) xResample::ResampleD2HAverage (getAddr(CmpId), Src->getAddr(CmpId), getStride(CmpId), Src->getStride(CmpId), getWidth(CmpId), getHeight(CmpId)); //2x downsampling - Horizontal only         - 4:4:4 --> 4:2:2  
    else if((  DstWidth==2*SrcWidth)&&(  DstHeight==  SrcHeight)) xResample::ResampleU2HNearest (getAddr(CmpId), Src->getAddr(CmpId), getStride(CmpId), Src->getStride(CmpId), getWidth(CmpId), getHeight(CmpId)); //2x upsampling   - Horizontal only         - 4:2:2 --> 4:4:4
    else if((  DstWidth==  SrcWidth)&&(2*DstHeight==  SrcHeight)) xResample::ResampleD2VAverage (getAddr(CmpId), Src->getAddr(CmpId), getStride(CmpId), Src->getStride(CmpId), getWidth(CmpId), getHeight(CmpId)); //2x downsampling - Vertival only           - 4:2:2 --> 4:2:0  
    else if((  DstWidth==  SrcWidth)&&(  DstHeight==2*SrcHeight)) xResample::ResampleU2VNearest (getAddr(CmpId), Src->getAddr(CmpId), getStride(CmpId), Src->getStride(CmpId), getWidth(CmpId), getHeight(CmpId)); //2x upsampling   - Vertival only           - 4:2:0 --> 4:2:2
    else xAssert(0);
  }
}
template <typename PelType> void xPic<PelType>::rescaleNearest(xPic* Src)
{
  for(int32 CmpIdx=0; CmpIdx < m_NumComponents; CmpIdx++)
  {
    eCmp CmpId = (eCmp)CmpIdx;
    xResample::ResampleNearest(getAddr(CmpId), Src->getAddr(CmpId), getStride(CmpId), Src->getStride(CmpId), getWidth(CmpId), getHeight(CmpId), Src->getWidth(CmpId), Src->getHeight(CmpId));
  }
}
template <typename PelType> void xPic<PelType>::rescaleBilinear(xPic* Src, xThreadPoolInterface* ThreadPoolIntf)
{
  auto RescaleCmpBilinear = [ThreadPoolIntf](PelType* Dst, PelType* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight, int32 SrcWidth, int32 SrcHeight)
  {
    if(ThreadPoolIntf == nullptr || ThreadPoolIntf->getNumChunks()==0)
    {
      xResample::ResampleBilinear(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight, SrcWidth, SrcHeight);
    }
    else if(ThreadPoolIntf->getNumChunks() == 1)
    {
      ThreadPoolIntf->addWaitingTask(std::bind(xResample::ResampleBilinear<PelType>, Dst, Src, DstStride, SrcStride, DstWidth, DstHeight, SrcWidth, SrcHeight));
      ThreadPoolIntf->waitUntilTasksFinished(1);
    }
    else
    {
      int32 NumChunks = ThreadPoolIntf->getNumChunks();
      int32 NumThreads = NumChunks > 0 ? NumChunks : ThreadPoolIntf->getNumThreads();
      int32 LinesPerThread = DstHeight / NumThreads;
      int32 LinesRemaining = DstHeight % NumThreads;

      int32 NextBeginning = 0;
      for(int32 ThreadId=0; ThreadId < NumThreads; ThreadId++)
      {
        int32 NextHeight = LinesPerThread + ((LinesRemaining--) > 0);
        ThreadPoolIntf->addWaitingTask(std::bind(xResample::ResampleBilinearWindowed<PelType>, Dst, Src, DstStride, SrcStride, DstWidth, DstHeight, SrcWidth, SrcHeight, NextBeginning, NextBeginning + NextHeight));
        NextBeginning += NextHeight;
      }
      ThreadPoolIntf->waitUntilTasksFinished(NumThreads);
    }
  };

  for(int32 CmpIdx=0; CmpIdx < m_NumComponents; CmpIdx++)
  {
    eCmp CmpId = (eCmp)CmpIdx;
    RescaleCmpBilinear(getAddr(CmpId), Src->getAddr(CmpId), getStride(CmpId), Src->getStride(CmpId), getWidth(CmpId), getHeight(CmpId), Src->getWidth(CmpId), Src->getHeight(CmpId));
  }
}
template <typename PelType> void xPic<PelType>::rescaleU2Nearest(xPic* Src)
{
  for(int32 CmpIdx=0; CmpIdx < m_NumComponents; CmpIdx++)
  {
    eCmp CmpId = (eCmp)CmpIdx;
    xResample::ResampleU2HVNearest(getAddr(CmpId), Src->getAddr(CmpId), getStride(CmpId), Src->getStride(CmpId), getWidth(CmpId), getHeight(CmpId));
  }
}
template <typename PelType> void xPic<PelType>::rescaleU2Bilinear(xPic* Src)
{
  for(int32 CmpIdx=0; CmpIdx < m_NumComponents; CmpIdx++)
  {
    eCmp CmpId = (eCmp)CmpIdx;
    xResample::ResampleU2HVBilinear(getAddr(CmpId), Src->getAddr(CmpId), getStride(CmpId), Src->getStride(CmpId), getWidth(CmpId), getHeight(CmpId));
  }
}
template <typename PelType> void xPic<PelType>::rescaleD2Avg(xPic* Src)
{
  for(int32 CmpIdx=0; CmpIdx < m_NumComponents; CmpIdx++)
  {
    eCmp CmpId = (eCmp)CmpIdx;
    xResample::ResampleD2HVAverage(getAddr(CmpId), Src->getAddr(CmpId), getStride(CmpId), Src->getStride(CmpId), getWidth(CmpId), getHeight(CmpId));
  }
}

//=============================================================================================================================================================================
// Precision
//=============================================================================================================================================================================
template <typename PelType> void xPic<PelType>::convertBitDepth(xPic* Src, int32 NewBitDepth)
{
  xAssert(isSameSizeMargin(Src));
  if constexpr(!std::is_integral_v<PelType>) 
  {
    copy(Src);
  }
  else
  {
  int32 DeltaBitDepth = NewBitDepth - Src->m_BitDepth;

  if(DeltaBitDepth == 0) //no convertion
  {
    copy(Src);
  }
  else
  {
    if(Src == this) //inplace operation
    {
      m_BitDepth = NewBitDepth;
    }
      for(int32 CmpIdx = 0; CmpIdx < m_NumComponents; CmpIdx++)
    {
      xPixelOps::Shift(m_CmpPelBuffer[CmpIdx], Src->m_CmpPelBuffer[CmpIdx], m_CmpNumPels[CmpIdx], DeltaBitDepth);
      }
    }
  }
}

//=============================================================================================================================================================================
// Crop & extend
//=============================================================================================================================================================================
template <typename PelType> void xPic<PelType>::extendMargin(eCmpExtMode Mode)
{
  switch(Mode)
  {
    case eCmpExtMode::FillGrey:  for(int32 CmpId=0; CmpId<m_NumComponents; CmpId++) { xPixelOps::ExtendMarginSolid (getAddr((eCmp)CmpId), getStride((eCmp)CmpId), getWidth((eCmp)CmpId), getHeight((eCmp)CmpId), getMargin(), getMidPelValue()); } break;
    case eCmpExtMode::EdgePixel: for(int32 CmpId=0; CmpId<m_NumComponents; CmpId++) { xPixelOps::ExtendMarginEdge  (getAddr((eCmp)CmpId), getStride((eCmp)CmpId), getWidth((eCmp)CmpId), getHeight((eCmp)CmpId), getMargin()); } break;
    case eCmpExtMode::Mirror:    for(int32 CmpId=0; CmpId<m_NumComponents; CmpId++) { xPixelOps::ExtendMarginMirror(getAddr((eCmp)CmpId), getStride((eCmp)CmpId), getWidth((eCmp)CmpId), getHeight((eCmp)CmpId), getMargin()); } break;
    case eCmpExtMode::Bayer:     for(int32 CmpId=0; CmpId<m_NumComponents; CmpId++) { xPixelOps::ExtendMarginBayer (getAddr((eCmp)CmpId), getStride((eCmp)CmpId), getWidth((eCmp)CmpId), getHeight((eCmp)CmpId), getMargin()); } break;
    default: xAssert(0); return; break;
  }
  m_IsMarginExtended = true;
}
template <typename PelType> void xPic<PelType>::clearMargin()
{
  for(int32 CmpIdx=0; CmpIdx < m_NumComponents; CmpIdx++)
  {
    eCmp CmpId = (eCmp)CmpIdx;
    xPixelOps::ClearMargin(getAddr(CmpId), getStride(CmpId), getWidth(CmpId), getHeight(CmpId), getMargin());
  }
  m_IsMarginExtended = false;
}

//=============================================================================================================================================================================
// Colorspace
//=============================================================================================================================================================================
template <typename PelType> void xPic<PelType>::convertColorspace(xPic* Src, eClrSpc ColorSpace)
{
  switch(m_ImageType)
  {
    case eImgTp::YUV:
      switch(Src->m_ImageType)
      {
        case eImgTp::YUV:
          copy(Src); break;
        case eImgTp::RGB:
          convertRGB2YUV(Src, ColorSpace); break;
        case eImgTp::RGBA:
        case eImgTp::XYZ:
        case eImgTp::Bayer:
        default:
          xAssert(0); break;
      }
      break;
    case eImgTp::RGB:
      switch(Src->m_ImageType)
      {
        case eImgTp::YUV:
          convertYUV2RGB(Src, ColorSpace); break;
        case eImgTp::RGB:
          copy(Src); break;          
        case eImgTp::RGBA:
        case eImgTp::XYZ:
        case eImgTp::Bayer:
        default:
          xAssert(0); break;
      }
      break;
    case eImgTp::RGBA:
      xAssert(0);
      break;
    case eImgTp::XYZ:
      switch(Src->m_ImageType)
      {
        case eImgTp::YUV:
          xAssert(0); break;
        case eImgTp::RGB:
          convertRGB2XYZ(Src); break;        
        case eImgTp::RGBA:
        case eImgTp::XYZ:
          copy(Src); break;  
        case eImgTp::Bayer:
        default:
          xAssert(0); break;
      }
    case eImgTp::Bayer:
    default:
      xAssert(0); break;
  }
}
template <typename PelType> void xPic<PelType>::convertRGB2YUV(xPic* Src, eClrSpc ColorSpace, bool IncludingMargin)
{
  xAssert(isSameSizeMargin(Src) && isSameBitDepth(Src) && m_ImageType==eImgTp::YUV && Src->m_ImageType == eImgTp::RGB);
  if(m_Margin && !IncludingMargin)
  { 
    xColorSpace::convertRGB2YUV(getAddr(CMP_LM), getAddr(CMP_CB), getAddr(CMP_CR), Src->getAddr(CMP_R), Src->getAddr(CMP_G), Src->getAddr(CMP_B), getStride(CMP_0), Src->getStride(CMP_0), m_Width, m_Height, m_BitDepth, ColorSpace);
  }
  else
  { 
    xColorSpace::convertRGB2YUV(getAddr(CMP_LM), getAddr(CMP_CB), getAddr(CMP_CR), Src->getAddr(CMP_R), Src->getAddr(CMP_G), Src->getAddr(CMP_B), getArea(), m_BitDepth, ColorSpace); 
  }
}
template <typename PelType> void xPic<PelType>::convertYUV2RGB(xPic* Src, eClrSpc ColorSpace, bool IncludingMargin)
{
  xAssert(isSameSizeMargin(Src) && isSameBitDepth(Src) && m_ImageType==eImgTp::RGB && Src->m_ImageType == eImgTp::YUV);
  if(m_Margin && !IncludingMargin)
  { 
    xColorSpace::convertYUV2RGB(getAddr(CMP_R), getAddr(CMP_G), getAddr(CMP_B), Src->getAddr(CMP_LM), Src->getAddr(CMP_CB), Src->getAddr(CMP_CR), getStride(CMP_0), Src->getStride(CMP_0), m_Width, m_Height, m_BitDepth, ColorSpace);
  }
  else            
  { 
    xColorSpace::convertYUV2RGB(getAddr(CMP_R), getAddr(CMP_G), getAddr(CMP_B), Src->getAddr(CMP_LM), Src->getAddr(CMP_CB), Src->getAddr(CMP_CR), getArea(), m_BitDepth, ColorSpace);
  }
}
template <typename PelType> void xPic<PelType>::convertRGB2XYZ(xPic* Src, bool IncludingMargin)
{
  xAssert(isSameSizeMargin(Src) && isSameBitDepth(Src) && m_ImageType==eImgTp::XYZ && Src->m_ImageType == eImgTp::RGB);
  if(m_Margin && !IncludingMargin)
  { 
    xColorSpace::convertRGB2XYZ(getAddr(CMP_0), getAddr(CMP_1), getAddr(CMP_2), Src->getAddr(CMP_R), Src->getAddr(CMP_G), Src->getAddr(CMP_B), getStride(CMP_0), Src->getStride(CMP_0), m_Width, m_Height);
  }
  else
  {
    xColorSpace::convertRGB2XYZ(getAddr(CMP_0), getAddr(CMP_1), getAddr(CMP_2), Src->getAddr(CMP_R), Src->getAddr(CMP_G), Src->getAddr(CMP_B), getArea());
  }
}

//=============================================================================================================================================================================
// Distortion & PSNR
//=============================================================================================================================================================================
template <typename PelType> uint64V4 xPic<PelType>::calcDist(xPic* Ref, eDistMetric DistMetric)
{
  xAssert(Ref!=nullptr && isCompatible(Ref));
  uint64V4 Dist;
  for(int32 CmpIdx=0; CmpIdx < m_NumComponents; CmpIdx++)
  {
    Dist[CmpIdx] = calcDist(Ref, DistMetric, (eCmp)CmpIdx); 
  }
  return Dist;
}
template <typename PelType> doubleV4 xPic<PelType>::calcPSNR(xPic* Ref)
{
  xAssert(Ref!=nullptr && isCompatible(Ref));
  doubleV4 PSNR = { 0,0,0,0 };
  for(int32 CmpIdx=0; CmpIdx < m_NumComponents; CmpIdx++)
  {
    eCmp CmpId = (eCmp)CmpIdx;
    //uint64 SSD  = calcDist(Ref, eDistMetric::DIST_SSD, CmpId);
    //PSNR[CmpIdx] = xDistortion::CalcPSNR(SSD, getArea(CmpId), m_BitDepth); 
  }
  return PSNR; 
}
template <typename PelType> uint64 xPic<PelType>::calcDist(xPic* Ref, eDistMetric DistMetric, eCmp CmpId)
{
  xAssert(Ref!=nullptr && isCompatible(Ref));
  //return (uint64)xDistortion::CalcDist(Ref->getAddr(CmpId), getAddr(CmpId), Ref->getStride(CmpId), getStride(CmpId), getWidth(CmpId), getHeight(CmpId), DistMetric); 
  return (uint64)0;
}
template <typename PelType> double xPic<PelType>::calcPSNR(xPic* Ref, eCmp CmpId)
{
  xAssert(Ref!=nullptr && isCompatible(Ref));
  //uint64 SSD  = calcDist(Ref, eDistMetric::DIST_SSD, CmpId);
  //double PSNR = xDistortion::CalcPSNR(SSD, getArea(CmpId), m_BitDepth); 
  //return PSNR; 
  return (uint64)0;
}
template <typename PelType> uint64 xPic<PelType>::calcWindowedDist(xPic* Ref, int32 OriginX, int32 OriginY, int32 Width, int32 Height, eDistMetric DistMetric, eCmp CmpId)
{
  xAssert(Ref!=nullptr && isCompatible(Ref));
  PelType* RefPtr = Ref->getAddr(CmpId) + OriginY*Ref->getStride(CmpId) + OriginX;
  PelType* OrgPtr =      getAddr(CmpId) + OriginY*     getStride(CmpId) + OriginX;
  //return (uint64)xDistortion::CalcDist(RefPtr, OrgPtr, Ref->getStride(CmpId), getStride(CmpId), Width, Height, DistMetric);
  return (uint64)0;
}
template <typename PelType> double xPic<PelType>::calcWindowedPSNR(xPic* Ref, int32 OriginX, int32 OriginY, int32 Width, int32 Height, eCmp CmpId)
{
  xAssert(Ref!=nullptr && isCompatible(Ref));
  uint64 SSD = calcWindowedDist(Ref, OriginX, OriginY, Width, Height, DIST_SSD, CmpId);
  //double PSNR = xDistortion::CalcPSNR(SSD, Width*Height, m_BitDepth); 
  //return PSNR; 
  return (uint64)0;
}
template <typename PelType> uint64 xPic<PelType>::calcSAD(xPic* Ref, eCmp CmpId)
{
  xAssert(Ref!=nullptr && isCompatible(Ref));
  //if(m_Margin) { return (uint64)xDistortion::CalcSAD(Ref->getAddr(CmpId), getAddr(CmpId), Ref->getStride(CmpId), getStride(CmpId), getWidth(CmpId), getHeight(CmpId)); }
  //else         { return (uint64)xDistortion::CalcSAD(Ref->getAddr(CmpId), getAddr(CmpId), getArea(CmpId)); }
  return (uint64)0;
}
template <typename PelType> uint64 xPic<PelType>::calcSSD(xPic* Ref, eCmp CmpId)
{
  xAssert(Ref!=nullptr && isCompatible(Ref));
  //if(m_Margin) { return (uint64)xDistortion::CalcSSD(Ref->getAddr(CmpId), getAddr(CmpId), Ref->getStride(CmpId), getStride(CmpId), getWidth(CmpId), getHeight(CmpId)); }
  //else         { return (uint64)xDistortion::CalcSSD(Ref->getAddr(CmpId), getAddr(CmpId), getArea(CmpId)); }
  return (uint64)0;
}

//=============================================================================================================================================================================
// Load and store RAW
//=============================================================================================================================================================================
template <typename PelType> void xPic<PelType>::getRAW(xFile& File)
{
  xAssert(File.valid());
  xPacked Packed;
  Packed.create(m_Width, m_Height, m_BitDepth, eImgTp::YUV, ePckTp::Planar, m_ChromaFormat, eCmpO::YUV);
  int32  Size = Packed.getPackedSize();
  uint8* Buff = Packed.getPackedBuffer();
  [[maybe_unused]]int32  Read = File.read(Buff, Size);
  xAssert(Read==Size);
  Packed.unpack(this);
  Packed.destroy();
}
template <typename PelType> void xPic<PelType>::putRAW(xFile& File)
{
  xAssert(File.valid());
  xPacked Packed;
  Packed.create(m_Width, m_Height, m_BitDepth, eImgTp::YUV, ePckTp::Planar, m_ChromaFormat, eCmpO::YUV);
  Packed.pack(this);
  int32  Size = Packed.getPackedSize();
  uint8* Buff = Packed.getPackedBuffer();
  [[maybe_unused]]int32  Written = File.write(Buff, Size);
  xAssert(Written==Size);
  Packed.destroy();
}

//=============================================================================================================================================================================
// Load and store BMP
//=============================================================================================================================================================================
template <typename PelType> void xPic<PelType>::getBMP(xFile& File)
{
  xBMP::readFromFile(this, &File);
}
template <typename PelType> void xPic<PelType>::putBMP(xFile& File)
{
  xBMP::writeToFile(&File, this);
}

//=============================================================================================================================================================================

//template class xPic<uint8 >;
//template class xPic< int8 >;
template class xPic<uint16>;
template class xPic< int16>;
//template class xPic<uint32>;
//template class xPic< int32>;
//template class xPic<uint64>;
//template class xPic< int64>;
//template class xPic<float >;
//template class xPic<double>;

//=============================================================================================================================================================================
// xPicYUVTank
//=============================================================================================================================================================================

template <typename PelType> void xPicTank<PelType>::create(int32 Width, int32 Height, int32 Margin, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat, int32 InitSize)
{
  m_Mutex.lock();
  m_Width         = Width;    
  m_Height        = Height;  
  m_Margin        = Margin;
  m_BitDepth      = BitDepth;
  m_ImageType     = ImageType;
  m_ChromaFormat  = ChromaFormat;
  m_CreatedUnits  = 0;
  m_SizeLimit     = uintSize_max;

  for(int32 i=0; i<InitSize; i++) { xCreateNewUnit(); }
  m_Mutex.unlock();
}
template <typename PelType> void xPicTank<PelType>::put(xPic<PelType>* Pic)
{
  m_Mutex.lock();
  xAssert(Pic != nullptr);
  Pic->setPOC(NOT_VALID);
  m_Buffer.push_back(Pic);
  if(m_Buffer.size() > m_SizeLimit) { xDestroyUnit(); }
  m_Mutex.unlock();
}
template <typename PelType> xPic<PelType>* xPicTank<PelType>::get()
{
  m_Mutex.lock();
  if(m_Buffer.empty()) { xCreateNewUnit(); }
  xPic<PelType>* Pic = m_Buffer.back();
  m_Buffer.pop_back();
  m_Mutex.unlock();
  return Pic;
}
template <typename PelType> void xPicTank<PelType>::xCreateNewUnit()
{
  xPic<PelType>* Tmp = new xPic<PelType>;
  Tmp->create(m_Width, m_Height, m_Margin, m_BitDepth, m_ImageType, m_ChromaFormat); 
  m_Buffer.push_back(Tmp);
  m_CreatedUnits++;
}
template <typename PelType> void xPicTank<PelType>::xDestroyUnit()
{
  if(!m_Buffer.empty())
  {
    xPic<PelType>* Tmp = m_Buffer.back();
    m_Buffer.pop_back();
    Tmp->destroy();
    delete Tmp;
  }
}

//=============================================================================================================================================================================

//template class xPicTank<uint8 >;
//template class xPicTank< int8 >;
template class xPicTank<uint16>;
template class xPicTank< int16>;
//template class xPicTank<uint32>;
//template class xPicTank< int32>;
//template class xPicTank<uint64>;
//template class xPicTank< int64>;
//template class xPicTank<float >;
//template class xPicTank<double>;

//=============================================================================================================================================================================
// Base functions
//=============================================================================================================================================================================
template <typename PelType> xPicI<PelType>::xPicI() 
{  
  m_Width           = NOT_VALID;
  m_Height          = NOT_VALID;
  m_Margin          = NOT_VALID;
  m_BitDepth        = NOT_VALID;
  m_ImageType       = eImgTp::INVALID;
  m_NumComponents   = 0;

  m_NumCmpPels      = NOT_VALID;
  m_PelStride       = NOT_VALID;
  m_PelBuffer       = nullptr;
  m_PelOrigin       = nullptr;

  m_POC             = NOT_VALID;  
  m_Timestamp       = NOT_VALID;  
  m_Name.clear();

  m_IsMarginExtended = false;
}
template <typename PelType> void xPicI<PelType>::create(int32 Width, int32 Height, int32 Margin, int32 BitDepth, eImgTp ImageType)
{
  m_Width           = Width;
  m_Height          = Height;
  m_Margin          = Margin;
  m_BitDepth        = BitDepth;
  m_ImageType       = ImageType;

  switch(ImageType)
  {
    case eImgTp::RGB : case eImgTp::XYZ : case eImgTp::YUV :                    m_NumComponents = 3; break;
    case eImgTp::RGBA: case eImgTp::RGBD: case eImgTp::YUVA: case eImgTp::YUVD: m_NumComponents = 4; break;
    case eImgTp::Bayer:                                                         m_NumComponents = 1; break;
    default: xAssert(0); break;
  }

  m_NumCmpPels      = (Width + (Margin << 1)) * (Height + (m_Margin << 1));
  m_PelStride       = (Width + (m_Margin<<1)) * c_NumberOfComponents;

  //alloc
  m_PelBuffer = (PelType*)xAlignedMalloc(m_NumCmpPels * c_NumberOfComponents * sizeof(PelType), X_AlignmentPel);
  m_PelOrigin = m_PelBuffer + (m_Margin * m_PelStride) + m_Margin;

  m_POC              = NOT_VALID;  
  m_Timestamp        = NOT_VALID;  
  m_Name.clear();

  m_IsMarginExtended = false;
}
template <typename PelType> void xPicI<PelType>::destroy()
{
  if(m_PelBuffer != nullptr) { xAlignedFree(m_PelBuffer); m_PelBuffer = nullptr; }
}
template <typename PelType> void xPicI<PelType>::duplicate(xPicI* Pic)
{
  create(Pic);
  copy(Pic);
  m_POC           = Pic->m_POC;
  m_Timestamp     = Pic->m_Timestamp;
  m_Name          = Pic->m_Name;
}
template <typename PelType> void xPicI<PelType>::clear()
{
  ::memset(m_PelBuffer, 0, m_NumCmpPels * c_NumberOfComponents * sizeof(PelType));
  m_POC              = NOT_VALID;  
  m_Timestamp        = NOT_VALID;  
  m_Name.clear();
  m_IsMarginExtended = false;
}
template <typename PelType> void xPicI<PelType>::copy(xPicI* Src)
{
  xAssert(Src!=nullptr && Src->m_PelBuffer!=nullptr && m_PelBuffer!=nullptr);
  xAssert(isSameSize(Src) && isSameBitDepth(Src) && isSameType(Src));
  if(m_Margin == Src->m_Margin)
  {  
    xPixelOps::Copy<PelType>(m_PelBuffer, Src->m_PelBuffer, m_NumCmpPels << c_Log2NumberOfComponents);
    m_IsMarginExtended = Src->m_IsMarginExtended;;
  }
  else     
  { 
    xPixelOps::Copy<PelType>(m_PelOrigin, Src->m_PelOrigin, m_PelStride, Src->m_PelStride, m_Width << c_Log2NumberOfComponents, m_Height);
    m_IsMarginExtended = false;
  }  
}
template <typename PelType> void xPicI<PelType>::copyEx(xPicI* Src, int32 DstX, int32 DstY, int32 SrcSX, int32 SrcSY, int32 SrcEX, int32 SrcEY)
{
  xAssert(Src!=nullptr && Src->m_PelBuffer!=nullptr && m_PelBuffer!=nullptr);

  xPixelOps::CopyPart<PelType>(m_PelOrigin, Src->m_PelOrigin, m_PelStride, Src->m_PelStride, int32V2{ DstX, DstY }, int32V2{ SrcSX, SrcSY }, int32V2{ SrcEX - SrcSX + 1, SrcEY - SrcSY + 1 });
}
template <typename PelType> void xPicI<PelType>::set(PelType Value)
{
  xMemsetX<PelType>(m_PelBuffer, (PelType)Value, m_NumCmpPels << c_Log2NumberOfComponents);
  m_IsMarginExtended = true;
}
template <typename PelType> void xPicI<PelType>::set(VecType Value)
{
  xMemsetX<VecType>((VecType*)m_PelBuffer, Value, m_NumCmpPels);
  m_IsMarginExtended = true;
}
template <typename PelType> void xPicI<PelType>::set(PelType Value, eCmp CmpId)
{
  xAssert(0);
}

//=============================================================================================================================================================================

//template class xPicI<uint8 >;
//template class xPicI< int8 >;
template class xPicI<uint16>;
template class xPicI< int16>;
//template class xPicI<uint32>;
//template class xPicI< int32>;
//template class xPicI<uint64>;
//template class xPicI< int64>;
//template class xPicI<float >;
//template class xPicI<double>;

//=============================================================================================================================================================================
// Buffer modification
//=============================================================================================================================================================================
template <typename PelType> bool xPicI<PelType>::bindBuffer(PelType* Buffer)
{
  xAssert(Buffer!=nullptr); 
  if(m_PelBuffer) { return false; }
  m_PelBuffer = Buffer;
  return true;
}
template <typename PelType> PelType* xPicI<PelType>::unbindBuffer()
{
  if(m_PelBuffer==nullptr) { return nullptr; }
  PelType* Tmp = m_PelBuffer;
  m_PelBuffer = nullptr;
  return Tmp;
}
template <typename PelType> bool xPicI<PelType>::swapBuffer(PelType*& Buffer)
{
  xAssert(Buffer!=nullptr);
  if(m_PelBuffer==nullptr) { return false; }
  std::swap(m_PelBuffer, Buffer); 
  return true;
}

//=============================================================================================================================================================================
// xPlane -  Crop & extend
//=============================================================================================================================================================================
template <typename PelType> void xPicI<PelType>::extendMargin(eCmpExtMode Mode)
{
  if(getMargin() != 0)
  {
    switch(Mode)
    {
      case eCmpExtMode::FillGrey : xPixelOps::ExtendMarginSolid (getAddrVec(), getStrideVec(), getWidth(), getHeight(), getMargin(), VecType(getMidPelValue())); break;
      case eCmpExtMode::EdgePixel: xPixelOps::ExtendMarginEdge  (getAddrVec(), getStrideVec(), getWidth(), getHeight(), getMargin()                           ); break;
      case eCmpExtMode::Mirror   : xPixelOps::ExtendMarginMirror(getAddrVec(), getStrideVec(), getWidth(), getHeight(), getMargin()                           ); break;
      case eCmpExtMode::Bayer    : xPixelOps::ExtendMarginBayer (getAddrVec(), getStrideVec(), getWidth(), getHeight(), getMargin()                           ); break;
      default: xAssert(0); return; break;
    }
  }
  m_IsMarginExtended = true;
}
template <typename PelType> void xPicI<PelType>::clearMargin()
{ 
  if(getMargin() != 0)
  {
    xPixelOps::ClearMargin(getAddrVec(), getStrideVec(), getWidth(), getHeight(), getMargin());
  }
  m_IsMarginExtended = false;
}

//=============================================================================================================================================================================
// Convertion
//=============================================================================================================================================================================
template <typename PelType> void xPicI<PelType>::rearrangeFromPlanar(const xPic<PelType>* Planar)
{
  //xAssert(Planar->isCompatible(m_Width, m_Height, m_BitDepth, m_ImageType, eCrF::CrF_444));

  if(m_NumComponents == 4)
  {
    xPixelOps::SOA4toAOS4<PelType>(m_PelOrigin, Planar->getAddr(CMP_0), Planar->getAddr(CMP_1), Planar->getAddr(CMP_2), Planar->getAddr(CMP_3), m_PelStride, Planar->getStride(CMP_0), m_Width, m_Height);
  }
  else
  {
    xPixelOps::SOA3toAOS4<PelType>(m_PelOrigin, Planar->getAddr(CMP_0), Planar->getAddr(CMP_1), Planar->getAddr(CMP_2), (PelType)0, m_PelStride, Planar->getStride(CMP_0), m_Width, m_Height);
  }
}
template <typename PelType> void xPicI<PelType>::rearrangeToPlanar(xPic<PelType>* Planar)
{
  if(m_NumComponents == 4)
  {
    xPixelOps::AOS4toSOA4<PelType>(Planar->getAddr(CMP_0), Planar->getAddr(CMP_1), Planar->getAddr(CMP_2), Planar->getAddr(CMP_3), m_PelOrigin, Planar->getStride(CMP_0), m_PelStride, m_Width, m_Height);
  }
  else
  {
    xPixelOps::AOS4toSOA3<PelType>(Planar->getAddr(CMP_0), Planar->getAddr(CMP_1), Planar->getAddr(CMP_2), m_PelOrigin, Planar->getStride(CMP_0), m_PelStride, m_Width, m_Height);
  }
}

//=============================================================================================================================================================================
// xPicITank
//=============================================================================================================================================================================
template <typename PelType> void xPicITank<PelType>::create(int32 Width, int32 Height, int32 Margin, int32 BitDepth, eImgTp ImageType, int32 InitSize)
{
  m_Mutex.lock();
  m_Width         = Width;    
  m_Height        = Height;  
  m_Margin        = Margin;
  m_BitDepth      = BitDepth;
  m_ImageType     = ImageType;
  m_CreatedUnits  = 0;
  m_SizeLimit     = uintSize_max;

  for(int32 i=0; i<InitSize; i++) { xCreateNewUnit(); }
  m_Mutex.unlock();
}
template <typename PelType> void xPicITank<PelType>::put(xPicI<PelType>* Pic)
{
  m_Mutex.lock();
  xAssert(Pic != nullptr);
  Pic->setPOC(NOT_VALID);
  m_Buffer.push_back(Pic);
  if(m_Buffer.size() > m_SizeLimit) { xDestroyUnit(); }
  m_Mutex.unlock();
}
template <typename PelType> xPicI<PelType>* xPicITank<PelType>::get()
{
  m_Mutex.lock();
  if(m_Buffer.empty()) { xCreateNewUnit(); }
  xPicI<PelType>* Pic = m_Buffer.back();
  m_Buffer.pop_back();
  m_Mutex.unlock();
  return Pic;
}
template <typename PelType> void xPicITank<PelType>::xCreateNewUnit()
{
  xPicI<PelType>* Tmp = new xPicI<PelType>;
  Tmp->create(m_Width, m_Height, m_Margin, m_BitDepth, m_ImageType); 
  m_Buffer.push_back(Tmp);
  m_CreatedUnits++;
}
template <typename PelType> void xPicITank<PelType>::xDestroyUnit()
{
  if(!m_Buffer.empty())
  {
    xPicI<PelType>* Tmp = m_Buffer.back();
    m_Buffer.pop_back();
    Tmp->destroy();
    delete Tmp;
  }
}

//=============================================================================================================================================================================

//template class xPicITank<uint8 >;
//template class xPicITank< int8 >;
template class xPicITank<uint16>;
template class xPicITank< int16>;
//template class xPicITank<uint32>;
//template class xPicITank< int32>;
//template class xPicITank<uint64>;
//template class xPicITank< int64>;
//template class xPicITank<float >;
//template class xPicITank<double>;

//=============================================================================================================================================================================

} //end of namespace AVLib