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

#include "xType.h"
#include "xConstants.h"
#include <string>

namespace AVlib {

//=============================================================================================================================================================================

enum ePelType : uint8 //1b-int/float, 1b-sign, 6b - length 
{
  PelType_NotDeff = 0,
  PelType_uint8   = 0x01,
  PelType_int8    = 0x41,
  PelType_uint16  = 0x02,
  PelType_int16   = 0x42,
  PelType_uint32  = 0x04,
  PelType_int32   = 0x44,
  PelType_uint64  = 0x08,
  PelType_int64   = 0x48,
  PelType_half    = 0x82,
  PelType_float   = 0x84,
  PelType_double  = 0x88,
};

template <typename PelType> constexpr ePelType DeterminePelType()
{
  if constexpr (std::is_same_v<PelType, uint8 >) { return PelType_uint8 ; }
  if constexpr (std::is_same_v<PelType,  int8 >) { return PelType_int8  ; }
  if constexpr (std::is_same_v<PelType, uint16>) { return PelType_uint16; }
  if constexpr (std::is_same_v<PelType,  int16>) { return PelType_int16 ; }
  if constexpr (std::is_same_v<PelType, uint32>) { return PelType_uint32; }
  if constexpr (std::is_same_v<PelType,  int32>) { return PelType_int32 ; }
  if constexpr (std::is_same_v<PelType, uint64>) { return PelType_uint64; }
  if constexpr (std::is_same_v<PelType,  int64>) { return PelType_int64 ; }
  //if constexpr (std::is_same_v<PelType,  half >) { return PelType_half  ; }
  if constexpr (std::is_same_v<PelType,  float>) { return PelType_float ; }
  if constexpr (std::is_same_v<PelType, double>) { return PelType_double; }
  return PelType_NotDeff;
}

//=============================================================================================================================================================================

enum class eImgTp : int8 //Image Type
{
  INVALID = NOT_VALID,
  UNKNOWN = 0,
  YUV,     //YUV (YCbCr, YCoCg, ...)
  YUVA,    //YUV+A(alpha)
  YUVD,    //YUV+D(depth)
  RGB,     //RGB
  RGBA,    //RGB+A(alpha)
  RGBD,    //RGB+D(depth)
  XYZ,     //XYZ
  Bayer,   //M - bayer
};

static inline eImgTp xStringToImgTp(std::string ImageType) {return ImageType=="YUV"  ? eImgTp::YUV  :
                                                                   ImageType=="YUVA" ? eImgTp::YUVA :
                                                                   ImageType=="YUVD" ? eImgTp::YUVD :
                                                                   ImageType=="RGB"  ? eImgTp::RGB  :
                                                                   ImageType=="RGBA" ? eImgTp::RGBA :
                                                                   ImageType=="RGBD" ? eImgTp::RGBD :
                                                                   ImageType=="XYZ"  ? eImgTp::XYZ  :
                                                                   ImageType=="Bayer"? eImgTp::Bayer:
                                                                                       eImgTp::INVALID;}
static inline std::string xImgTpToString(eImgTp ImageType) {return ImageType==eImgTp::YUV     ? "YUV"  :
                                                                   ImageType==eImgTp::YUVA    ? "YUVA" :
                                                                   ImageType==eImgTp::YUVD    ? "YUVD" :
                                                                   ImageType==eImgTp::RGB     ? "RGB"  :
                                                                   ImageType==eImgTp::RGBA    ? "RGBA" :
                                                                   ImageType==eImgTp::RGBD    ? "RGBD" :
                                                                   ImageType==eImgTp::Bayer   ? "Bayer":
                                                                   ImageType==eImgTp::UNKNOWN ? "UNKNOWN" :
                                                                                                "INVALID";}

//=============================================================================================================================================================================

enum class ePckTp : int8
{
  INVALID     = NOT_VALID,
  UNKNOWN     = 0,
  Planar      = 1,
  SemiPlanar  = 2,
  Interleaved = 3,
};

static inline ePckTp xStringToPckTp(std::string PackedType) {return PackedType=="Planar"      ? ePckTp::Planar      :
                                                                    PackedType=="SemiPlanar"  ? ePckTp::SemiPlanar  :
                                                                    PackedType=="Interleaved" ? ePckTp::Interleaved :
                                                                                                ePckTp::INVALID;}
static inline std::string xPckTpToString(ePckTp PackedType) {return PackedType==ePckTp::Planar      ? "Planar"      :
                                                                    PackedType==ePckTp::SemiPlanar  ? "SemiPlanar"  :
                                                                    PackedType==ePckTp::Interleaved ? "Interleaved" :
                                                                                                      "INVALID";}

//=============================================================================================================================================================================

enum class eCmpO : int8 //color planes order
{
  INVALID = NOT_VALID,
  UNKNOWN = 0,
  YUV     ,
  YVU     ,

  AYUV    ,
  YUVA    ,

  DYUV    ,
  YUVD    ,
  
  YUYV    ,
  UYVY    ,
  YVYU    ,

  RGB     ,
  RGBA    ,
};

static inline eCmpO xStringToCmpO(std::string ComponentOrder) {return ComponentOrder=="YUV"  ? eCmpO::YUV  :
                                                                      ComponentOrder=="YVU"  ? eCmpO::YVU  :
                                                                      ComponentOrder=="AYUV" ? eCmpO::AYUV :
                                                                      ComponentOrder=="YUVA" ? eCmpO::YUVA :
                                                                      ComponentOrder=="DYUV" ? eCmpO::DYUV :
                                                                      ComponentOrder=="YUVD" ? eCmpO::YUVD :
                                                                      ComponentOrder=="YUYV" ? eCmpO::YUYV :
                                                                      ComponentOrder=="UYVY" ? eCmpO::UYVY :
                                                                      ComponentOrder=="YVYU" ? eCmpO::YVYU :
                                                                      eCmpO::INVALID;}
static inline std::string xCmpOToString(eCmpO ComponentOrder) {return ComponentOrder==eCmpO::YUV  ? "YUV"  :
                                                                      ComponentOrder==eCmpO::YVU  ? "YVU"  :
                                                                      ComponentOrder==eCmpO::AYUV ? "AYUV" :
                                                                      ComponentOrder==eCmpO::YUVA ? "YUVA" :
                                                                      ComponentOrder==eCmpO::AYUV ? "DYUV" :
                                                                      ComponentOrder==eCmpO::YUVA ? "YUVD" :
                                                                      ComponentOrder==eCmpO::YUYV ? "YUYV" :
                                                                      ComponentOrder==eCmpO::UYVY ? "UYVY" :
                                                                      ComponentOrder==eCmpO::YVYU ? "YVYU" :
                                                                      "INVALID";}

//=============================================================================================================================================================================

enum eCrF : int16 //Chroma Format
{
  CrF_INVALID = NOT_VALID,
  CrF_UNKNOWN = 0,
  CrF_444     = 444,
  CrF_422     = 422,
  CrF_420     = 420,
  CrF_400     = 400,
  CrF_x44     =  44, //no luma
  CrF_x22     =  22, //no luma
  CrF_x20     =  20, //no luma
};

//=============================================================================================================================================================================

enum class eClrSpc : int8 //Color Space
{  
  INVALID = NOT_VALID,
  Unknown = 0,
  BT601   = 1,
  BT709,
  JPEG2000,
  SMPTE240M,
};

static inline eClrSpc xStringToClrSpc(std::string ColorSpace) {return ColorSpace=="BT601"     ? eClrSpc::BT601  :
                                                                      ColorSpace=="BT709"     ? eClrSpc::BT709  :
                                                                      ColorSpace=="JPEG2000"  ? eClrSpc::JPEG2000 :
                                                                      ColorSpace=="SMPTE240M" ? eClrSpc::SMPTE240M :                                                                  
                                                                      eClrSpc::INVALID;}
static inline std::string xClrSpcToString(eClrSpc ColorSpace) {return ColorSpace==eClrSpc::BT601     ? "BT601"  :
                                                                      ColorSpace==eClrSpc::BT709     ? "BT709"  :
                                                                      ColorSpace==eClrSpc::JPEG2000  ? "JPEG2000" :
                                                                      ColorSpace==eClrSpc::SMPTE240M ? "SMPTE240M" :
                                                                      ColorSpace==eClrSpc::Unknown   ? "UNKNOWN" :
                                                                      "INVALID";}

//=============================================================================================================================================================================

enum class eBayerPattern : int32
{
  INVALID = NOT_VALID,
  GBRG    = 0,
  GRBG    = 1,
  BGGR    = 2,
  RGGB    = 3,
};

static inline eBayerPattern xStringToBayerPattern(std::string BayerPattern) {return BayerPattern=="GBRG" ? eBayerPattern::GBRG :
                                                                                    BayerPattern=="GRBG" ? eBayerPattern::GRBG :
                                                                                    BayerPattern=="BGGR" ? eBayerPattern::BGGR :
                                                                                    BayerPattern=="RGGB" ? eBayerPattern::RGGB :                                                                  
                                                                                    eBayerPattern::INVALID;}
static inline std::string xBayerPatternToString(eBayerPattern BayerPattern) {return BayerPattern==eBayerPattern::GBRG ? "GBRG" :
                                                                                    BayerPattern==eBayerPattern::GRBG ? "GRBG" :
                                                                                    BayerPattern==eBayerPattern::BGGR ? "BGGR" :
                                                                                    BayerPattern==eBayerPattern::RGGB ? "RGGB" :                                                                  
                                                                                    "INVALID";}

//=============================================================================================================================================================================

enum eBayerAlgorithm : int32
{
  INVALID     = NOT_VALID,
  Scatter     = 0, //scatter points into RGB planes, leaving unknown set to 0
  Nearest        , //nearest neighbor interpolation - fastest, poor quality
  Bilinear       , //bilinear interpolation - fast, moderate quality
  VNG            , //variable number of gradients
  PG             , //pixel grouping
  AHD            , //adaptive homogeneity directed (AHD) - shamefully slow, very good quality
  IAHD           , //improved AHD - moderate slow, best quality
  DLMMSE         , //directionel linear minimum mean-square error
};

static inline eBayerAlgorithm xStringToBayerAlgorithm(std::string BayerAlgorithm) { return BayerAlgorithm=="Scatter"    ? eBayerAlgorithm::Scatter    :
                                                                                           BayerAlgorithm=="Nearest"    ? eBayerAlgorithm::Nearest    :
                                                                                           BayerAlgorithm=="Bilinear"   ? eBayerAlgorithm::Bilinear   :
                                                                                           BayerAlgorithm=="VNG"        ? eBayerAlgorithm::VNG        :
                                                                                           BayerAlgorithm=="PG"         ? eBayerAlgorithm::PG         :
                                                                                           BayerAlgorithm=="AHD"        ? eBayerAlgorithm::AHD        :
                                                                                           BayerAlgorithm=="IAHD"       ? eBayerAlgorithm::IAHD       :  
                                                                                           BayerAlgorithm=="DLMMSE"     ? eBayerAlgorithm::DLMMSE     :
                                                                                           eBayerAlgorithm::INVALID;}
static inline std::string xBayerAlgorithmToString(eBayerAlgorithm BayerAlgorithm) {return BayerAlgorithm==eBayerAlgorithm::Scatter    ? "Scatter"    :
                                                                                          BayerAlgorithm==eBayerAlgorithm::Nearest    ? "Nearest"    :
                                                                                          BayerAlgorithm==eBayerAlgorithm::Bilinear   ? "Bilinear"   :
                                                                                          BayerAlgorithm==eBayerAlgorithm::VNG        ? "VNG"        : 
                                                                                          BayerAlgorithm==eBayerAlgorithm::PG         ? "PG"         :
                                                                                          BayerAlgorithm==eBayerAlgorithm::AHD        ? "AHD"        :
                                                                                          BayerAlgorithm==eBayerAlgorithm::IAHD       ? "IAHD"       :
                                                                                          BayerAlgorithm==eBayerAlgorithm::DLMMSE     ? "DLMMSE"     :
                                                                                          "INVALID";}

//=============================================================================================================================================================================

enum class eCmpExtMode : uint8
{
  FillGrey,
  EdgePixel,
  Mirror,
  Bayer,
};

//=============================================================================================================================================================================

enum ePreIntL : int32 //pre-interpolation level
{
  PIL_INVALID     = NOT_VALID,
  PIL_NOTHING     = 0,
  PIL_HALF_PEL    = 1,
  PIL_QUARTER_PEL = 2,
  PIL_HORIZONTAL  = 3,
};

//=============================================================================================================================================================================

enum eSlcTp : int8 //Slice Type
{
  X_SLICE = NOT_VALID,
  B_SLICE = 0,
  P_SLICE = 1,
  I_SLICE = 2,
  NUM_OF_SLICE_TYPES = 3
};

//=============================================================================================================================================================================

enum ePrdDir : int8
{
  PRED_DIR_IVNALID = NOT_VALID,
  PRED_DIR_L0      = 1,
  PRED_DIR_L1      = 2,
  PRED_DIR_BI      = 3,
  PRED_DIR_NUM,
};

enum class ePrdDirQ : int8
{
  PRED_DIR_IVNALID = NOT_VALID,
  PRED_DIR_L0      = 1,
  PRED_DIR_L1      = 2,
  PRED_DIR_L2      = 4,
  PRED_DIR_L3      = 8,
  PRED_DIR_BI      = 3,
  PRED_DIR_QUAD    = 15,
};

//=============================================================================================================================================================================

enum eRefPicList : int32
{
  REF_PIC_IVNALID = NOT_VALID,
  REF_PIC_L0      = 0,   // reference list 0
  REF_PIC_L1      = 1,   // reference list 1
  REF_PIC_L_NUM   = 2
};

enum class eRefPicListQ : int32
{
  REF_PIC_IVNALID = NOT_VALID,
  REF_PIC_L0      = 0,   // reference list 0
  REF_PIC_L1      = 1,   // reference list 1
  REF_PIC_L2      = 2,   // reference list 0
  REF_PIC_L3      = 3,   // reference list 1
  REF_PIC_L_NUM   = 4
};

//=============================================================================================================================================================================

enum ePrdType : int8
{
  PRED_TYPE_UNI = 0,
  PRED_TYPE_BI  = 1,
  PRED_TYPE_NUM,
};

enum class ePrdTypeQ : int8
{
  PRED_TYPE_UNI = 0,
  PRED_TYPE_BI  = 1,
  PRED_TYPE_QUAD= 2,
  PRED_TYPE_NUM,
};

//=============================================================================================================================================================================

enum eMvSearch : int32
{
  MV_SEARCH_FULL        = 0,
  MV_SEARCH_TZ          = 1,
  MV_SEARCH_LOGARITHMIC = 2
};

enum eTZPattern : int32
{
  TZ_PATTERN_SQUARE        = 0,
  TZ_PATTERN_DIAMOND       = 1,
  TZ_PATTERN_HEXAGONAL     = 2,
  TZ_PATTERN_CIRCULAR      = 3,
  TZ_PATTERN_ALTERNATING   = 4,
};

enum eTZRaster : int32
{
  TZ_GRID_STD       = 0,
  TZ_GRID_ROTATED   = 1,
};

enum eTZRefine : int32
{
  TZ_REFINE_NOTHING = 0,
  TZ_REFINE_PATTERN = 1,
  TZ_REFINE_STAR    = 2,
  TZ_REFINE_3POINT  = 3,
};

enum eCmp : int8
{
  CMP_INVALID = NOT_VALID,

  //generic
  CMP_0  = 0,
  CMP_1  = 1,
  CMP_2  = 2,
  CMP_3  = 3,

  //Y Cb Cr
  CMP_LM = 0,     //Luma      (Y)
  CMP_CB = 1,     //Chroma Cb (U)
  CMP_CR = 2,     //Chroma Cr (V)
  
  //R G B
  CMP_R  = 0,
  CMP_G  = 1,
  CMP_B  = 2,

  //additional
  CMP_A  = 3,
  CMP_D  = 3,
  
  CMP_CHROMA = 3, //Chroma Cb+Cr (U+V)
  CMP_NUM    = 3,
  CMP_ALL,
  CMP_NONE,
};

enum class eCmpType : int8
{
  INVALID = NOT_VALID,
  None    = 0,

  //Y Cb Cr
  Lm,     //Luma      (Y)
  Cb,     //Chroma Cb (U)
  Cr,     //Chroma Cr (V)
  
  //R G B
  Red,
  Green,
  Blue,

  //additional
  Alpha,
  Depth,  
};


enum class eChanel : int8
{
  INVALID = NOT_VALID,
  Luma    = 0,
  Chroma  = 1,
  NUM,
};

enum eDistMetric : int32
{
  DIST_SAD  = 0,
  DIST_SATD = 1,
  DIST_SSD  = 2,
  DIST_SSTD = 3,
  DIST_METRIC_NUM
};

enum eMotionVectorSrc : int32
{
  MVS_Zero        = 1,
  MVS_Parrent     = 2,
  MVS_BellowLeft  = 4,
  MVS_Left        = 8,
  MVS_AboveLeft   = 16,
  MVS_Above       = 32,
  MVS_AboveRight  = 64,
  MVS_Predictors  = 128, 
  MVS_PrevScaled  = 256,
};

enum eNeibCU : int32
{
  NCU_INVALID     = NOT_VALID,
  NCU_BellowLeft  = 0,
  NCU_Left        = 1,
  NCU_AboveLeft   = 2,
  NCU_Above       = 3,
  NCU_AboveRight  = 4,
  NCU_NUM,
};

enum eNeibLCU : int32
{
  NLCU_CURRENT     = -2,
  NLCU_INVALID     = NOT_VALID,
  NLCU_BellowLeft  = 0,
  NLCU_Left        = 1,
  NLCU_AboveLeft   = 2,
  NLCU_Above       = 3,
  NLCU_AboveRight  = 4,
  NLCU_Right       = 5,
  NLCU_BellowRight = 6,
  NLCU_Bellow      = 7,
  NLCU_NUM,
};

enum class eNeibUnit : int8
{
  CURRENT     = -2,
  INVALID     = NOT_VALID,
  BellowLeft  = 0,
  Left        = 1,
  AboveLeft   = 2,
  Above       = 3,
  AboveRight  = 4,
  Right       = 5,
  BellowRight = 6,
  Bellow      = 7,
  NUM,
};

//=============================================================================================================================================================================

} //end of namespace AVLib

