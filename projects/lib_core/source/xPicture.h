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
#include "xFile.h"
#include "xThreadPoolShared.h"
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

#define AVlib_xPicture_DEFAULT

namespace AVlib {

//=============================================================================================================================================================================

template <typename PelType AVlib_xPicture_DEFAULT> class xPicTank;
template <typename PelType AVlib_xPicture_DEFAULT> class xPicITank;

//=============================================================================================================================================================================

template <typename PelType AVlib_xPicture_DEFAULT> class xPic //Planar picture
{
public:
//typedef int16        PelType;
  typedef PelType      T;
  typedef xMemMap2D<T> MemMap;
  typedef xPicTank<T>  tCorespondingTank;
  static const uint32 c_NumberOfComponents = 4;

protected:
  //dimensions, margin, stride
  int32       m_Width;
  int32       m_Height;
  int32       m_Margin;
  int16       m_BitDepth;
  eImgTp      m_ImageType;
  eCrF        m_ChromaFormat;
  int32       m_NumComponents;

  int8V2      m_CmpShift    [c_NumberOfComponents];
  int32       m_CmpNumPels  [c_NumberOfComponents]; //size of component 
  int32       m_CmpPelStride[c_NumberOfComponents]; //pel stride
  PelType*    m_CmpPelBuffer[c_NumberOfComponents]; //picture buffer
  PelType*    m_CmpPelOrigin[c_NumberOfComponents]; //pel origin, pel access -> m_PelOrg[y*m_PelStride + x]
  
  int64       m_POC;
  int64       m_Timestamp;
  std::string m_Name;

  bool        m_IsMarginExtended;

public:
  //Base functions
  xPic                             ();
  xPic                             (int32 Width, int32 Height, int32 Margin, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat) { create(Width, Height, Margin, BitDepth, ImageType, ChromaFormat); }
  xPic                             (int32V2 Size,              int32 Margin, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat) { create(Size, Margin, BitDepth, ImageType, ChromaFormat); }
  xPic                             (xPic* Pic) { create(Pic); }
  ~xPic                            () { destroy(); }

  void        create               (int32 Width, int32 Height, int32 Margin, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat);
  void        create               (int32V2 Size,              int32 Margin, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat) { create(Size.getX(), Size.getY(), Margin, BitDepth, ImageType, ChromaFormat); }
  void        create               (xPic* Pic) { create(Pic->getWidth(), Pic->getHeight(), Pic->getMargin(), Pic->getBitDepth(), Pic->getImageType(), Pic->getChromaFormat()); }
  void        destroy              ();
  void        duplicate            (xPic* Pic);
  void        init                 () { m_IsMarginExtended = false; }
  void        clear                ();
  void        copy                 (xPic* Src);
  void        copyEx               (xPic* Src, int32 DstX, int32 DstY, int32 SrcSX, int32 SrcSY, int32 SrcEX, int32 SrcEY);
  void        set                  (PelType Value);
  void        set                  (PelType Value, eCmp CmpId);

  //Interfaces general                     
  int32       getWidth             (                ) const { return m_Width; }
  int32       getHeight            (                ) const { return m_Height; }
  int32V2     getSize              (                ) const { return {m_Width, m_Height}; }
  int32       getArea              (                ) const { return getWidth()*getHeight(); }
  int32       getMargin            (                ) const { return m_Margin; }
  int16       getBitDepth          (                ) const { return m_BitDepth; }
  eImgTp      getImageType         (                ) const { return m_ImageType; }
  eCrF        getChromaFormat      (                ) const { return m_ChromaFormat; }
  int32       getNumComponents     (                ) const { return m_NumComponents; }

  //Interfaces per component                  
  int32       getHorizontalShift   (eCmp CmpId      ) const { return m_CmpShift[CmpId].getX(); }
  int32       getVerticalShift     (eCmp CmpId      ) const { return m_CmpShift[CmpId].getY(); }
  int32V2     getShift             (eCmp CmpId      ) const { return (int32V2)m_CmpShift[CmpId]; }                                  
  int32       getWidth             (eCmp CmpId      ) const { return m_Width  >> m_CmpShift[CmpId].getX(); }
  int32       getHeight            (eCmp CmpId      ) const { return m_Height >> m_CmpShift[CmpId].getY(); }
  int32V2     getSize              (eCmp CmpId      ) const { return {getWidth(), getHeight()}; }
  int32       getArea              (eCmp CmpId      ) const { return getWidth(CmpId)*getHeight(CmpId); }

  void        setPOC               (int64 POC       )       { m_POC = POC; }
  int64       getPOC               (                ) const { return m_POC; }
  void        setTimestamp         (int64 Timestamp )       { m_Timestamp = Timestamp; }
  int64       getTimestamp         (                ) const { return m_Timestamp;      }
  void        setName              (std::string Name)       { m_Name = Name; }
  std::string getName              (                ) const { return m_Name; }

  //Tool functions                 
  ePelType    getPelType           () const { return DeterminePelType<PelType>(); }
  PelType     getMaxPelValue       () const { if constexpr (std::is_integral_v<PelType>) { return (PelType)xBitDepth2MaxValue(m_BitDepth);} else { return (PelType)1.0; } }
  PelType     getMidPelValue       () const { if constexpr (std::is_integral_v<PelType>) { return ((PelType)1<<(m_BitDepth-1)); } else { return (PelType)0.5; } }
  PelType     getMinPelValue       () const { return (PelType)0; }

  //Compatibility functions        
  bool        isSameSize           (int32 Width, int32 Height, eCmp CmpId ) const { return (getWidth(CmpId) == Width && getHeight(CmpId) == Height); }
  bool        isSameSize           (int32 Width, int32 Height             ) const { return (m_Width == Width && m_Height == Height); }
  bool        isSameMargin         (int32 Margin                          ) const { return (m_Margin == Margin); }
  bool        isSameSizeMargin     (int32 SizeX, int32 SizeY, int32 Margin) const { return (isSameSize(SizeX, SizeY) && isSameMargin(Margin)); }
  bool        isSameBitDepth       (int32 BitDepth                        ) const { return m_BitDepth == BitDepth; }
  bool        isSameType           (eImgTp ImageType                      ) const { return (m_ImageType == ImageType); }
  bool        isSameChromaFrmt     (eCrF  ChromaFormat                    ) const { return m_ChromaFormat == ChromaFormat; }
  bool        isCompatible         (int32 Width, int32 Height, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat) const { return (isSameSize(Width, Height) && isSameBitDepth(BitDepth) && isSameType(ImageType) && isSameChromaFrmt(ChromaFormat)); } 
  bool        isCompatible         (int32 Width, int32 Height, int32 Margin, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat) const { return (isSameSizeMargin(Width, Height, Margin) && isSameBitDepth(BitDepth) && isSameType(ImageType) && isSameChromaFrmt(ChromaFormat)); } 

  bool        isSameSize           (xPic* Pic) const { xAssert(Pic!=nullptr); return isSameSize(Pic->m_Width, Pic->m_Height); }
  bool        isSameMargin         (xPic* Pic) const { xAssert(Pic!=nullptr); return isSameMargin(Pic->getMargin()); }
  bool        isSameSizeMargin     (xPic* Pic) const { return (isSameSize(Pic) && isSameMargin(Pic)); }
  bool        isSameBitDepth       (xPic* Pic) const { xAssert(Pic!=nullptr); return isSameBitDepth(Pic->m_BitDepth); }
  bool        isSameType           (xPic* Pic) const { xAssert(Pic!=nullptr); return isSameType(Pic->m_ImageType); }
  bool        isSameChromaFrmt     (xPic* Pic) const { xAssert(Pic!=nullptr); return isSameChromaFrmt(Pic->m_ChromaFormat); }
  bool        isCompatible         (xPic* Pic) const { return (isSameSizeMargin(Pic) && isSameBitDepth(Pic) && isSameType(Pic) && isSameChromaFrmt(Pic)); }

  //Buffer modification            
  bool        bindBuffer           (PelType*  Buffer, eCmp CmpId);
  PelType*    unbindBuffer         (                  eCmp CmpId);
  bool        swapBuffer           (PelType*& Buffer, eCmp CmpId);
  int32       getPelsInBuff        (                  eCmp CmpId) { return m_CmpNumPels[CmpId]; }
  int32       getBuffSize          (                  eCmp CmpId) { return m_CmpNumPels[CmpId]*sizeof(PelType); }
  
  //Resize and rescale
  void        rescaleFast          (xPic* Src) { rescaleFast(Src, nullptr); }
  void        rescaleFast          (xPic* Src, xThreadPoolInterface* ThreadPoolIntf);
  void        rescaleChroma        (xPic* Src, bool AllowBuffersSwap);

  void        rescaleNearest       (xPic* Src);
  void        rescaleBilinear      (xPic* Src) { rescaleBilinear(Src, nullptr); }
  void        rescaleBilinear      (xPic* Src, xThreadPoolInterface* ThreadPoolIntf);
  void        rescaleU2Nearest     (xPic* Src);
  void        rescaleU2Bilinear    (xPic* Src);
  void        rescaleD2Avg         (xPic* Src);

  //Precision   
  void        convertBitDepth      (xPic* Src, int32 NewBitDepth);
  void        convertBitDepth      (xPic* Src) { convertBitDepth(Src, m_BitDepth); }
  
  //Crop & extend     
//void        crop                 (xPic* Src, int32 SrcSX, int32 SrcSY, int32 SrcEX, int32 SrcEY);
//void        extend               (xPic* Src, int32 SrcSX, int32 SrcSY, int32 SrcEX, int32 SrcEY, eCmpExtMode Mode);
  void        extendMargin         (eCmpExtMode Mode);
  void        clearMargin          ();
  bool        getIsMarginExtended  () { return m_IsMarginExtended; }

  //Colorspace                  
  void        convertColorspace    (xPic* Src, eClrSpc ColorSpace); 
  void        convertRGB2YUV       (xPic* Src, eClrSpc ColorSpace) { convertRGB2YUV(Src, ColorSpace, false); }
  void        convertYUV2RGB       (xPic* Src, eClrSpc ColorSpace) { convertYUV2RGB(Src, ColorSpace, false); }
  void        convertRGB2XYZ       (xPic* Src                    ) { convertRGB2XYZ(Src,             false); }
  void        convertRGB2YUV       (xPic* Src, eClrSpc ColorSpace, bool IncludingMargin);
  void        convertYUV2RGB       (xPic* Src, eClrSpc ColorSpace, bool IncludingMargin);
  void        convertRGB2XYZ       (xPic* Src,                     bool IncludingMargin);

  //Arithmetic operations          
  //void        add                  (xPic* Src0, xPic* Src1);
  //void        addAndClip           (xPic* Src0, xPic* Src1);
  //void        sub                  (xPic* Src0, xPic* Src1);
  //void        average              (xPic* Src0, xPic* Src1);
  //void        averageAndClip       (xPic* Src0, xPic* Src1);
  //void        clip                 (xPic* Src, int16 ClippingRangeLow, int16 ClippingRangeHigh);

  //Distortion & PSNR      
  uint64V4    calcDist             (xPic* Ref, eDistMetric DistMetric);
  doubleV4    calcPSNR             (xPic* Ref);
  uint64      calcDist             (xPic* Ref, eDistMetric DistMetric, eCmp CmpId);
  double      calcPSNR             (xPic* Ref, eCmp CmpId);
  uint64      calcWindowedDist     (xPic* Ref, int32 OriginX, int32 OriginY, int32 Width, int32 Height, eDistMetric DistMetric, eCmp CmpId);
  double      calcWindowedPSNR     (xPic* Ref, int32 OriginX, int32 OriginY, int32 Width, int32 Height, eCmp CmpId);    
  uint64      calcSAD              (xPic* Ref, eCmp CmpId);
  uint64      calcSSD              (xPic* Ref, eCmp CmpId);

  //Load and store RAW         
  void        getRAW               (xFile& File);
  void        putRAW               (xFile& File);

  //load and store BMP          
  void        getBMP               (xFile& File);
  void        putBMP               (xFile& File);

  //Access starting position of original picture 
  PelType*       getAddr           (                  eCmp CmpId)       { return m_CmpPelOrigin[CmpId]; }
  const PelType* getAddr           (                  eCmp CmpId) const { return m_CmpPelOrigin[CmpId]; }
  int32          getStride         (                  eCmp CmpId) const { return m_CmpPelStride[CmpId]; }
  int32          getPitch          (                            ) const { return 1;                     }
  int32          getOffset         (int32V2 Position, eCmp CmpId) const { return (Position.getY()>>m_CmpShift[CmpId].getY()) * m_CmpPelStride[CmpId] + (Position.getX()>>m_CmpShift[CmpId].getX()); }
  PelType*       getAddr           (int32V2 Position, eCmp CmpId)       { return getAddr(CmpId) + getOffset(Position, CmpId); }
  const PelType* getAddr           (int32V2 Position, eCmp CmpId) const { return getAddr(CmpId) + getOffset(Position, CmpId); }
  PelType&       accessPel         (int32V2 Position, eCmp CmpId)       { return *(getAddr(CmpId) + getOffset(Position, CmpId)); }
  PelType&       accessPel         (int32   Offset  , eCmp CmpId)       { return *(getAddr(CmpId) + Offset); }
  const PelType& accessPel         (int32V2 Position, eCmp CmpId) const { return *(getAddr(CmpId) + getOffset(Position, CmpId)); }
  const PelType& accessPel         (int32   Offset  , eCmp CmpId) const { return *(getAddr(CmpId) + Offset); }
  MemMap         getMemoryMapper   (                  eCmp CmpId)       { return MemMap(m_CmpPelOrigin[CmpId], m_CmpPelStride[CmpId]); }
  
  //Access all pixel values (slow)
  xVec3<T>    getPixelValuesYUV    (int32V2 Position) const { int32 OffsetC = getOffset(Position, CMP_CB); return { *(getAddr(CMP_LM) + getOffset(Position, CMP_LM)), *(getAddr(CMP_CB) + OffsetC), *(getAddr(CMP_CR) + OffsetC) }; }
  xVec3<T>    getPixelValues3      (int32V2 Position) const { return { accessPel(Position, CMP_0), accessPel(Position, CMP_1), accessPel(Position, CMP_2) }; }
  xVec4<T>    getPixelValues4      (int32V2 Position) const { return { accessPel(Position, CMP_0), accessPel(Position, CMP_1), accessPel(Position, CMP_2), accessPel(Position, CMP_3) }; }
  
  //Access starting position of original picture for specific block unit
  int32       getLog2BlockWidth    (int32 Log2BlockSize, eCmp CmpId) { return Log2BlockSize - m_CmpShift[CmpId].getX(); }
  int32       getLog2BlockHeight   (int32 Log2BlockSize, eCmp CmpId) { return Log2BlockSize - m_CmpShift[CmpId].getY(); }
  int32       getBlockWidth        (int32 BlockSize    , eCmp CmpId) { return BlockSize >> m_CmpShift[CmpId].getX(); }
  int32       getBlockHeight       (int32 BlockSize    , eCmp CmpId) { return BlockSize >> m_CmpShift[CmpId].getY(); }

  int32       getBlockPelOffsetLog2(int32V2 BlockPos, int32   Log2BlockSize, eCmp CmpId) { return ((BlockPos.getY() * m_CmpPelStride[CmpId]) << getLog2BlockHeight(Log2BlockSize       , CmpId)) + (BlockPos.getX() << getLog2BlockWidth(Log2BlockSize,        CmpId)); }
  int32       getBlockPelOffsetLog2(int32V2 BlockPos, int32V2 Log2BlockSize, eCmp CmpId) { return ((BlockPos.getY() * m_CmpPelStride[CmpId]) << getLog2BlockHeight(Log2BlockSize.getY(), CmpId)) + (BlockPos.getX() << getLog2BlockWidth(Log2BlockSize.getX(), CmpId)); }
  int32       getBlockPelOffset    (int32V2 BlockPos, int32   BlockSize    , eCmp CmpId) { return (BlockPos.getY() * getBlockHeight(BlockSize       , CmpId) * m_CmpPelStride[CmpId]) + (BlockPos.getX() * getBlockWidth(BlockSize       , CmpId)); }
  int32       getBlockPelOffset    (int32V2 BlockPos, int32V2 BlockSize    , eCmp CmpId) { return (BlockPos.getY() * getBlockHeight(BlockSize.getY(), CmpId) * m_CmpPelStride[CmpId]) + (BlockPos.getX() * getBlockWidth(BlockSize.getX(), CmpId)); }

  PelType*    getBlockPelAddrLog2  (int32V2 BlockPos, int32   Log2BlockSize, eCmp CmpId) { return getAddr(CmpId) + getBlockPelOffsetLog2(BlockPos, Log2BlockSize, CmpId); }
  PelType*    getBlockPelAddrLog2  (int32V2 BlockPos, int32V2 Log2BlockSize, eCmp CmpId) { return getAddr(CmpId) + getBlockPelOffsetLog2(BlockPos, Log2BlockSize, CmpId); }
  PelType*    getBlockPelAddr      (int32V2 BlockPos, int32   BlockSize    , eCmp CmpId) { return getAddr(CmpId) + getBlockPelOffset    (BlockPos, BlockSize    , CmpId); }
  PelType*    getBlockPelAddr      (int32V2 BlockPos, int32V2 BlockSize    , eCmp CmpId) { return getAddr(CmpId) + getBlockPelOffset    (BlockPos, BlockSize    , CmpId); }
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef AVlib_xPicture_IMPLEMENTATION
//extern template class xPic<uint8 >;
//extern template class xPic< int8 >;
extern template class xPic<uint16>;
extern template class xPic< int16>;
//extern template class xPic<uint32>;
//extern template class xPic< int32>;
//extern template class xPic<uint64>;
//extern template class xPic< int64>;
//extern template class xPic<float >;
//extern template class xPic<double>;
#endif // !AVlib_xPicture_IMPLEMENTATION

//=============================================================================================================================================================================

template <typename PelType> class xPicTank
{
protected:
  std::vector<xPic<PelType>*> m_Buffer;
  std::mutex                  m_Mutex;
  uintSize                    m_CreatedUnits;
  uintSize                    m_SizeLimit;

  //Unit creation parameters
  int32          m_Width;
  int32          m_Height;
  int32          m_Margin;
  int32          m_BitDepth;
  eImgTp         m_ImageType;
  eCrF           m_ChromaFormat;
                
public:          
  void           create         (int32 Width, int32 Height, int32 Margin, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat, int32 InitSize);
  void           create         (xPic<PelType>* Pic, int32 InitSize) { create(Pic->getWidth(), Pic->getHeight(), Pic->getMargin(), Pic->getBitDepth(), Pic->getImageType(), Pic->getChromaFormat(), InitSize); }
  void           recreate       (int32 Width, int32 Height, int32 Margin, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat, int32 Log2UnitSize, int32 InitSize) { destroy(); create(Width, Height, Margin, BitDepth, ImageType, ChromaFormat, InitSize); }
  void           recreate       (xPic<PelType>* Pic, int32 InitSize) { destroy(); create(Pic, InitSize); }
  void           destroy        () { while(!m_Buffer.empty()) { xDestroyUnit(); } }
                
  void           setSizeLimit   (uintSize SizeLimit) { m_SizeLimit = SizeLimit; while(m_Buffer.size() > SizeLimit) { xDestroyUnit(); } }
                
  void           put            (xPic<PelType>* Pic);
  xPic<PelType>* get            ();

  bool           isCompatible   (int32 Width, int32 Height, int32 Margin, int32 BitDepth, eCrF ChromaFormat, eClrSpc ColorSpace, int32 Log2BlockSize) { return (m_Width==Width && m_Height==Height && m_Margin==Margin && m_BitDepth == BitDepth && m_ChromaFormat==ChromaFormat); }
  bool           isCompatible   (xPic<PelType>* Pic) { xAssert(Pic != nullptr); return Pic->isCompatible(m_Width, m_Height, m_Margin, m_BitDepth, m_ImageType, m_ChromaFormat); }
               
  uintSize       getLoad        () { return m_Buffer.size(); }
  uintSize       getCreatedUnits() { return m_CreatedUnits;  }
               
protected:     
  void           xCreateNewUnit ();
  void           xDestroyUnit   ();
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef AVlib_xPicture_IMPLEMENTATION
//extern template class xPicTank<uint8 >;
//extern template class xPicTank< int8 >;
extern template class xPicTank<uint16>;
extern template class xPicTank< int16>;
//extern template class xPicTank<uint32>;
//extern template class xPicTank< int32>;
//extern template class xPicTank<uint64>;
//extern template class xPicTank< int64>;
//extern template class xPicTank<float >;
//extern template class xPicTank<double>;
#endif // !AVlib_xPlane_IMPLEMENTATION

//=============================================================================================================================================================================

template <typename PelType AVlib_xPicture_DEFAULT> class xPicI //interleaved picture
{
public:
  static const uint32 c_Log2NumberOfComponents = 2;
  static const uint32 c_NumberOfComponents     = 4;

  using   VecType = xVec4<PelType>;

  typedef PelType      T;
  typedef xMemMap2D<T> MemMap;
  typedef xPicITank<T> tCorespondingTank;


protected:
  //dimensions, margin, stride
  int32       m_Width;
  int32       m_Height;
  int32       m_Margin;
  int16       m_BitDepth;
  eImgTp      m_ImageType;
  int32       m_NumComponents;

  int32       m_NumCmpPels;      //size of one component 
  int32       m_PelStride;       //pel stride
  PelType*    m_PelBuffer;       //picture buffer
  PelType*    m_PelOrigin;       //pel origin, pel access -> m_PelOrg[y*m_PelStride + x]

  int64       m_POC;
  int64       m_Timestamp;
  std::string m_Name;

  bool        m_IsMarginExtended;

public:
  //Base functions
  xPicI                            ();
  xPicI                            (int32 Width, int32 Height, int32 Margin, int32 BitDepth, eImgTp ImageType) { create(Width, Height, Margin, BitDepth, ImageType); }
  xPicI                            (xPicI* Pic) { create(Pic); }
  ~xPicI                           () { destroy(); }

  void        create               (int32 Width, int32 Height, int32 Margin, int32 BitDepth, eImgTp ImageType);
  void        create               (int32V2 Size, int32 Margin, int32 BitDepth, eImgTp ImageType) { create(Size.getX(), Size.getY(), Margin, BitDepth, ImageType); }
  void        create               (xPicI* Pic) { create(Pic->getWidth(), Pic->getHeight(), Pic->getMargin(), Pic->getBitDepth(), Pic->getImageType()); }
  void        destroy              ();
  void        duplicate            (xPicI* Pic);
  void        init                 () { m_IsMarginExtended = false; }
  void        clear                ();
  void        copy                 (xPicI* Src);
  void        copyEx               (xPicI* Src, int32 DstX, int32 DstY, int32 SrcSX, int32 SrcSY, int32 SrcEX, int32 SrcEY);
  void        set                  (PelType Value);
  void        set                  (VecType Value);
  void        set                  (PelType Value, eCmp CmpId);

  //Interfaces general                     
  int32       getWidth             (                ) { return m_Width; }
  int32       getHeight            (                ) { return m_Height; }
  int32V2     getSize              (                ) { return {m_Width, m_Height}; }
  int32       getArea              (                ) { return getWidth()*getHeight(); }
  int32       getMargin            (                ) { return m_Margin; }
  int16       getBitDepth          (                ) { return m_BitDepth; }
  eImgTp      getImageType         (                ) { return m_ImageType; }
  eCrF        getChromaFormat      (                ) { return eCrF::CrF_444; }
  int32       getNumComponents     (                ) { return m_NumComponents; }

  void        setPOC               (int64 POC       ) { m_POC = POC; }
  int64       getPOC               (                ) { return m_POC; }
  void        setTimestamp         (int64 Timestamp ) { m_Timestamp = Timestamp; }
  int64       getTimestamp         (                ) { return m_Timestamp;      }
  void        setName              (std::string Name) { m_Name = Name; }
  std::string getName              (                ) { return m_Name; }

  //Tool functions                 
  ePelType    getPelType           () { return DeterminePelType<PelType>(); }
  PelType     getMaxPelValue       () { if constexpr (std::is_integral_v<PelType>) { return (PelType)xBitDepth2MaxValue(m_BitDepth);} else { return (PelType)1.0; } }
  PelType     getMidPelValue       () { if constexpr (std::is_integral_v<PelType>) { return (PelType)xBitDepth2MidValue(m_BitDepth); } else { return (PelType)0.5; } }
  PelType     getMinPelValue       () { return (PelType)0; }

  //Compatibility functions        
  bool        isSameSize           (int32 Width, int32 Height             ) { return (m_Width == Width && m_Height == Height); }
  bool        isSameMargin         (int32 Margin                          ) { return m_Margin == Margin; }
  bool        isSameSizeMargin     (int32 SizeX, int32 SizeY, int32 Margin) { return (isSameSize(SizeX, SizeY) && isSameMargin(Margin)); }
  bool        isSameBitDepth       (int32 BitDepth                        ) { return m_BitDepth == BitDepth; }
  bool        isSameType           (eImgTp ImageType                      ) { return m_ImageType == ImageType; }
  bool        isSameChromaFrmt     (eCrF  ChromaFormat                    ) { return eCrF::CrF_444 == ChromaFormat; }
  bool        isCompatible         (int32 Width, int32 Height, int32 BitDepth, eImgTp ImageType) { return (isSameSize(Width, Height) && isSameBitDepth(BitDepth) && isSameType(ImageType)); } 
  bool        isCompatible         (int32 Width, int32 Height, int32 Margin, int32 BitDepth, eImgTp ImageType) { return (isSameSizeMargin(Width, Height, Margin) && isSameBitDepth(BitDepth) && isSameType(ImageType)); } 

  bool        isSameSize           (xPicI* Pic) { xAssert(Pic!=nullptr); return isSameSize(Pic->m_Width, Pic->m_Height); }
  bool        isSameMargin         (xPicI* Pic) { xAssert(Pic!=nullptr); return isSameMargin(Pic->getMargin()); }
  bool        isSameSizeMargin     (xPicI* Pic) { return (isSameSize(Pic) && isSameMargin(Pic)); }
  bool        isSameBitDepth       (xPicI* Pic) { xAssert(Pic!=nullptr); return isSameBitDepth(Pic->m_BitDepth); }
  bool        isSameType           (xPicI* Pic) { xAssert(Pic!=nullptr); return isSameType(Pic->m_ImageType); }
  bool        isCompatible         (xPicI* Pic) { return (isSameSizeMargin(Pic) && isSameBitDepth(Pic)); }

  //Buffer modification            
  bool        bindBuffer           (PelType*  Buffer);
  PelType*    unbindBuffer         (                );
  bool        swapBuffer           (PelType*& Buffer);
  int32       getPelsInBuff        (                ) { return m_NumCmpPels*c_NumberOfComponents; }
  int32       getBuffSize          (                ) { return m_NumCmpPels*c_NumberOfComponents*sizeof(PelType); }

  //Crop & extend                  
  void        extendMargin         (eCmpExtMode Mode);
  void        clearMargin          ();
  bool        getIsMarginExtended  () const { return m_IsMarginExtended; }

  //Convertion
  void        rearrangeFromPlanar  (const xPic<T>* Planar);
  void        rearrangeToPlanar    (      xPic<T>* Planar);

  template <typename PlanarPelType> void rearrangeConvertFromPlanar(const xPic<PlanarPelType>* Planar);
  template <typename PlanarPelType> void rearrangeConvertToPlanar  (      xPic<PlanarPelType>* Planar);

  //Access starting position of original picture 
  PelType*    getAddr              (                            ) { return m_PelOrigin;         }
  const PelType* getAddr              (                            ) const { return m_PelOrigin;         }
  PelType*    getAddr              (                  eCmp CmpId) { return m_PelOrigin + CmpId; }
  const PelType* getAddr              (                  eCmp CmpId) const { return m_PelOrigin + CmpId; }
  int32          getStride            (                            ) const { return m_PelStride;         }
  int32          getPitch             (                            ) const { return c_NumberOfComponents;}
  int32          getOffset            (int32V2 Position            ) const { return Position.getY() * m_PelStride + Position.getX() * c_NumberOfComponents; }
  PelType*    getAddr              (int32V2 Position, eCmp CmpId) { return getAddr(CmpId) + getOffset(Position); }
  const PelType* getAddr              (int32V2 Position, eCmp CmpId) const { return getAddr(CmpId) + getOffset(Position); }
  PelType&    accessPel            (int32V2 Position, eCmp CmpId) { return *(getAddr(CmpId) + getOffset(Position)); }
  PelType&    accessPel            (int32   Offset  , eCmp CmpId) { return *(getAddr(CmpId) + Offset); }
  const PelType& accessPel            (int32V2 Position, eCmp CmpId) const { return *(getAddr(CmpId) + getOffset(Position)); }
  const PelType& accessPel            (int32   Offset  , eCmp CmpId) const { return *(getAddr(CmpId) + Offset); }

  //Accessing as vector
  VecType*       getAddrVec           (                            )       { return (VecType*)m_PelOrigin;                   }
  const VecType* getAddrVec           (                            ) const { return (VecType*)m_PelOrigin;                   }
  int32          getStrideVec         (                            ) const { return m_PelStride >> c_Log2NumberOfComponents; }

  //Access all pixel values (slow)
  xVec3<T>      getPixelValues3(                   int32V2 Position) const { return xVec3<T>(m_PelOrigin + getOffset(Position)); }
  VecType       getPixelValues4(                   int32V2 Position) const { return xVec4<T>(m_PelOrigin + getOffset(Position)); }
  void          setPixelValues3(const xVec3<T>& V, int32V2 Position)       { V.get(m_PelOrigin + getOffset(Position)); }
  void          setPixelValues4(const xVec4<T>& V, int32V2 Position)       { V.get(m_PelOrigin + getOffset(Position)); }
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename PelType> template <typename PlanarPelType> void xPicI<PelType>::rearrangeConvertFromPlanar(const xPic<PlanarPelType>* Planar)
{
  //xAssert(Planar->isCompatible(m_Width, m_Height, m_BitDepth, m_ImageType, eCrF::CrF_444));

  if (m_NumComponents == 4)
  {
    xPixelOps::ConvertSOA4toAOS4<PelType, PlanarPelType>(m_PelOrigin, Planar->getAddr(CMP_0), Planar->getAddr(CMP_1), Planar->getAddr(CMP_2), Planar->getAddr(CMP_3), m_PelStride, Planar->getStride(CMP_0), m_Width, m_Height);
  }
  else
  {
    xPixelOps::ConvertSOA3toAOS4<PelType, PlanarPelType>(m_PelOrigin, Planar->getAddr(CMP_0), Planar->getAddr(CMP_1), Planar->getAddr(CMP_2), (PlanarPelType)0, m_PelStride, Planar->getStride(CMP_0), m_Width, m_Height);
  }
}
template <typename PelType> template <typename PlanarPelType> void xPicI<PelType>::rearrangeConvertToPlanar(xPic<PlanarPelType>* Planar)
{
  if (m_NumComponents == 4)
  {
    xPixelOps::ConvertAOS4toSOA4<PlanarPelType, PelType>(Planar->getAddr(CMP_0), Planar->getAddr(CMP_1), Planar->getAddr(CMP_2), Planar->getAddr(CMP_3), m_PelOrigin, Planar->getStride(CMP_0), m_PelStride, m_Width, m_Height);
  }
  else
  {
    xPixelOps::ConvertAOS4toSOA3<PlanarPelType, PelType>(Planar->getAddr(CMP_0), Planar->getAddr(CMP_1), Planar->getAddr(CMP_2), m_PelOrigin, Planar->getStride(CMP_0), m_PelStride, m_Width, m_Height);
  }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef AVlib_xPicture_IMPLEMENTATION
//extern template class xPicI<uint8 >;
//extern template class xPicI< int8 >;
extern template class xPicI<uint16>;
extern template class xPicI< int16>;
//extern template class xPicI<uint32>;
//extern template class xPicI< int32>;
//extern template class xPicI<uint64>;
//extern template class xPicI< int64>;
//extern template class xPicI<float >;
//extern template class xPicI<double>;
#endif // !AVlib_xPicture_IMPLEMENTATION

//=============================================================================================================================================================================

template <typename PelType> class xPicITank
{
protected:
  std::vector<xPicI<PelType>*> m_Buffer;
  std::mutex                   m_Mutex;
  uintSize                     m_CreatedUnits;
  uintSize                     m_SizeLimit;

  //Unit creation parameters
  int32           m_Width;
  int32           m_Height;
  int32           m_Margin;
  int32           m_BitDepth;
  eImgTp          m_ImageType;
               
public:           
  void            create         (int32 Width, int32 Height, int32 Margin, int32 BitDepth, eImgTp ImageType, int32 InitSize);
  void            create         (xPicI<PelType>* Pic, int32 InitSize) { create(Pic->getWidth(), Pic->getHeight(), Pic->getMargin(), Pic->getBitDepth(), Pic->getImageType(), InitSize); }
  void            recreate       (int32 Width, int32 Height, int32 Margin, int32 BitDepth, eImgTp ImageType, eCrF ChromaFormat, int32 Log2UnitSize, int32 InitSize) { destroy(); create(Width, Height, Margin, BitDepth, ImageType, InitSize); }
  void            recreate       (xPicI<PelType>* Pic, int32 InitSize) { destroy(); create(Pic, InitSize); }
  void            destroy        () { while(!m_Buffer.empty()) { xDestroyUnit(); } }
               
  void            setSizeLimit   (uintSize SizeLimit) { m_SizeLimit = SizeLimit; while(m_Buffer.size() > SizeLimit) { xDestroyUnit(); } }
               
  void            put            (xPicI<PelType>* Pic);
  xPicI<PelType>* get            ();

  bool         isCompatible   (xPicI<PelType>* Pic) { xAssert(Pic != nullptr); return Pic->isCompatible(m_Width, m_Height, m_Margin, m_BitDepth, m_ImageType); }

  uintSize     getLoad        () { return m_Buffer.size(); }
  uintSize     getCreatedUnits() { return m_CreatedUnits;  }

protected:
  void         xCreateNewUnit ();
  void         xDestroyUnit   ();
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef AVlib_xPicture_IMPLEMENTATION
//extern template class xPicITank<uint8 >;
//extern template class xPicITank< int8 >;
extern template class xPicITank<uint16>;
extern template class xPicITank< int16>;
//extern template class xPicITank<uint32>;
//extern template class xPicITank< int32>;
//extern template class xPicITank<uint64>;
//extern template class xPicITank< int64>;
//extern template class xPicITank<float >;
//extern template class xPicITank<double>;
#endif // !AVlib_xPlane_IMPLEMENTATION

//=============================================================================================================================================================================

} //end of namespace AVLib
