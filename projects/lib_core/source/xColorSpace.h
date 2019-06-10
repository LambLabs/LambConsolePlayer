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

namespace AVlib {

//===============================================================================================================================================================================================================

template <typename T = int32> class xColorSpaceCoeff
{
public:
  static constexpr uint32 c_Precision  = std::is_integral_v<T> ? xcMin((uint32)sizeof(T) << 2, (uint32)16)       : 0;
  static constexpr T      c_Mul        = std::is_integral_v<T> ? (T)(((uint64)1) << c_Precision) : 1;
  static constexpr T      c_Add        = std::is_integral_v<T> ? (T)((1 << c_Precision) >> 1) : 0;
  static constexpr double c_Rnd        = std::is_integral_v<T> ? 0.5 : 0;

public:
  static constexpr T c_RGB2YCbCr[5][9] = //{Y_R, Y_G, Y_B, U_R, U_G ,U_B, V_R, V_B, V_G}
  {
    {(T)(c_Mul *  1.00000), (T)(c_Mul *  0.00000), (T)(c_Mul *  0.00000), (T)(c_Mul *  0.00000), (T)(c_Mul *  1.00000), (T)(c_Mul *  0.00000), (T)(c_Mul *  0.00000), (T)(c_Mul *  0.00000), (T)(c_Mul *  1.00000),}, //UNKNOWN
    {(T)(c_Mul *  0.29900), (T)(c_Mul *  0.58700), (T)(c_Mul *  0.11400), (T)(c_Mul * -0.16874), (T)(c_Mul * -0.33126), (T)(c_Mul *  0.50000), (T)(c_Mul *  0.50000), (T)(c_Mul * -0.41869), (T)(c_Mul * -0.08131),}, //BT.601 / JPEG / BT.470-2 / SMPTE 170M
    {(T)(c_Mul *  0.21260), (T)(c_Mul *  0.71520), (T)(c_Mul *  0.07220), (T)(c_Mul * -0.11457), (T)(c_Mul * -0.38543), (T)(c_Mul *  0.50000), (T)(c_Mul *  0.50000), (T)(c_Mul * -0.45415), (T)(c_Mul * -0.04585),}, //BT.709
    {(T)(c_Mul *  0.25   ), (T)(c_Mul *  0.75   ), (T)(c_Mul *  0.25   ), (T)(c_Mul *  0      ), (T)(c_Mul * -1      ), (T)(c_Mul *  1      ), (T)(c_Mul *  1      ), (T)(c_Mul * -1      ), (T)(c_Mul *  0      ),}, //JPEG2000
    {(T)(c_Mul *  0.21200), (T)(c_Mul *  0.70100), (T)(c_Mul *  0.08700), (T)(c_Mul * -0.11610), (T)(c_Mul * -0.38390), (T)(c_Mul *  0.50000), (T)(c_Mul *  0.50000), (T)(c_Mul * -0.44480), (T)(c_Mul * -0.05520),}, //SMPTE240M
  };
  static constexpr T c_YCbCr2RGB[5][9] = //{R_Y, R_U, R_V, G_Y, G_U, G_V, B_Y, B_U, B_V}
  {
    {(T)(c_Mul *  1.00000), (T)(c_Mul *  0.00000), (T)(c_Mul *  0.00000), (T)(c_Mul *  0.00000), (T)(c_Mul *  1.00000), (T)(c_Mul *  0.00000), (T)(c_Mul *  0.00000), (T)(c_Mul *  0.00000), (T)(c_Mul *  1.00000),}, //UNKNOWN
    {(T)(c_Mul *  1.0    ), (T)(c_Mul *  0      ), (T)(c_Mul *  1.402  ), (T)(c_Mul *  1.0    ), (T)(c_Mul * -0.34414), (T)(c_Mul * -0.71414), (T)(c_Mul *  1.0    ), (T)(c_Mul *  1.772  ), (T)(c_Mul *  0      ),}, //BT.601 / JPEG / BT.470-2 / SMPTE 170M
    {(T)(c_Mul *  1.0    ), (T)(c_Mul *  0      ), (T)(c_Mul *  1.5748 ), (T)(c_Mul *  1.0    ), (T)(c_Mul * -0.18732), (T)(c_Mul * -0.46812), (T)(c_Mul *  1.0    ), (T)(c_Mul *  1.8556 ), (T)(c_Mul *  0      ),}, //BT.709
    {(T)(c_Mul *  1.0    ), (T)(c_Mul * -0.25   ), (T)(c_Mul *  0.75   ), (T)(c_Mul *  1.0    ), (T)(c_Mul * -0.25   ), (T)(c_Mul * -0.25   ), (T)(c_Mul *  1.0    ), (T)(c_Mul *  0.75   ), (T)(c_Mul * -0.25   ),}, //JPEG2000
    {(T)(c_Mul *  1.0    ), (T)(c_Mul *  0      ), (T)(c_Mul *  1.576  ), (T)(c_Mul *  1.0    ), (T)(c_Mul * -0.22662), (T)(c_Mul * -0.47662), (T)(c_Mul *  1.0    ), (T)(c_Mul *  1.826  ), (T)(c_Mul *  0      ),}, //SMPTE240M
  };
  static constexpr T c_RGB2XYZ[9] = { (T)(c_Mul *  0.4124564), (T)(c_Mul *  0.3575761   ), (T)(c_Mul *  0.1804375   ), (T)(c_Mul *  0.2126729), (T)(c_Mul *  0.7151522    ), (T)(c_Mul *  0.0721750    ), (T)(c_Mul *  0.0193339), (T)(c_Mul *  0.1191920   ), (T)(c_Mul *  0.9503041    ),}; //{X_R, X_G, X_B, Y_R, Y_G ,Y_B, Z_R, Z_B, Z_G} 
  static constexpr T c_YUV2XYZ[9] = { (T)(c_Mul *  0.9505   ), (T)(c_Mul *  0.1968024790), (T)(c_Mul *  0.3228406283), (T)(c_Mul *  1        ), (T)(c_Mul *  -0.1181649398), (T)(c_Mul *  -0.2126547850), (T)(c_Mul *  1.0890   ), (T)(c_Mul *  1.6432475335), (T)(c_Mul *  -0.0579345105),}; //{X_Y, X_U, X_V, Y_Y, Y_U ,Y_V, Z_Y, Z_U, Z_V}
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef AVlib_xColorSpace_IMPLEMENTATION
extern template class xColorSpaceCoeff<int32>;
extern template class xColorSpaceCoeff<int64>;
extern template class xColorSpaceCoeff<flt32>;
extern template class xColorSpaceCoeff<flt64>;
#endif // !AVlib_xColorSpace_IMPLEMENTATION

//===============================================================================================================================================================================================================

class xColorSpaceSTD
{
public:
  template <typename PelType, eClrSpc ClrSpc> static void ConvertRGB2YUV(PelType* restrict Y, PelType* restrict U, PelType* restrict V, const PelType* restrict R, const PelType* restrict G, const PelType* restrict B, uint32 Area, uint32 BitDepth);
  template <typename PelType, eClrSpc ClrSpc> static void ConvertYUV2RGB(PelType* restrict R, PelType* restrict G, PelType* restrict B, const PelType* restrict Y, const PelType* restrict U, const PelType* restrict V, uint32 Area, uint32 BitDepth);

  template <typename PelType, typename CoeffType = std::conditional_t<std::is_integral_v<PelType>, std::conditional_t<sizeof(PelType) <= 4, int32, int64>, PelType>>
  static void ConvertDEF2ABC(PelType* restrict A, PelType* restrict B, PelType* restrict C, const PelType* restrict D, const PelType* restrict E, const PelType* restrict F, uint32 Area, const CoeffType ConvertMatrix[9]);

};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename PelType, eClrSpc ClrSpc> void xColorSpaceSTD::ConvertRGB2YUV(PelType* restrict Y, PelType* restrict U, PelType* restrict V, const PelType* restrict R, const PelType* restrict G, const PelType* restrict B, uint32 Area, uint32 BitDepth)
{
  using CoeffType = std::conditional_t<std::is_integral_v<PelType>, std::conditional_t<sizeof(PelType) <= 4, int32, int64>, PelType>;

  constexpr CoeffType Y_R = xColorSpaceCoeff<CoeffType>::c_RGB2YCbCr[(uint32)ClrSpc][0];
  constexpr CoeffType Y_G = xColorSpaceCoeff<CoeffType>::c_RGB2YCbCr[(uint32)ClrSpc][1];
  constexpr CoeffType Y_B = xColorSpaceCoeff<CoeffType>::c_RGB2YCbCr[(uint32)ClrSpc][2];
  constexpr CoeffType U_R = xColorSpaceCoeff<CoeffType>::c_RGB2YCbCr[(uint32)ClrSpc][3];
  constexpr CoeffType U_G = xColorSpaceCoeff<CoeffType>::c_RGB2YCbCr[(uint32)ClrSpc][4];
  constexpr CoeffType U_B = xColorSpaceCoeff<CoeffType>::c_RGB2YCbCr[(uint32)ClrSpc][5];
  constexpr CoeffType V_R = xColorSpaceCoeff<CoeffType>::c_RGB2YCbCr[(uint32)ClrSpc][6];
  constexpr CoeffType V_G = xColorSpaceCoeff<CoeffType>::c_RGB2YCbCr[(uint32)ClrSpc][7];
  constexpr CoeffType V_B = xColorSpaceCoeff<CoeffType>::c_RGB2YCbCr[(uint32)ClrSpc][8];
  constexpr CoeffType Add = xColorSpaceCoeff<CoeffType>::c_Add;
  constexpr uint32    Shr = xColorSpaceCoeff<CoeffType>::c_Precision;

  if constexpr(std::is_integral_v<PelType>)
  {
    const CoeffType Half= 1<<(BitDepth-1);
    for(uint32 i=0; i<Area; i++)
    {
      CoeffType r = R[i];
      CoeffType g = G[i];
      CoeffType b = B[i];
      CoeffType y = ((Y_R*r + Y_G*g + Y_B*b + Add)>>Shr);
      CoeffType u = ((U_R*r + U_G*g + U_B*b + Add)>>Shr);
      CoeffType v = ((V_R*r + V_G*g + V_B*b + Add)>>Shr);
      Y[i] = (PelType)y;
      U[i] = (PelType)(u + Half);
      V[i] = (PelType)(v + Half);
    }
  }
  else //float
  {
    for(uint32 i=0; i<Area; i++)
    {
      CoeffType r = R[i];
      CoeffType g = G[i];
      CoeffType b = B[i];
      CoeffType y = Y_R*r + Y_G*g + Y_B*b;
      CoeffType u = U_R*r + U_G*g + U_B*b;
      CoeffType v = V_R*r + V_G*g + V_B*b;    
      Y[i] = (PelType)y;
      U[i] = (PelType)u;
      V[i] = (PelType)v;
    }
  }
}
template <typename PelType, eClrSpc ClrSpc> void xColorSpaceSTD::ConvertYUV2RGB(PelType* restrict R, PelType* restrict G, PelType* restrict B, const PelType* restrict Y, const PelType* restrict U, const PelType* restrict V, uint32 Area, uint32 BitDepth)
{
  using CoeffType = std::conditional_t<std::is_integral_v<PelType>, std::conditional_t<sizeof(PelType) <= 4, int32, int64>, PelType>;

  constexpr CoeffType R_Y = xColorSpaceCoeff<CoeffType>::c_YCbCr2RGB[(uint32)ClrSpc][0];
  constexpr CoeffType R_U = xColorSpaceCoeff<CoeffType>::c_YCbCr2RGB[(uint32)ClrSpc][1];
  constexpr CoeffType R_V = xColorSpaceCoeff<CoeffType>::c_YCbCr2RGB[(uint32)ClrSpc][2];
  constexpr CoeffType G_Y = xColorSpaceCoeff<CoeffType>::c_YCbCr2RGB[(uint32)ClrSpc][3];
  constexpr CoeffType G_U = xColorSpaceCoeff<CoeffType>::c_YCbCr2RGB[(uint32)ClrSpc][4];
  constexpr CoeffType G_V = xColorSpaceCoeff<CoeffType>::c_YCbCr2RGB[(uint32)ClrSpc][5];
  constexpr CoeffType B_Y = xColorSpaceCoeff<CoeffType>::c_YCbCr2RGB[(uint32)ClrSpc][6];
  constexpr CoeffType B_U = xColorSpaceCoeff<CoeffType>::c_YCbCr2RGB[(uint32)ClrSpc][7];
  constexpr CoeffType B_V = xColorSpaceCoeff<CoeffType>::c_YCbCr2RGB[(uint32)ClrSpc][8];
  constexpr CoeffType One = xColorSpaceCoeff<CoeffType>::c_Mul;
  constexpr CoeffType Add = xColorSpaceCoeff<CoeffType>::c_Add;
  constexpr uint32    Shr = xColorSpaceCoeff<CoeffType>::c_Precision;

  if constexpr(std::is_integral_v<PelType>)
  {
    const int32 Half= 1<<(BitDepth-1);
    if constexpr(R_Y == One && G_Y == One && B_Y == One && R_U == 0 && B_V == 0)
    {
      for(uint32 i=0; i<Area; i++)
      {
        CoeffType y = (((CoeffType)Y[i])<<Shr) + Add;
        CoeffType u = U[i] - Half;
        CoeffType v = V[i] - Half;
        CoeffType r = (      + R_V*v + y)>>Shr;
        CoeffType g = (G_U*u + G_V*v + y)>>Shr;
        CoeffType b = (B_U*u         + y)>>Shr;
        R[i] = (PelType)r;
        G[i] = (PelType)g;
        B[i] = (PelType)b;
      }
    }
    else
    {
      for(uint32 i=0; i<Area; i++)
      {
        CoeffType y = Y[i];
        CoeffType u = U[i] - Half;
        CoeffType v = V[i] - Half;
        CoeffType r = ((R_Y*y + R_U*u + R_V*v + Add)>>Shr);
        CoeffType g = ((G_Y*y + G_U*u + G_V*v + Add)>>Shr);
        CoeffType b = ((B_Y*y + B_U*u + B_V*v + Add)>>Shr);
        R[i] = (PelType)r;
        G[i] = (PelType)g;
        B[i] = (PelType)b;
      }
    }
  }
  else
  {
    if constexpr(R_Y == One && G_Y == One && B_Y == One && R_U == 0 && B_V == 0)
    {
      for(uint32 i=0; i<Area; i++)
      {
        CoeffType y = Y[i];
        CoeffType u = U[i];
        CoeffType v = V[i];
        CoeffType r = (      + R_V*v + y);
        CoeffType g = (G_U*u + G_V*v + y);
        CoeffType b = (B_U*u         + y);
        R[i] = (PelType)r;
        G[i] = (PelType)g;
        B[i] = (PelType)b;
      }
    }
    else
    {
      for(uint32 i=0; i<Area; i++)
      {
        CoeffType y = Y[i];
        CoeffType u = U[i];
        CoeffType v = V[i];
        CoeffType r = (R_Y*y + R_U*u + R_V*v);
        CoeffType g = (G_Y*y + G_U*u + G_V*v);
        CoeffType b = (B_Y*y + B_U*u + B_V*v);
        R[i] = (PelType)r;
        G[i] = (PelType)g;
        B[i] = (PelType)b;
      }
    }
  }
}
template <typename PelType, typename CoeffType> void xColorSpaceSTD::ConvertDEF2ABC(PelType* restrict A, PelType* restrict B, PelType* restrict C, const PelType* restrict D, const PelType* restrict E, const PelType* restrict F, uint32 Area, const CoeffType ConvertMatrix[9])
{
  const CoeffType A_Ds = ConvertMatrix[0];
  const CoeffType A_Es = ConvertMatrix[1];
  const CoeffType A_Fs = ConvertMatrix[2];
  const CoeffType B_Ds = ConvertMatrix[3];
  const CoeffType B_Es = ConvertMatrix[4];
  const CoeffType B_Fs = ConvertMatrix[5];
  const CoeffType C_Ds = ConvertMatrix[6];
  const CoeffType C_Es = ConvertMatrix[7];
  const CoeffType C_Fs = ConvertMatrix[8];
  constexpr CoeffType Add = xColorSpaceCoeff<CoeffType>::c_Add;
  constexpr uint32    Shr = xColorSpaceCoeff<CoeffType>::c_Precision;

  if constexpr(std::is_integral_v<PelType>)
  {
    for(uint32 i=0; i<Area; i++)
    {
      CoeffType d = D[i];
      CoeffType e = E[i];
      CoeffType f = F[i];
      CoeffType a = ((A_Ds*d + A_Es*e + A_Fs*f + Add)>>Shr);
      CoeffType b = ((B_Ds*d + B_Es*e + B_Fs*f + Add)>>Shr);
      CoeffType c = ((C_Ds*d + C_Es*e + C_Fs*f + Add)>>Shr);
      A[i] = (PelType)a;
      B[i] = (PelType)b;
      C[i] = (PelType)c;
    }
  }
  else //float
  {
    for(uint32 i=0; i<Area; i++)
    {
      CoeffType d = D[i];
      CoeffType e = E[i];
      CoeffType f = F[i];
      CoeffType a = (A_Ds*d + A_Es*e + A_Fs*f);
      CoeffType b = (B_Ds*d + B_Es*e + B_Fs*f);
      CoeffType c = (C_Ds*d + C_Es*e + C_Fs*f);
      A[i] = (PelType)a;
      B[i] = (PelType)b;
      C[i] = (PelType)c;
    }
  }
}

//===============================================================================================================================================================================================================

class xColorSpaceSSE
{
public:
  static void ConvertRGB2YUV_BT601(int16* restrict Y, int16* restrict U, int16* restrict V, const int16* restrict R, const int16* restrict G, const int16* restrict B, uint32 Area, uint32 BitDepth);
  static void ConvertYUV2RGB_BT601(int16* restrict R, int16* restrict G, int16* restrict B, const int16* restrict Y, const int16* restrict U, const int16* restrict V, uint32 Area, uint32 BitDepth);
  static void ConvertRGB2YUV_BT709(int16* restrict Y, int16* restrict U, int16* restrict V, const int16* restrict R, const int16* restrict G, const int16* restrict B, uint32 Area, uint32 BitDepth);
  static void ConvertYUV2RGB_BT709(int16* restrict R, int16* restrict G, int16* restrict B, const int16* restrict Y, const int16* restrict U, const int16* restrict V, uint32 Area, uint32 BitDepth);
  static void ConvertRGB2YUV_ANY  (int16* restrict Y, int16* restrict U, int16* restrict V, const int16* restrict R, const int16* restrict G, const int16* restrict B, uint32 Area, uint32 BitDepth, eClrSpc YUVColorSpace);
  static void ConvertYUV2RGB_ANY  (int16* restrict R, int16* restrict G, int16* restrict B, const int16* restrict Y, const int16* restrict U, const int16* restrict V, uint32 Area, uint32 BitDepth, eClrSpc YUVColorSpace);
  static void ConvertDEF2ABC      (int16* restrict A, int16* restrict B, int16* restrict C, const int16* restrict D, const int16* restrict E, const int16* restrict F, uint32 Area, const int32 ConvertMatrix[9]);
};

//===============================================================================================================================================================================================================

class xColorSpaceAVX
{
public:
  static void ConvertRGB2YUV_BT601(int16* restrict Y, int16* restrict U, int16* restrict V, const int16* restrict R, const int16* restrict G, const int16* restrict B, uint32 Area, uint32 BitDepth);
  static void ConvertYUV2RGB_BT601(int16* restrict R, int16* restrict G, int16* restrict B, const int16* restrict Y, const int16* restrict U, const int16* restrict V, uint32 Area, uint32 BitDepth);
  static void ConvertRGB2YUV_BT709(int16* restrict Y, int16* restrict U, int16* restrict V, const int16* restrict R, const int16* restrict G, const int16* restrict B, uint32 Area, uint32 BitDepth);
  static void ConvertYUV2RGB_BT709(int16* restrict R, int16* restrict G, int16* restrict B, const int16* restrict Y, const int16* restrict U, const int16* restrict V, uint32 Area, uint32 BitDepth);
  static void ConvertRGB2YUV_ANY  (int16* restrict Y, int16* restrict U, int16* restrict V, const int16* restrict R, const int16* restrict G, const int16* restrict B, uint32 Area, uint32 BitDepth, eClrSpc YUVColorSpace);
  static void ConvertYUV2RGB_ANY  (int16* restrict R, int16* restrict G, int16* restrict B, const int16* restrict Y, const int16* restrict U, const int16* restrict V, uint32 Area, uint32 BitDepth, eClrSpc YUVColorSpace);
};

//===============================================================================================================================================================================================================

class xColorSpace
{
protected:
  template <typename PelType> static void xConvertRGB2YUV_BT601    (PelType* Y, PelType* U, PelType* V, const PelType* R, const PelType* G, const PelType* B, uint32 Area, uint32 BitDepth) { xColorSpaceSTD::ConvertRGB2YUV<PelType, eClrSpc::BT601    >(Y, U, V, R, G, B, Area, BitDepth); }
  template <typename PelType> static void xConvertYUV2RGB_BT601    (PelType* R, PelType* G, PelType* B, const PelType* Y, const PelType* U, const PelType* V, uint32 Area, uint32 BitDepth) { xColorSpaceSTD::ConvertYUV2RGB<PelType, eClrSpc::BT601    >(R, G, B, Y, U, V, Area, BitDepth); }
  template <typename PelType> static void xConvertRGB2YUV_BT709    (PelType* Y, PelType* U, PelType* V, const PelType* R, const PelType* G, const PelType* B, uint32 Area, uint32 BitDepth) { xColorSpaceSTD::ConvertRGB2YUV<PelType, eClrSpc::BT709    >(Y, U, V, R, G, B, Area, BitDepth); }
  template <typename PelType> static void xConvertYUV2RGB_BT709    (PelType* R, PelType* G, PelType* B, const PelType* Y, const PelType* U, const PelType* V, uint32 Area, uint32 BitDepth) { xColorSpaceSTD::ConvertYUV2RGB<PelType, eClrSpc::BT709    >(R, G, B, Y, U, V, Area, BitDepth); }
  template <typename PelType> static void xConvertRGB2YUV_JPEG2000 (PelType* Y, PelType* U, PelType* V, const PelType* R, const PelType* G, const PelType* B, uint32 Area, uint32 BitDepth) { xColorSpaceSTD::ConvertRGB2YUV<PelType, eClrSpc::JPEG2000 >(Y, U, V, R, G, B, Area, BitDepth); }
  template <typename PelType> static void xConvertYUV2RGB_JPEG2000 (PelType* R, PelType* G, PelType* B, const PelType* Y, const PelType* U, const PelType* V, uint32 Area, uint32 BitDepth) { xColorSpaceSTD::ConvertYUV2RGB<PelType, eClrSpc::JPEG2000 >(R, G, B, Y, U, V, Area, BitDepth); }
  template <typename PelType> static void xConvertRGB2YUV_SMPTE240M(PelType* Y, PelType* U, PelType* V, const PelType* R, const PelType* G, const PelType* B, uint32 Area, uint32 BitDepth) { xColorSpaceSTD::ConvertRGB2YUV<PelType, eClrSpc::SMPTE240M>(Y, U, V, R, G, B, Area, BitDepth); }
  template <typename PelType> static void xConvertYUV2RGB_SMPTE240M(PelType* R, PelType* G, PelType* B, const PelType* Y, const PelType* U, const PelType* V, uint32 Area, uint32 BitDepth) { xColorSpaceSTD::ConvertYUV2RGB<PelType, eClrSpc::SMPTE240M>(R, G, B, Y, U, V, Area, BitDepth); }
  
  template <typename PelType, typename CoeffType = std::conditional_t<std::is_integral_v<PelType>, std::conditional_t<sizeof(PelType) <= 4, int32, int64>, PelType>>
  static void xConvertDEF2ABC(PelType* A, PelType* B, PelType* C, const PelType* D, const PelType* E, const PelType* F, uint32 Area, const CoeffType ConvertMatrix[9]) { xColorSpaceSTD::ConvertDEF2ABC<PelType>(A, B, C, D, E, F, Area, ConvertMatrix);}

// moved outside class due to bug 85282 in gcc (CWG 727)
//#if X_USE_AVX && X_AVX2
//  template <> inline void xConvertRGB2YUV_BT601    (int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertRGB2YUV_BT601(R, G, B, Y, U, V, Area, BitDepth                    ); }
//  template <> inline void xConvertYUV2RGB_BT601    (int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertYUV2RGB_BT601(Y, U, V, R, G, B, Area, BitDepth                    ); }
//  template <> inline void xConvertRGB2YUV_BT709    (int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertRGB2YUV_BT709(R, G, B, Y, U, V, Area, BitDepth                    ); }
//  template <> inline void xConvertYUV2RGB_BT709    (int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertYUV2RGB_BT709(Y, U, V, R, G, B, Area, BitDepth                    ); }
//  template <> inline void xConvertRGB2YUV_JPEG2000 (int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertRGB2YUV_ANY  (R, G, B, Y, U, V, Area, BitDepth, eClrSpc::JPEG2000 ); }
//  template <> inline void xConvertYUV2RGB_JPEG2000 (int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertYUV2RGB_ANY  (Y, U, V, R, G, B, Area, BitDepth, eClrSpc::JPEG2000 ); }
//  template <> inline void xConvertRGB2YUV_SMPTE240M(int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertRGB2YUV_ANY  (R, G, B, Y, U, V, Area, BitDepth, eClrSpc::SMPTE240M); }
//  template <> inline void xConvertYUV2RGB_SMPTE240M(int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertYUV2RGB_ANY  (Y, U, V, R, G, B, Area, BitDepth, eClrSpc::SMPTE240M); }
//  template <> inline void xConvertDEF2ABC          (int16* A, int16* B, int16* C, const int16* D, const int16* E, const int16* F, uint32 Area, const int32 ConvertMatrix[9]) { xColorSpaceSSE::ConvertDEF2ABC(A, B, C, D, E, F, Area, ConvertMatrix);}
//#elif X_USE_SSE && X_SSE2
//  template <> inline void xConvertRGB2YUV_BT601    (int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertRGB2YUV_BT601(R, G, B, Y, U, V, Area, BitDepth                    ); }
//  template <> inline void xConvertYUV2RGB_BT601    (int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertYUV2RGB_BT601(Y, U, V, R, G, B, Area, BitDepth                    ); }
//  template <> inline void xConvertRGB2YUV_BT709    (int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertRGB2YUV_BT709(R, G, B, Y, U, V, Area, BitDepth                    ); }
//  template <> inline void xConvertYUV2RGB_BT709    (int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertYUV2RGB_BT709(Y, U, V, R, G, B, Area, BitDepth                    ); }
//  template <> inline void xConvertRGB2YUV_JPEG2000 (int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertRGB2YUV_ANY  (R, G, B, Y, U, V, Area, BitDepth, eClrSpc::JPEG2000 ); }
//  template <> inline void xConvertYUV2RGB_JPEG2000 (int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertYUV2RGB_ANY  (Y, U, V, R, G, B, Area, BitDepth, eClrSpc::JPEG2000 ); }
//  template <> inline void xConvertRGB2YUV_SMPTE240M(int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertRGB2YUV_ANY  (R, G, B, Y, U, V, Area, BitDepth, eClrSpc::SMPTE240M); }
//  template <> inline void xConvertYUV2RGB_SMPTE240M(int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertYUV2RGB_ANY  (Y, U, V, R, G, B, Area, BitDepth, eClrSpc::SMPTE240M); }
//  template <> inline void xConvertDEF2ABC          (int16* A, int16* B, int16* C, const int16* D, const int16* E, const int16* F, uint32 Area, const int32 ConvertMatrix[9]) { xColorSpaceSSE::ConvertDEF2ABC(A, B, C, D, E, F, Area, ConvertMatrix);}
//#endif

public:
  template <typename PelType> static void convertRGB2YUV(PelType* Y, PelType* U, PelType* V, const PelType* R, const PelType* G, const PelType* B,                                     uint32 Area,                 uint32 BitDepth, eClrSpc YUVColorSpace);
  template <typename PelType> static void convertRGB2YUV(PelType* Y, PelType* U, PelType* V, const PelType* R, const PelType* G, const PelType* B, uint32 YUVStride, uint32 RGBStride, uint32 Width, uint32 Height, uint32 BitDepth, eClrSpc YUVColorSpace);
  template <typename PelType> static void convertYUV2RGB(PelType* R, PelType* G, PelType* B, const PelType* Y, const PelType* U, const PelType* V,                                     uint32 Area,                 uint32 BitDepth, eClrSpc YUVColorSpace);
  template <typename PelType> static void convertYUV2RGB(PelType* R, PelType* G, PelType* B, const PelType* Y, const PelType* U, const PelType* V, uint32 RGBStride, uint32 YUVStride, uint32 Width, uint32 Height, uint32 BitDepth, eClrSpc YUVColorSpace);
  template <typename PelType> static void convertRGB2XYZ(PelType* X, PelType* Y, PelType* Z, const PelType* R, const PelType* G, const PelType* B,                                     uint32 Area                                                        );
  template <typename PelType> static void convertRGB2XYZ(PelType* X, PelType* Y, PelType* Z, const PelType* R, const PelType* G, const PelType* B, uint32 XYZStride, uint32 RGBStride, uint32 Width, uint32 Height                                        );
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#if X_USE_AVX && X_AVX2
template <> inline void xColorSpace::xConvertRGB2YUV_BT601    (int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertRGB2YUV_BT601(R, G, B, Y, U, V, Area, BitDepth                    ); }
template <> inline void xColorSpace::xConvertYUV2RGB_BT601    (int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertYUV2RGB_BT601(Y, U, V, R, G, B, Area, BitDepth                    ); }
template <> inline void xColorSpace::xConvertRGB2YUV_BT709    (int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertRGB2YUV_BT709(R, G, B, Y, U, V, Area, BitDepth                    ); }
template <> inline void xColorSpace::xConvertYUV2RGB_BT709    (int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertYUV2RGB_BT709(Y, U, V, R, G, B, Area, BitDepth                    ); }
template <> inline void xColorSpace::xConvertRGB2YUV_JPEG2000 (int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertRGB2YUV_ANY  (R, G, B, Y, U, V, Area, BitDepth, eClrSpc::JPEG2000 ); }
template <> inline void xColorSpace::xConvertYUV2RGB_JPEG2000 (int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertYUV2RGB_ANY  (Y, U, V, R, G, B, Area, BitDepth, eClrSpc::JPEG2000 ); }
template <> inline void xColorSpace::xConvertRGB2YUV_SMPTE240M(int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertRGB2YUV_ANY  (R, G, B, Y, U, V, Area, BitDepth, eClrSpc::SMPTE240M); }
template <> inline void xColorSpace::xConvertYUV2RGB_SMPTE240M(int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceAVX::ConvertYUV2RGB_ANY  (Y, U, V, R, G, B, Area, BitDepth, eClrSpc::SMPTE240M); }
template <> inline void xColorSpace::xConvertDEF2ABC          (int16* A, int16* B, int16* C, const int16* D, const int16* E, const int16* F, uint32 Area, const int32 ConvertMatrix[9]) { xColorSpaceSSE::ConvertDEF2ABC(A, B, C, D, E, F, Area, ConvertMatrix);}
#elif X_USE_SSE && X_SSE2
template <> inline void xColorSpace::xConvertRGB2YUV_BT601    (int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertRGB2YUV_BT601(R, G, B, Y, U, V, Area, BitDepth                    ); }
template <> inline void xColorSpace::xConvertYUV2RGB_BT601    (int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertYUV2RGB_BT601(Y, U, V, R, G, B, Area, BitDepth                    ); }
template <> inline void xColorSpace::xConvertRGB2YUV_BT709    (int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertRGB2YUV_BT709(R, G, B, Y, U, V, Area, BitDepth                    ); }
template <> inline void xColorSpace::xConvertYUV2RGB_BT709    (int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertYUV2RGB_BT709(Y, U, V, R, G, B, Area, BitDepth                    ); }
template <> inline void xColorSpace::xConvertRGB2YUV_JPEG2000 (int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertRGB2YUV_ANY  (R, G, B, Y, U, V, Area, BitDepth, eClrSpc::JPEG2000 ); }
template <> inline void xColorSpace::xConvertYUV2RGB_JPEG2000 (int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertYUV2RGB_ANY  (Y, U, V, R, G, B, Area, BitDepth, eClrSpc::JPEG2000 ); }
template <> inline void xColorSpace::xConvertRGB2YUV_SMPTE240M(int16* R, int16* G, int16* B, const int16* Y, const int16* U, const int16* V, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertRGB2YUV_ANY  (R, G, B, Y, U, V, Area, BitDepth, eClrSpc::SMPTE240M); }
template <> inline void xColorSpace::xConvertYUV2RGB_SMPTE240M(int16* Y, int16* U, int16* V, const int16* R, const int16* G, const int16* B, uint32 Area, uint32 BitDepth) { xColorSpaceSSE::ConvertYUV2RGB_ANY  (Y, U, V, R, G, B, Area, BitDepth, eClrSpc::SMPTE240M); }
template <> inline void xColorSpace::xConvertDEF2ABC          (int16* A, int16* B, int16* C, const int16* D, const int16* E, const int16* F, uint32 Area, const int32 ConvertMatrix[9]) { xColorSpaceSSE::ConvertDEF2ABC(A, B, C, D, E, F, Area, ConvertMatrix);}
#endif

template <class PelType> void xColorSpace::convertRGB2YUV(PelType* Y, PelType* U, PelType* V, const PelType* R, const PelType* G, const PelType* B, uint32 Area, uint32 BitDepth, eClrSpc YUVColorSpace)
{
  switch (YUVColorSpace)
  {
    case eClrSpc::BT601    : xConvertRGB2YUV_BT601    (Y, U, V, R, G, B, Area, BitDepth); break;
    case eClrSpc::BT709    : xConvertRGB2YUV_BT709    (Y, U, V, R, G, B, Area, BitDepth); break;
    case eClrSpc::JPEG2000 : xConvertRGB2YUV_JPEG2000 (Y, U, V, R, G, B, Area, BitDepth); break;
    case eClrSpc::SMPTE240M: xConvertRGB2YUV_SMPTE240M(Y, U, V, R, G, B, Area, BitDepth); break;
    default: xAssert(0); break;
  }
}
template <class PelType> void xColorSpace::convertRGB2YUV(PelType* Y, PelType* U, PelType* V, const PelType* R, const PelType* G, const PelType* B, uint32 YUVStride, uint32 RGBStride, uint32 Width, uint32 Height, uint32 BitDepth, eClrSpc YUVColorSpace)
{
  for(uint32 y=0; y<Height; y++)
  {
    convertRGB2YUV(Y, U, V, R, G, B, Width, BitDepth, YUVColorSpace);
    R += RGBStride; G += RGBStride; B += RGBStride;
    Y += YUVStride; U += YUVStride; V += YUVStride;
  }
}
template <class PelType> void xColorSpace::convertYUV2RGB(PelType* R, PelType* G, PelType* B, const PelType* Y, const PelType* U, const PelType* V, uint32 Area, uint32 BitDepth, eClrSpc YUVColorSpace)
{
  switch (YUVColorSpace)
  {
    case eClrSpc::BT601    : xConvertYUV2RGB_BT601    (R, G, B, Y, U, V, Area, BitDepth); break;
    case eClrSpc::BT709    : xConvertYUV2RGB_BT709    (R, G, B, Y, U, V, Area, BitDepth); break;
    case eClrSpc::JPEG2000 : xConvertYUV2RGB_JPEG2000 (R, G, B, Y, U, V, Area, BitDepth); break;
    case eClrSpc::SMPTE240M: xConvertYUV2RGB_SMPTE240M(R, G, B, Y, U, V, Area, BitDepth); break;
    default: xAssert(0); break;
  }
}
template <class PelType> void xColorSpace::convertYUV2RGB(PelType* R, PelType* G, PelType* B, const PelType* Y, const PelType* U, const PelType* V, uint32 RGBStride, uint32 YUVStride, uint32 Width, uint32 Height, uint32 BitDepth, eClrSpc YUVColorSpace)
{
  for(uint32 y=0; y<Height; y++)
  {
    convertYUV2RGB(R, G, B, Y, U, V, Width, BitDepth, YUVColorSpace);
    R += RGBStride; G += RGBStride; B += RGBStride;
    Y += YUVStride; U += YUVStride; V += YUVStride;
  }
}
template <class PelType> void xColorSpace::convertRGB2XYZ(PelType* X, PelType* Y, PelType* Z, const PelType* R, const PelType* G, const PelType* B, uint32 Area                                                        )
{
  using CoeffType = std::conditional_t<std::is_integral_v<PelType>, std::conditional_t<sizeof(PelType) <= 4, int32, int64>, PelType>;
  xConvertDEF2ABC(X, Y, Z, R, G, B, Area, xColorSpaceCoeff<CoeffType>::c_RGB2XYZ);
}
template <class PelType> void xColorSpace::convertRGB2XYZ(PelType* X, PelType* Y, PelType* Z, const PelType* R, const PelType* G, const PelType* B, uint32 XYZStride, uint32 RGBStride, uint32 Width, uint32 Height)
{
  using CoeffType = std::conditional_t<std::is_integral_v<PelType>, std::conditional_t<sizeof(PelType) <= 4, int32, int64>, PelType>;
  for(uint32 y=0; y<Height; y++)
  {
    xConvertDEF2ABC(X, Y, Z, R, G, B, Width, xColorSpaceCoeff<CoeffType>::c_RGB2XYZ);
    R += RGBStride; G += RGBStride; B += RGBStride;
    X += XYZStride; Y += XYZStride; Z += XYZStride;
  }
}

//===============================================================================================================================================================================================================

} //end of namespace AVLib