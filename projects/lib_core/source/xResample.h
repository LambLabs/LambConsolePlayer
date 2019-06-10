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

class xResampleSTD
{
public:
  template <typename PelType> static PelType GetPelNearest (const PelType* restrict Src, int32 SrcStride, int32 Width, int32 Height, float PosX, float PosY);
  template <typename PelType> static PelType GetPelBilinear(const PelType* restrict Src, int32 SrcStride, int32 Width, int32 Height, float PosX, float PosY);

public:
  template <typename PelType> static void ResampleNearest         (PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight, int32 SrcWidth, int32 SrcHeight);
  template <typename PelType> static void ResampleBilinear        (PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight, int32 SrcWidth, int32 SrcHeight);
  template <typename PelType> static void ResampleNearestWindowed (PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight, int32 SrcWidth, int32 SrcHeight, int32 WindowTop, int32 WindowBottom);
  template <typename PelType> static void ResampleBilinearWindowed(PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight, int32 SrcWidth, int32 SrcHeight, int32 WindowTop, int32 WindowBottom);

  template <typename PelType> static void ResampleU2HVNearest     (PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight);
  template <typename PelType> static void ResampleU2HNearest      (PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight);  
  template <typename PelType> static void ResampleU2VNearest      (PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight);

  template <typename PelType> static void ResampleD2HVAverage     (PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight);
  template <typename PelType> static void ResampleD2VAverage      (PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight);
  template <typename PelType> static void ResampleD2HAverage      (PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight);

  template <typename PelType> static void ResampleU2HVBilinear    (PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight);
};

//===============================================================================================================================================================================================================

class xResampleSSE
{
public:
  static void ResampleU2HVNearest(int16* restrict Dst, const int16* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight);
  static void ResampleD2HVAverage(int16* restrict Dst, const int16* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight);
};

//===============================================================================================================================================================================================================

class xResampleAVX
{
public:
  static void ResampleU2HVNearest(int16* restrict Dst, const int16* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight);
  static void ResampleD2HVAverage(int16* restrict Dst, const int16* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight);
};

//===============================================================================================================================================================================================================

class xResample
{
public:
  template <typename PelType> static void ResampleNearest         (PelType* Dst, const PelType* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight, int32 SrcWidth, int32 SrcHeight) { xResampleSTD::ResampleNearest (Dst, Src, DstStride, SrcStride, DstWidth, DstHeight, SrcWidth, SrcHeight); }
  template <typename PelType> static void ResampleBilinear        (PelType* Dst, const PelType* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight, int32 SrcWidth, int32 SrcHeight) { xResampleSTD::ResampleBilinear(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight, SrcWidth, SrcHeight); }
  template <typename PelType> static void ResampleNearestWindowed (PelType* Dst, const PelType* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight, int32 SrcWidth, int32 SrcHeight, int32 WindowTop, int32 WindowBottom) { xResampleSTD::ResampleNearestWindowed (Dst, Src, DstStride, SrcStride, DstWidth, DstHeight, SrcWidth, SrcHeight, WindowTop, WindowBottom); }
  template <typename PelType> static void ResampleBilinearWindowed(PelType* Dst, const PelType* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight, int32 SrcWidth, int32 SrcHeight, int32 WindowTop, int32 WindowBottom) { xResampleSTD::ResampleBilinearWindowed(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight, SrcWidth, SrcHeight, WindowTop, WindowBottom); }

  template <typename PelType> static void ResampleU2HVNearest (PelType* Dst, const PelType* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleSTD::ResampleU2HVNearest(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }
  template <typename PelType> static void ResampleU2HNearest  (PelType* Dst, const PelType* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleSTD::ResampleU2HNearest(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }
  template <typename PelType> static void ResampleU2VNearest  (PelType* Dst, const PelType* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleSTD::ResampleU2VNearest(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }
  
  template <typename PelType> static void ResampleD2HVAverage (PelType* Dst, const PelType* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleSTD::ResampleD2HVAverage(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }
  template <typename PelType> static void ResampleD2HAverage  (PelType* Dst, const PelType* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleSTD::ResampleD2HAverage(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }
  template <typename PelType> static void ResampleD2VAverage  (PelType* Dst, const PelType* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleSTD::ResampleD2VAverage(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }

  template <typename PelType> static void ResampleU2HVBilinear(PelType* Dst, const PelType* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleSTD::ResampleU2HVBilinear(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }

// moved outside class due to bug 85282 in gcc (CWG 727)
//#if X_USE_AVX && X_AVX2
//  template <> inline void ResampleU2HVNearest(int16* Dst, const int16* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleAVX::ResampleU2HVNearest(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }
//  template <> inline void ResampleD2HVAverage(int16* Dst, const int16* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleAVX::ResampleD2HVAverage(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }
//#elif X_USE_SSE && X_SSE4_2
//  template <> inline void ResampleU2HVNearest(int16* Dst, const int16* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleSSE::ResampleU2HVNearest(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }
//  template <> inline void ResampleD2HVAverage(int16* Dst, const int16* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleSSE::ResampleD2HVAverage(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }
//#endif
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#if X_USE_AVX && X_AVX2
template <> inline void xResample::ResampleU2HVNearest(int16* Dst, const int16* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleAVX::ResampleU2HVNearest(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }
template <> inline void xResample::ResampleD2HVAverage(int16* Dst, const int16* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleAVX::ResampleD2HVAverage(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }
#elif X_USE_SSE && X_SSE4_2
template <> inline void xResample::ResampleU2HVNearest(int16* Dst, const int16* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleSSE::ResampleU2HVNearest(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }
template <> inline void xResample::ResampleD2HVAverage(int16* Dst, const int16* Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight) { xResampleSSE::ResampleD2HVAverage(Dst, Src, DstStride, SrcStride, DstWidth, DstHeight); }
#endif

//===============================================================================================================================================================================================================

template <typename PelType> PelType xResampleSTD::GetPelNearest(const PelType* restrict Src, int32 SrcStride, int32 Width, int32 Height, float PosX, float PosY)
{
  int32   X = xClipU((int32)round(PosX), Width -1);
  int32   Y = xClipU((int32)round(PosY), Height-1);
  PelType S = Src[Y*SrcStride + X];
  return S;
}
template <typename PelType> PelType xResampleSTD::GetPelBilinear(const PelType* restrict Src, int32 SrcStride, int32 Width, int32 Height, float PosX, float PosY)
{
  using IntermType = std::conditional_t<std::is_same_v<PelType, double>, PelType, float>;

  int32 X0 = xClipU(((int32)floor(PosX))  , Width -1);
  int32 X1 = xClipU(((int32)floor(PosX))+1, Width -1);
  int32 Y0 = xClipU(((int32)floor(PosY))  , Height-1);
  int32 Y1 = xClipU(((int32)floor(PosY))+1, Height-1);

  PelType A = Src[Y0*SrcStride + X0];
  PelType B = Src[Y0*SrcStride + X1];
  PelType C = Src[Y1*SrcStride + X0];
  PelType D = Src[Y1*SrcStride + X1];

  IntermType fX1 = PosX - X0;
  IntermType fX0 = 1.0f - fX1;
  IntermType fY1 = PosY - Y0;
  IntermType fY0 = 1.0f - fY1;

  IntermType R = (A*fX0 + B*fX1)*fY0 + (C*fX0 + D*fX1)*fY1;

  return xConvertSaturateRoundType<PelType, IntermType>(R);
}
template <typename PelType> void xResampleSTD::ResampleNearest(PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight, int32 SrcWidth, int32 SrcHeight)
{
  float ScaleY = (float)SrcHeight/(float)DstHeight;
  float ScaleX = (float)SrcWidth /(float)DstWidth;
  float CorrectionY = 0.5f*(ScaleY-1);
  float CorrectionX = 0.5f*(ScaleX-1);

  for(int32 y=0; y<DstHeight; y++)
  {
    float SrcY = y*ScaleY + CorrectionY;
    for(int32 x=0; x<DstWidth; x++)
    {
      float SrcX = x*ScaleX + CorrectionX;
      Dst[x] = GetPelNearest(Src, SrcStride, SrcWidth, SrcHeight, SrcX, SrcY);
    }
    Dst += DstStride;
  }
}
template <typename PelType> void xResampleSTD::ResampleBilinear(PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight, int32 SrcWidth, int32 SrcHeight)
{
  float ScaleY = (float)SrcHeight/(float)DstHeight;
  float ScaleX = (float)SrcWidth /(float)DstWidth;
  float CorrectionY = 0.5f*(ScaleY-1);
  float CorrectionX = 0.5f*(ScaleX-1);

  for(int32 y=0; y<DstHeight; y++)
  {
    float SrcY = y*ScaleY + CorrectionY;
    for(int32 x=0; x<DstWidth; x++)
    {
      float SrcX = x*ScaleX + CorrectionX;
      Dst[x] = GetPelBilinear(Src, SrcStride, SrcWidth, SrcHeight, SrcX, SrcY);
    }
    Dst += DstStride;
  }
}
template <typename PelType> void xResampleSTD::ResampleNearestWindowed(PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight, int32 SrcWidth, int32 SrcHeight, int32 WindowTop, int32 WindowBottom)
{
  xAssert(0);
}
template <typename PelType> void xResampleSTD::ResampleBilinearWindowed(PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight, int32 SrcWidth, int32 SrcHeight, int32 WindowTop, int32 WindowBottom)
{
  float ScaleY = (float)SrcHeight/(float)DstHeight;
  float ScaleX = (float)SrcWidth /(float)DstWidth;
  float CorrectionY = 0.5f*(ScaleY-1);
  float CorrectionX = 0.5f*(ScaleX-1);

  Dst += DstStride*WindowTop;
  for(int32 y=WindowTop; y<WindowBottom; y++)
  {
    float SrcY = y*ScaleY + CorrectionY;
    for(int32 x=0; x<DstWidth; x++)
    {
      float SrcX = x*ScaleX + CorrectionX;
      Dst[x] = GetPelBilinear(Src, SrcStride, SrcWidth, SrcHeight, SrcX, SrcY);
    }
    Dst += DstStride;
  }
}
template <typename PelType> void xResampleSTD::ResampleU2HVNearest(PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight)
{
  PelType *restrict DstL0 = Dst;
  PelType *restrict DstL1 = Dst + DstStride;
  int32   DstStrideMul2   = DstStride<<1;

  for(int32 y=0; y<DstHeight; y+=2)
  {
    for(int32 x=0; x<DstWidth; x+=2)
    {
      PelType S = Src[x>>1];
      DstL0[x  ] = S;
      DstL0[x+1] = S;
      DstL1[x  ] = S;
      DstL1[x+1] = S;
    }
    Src += SrcStride;
    DstL0 += DstStrideMul2;
    DstL1 += DstStrideMul2;
  }
}
template <typename PelType> void xResampleSTD::ResampleU2HNearest(PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight)
{
  for(int32 y=0; y<DstHeight; y++)
  {
    for(int32 x=0; x<DstWidth; x+=2)
    {
      PelType S = Src[x>>1];
      Dst[x  ] = S;
      Dst[x+1] = S;
    }
    Src += SrcStride;
    Dst += DstStride;
  }
}
template <typename PelType> void xResampleSTD::ResampleU2VNearest(PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight)
{
  PelType *restrict DstL0 = Dst;
  PelType *restrict DstL1 = Dst + DstStride;
  int32 DstStrideMul2   = DstStride<<1;

  for(int32 y=0; y<DstHeight; y+=2)
  {
    for(int32 x=0; x<DstWidth; x++)
    {
      PelType S = Src[x];
      DstL0[x  ] = S;
      DstL1[x  ] = S;
    }
    Src += SrcStride;
    DstL0 += DstStrideMul2;
    DstL1 += DstStrideMul2;
  }
}
template <typename PelType> void xResampleSTD::ResampleD2HVAverage(PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight)
{
  using IntermType = std::conditional_t<std::is_integral_v<PelType>, std::conditional_t<sizeof(PelType) <= 4, int32, int64>, PelType>;

  const PelType *restrict SrcL0 = Src;
  const PelType *restrict SrcL1 = Src + SrcStride;
  int32   SrcStrideMul2   = SrcStride<<1;

  for(int32 y=0; y<DstHeight; y++)
  {
    for(int32 x=0; x<DstWidth; x++)
    {
      int32 SrcX = x<<1;
      IntermType D = ((IntermType)SrcL0[SrcX  ] + (IntermType)SrcL0[SrcX+1] + (IntermType)SrcL1[SrcX  ] + (IntermType)SrcL1[SrcX+1] + 2)>>2;
      Dst[x] = (PelType)D;
    }
    SrcL0 += SrcStrideMul2;
    SrcL1 += SrcStrideMul2;
    Dst += DstStride;
  }
}
template <typename PelType> void xResampleSTD::ResampleD2HAverage(PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight)
{
  using IntermType = std::conditional_t<std::is_integral_v<PelType>, std::conditional_t<sizeof(PelType) <= 4, int32, int64>, PelType>;

  for(int32 y=0; y<DstHeight; y++)
  {
    for(int32 x=0; x<DstWidth; x++)
    {
      int32 SrcX = x<<1;
      IntermType D = ((IntermType)Src[SrcX  ] + (IntermType)Src[SrcX+1] + 1)>>1;
      Dst[x] = (PelType)D;
    }
    Src += SrcStride;
    Dst += DstStride;
  }
}
template <typename PelType> void xResampleSTD::ResampleD2VAverage(PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight)
{
  using IntermType = std::conditional_t<std::is_integral_v<PelType>, std::conditional_t<sizeof(PelType) <= 4, int32, int64>, PelType>;

  const PelType *restrict SrcL0 = Src;
  const PelType *restrict SrcL1 = Src + SrcStride;
  int32 SrcStrideMul2   = SrcStride<<1;

  for(int32 y=0; y<DstHeight; y++)
  {
    for(int32 x=0; x<DstWidth; x++)
    {
      int32 SrcX = x<<1;
      IntermType D = ((IntermType)SrcL0[SrcX  ] + (IntermType)SrcL1[SrcX  ] + 1)>>1;
      Dst[x] = (PelType)D;
    }
    SrcL0 += SrcStrideMul2;
    SrcL1 += SrcStrideMul2;
    Dst += DstStride;
  }
}
template <typename PelType> void xResampleSTD::ResampleU2HVBilinear(PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight)
{
  using IntermType = std::conditional_t<std::is_integral_v<PelType>, std::conditional_t<sizeof(PelType) <= 4, int32, int64>, PelType>;

  int32 DstStrideMul2 = DstStride<<1;

  //left-top sample
  Dst[0] = Src[0];

  //top line
  for(int32 x=1; x<DstWidth-1; x+=2)
  {
    int32      SrcX = x>>1;
    IntermType SrcA = Src[SrcX  ];
    IntermType SrcB = Src[SrcX+1];
    Dst[x    ] = (PelType)((((IntermType)177 * SrcA + (IntermType)79 * SrcB) + (IntermType)128) >> 8);
    Dst[x + 1] = (PelType)((((IntermType)177 * SrcB + (IntermType)79 * SrcA) + (IntermType)128) >> 8);
  }

  //right-top sample
  Dst[DstWidth-1] = Src[(DstWidth>>1)-1];

  Dst += DstStride;

  //center lines
  const PelType* restrict SrcL0 = Src;
  const PelType* restrict SrcL1 = Src + SrcStride;
  PelType* restrict DstL0 = Dst;
  PelType* restrict DstL1 = Dst + DstStride;

  for(int32 y=1; y<DstHeight-1; y+=2)
  {
    //left collumn
    IntermType SrcA = SrcL0[0];
    IntermType SrcB = SrcL1[0];
    DstL0[0] = (PelType)((((IntermType)177 * SrcA + (IntermType)79 * SrcB) + (IntermType)128) >> 8);
    DstL1[0] = (PelType)((((IntermType)177 * SrcB + (IntermType)79 * SrcA) + (IntermType)128) >> 8);

    //center collumns
    for(int32 x=1; x<DstWidth-1; x+=2)
    {
      int32      SrcX = x>>1;
      IntermType SrcA = SrcL0[SrcX  ];
      IntermType SrcB = SrcL0[SrcX+1];
      IntermType SrcC = SrcL1[SrcX  ];
      IntermType SrcD = SrcL1[SrcX+1];
      DstL0[x    ] = (PelType)((((IntermType)116 * SrcA + (IntermType)51 * (SrcB + SrcC) + (IntermType)38 * SrcD) + (IntermType)128) >> 8);
      DstL0[x + 1] = (PelType)((((IntermType)116 * SrcB + (IntermType)51 * (SrcA + SrcD) + (IntermType)38 * SrcC) + (IntermType)128) >> 8);
      DstL1[x    ] = (PelType)((((IntermType)116 * SrcC + (IntermType)51 * (SrcA + SrcD) + (IntermType)38 * SrcB) + (IntermType)128) >> 8);
      DstL1[x + 1] = (PelType)((((IntermType)116 * SrcD + (IntermType)51 * (SrcB + SrcC) + (IntermType)38 * SrcA) + (IntermType)128) >> 8);
    }

    //right collumn
    IntermType SrcC = SrcL0[(DstWidth>>1)-1]; 
    IntermType SrcD = SrcL1[(DstWidth>>1)-1];
    DstL0[DstWidth - 1] = (PelType)((((IntermType)177 * SrcC + (IntermType)79 * SrcD) + (IntermType)128) >> 8);
    DstL1[DstWidth - 1] = (PelType)((((IntermType)177 * SrcD + (IntermType)79 * SrcC) + (IntermType)128) >> 8);

    SrcL0 += SrcStride;
    SrcL1 += SrcStride;      
    DstL0 += DstStrideMul2;
    DstL1 += DstStrideMul2;
  }

  //left-bottom sample
  DstL0[0] = SrcL0[0]; 

  //bottom line
  for(int32 x=1; x<DstWidth-1; x+=2) 
  {
    int32 SrcX = x>>1;
    IntermType SrcA = Src[SrcX  ];
    IntermType SrcB = Src[SrcX+1];
    Dst[x    ] = (PelType)((((IntermType)177 * SrcA + (IntermType)79 * SrcB) + (IntermType)128) >> 8);
    Dst[x + 1] = (PelType)((((IntermType)177 * SrcB + (IntermType)79 * SrcA) + (IntermType)128) >> 8);
  }

  //right-bottom sample
  DstL0[DstWidth-1] = SrcL0[(DstWidth>>1)-1];
}

//===============================================================================================================================================================================================================

} //end of namespace AVLib