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

#include "xLicense.h"
#include "xCommon.h"
#include "xPixelOps.h"
#include <typeinfo>
#include <type_traits>
#include <mutex>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace AVlib {

//=============================================================================================================================================================================

template <typename PelType> class xPlaneTank;

//=============================================================================================================================================================================
// xPlane
//=============================================================================================================================================================================
template <typename PelType> class xPlane
{
public:
  typedef PelType       T;
  typedef xMemMap2D<T>  MemMap;
  typedef xPlaneTank<T> tCorespondingTank;

protected:
  int32       m_Width;
  int32       m_Height;
  int32       m_Margin;
  int32       m_BitDepth;

  int32       m_NumPelsInBuffer; //size of picture buffer (including margin)
  PelType*    m_PelBuffer;       //picture buffer
  PelType*    m_PelOrigin;       //pel origin, pel access -> m_PelOrg[y*m_PelStride + x]
  int32       m_PelStride;       //pel stride

  int64       m_POC;
  int64       m_Timestamp;
  std::string m_Name;
  
  bool        m_IsMarginExtended;

public:
  //Base functions
  xPlane                           ();
  xPlane                           (int32 Width, int32 Height, int32 Margin, int32 BitDepth) { create(Width, Height, Margin, BitDepth); }
  xPlane                           (int32V2 Size,              int32 Margin, int32 BitDepth) { create(Size,          Margin, BitDepth); }
  xPlane                           (xPlane* RefPlane) { create(RefPlane); }
  ~xPlane                          () { destroy(); }
                                   
  void        create               (int32 Width, int32 Height, int32 Margin, int32 BitDepth);
  void        create               (int32V2 Size,              int32 Margin, int32 BitDepth) { create(Size.getX(), Size.getY(), Margin, BitDepth); }
  void        create               (xPlane* RefPlane) { create(RefPlane->m_Width, RefPlane->m_Height, RefPlane->m_Margin, RefPlane->m_BitDepth); }
  void        destroy              ();
  void        duplicate            (xPlane* RefPlane);
  void        init                 () { m_IsMarginExtended = false; }
  void        clear                ();
  void        resize               (int32 Width, int32 Height, int32 Margin, int32 BitDepth);
  void        copy                 (xPlane* Src);
  void        copyEx               (xPlane* Src, int32 DstX, int32 DstY, int32 SrcSX, int32 SrcSY, int32 SrcEX, int32 SrcEY);
  void        set                  (PelType Value);
                                   
  //Interfaces                     
  int32       getWidth             (                ) const { return m_Width; }
  int32       getHeight            (                ) const { return m_Height; }
  int32V2     getSize              (                ) const { return {m_Width, m_Height}; }
  int32       getArea              (                ) const { return m_Width*m_Height; }
  int32       getMargin            (                ) const { return m_Margin; }
  int32       getBitDepth          (                ) const { return m_BitDepth; }
                                                    
  void        setPOC               (int64 POC       )       { m_POC = POC; }
  int64       getPOC               (                ) const { return m_POC; }
  void        setTimestamp         (int64 Timestamp )       { m_Timestamp = Timestamp; }
  int64       getTimestamp         (                ) const { return m_Timestamp; }
  void        setName              (std::string Name)       { m_Name = Name; }
  std::string getName              (                ) const { return m_Name; }
                                   
  //Tool functions                 
  ePelType    getPelType           () const { return DeterminePelType<PelType>(); }
  PelType     getMaxPelValue       () const { if constexpr (std::is_integral_v<PelType>) { return xBitDepth2MaxValue(m_BitDepth);} else { return (PelType)1.0; } }
  PelType     getMidPelValue       () const { if constexpr (std::is_integral_v<PelType>) { return ((PelType)1<<(m_BitDepth-1)); } else { return (PelType)0.5; } }
  PelType     getMinPelValue       () const { return (PelType)0; }
                                   
  //Compatibility functions        
  bool        isSameSize           (int32 Width, int32 Height) const { return (m_Width == Width && m_Height == Height); }
  bool        isSameMargin         (int32 Margin) const { return (m_Margin == Margin); }
  bool        isSameSizeMargin     (int32 Width, int32 Height, int32 Margin) const { return (isSameSize(Width, Height) && isSameMargin(Margin)); }
  bool        isSameBitDepth       (int32 BitDepth) const { return (m_BitDepth == BitDepth); }
  bool        isSamePelType        (ePelType Type) const { return getPelType() == Type; }
  bool        isCompatible         (int32 Width, int32 Height, int32 BitDepth) const { return (isSameSize(Width, Height) && isSameBitDepth(BitDepth)); }
  bool        isCompatible         (int32 Width, int32 Height, int32 Margin, int32 BitDepth) const { return (isSameSizeMargin(Width, Height, Margin) && isSameBitDepth(BitDepth)); }
                                   
  bool        isSameSize           (xPlane* RefPlane) const { xAssert(RefPlane!=nullptr); return isSameSize(RefPlane->m_Width, RefPlane->m_Height); }
  bool        isSameMargin         (xPlane* RefPlane) const { xAssert(RefPlane!=nullptr); return isSameMargin(RefPlane->m_Margin); }
  bool        isSameSizeMargin     (xPlane* RefPlane) const { xAssert(RefPlane!=nullptr); return isSameSizeMargin(RefPlane->m_Width, RefPlane->m_Height, RefPlane->m_Margin); }
  bool        isSameBitDepth       (xPlane* RefPlane) const { xAssert(RefPlane!=nullptr); return isSameBitDepth(RefPlane->m_BitDepth); }
  bool        isCompatible         (xPlane* RefPlane) const { xAssert(RefPlane!=nullptr); return isCompatible(RefPlane->m_Width, RefPlane->m_Height, RefPlane->m_Margin, RefPlane->m_BitDepth); }
                                   
  //Buffer modification            
  //bool        bindBuffer           (PelType*  Buffer);
  //PelType*    unbindBuffer         (                );
  //bool        swapBuffer           (PelType*& Buffer);
  int32       getPelsInBuff        (                ) const { return m_NumPelsInBuffer; }
  int32       getBuffSize          (                ) const { return m_NumPelsInBuffer*sizeof(PelType); }
                                   
  //Crop & extend                  
  void        extendMargin         (eCmpExtMode Mode);
  void        clearMargin          ();
  bool        getIsMarginExtended  () const { return m_IsMarginExtended; }

  //Access starting position of original picture 
  PelType*       getAddr           (                )       { return m_PelOrigin; }; 
  const PelType* getAddr           (                ) const { return m_PelOrigin; }; 
  int32          getStride         (                ) const { return m_PelStride; }; 
  int32          getOffset         (int32V2 Position) const { return Position.getY() * m_PelStride + Position.getX(); }
  PelType*       getAddr           (int32V2 Position)       { return getAddr() + getOffset(Position); }
  const PelType* getAddr           (int32V2 Position) const { return getAddr() + getOffset(Position); }
  PelType&       accessPel         (int32V2 Position)       { return *(getAddr() + getOffset(Position)); }
  const PelType& accessPel         (int32V2 Position) const { return *(getAddr() + getOffset(Position)); }
  PelType&       accessPel         (int32   Offset  )       { return *(getAddr() + Offset); }
  const PelType& accessPel         (int32   Offset  ) const { return *(getAddr() + Offset); }
  MemMap         getMemoryMapper   (                )       { return MemMap(m_PelOrigin, m_PelStride); }

  //Access starting position of original picture for specific block unit
  PelType*    getBlockPelAddr      (int32 BlockWidth,     int32 BlockHeight,     int32 BlockPosX, int32 BlockPosY) { return m_PelOrigin + ( BlockPosY * m_PelStride   * BlockHeight    ) + (BlockPosX  * BlockWidth    ); }
  PelType*    getBlockPelAddrLog2  (int32 Log2BlockWidth, int32 Log2BlockHeight, int32 BlockPosX, int32 BlockPosY) { return m_PelOrigin + ((BlockPosY * m_PelStride) << Log2BlockHeight) + (BlockPosX << Log2BlockWidth); }
  PelType*    getBlockPelAddr      (int32 BlockWidth,     int32 BlockHeight,     int16V2 BlockPelPos) { return m_PelOrigin + ( BlockPelPos.getY() * m_PelStride   * BlockHeight    ) + (BlockPelPos.getX()  * BlockWidth    ); }
  PelType*    getBlockPelAddrLog2  (int32 Log2BlockWidth, int32 Log2BlockHeight, int16V2 BlockPelPos) { return m_PelOrigin + ((BlockPelPos.getY() * m_PelStride) << Log2BlockHeight) + (BlockPelPos.getX() << Log2BlockWidth); }

  int32       getBlockPelOffsetLog2(int32V2 BlockPos, int32   Log2BlockSize) const { return ((BlockPos.getY() * m_PelStride) << Log2BlockSize       ) + (BlockPos.getX() << Log2BlockSize       ); }
  int32       getBlockPelOffsetLog2(int32V2 BlockPos, int32V2 Log2BlockSize) const { return ((BlockPos.getY() * m_PelStride) << Log2BlockSize.getY()) + (BlockPos.getX() << Log2BlockSize.getX()); }
  int32       getBlockPelOffset    (int32V2 BlockPos, int32   BlockSize    ) const { return (BlockPos.getY() * BlockSize        * m_PelStride) + (BlockPos.getX() * BlockSize       ); }
  int32       getBlockPelOffset    (int32V2 BlockPos, int32V2 BlockSize    ) const { return (BlockPos.getY() * BlockSize.getY() * m_PelStride) + (BlockPos.getX() * BlockSize.getX()); }

  PelType*    getBlockPelAddrLog2  (int32V2 BlockPos, int32   Log2BlockSize) { return getAddr() + getBlockPelOffsetLog2(BlockPos, Log2BlockSize); }
  PelType*    getBlockPelAddrLog2  (int32V2 BlockPos, int32V2 Log2BlockSize) { return getAddr() + getBlockPelOffsetLog2(BlockPos, Log2BlockSize); }
  PelType*    getBlockPelAddr      (int32V2 BlockPos, int32   BlockSize    ) { return getAddr() + getBlockPelOffset    (BlockPos, BlockSize    ); }
  PelType*    getBlockPelAddr      (int32V2 BlockPos, int32V2 BlockSize    ) { return getAddr() + getBlockPelOffset    (BlockPos, BlockSize    ); }
};

//=============================================================================================================================================================================
// xPlaneTank
//=============================================================================================================================================================================
template <typename PelType> class xPlaneTank
{
protected:
  std::vector<xPlane<PelType>*> m_Buffer;
  std::mutex       m_Mutex;
  uintSize         m_CreatedUnits;
  uintSize         m_SizeLimit;

  //Unit creation parameters
  int32            m_Width;
  int32            m_Height;
  int32            m_Margin;
  int32            m_BitDepth;

public:
  void             create         (int32 Width, int32 Height, int32 Margin, int32 BitDepth, uintSize InitSize);
  void             create         (xPlane<PelType>* Plane, uintSize InitSize) { create(Plane->getWidth(), Plane->getHeight(), Plane->getMargin(), Plane->getBitDepth(), InitSize); }
  void             recreate       (int32 Width, int32 Height, int32 Margin, int32 BitDepth, uintSize InitSize) { destroy(); create(Width, Height, Margin, BitDepth, InitSize); }
  void             recreate       (xPlane<PelType>* Plane, uintSize InitSize) { destroy(); create(Plane, InitSize); }
  void             destroy        () { while(!m_Buffer.empty()) { xDestroyUnit(); } }
                
  void             setSizeLimit   (uintSize SizeLimit) { m_SizeLimit = SizeLimit; while(m_Buffer.size() > SizeLimit) { xDestroyUnit(); } }
                
  void             put            (xPlane<PelType>* Plane);
  xPlane<PelType>* get            ();

  bool             isCompatible   (xPlane<PelType>* Plane) { xAssert(Plane != nullptr); return Plane->isCompatible(m_Width, m_Height, m_Margin, m_BitDepth); }
                
  uintSize         getLoad        () { return m_Buffer.size(); }
  uintSize         getCreatedUnits() { return m_CreatedUnits;  }
                
protected:      
  void             xCreateNewUnit ();
  void             xDestroyUnit   ();
};

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
  std::memset((void*)m_PelBuffer, 0, (m_NumPelsInBuffer)*sizeof(PelType));
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
// xPlane -  Crop & extend
//=============================================================================================================================================================================
template <typename PelType> void xPlane<PelType>::extendMargin(eCmpExtMode Mode)
{
  if(getMargin() != 0)
  {
    switch(Mode)
    {
      case eCmpExtMode::FillGrey : xPixelOps::ExtendMarginSolid (getAddr(), getStride(), getWidth(), getHeight(), getMargin(), (PelType)xBitDepth2MidValue(getBitDepth())); break;
      case eCmpExtMode::EdgePixel: xPixelOps::ExtendMarginEdge  (getAddr(), getStride(), getWidth(), getHeight(), getMargin()                                            ); break;
      case eCmpExtMode::Mirror   : xPixelOps::ExtendMarginMirror(getAddr(), getStride(), getWidth(), getHeight(), getMargin()                                            ); break;
      case eCmpExtMode::Bayer    : xPixelOps::ExtendMarginBayer (getAddr(), getStride(), getWidth(), getHeight(), getMargin()                                            ); break;
      default: xAssert(0); return; break;
    }
  }
  m_IsMarginExtended = true;
}
template <typename PelType> void xPlane<PelType>::clearMargin()
{ 
  if(getMargin() != 0)
  {
    xPixelOps::ClearMargin(getAddr(), getStride(), getWidth(), getHeight(), getMargin());
  }
  m_IsMarginExtended = false;
}


//=============================================================================================================================================================================
// xPlaneTank - base functions
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
// instantiation for base types
//=============================================================================================================================================================================

#ifndef AVlib_xPlane_IMPLEMENTATION
extern template class xPlane<uint8 >;
extern template class xPlane< int8 >;
extern template class xPlane<uint16>;
extern template class xPlane< int16>;
extern template class xPlane<uint32>;
extern template class xPlane< int32>;
extern template class xPlane<uint64>;
extern template class xPlane< int64>;
extern template class xPlane<float >;
extern template class xPlane<double>;
#endif // !AVlib_xPlane_IMPLEMENTATION

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef AVlib_xPlane_IMPLEMENTATION
extern template class xPlaneTank<uint8 >;
extern template class xPlaneTank< int8 >;
extern template class xPlaneTank<uint16>;
extern template class xPlaneTank< int16>;
extern template class xPlaneTank<uint32>;
extern template class xPlaneTank< int32>;
extern template class xPlaneTank<uint64>;
extern template class xPlaneTank< int64>;
extern template class xPlaneTank<float >;
extern template class xPlaneTank<double>;
#endif // !AVlib_xPlane_IMPLEMENTATION

//=============================================================================================================================================================================

} //end of namespace AVLib
