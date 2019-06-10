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

#define AVlib_xPlane_IMPLEMENTATION
#include "xPlane.h"

namespace AVlib {

//=============================================================================================================================================================================
// xPlane - base functions
//=============================================================================================================================================================================
template <typename PelType> xPlane<PelType>::xPlane()
{
  m_Width            = NOT_VALID;    
  m_Height           = NOT_VALID;  
  m_Margin           = NOT_VALID;
  m_BitDepth         = NOT_VALID;

  m_NumPelsInBuffer  = NOT_VALID;
  m_PelBuffer        = nullptr;
  m_PelOrigin        = nullptr;
  m_PelStride        = NOT_VALID;

  m_POC              = NOT_VALID;  
  m_Timestamp        = NOT_VALID;  
  m_Name.clear();

  m_IsMarginExtended = false;
}
template <typename PelType> void xPlane<PelType>::create(int32 Width, int32 Height, int32 Margin, int32 BitDepth)
{
  xAssert(Width>=0 && Height>=0 && Margin>=0);
  if constexpr (std::is_integral_v<PelType>) { xAssert(BitDepth>0); }

  m_Width            = Width;    
  m_Height           = Height;  
  m_Margin           = Margin;
  m_BitDepth         = BitDepth;

  m_NumPelsInBuffer  = (Width + 2*Margin)*(Height + 2*Margin);
  m_PelBuffer        = nullptr;
  m_PelOrigin        = nullptr;
  m_PelStride        = Width + 2*Margin;

  m_POC              = NOT_VALID;  
  m_Timestamp        = NOT_VALID;  
  m_Name.clear();

  m_IsMarginExtended = false;

  if(Width==0 && Height==0) return;

  m_PelBuffer    = (PelType*)xAlignedMalloc((m_NumPelsInBuffer)*sizeof(PelType), X_AlignmentPel);
  m_PelOrigin    = m_PelBuffer + Margin*m_PelStride + Margin;
  std::memset(m_PelBuffer, 0, (m_NumPelsInBuffer)*sizeof(PelType));
}
template <typename PelType> void xPlane<PelType>::destroy()
{
  m_Width            = NOT_VALID;    
  m_Height           = NOT_VALID;  
  m_Margin           = NOT_VALID;
  m_BitDepth         = NOT_VALID;

  m_NumPelsInBuffer  = NOT_VALID;
  if(m_PelBuffer != nullptr) { xAlignedFree(m_PelBuffer); m_PelBuffer=nullptr; }
  m_PelOrigin        = nullptr;
  m_PelStride        = NOT_VALID;

  m_POC              = NOT_VALID;  
  m_Timestamp        = NOT_VALID;  
  m_Name.clear();

  m_IsMarginExtended = false;
}
template <typename PelType> void xPlane<PelType>::duplicate(xPlane* RefPlane)
{
  create(RefPlane);
  m_POC           = RefPlane->m_POC;
  m_Timestamp     = RefPlane->m_Timestamp;
  m_Name          = RefPlane->m_Name;
  copy(RefPlane);
}
template <typename PelType> void xPlane<PelType>::clear()
{
  m_POC              = NOT_VALID;  
  m_Timestamp        = NOT_VALID;  
  m_Name.clear();
  std::memset((void*)m_PelBuffer, 0, m_NumPelsInBuffer * sizeof(PelType));
  m_IsMarginExtended = false;
}
template <typename PelType> void xPlane<PelType>::resize(int32 Width, int32 Height, int32 Margin, int32 BitDepth)
{
  xAssert(Width>=0 && Height>=0 && Margin>=0);
  if constexpr (std::is_integral_v<PelType>) { xAssert(BitDepth>0); }

  m_Width            = Width;    
  m_Height           = Height;  
  m_Margin           = Margin;
  m_BitDepth         = BitDepth;

  m_NumPelsInBuffer  = NOT_VALID;
  if(m_PelBuffer != nullptr) { xAlignedFree(m_PelBuffer); m_PelBuffer=nullptr; }
  m_PelOrigin        = nullptr;
  m_PelStride        = NOT_VALID;

  if(Width==0 && Height==0) return;

  m_PelBuffer    = (PelType*)xAlignedMalloc((m_NumPelsInBuffer)*sizeof(PelType), X_AlignmentPel);
  m_PelOrigin    = m_PelBuffer + Margin*m_PelStride + Margin;
  std::memset(m_PelBuffer, 0, (m_NumPelsInBuffer)*sizeof(PelType));
}
template <typename PelType> void xPlane<PelType>::copy(xPlane* Src)
{
  xAssert(Src!=nullptr && Src->m_PelBuffer!=nullptr && m_PelBuffer!=nullptr);
  xAssert(isSameBitDepth(Src) && isSameSize(Src));
  if(m_Margin == Src->m_Margin)
  { 
    xPixelOps::Copy<PelType>(m_PelBuffer, Src->m_PelBuffer, m_NumPelsInBuffer);
    m_IsMarginExtended = Src->m_IsMarginExtended; 
  }
  else    
  {
    xPixelOps::Copy<PelType>(m_PelOrigin, Src->m_PelOrigin, m_PelStride, Src->m_PelStride, m_Width, m_Height);
    m_IsMarginExtended = false; 
  }  
}
template <typename PelType> void xPlane<PelType>::copyEx(xPlane* Src, int32 DstX, int32 DstY, int32 SrcSX, int32 SrcSY, int32 SrcEX, int32 SrcEY)
{
  xAssert(Src!=nullptr && Src->m_PelBuffer!=nullptr && m_PelBuffer!=nullptr);
  xPixelOps::CopyPart<PelType>(m_PelOrigin, Src->m_PelOrigin, m_PelStride, Src->m_PelStride, int32V2{ DstX, DstY }, int32V2{ SrcSX, SrcSY }, int32V2{ SrcEX - SrcSX + 1, SrcEY - SrcSY + 1 });
}
template <typename PelType> void xPlane<PelType>::set(PelType Value)
{
  if(Value == (PelType)0)
  {
    memset(m_PelBuffer, 0, m_NumPelsInBuffer * sizeof(PelType));
  }
  else if(std::is_integral_v<PelType> && Value == (PelType)-1)
  {
    memset(m_PelBuffer, 0xFF, m_NumPelsInBuffer * sizeof(PelType));
  }
  else
  {
    xMemsetX<PelType>(m_PelBuffer, (PelType)Value, m_NumPelsInBuffer);
  }
  m_IsMarginExtended = true;
}

//=============================================================================================================================================================================

template class xPlane<uint8 >;
template class xPlane< int8 >;
template class xPlane<uint16>;
template class xPlane< int16>;
template class xPlane<uint32>;
template class xPlane< int32>;
template class xPlane<uint64>;
template class xPlane< int64>;
template class xPlane<float >;
template class xPlane<double>;

//=============================================================================================================================================================================
// xPlaneTank
//=============================================================================================================================================================================

template <typename PelType> void xPlaneTank<PelType>::create(int32 Width, int32 Height, int32 Margin, int32 BitDepth, uintSize InitSize)
{
  m_Mutex.lock();
  m_Width        = Width;    
  m_Height       = Height;  
  m_Margin       = Margin;
  m_BitDepth     = BitDepth;
  m_CreatedUnits = 0;
  m_SizeLimit    = uintSize_max;

  for(uintSize i=0; i<InitSize; i++) { xCreateNewUnit(); }
  m_Mutex.unlock();
}
template <typename PelType> void xPlaneTank<PelType>::put(xPlane<PelType>* Plane)
{
  m_Mutex.lock();
  xAssert(Plane != nullptr);
  Plane->setPOC      (NOT_VALID);
  Plane->setTimestamp(NOT_VALID);
  m_Buffer.push_back(Plane);
  if(m_Buffer.size() > m_SizeLimit) { xDestroyUnit(); }
  m_Mutex.unlock();
}
template <typename PelType> xPlane<PelType>* xPlaneTank<PelType>::get()
{
  m_Mutex.lock();
  if(m_Buffer.empty()) { xCreateNewUnit(); }
  xPlane<PelType>* Plane = m_Buffer.back();
  m_Buffer.pop_back();
  m_Mutex.unlock();
  return Plane;
}
template <typename PelType> void xPlaneTank<PelType>::xCreateNewUnit()
{
  xPlane<PelType>* Tmp = new xPlane<PelType>;
  Tmp->create(m_Width, m_Height, m_Margin, m_BitDepth); 
  m_Buffer.push_back(Tmp);
  m_CreatedUnits++;
}
template <typename PelType> void xPlaneTank<PelType>::xDestroyUnit()
{
  if(!m_Buffer.empty())
  {
    xPlane<PelType>* Tmp = m_Buffer.back();
    m_Buffer.pop_back();
    Tmp->destroy();
    delete Tmp;
  }
}

//=============================================================================================================================================================================

template class xPlaneTank<uint8 >;
template class xPlaneTank< int8 >;
template class xPlaneTank<uint16>;
template class xPlaneTank< int16>;
template class xPlaneTank<uint32>;
template class xPlaneTank< int32>;
template class xPlaneTank<uint64>;
template class xPlaneTank< int64>;
template class xPlaneTank<float >;
template class xPlaneTank<double>;

//=============================================================================================================================================================================

} //end of namespace AVLib