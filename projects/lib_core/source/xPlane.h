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

namespace AVlib {

//=============================================================================================================================================================================

template <typename PelType> class xPlaneTank;

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
  xPlane                           (xPlane* RefPlane) { create(RefPlane); }
  ~xPlane                          () { destroy(); }
                                   
  void        create               (int32 Width, int32 Height, int32 Margin, int32 BitDepth);
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
  //void        extendMargin         (eCmpExtMode Mode);
  //void        clearMargin          ();
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

typedef xPlane<uint8 > xPlaneU8 ;
typedef xPlane< int8 > xPlaneI8 ;
typedef xPlane<uint16> xPlaneU16;
typedef xPlane< int16> xPlaneI16;
typedef xPlane<uint32> xPlaneU32;
typedef xPlane< int32> xPlaneI32;
typedef xPlane<uint64> xPlaneU64;
typedef xPlane< int64> xPlaneI64;
typedef xPlane<float > xPlaneF  ;
typedef xPlane<double> xPlaneD  ;

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
