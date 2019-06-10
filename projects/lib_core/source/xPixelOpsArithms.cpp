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

#include "xPixelOpsArithm.h"

namespace AVlib {

//===============================================================================================================================================================================================================

void xPixelOpsArithm::xAddSat_STD(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area)
{
  for(int32 i=0; i<Area; i++)
  { 
    Dst[i] = (int16)xClipS16(((int32)Src0[i]) + ((int32)Src1[i]));   
  }
}
void xPixelOpsArithm::xAddSat_STD(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height)
{
  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {      
      Dst[x] = (int16)xClipS16(((int32)Src0[x]) + ((int32)Src1[x]));
    }
    Src0 += Src0Stride;
    Src1 += Src1Stride;
    Dst  += DstStride;  
  }
}
void xPixelOpsArithm::xAddAndClipU_STD(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area, int16 ClippingRange)
{
  for(int32 i=0; i<Area; i++)
  { 
    Dst[i] = (int16)xClipU<int32>((int32)Src0[i] + (int32)Src1[i], (int32)ClippingRange);   
  }
}
void xPixelOpsArithm::xAddAndClipU_STD(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange)
{
  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {      
      Dst[x] = xClipU<int16>(Src0[x] + Src1[x], ClippingRange);
    }
    Src0 += Src0Stride;
    Src1 += Src1Stride;
    Dst  += DstStride;  
  }
}
void xPixelOpsArithm::xSubSat_STD(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area)
{
  for(int32 i=0; i<Area; i++)
  {
    Dst[i] = (int16)xClipS16(((int32)Src0[i]) - ((int32)Src1[i]));   
  }
}
void xPixelOpsArithm::xSubSat_STD(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height)
{
  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {      
      Dst[x] = (int16)xClipS16(((int32)Src0[x]) - ((int32)Src1[x]));   
    }
    Src0 += Src0Stride;
    Src1 += Src1Stride;
    Dst  += DstStride;  
  }
}
void xPixelOpsArithm::xMulConst_STD(int16* restrict Dst, int16* restrict Src, int32 Area, const int16 Multiplier)
{
  for(int32 i=0; i<Area; i++)
  { 
    Dst[i] = Src[i] * Multiplier;   
  }
}
void xPixelOpsArithm::xMulConst_STD(int16* restrict Dst, int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Multiplier)
{
  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {      
      Dst[x] = Src[x] * Multiplier;
    }
    Src += SrcStride;
    Dst += DstStride;  
  }
}
void xPixelOpsArithm::xBiPredRefEst_STD(int16* restrict Dst, int16* restrict Org, int16* restrict Prd0, int32 Area)
{
  for(int32 i=0; i<Area; i++)
  {
    Dst[i] = (Org[i]<<1) - Prd0[i];
  }
}
void xPixelOpsArithm::xBiPredRefEst_STD(int16* restrict Dst, int16* restrict Org, int16* restrict Prd0, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Width, int32 Height)
{
  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {
      Dst[x] = (Org[x]<<1) - Prd0[x];
    }
    Org  += OrgStride;
    Prd0 += Prd0Stride;
    Dst  += DstStride;    
  }
}
void xPixelOpsArithm::xQuadPredRefEst_STD(int16* restrict Dst, int16* restrict Org, int16* restrict Prd0, int16* restrict Prd1, int16* restrict Prd2, int32 Area)
{
  for(int32 i=0; i<Area; i++)
  {
    Dst[i] = (Org[i]<<2) - Prd0[i] - Prd1[i] - Prd2[i];
  }
}
void xPixelOpsArithm::xQuadPredRefEst_STD(int16* restrict Dst, int16* restrict Org, int16* restrict Prd0, int16* restrict Prd1, int16* restrict Prd2, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Prd1Stride, int32 Prd2Stride, int32 Width, int32 Height)
{
  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {
      Dst[x] = (Org[x]<<2) - Prd0[x] - Prd1[x] - Prd2[x];
    }
    Org  += OrgStride;
    Prd0 += Prd0Stride;
    Prd1 += Prd1Stride;
    Prd2 += Prd2Stride;
    Dst  += DstStride;    
  }
}
void xPixelOpsArithm::xAvg_STD(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area)
{
  for(int32 i=0; i<Area; i++)
  { 
    Dst[i] = (Src0[i] + Src1[i] + 1)>>1;
  }
}
void xPixelOpsArithm::xAvg_STD(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height)
{
  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {
      Dst[x] = (Src0[x] + Src1[x] + 1)>>1;
    }
    Src0 += Src0Stride;
    Src1 += Src1Stride;
    Dst  += DstStride;    
  }
}
void xPixelOpsArithm::xAvgAndClipU_STD(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area, int16 ClippingRange)
{
  for(int32 i=0; i<Area; i++)
  { 
    Dst[i] = xClipU<int16>(((Src0[i] + Src1[i] + 1)>>1), ClippingRange);
  }
}
void xPixelOpsArithm::xAvgAndClipU_STD(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange)
{
  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {
      Dst[x] = xClipU<int16>(((Src0[x] + Src1[x] + 1)>>1), ClippingRange);
    }
    Src0 += Src0Stride;
    Src1 += Src1Stride;
    Dst  += DstStride;    
  }
}
void xPixelOpsArithm::xClip_STD(int16* restrict Dst, int16* restrict Src, int32 Area, int16 ClippingRangeLow, int16 ClippingRangeHigh)
{
  for(int32 i=0; i<Area; i++)
  { 
    Dst[i] = xClip<int16>(Src[i], ClippingRangeLow, ClippingRangeHigh);
  }
}
void xPixelOpsArithm::xClip_STD(int16* restrict Dst, int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int16 ClippingRangeLow, int16 ClippingRangeHigh)
{
  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {
      Dst[x] = xClip<int16>(Src[x], ClippingRangeLow, ClippingRangeHigh);
    }
    Src += SrcStride;
    Dst += DstStride;    
  }
}
void xPixelOpsArithm::xThreshold_STD(int16* restrict Dst, int16* restrict Src, int32 Area, const int16 Threshold)
{
  for(int32 i=0; i<Area; i++)
  { 
    Dst[i] = xThreshold(Src[i], Threshold);   
  }
}
void xPixelOpsArithm::xThreshold_STD(int16* restrict Dst, int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Threshold)
{
  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {
      Dst[x] = xThreshold(Src[x], Threshold);   
    }
    Src += SrcStride;
    Dst += DstStride;    
  }
}
uint32 xPixelOpsArithm::xCountNonZero_STD(int16* restrict Src, int32 Area)
{
  uint32 NumNonZero = 0;

  for(int32 i=0; i<Area; i+=16)
  {
    if(Src[i]!=0) { NumNonZero++; }
  }
  return NumNonZero;
}
uint32 xPixelOpsArithm::xCountNonZero_STD(int16* restrict Src, int32 SrcStride, int32 Width, int32 Height)
{
  uint32 NumNonZero = 0;

  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {
      if(Src[x]!=0) { NumNonZero++; }
    }
    Src += SrcStride;
  }

  return NumNonZero;
}
int32 xPixelOpsArithm::xSumAll_STD(int16* restrict Src, int32 Area)
{
  int32 Sum = 0;

  for(int32 i=0; i<Area; i++)
  { 
    Sum += Src[i];   
  }
  return Sum;
}
int32 xPixelOpsArithm::xSumAll_STD(int16* restrict Src, int32 SrcStride, int32 Width, int32 Height)
{
  int32 Sum = 0;

  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {
      Sum += Src[x];
    }
    Src += SrcStride;
  }
  return Sum;
}

//===============================================================================================================================================================================================================

#if X_SSE4_2
void xPixelOpsArithm::xAddSat_SSE(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area)
{
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);
  for(int32 i=0; i<Area8; i+=8)
  {
    __m128i Src0Block  = _mm_loadu_si128((__m128i*)&Src0[i]);
    __m128i Src1Block  = _mm_loadu_si128((__m128i*)&Src1[i]);
    __m128i Add        = _mm_adds_epi16(Src0Block, Src1Block);
    _mm_storeu_si128((__m128i*)&Dst[i], Add);
  }
  for(int32 i=Area8; i<Area; i++)
  {
    Dst[i] = (int16)xClipS16(((int32)Src0[i]) + ((int32)Src1[i]));
  }
}
void xPixelOpsArithm::xAddSat_SSE(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height)
{
  if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=8)
      {
        __m128i Src0Block  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i Add        = _mm_adds_epi16(Src0Block, Src1Block); //
        _mm_storeu_si128((__m128i*)&Dst[x], Add);
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;  
    }
  }
  else
  {
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width8; x+=8)
      {
        __m128i Src0Block  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i Add        = _mm_adds_epi16(Src0Block, Src1Block); //
        _mm_storeu_si128((__m128i*)&Dst[x], Add);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i Src0Block  = _mm_loadl_epi64((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadl_epi64((__m128i*)&Src1[x]);
        __m128i Add        = _mm_adds_epi16(Src0Block, Src1Block); //
        _mm_storel_epi64((__m128i*)&Dst[x], Add);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = (int16)xClipS16(((int32)Src0[x]) + ((int32)Src1[x]));
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;  
    }
  }
}
void xPixelOpsArithm::xAddAndClipU_SSE(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area, int16 ClippingRange)
{
  const __m128i Zero   = _mm_setzero_si128();
  const __m128i MaxVal = _mm_set1_epi16(ClippingRange);
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);

  for(int32 i=0; i<Area8; i+=8)
  {
    __m128i Src0Block  = _mm_loadu_si128((__m128i*)&Src0[i]);
    __m128i Src1Block  = _mm_loadu_si128((__m128i*)&Src1[i]);
    __m128i AddClip    = _mm_adds_epi16(Src0Block, Src1Block); //
    AddClip = _mm_min_epi16(AddClip, MaxVal);
    AddClip = _mm_max_epi16(AddClip, Zero);
    _mm_storeu_si128((__m128i*)&Dst[i], AddClip);
  }
  for(int32 i=Area8; i<Area; i+=8)
  {
    Dst[i] = (int16)xClipU<int32>((int32)Src0[i] + (int32)Src1[i], (int32)ClippingRange);
  }
}
void xPixelOpsArithm::xAddAndClipU_SSE(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange)
{
  const __m128i Zero   = _mm_setzero_si128();
  const __m128i MaxVal = _mm_set1_epi16(ClippingRange);

  if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=8)
      {
        __m128i Src0Block  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i AddClip    = _mm_adds_epi16(Src0Block, Src1Block); //
        AddClip = _mm_min_epi16(AddClip, MaxVal);
        AddClip = _mm_max_epi16(AddClip, Zero);
        _mm_storeu_si128((__m128i*)&Dst[x], AddClip);
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;  
    }
  }
  else
  {
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width8; x+=8)
      {
        __m128i Src0Block  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i AddClip    = _mm_adds_epi16(Src0Block, Src1Block); //
        AddClip = _mm_min_epi16(AddClip, MaxVal);
        AddClip = _mm_max_epi16(AddClip, Zero);
        _mm_storeu_si128((__m128i*)&Dst[x], AddClip);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i Src0Block  = _mm_loadl_epi64((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadl_epi64((__m128i*)&Src1[x]);
        __m128i AddClip    = _mm_adds_epi16(Src0Block, Src1Block); //
        AddClip = _mm_min_epi16(AddClip, MaxVal);
        AddClip = _mm_max_epi16(AddClip, Zero);
        _mm_storel_epi64((__m128i*)&Dst[x], AddClip);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = xClipU<int16>(Src0[x] + Src1[x], ClippingRange);
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;  
    }
  }
}
void xPixelOpsArithm::xSubSat_SSE(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area)
{
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);

  for(int32 i=0; i<Area8; i+=8)
  {
    __m128i Src0Block  = _mm_loadu_si128((__m128i*)&Src0[i]);
    __m128i Src1Block  = _mm_loadu_si128((__m128i*)&Src1[i]);
    __m128i Sub        = _mm_subs_epi16(Src0Block, Src1Block);
    _mm_storeu_si128((__m128i*)&Dst[i], Sub);
  }
  for(int32 i=Area8; i<Area; i++)
  {
    Dst[i] = (int16)xClipS16(((int32)Src0[i]) - ((int32)Src1[i]));   
  }
}
void xPixelOpsArithm::xSubSat_SSE(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height)
{
  if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=8)
      {
        __m128i Src0Block  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i Sub        = _mm_subs_epi16(Src0Block, Src1Block); //
        _mm_storeu_si128((__m128i*)&Dst[x], Sub);
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;  
    }
  }
  else
  {
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width8; x+=8)
      {
        __m128i Src0Block  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i Sub        = _mm_subs_epi16(Src0Block, Src1Block); //
        _mm_storeu_si128((__m128i*)&Dst[x], Sub);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i Src0Block  = _mm_loadl_epi64((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadl_epi64((__m128i*)&Src1[x]);
        __m128i Sub        = _mm_subs_epi16(Src0Block, Src1Block); //
        _mm_storel_epi64((__m128i*)&Dst[x], Sub);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = (int16)xClipS16(((int32)Src0[x]) - ((int32)Src1[x]));
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;  
    }
  }
}
void xPixelOpsArithm::xMulConst_SSE(int16* restrict Dst, int16* restrict Src, int32 Area, const int16 Multiplier)
{
  __m128i MultiplierV = _mm_set1_epi16(Multiplier);
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);
  for(int32 i=0; i<Area8; i+=8)
  {
    __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[i]);
    __m128i MulV = _mm_mullo_epi16(SrcV, MultiplierV); //
    _mm_storeu_si128((__m128i*)&Dst[i], MulV);
  }
  for(int32 i=Area8; i<Area; i++)
  {
    Dst[i] = Src[i] * Multiplier; 
  }
}
void xPixelOpsArithm::xMulConst_SSE(int16* restrict Dst, int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Multiplier)
{
  __m128i MultiplierV = _mm_set1_epi16(Multiplier);

  if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=8)
      {
        __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x]);
        __m128i MulV = _mm_mullo_epi16(SrcV, MultiplierV); //
        _mm_storeu_si128((__m128i*)&Dst[x], MulV);
      }
      Src += SrcStride;
      Dst += DstStride;  
    }
  }
  else
  {
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width8; x+=8)
      {
        __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x]);
        __m128i MulV = _mm_mullo_epi16(SrcV, MultiplierV); //
        _mm_storeu_si128((__m128i*)&Dst[x], MulV);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i SrcV  = _mm_loadl_epi64((__m128i*)&Src[x]);
        __m128i MulV = _mm_mullo_epi16(SrcV, MultiplierV); //
        _mm_storel_epi64((__m128i*)&Dst[x], MulV);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = Src[x] * Multiplier;
      }
      Src += SrcStride;
      Dst += DstStride;  
    }
  }
}
void xPixelOpsArithm::xBiPredRefEst_SSE(int16* restrict Dst, int16* restrict Org, int16* restrict Prd0, int32 Area)
{
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);

  for(int32 i=0; i<Area8; i+=8)
  {
    __m128i OrgBlock   = _mm_loadu_si128((__m128i*)&Org[i]);
    __m128i Prd0Block  = _mm_loadu_si128((__m128i*)&Prd0[i]);
    OrgBlock           = _mm_slli_epi16(OrgBlock, 1);
    __m128i Sub        = _mm_subs_epi16(OrgBlock, Prd0Block);
    _mm_storeu_si128((__m128i*)&Dst[i], Sub);
  }
  for(int32 i=Area8; i<Area; i++)
  {
    Dst[i] = (Org[i]<<1) - Prd0[i];
  }
}
void xPixelOpsArithm::xBiPredRefEst_SSE(int16* restrict Dst, int16* restrict Org, int16* restrict Prd0, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Width, int32 Height)
{
  if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=8)
      {
        __m128i OrgBlock   = _mm_loadu_si128((__m128i*)&Org[x]);
        __m128i Prd0Block  = _mm_loadu_si128((__m128i*)&Prd0[x]);
        OrgBlock           = _mm_slli_epi16(OrgBlock, 1);
        __m128i Sub        = _mm_subs_epi16(OrgBlock, Prd0Block);
        _mm_storeu_si128((__m128i*)&Dst[x], Sub);
      }
      Org += OrgStride;
      Prd0 += Prd0Stride;
      Dst  += DstStride;    
    }
  }
  else
  {
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width8; x+=8)
      {
        __m128i OrgBlock   = _mm_loadu_si128((__m128i*)&Org[x]);
        __m128i Prd0Block  = _mm_loadu_si128((__m128i*)&Prd0[x]);
        OrgBlock           = _mm_slli_epi16(OrgBlock, 1);
        __m128i Sub        = _mm_subs_epi16(OrgBlock, Prd0Block);
        _mm_storeu_si128((__m128i*)&Dst[x], Sub);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i OrgBlock   = _mm_loadl_epi64((__m128i*)&Org[x]);
        __m128i Prd0Block  = _mm_loadl_epi64((__m128i*)&Prd0[x]);
        OrgBlock           = _mm_slli_epi16(OrgBlock, 1);
        __m128i Sub        = _mm_subs_epi16(OrgBlock, Prd0Block);
        _mm_storel_epi64((__m128i*)&Dst[x], Sub);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = (Org[x]<<1) - Prd0[x];
      }
      Org  += OrgStride;
      Prd0 += Prd0Stride;
      Dst  += DstStride;    
    }
  }
}
void xPixelOpsArithm::xQuadPredRefEst_SSE(int16* restrict Dst, int16* restrict Org, int16* restrict Prd0, int16* restrict Prd1, int16* restrict Prd2, int32 Area)
{
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);

  for(int32 i=0; i<Area8; i+=8)
  {
    __m128i OrgBlock   = _mm_loadu_si128((__m128i*)&Org[i]);
    __m128i Prd0Block  = _mm_loadu_si128((__m128i*)&Prd0[i]);
    __m128i Prd1Block  = _mm_loadu_si128((__m128i*)&Prd1[i]);
    __m128i Prd2Block  = _mm_loadu_si128((__m128i*)&Prd2[i]);
    OrgBlock           = _mm_slli_epi16(OrgBlock, 1);
    __m128i TmpSub     = _mm_subs_epi16(OrgBlock, Prd0Block);
    __m128i TmpSum     = _mm_adds_epi16(Prd1Block, Prd2Block);
    __m128i Sub        = _mm_subs_epi16(TmpSub, TmpSum);
    _mm_storeu_si128((__m128i*)&Dst[i], Sub);
  }
  for(int32 i=Area8; i<Area; i++)
  {
    Dst[i] = (Org[i]<<2) - Prd0[i] - Prd1[i] - Prd2[i];
  }
}
void xPixelOpsArithm::xQuadPredRefEst_SSE(int16* restrict Dst, int16* restrict Org, int16* restrict Prd0, int16* restrict Prd1, int16* restrict Prd2, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Prd1Stride, int32 Prd2Stride, int32 Width, int32 Height)
{
  if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=8)
      {
        __m128i OrgBlock   = _mm_loadu_si128((__m128i*)&Org[x]);
        __m128i Prd0Block  = _mm_loadu_si128((__m128i*)&Prd0[x]);
        __m128i Prd1Block  = _mm_loadu_si128((__m128i*)&Prd1[x]);
        __m128i Prd2Block  = _mm_loadu_si128((__m128i*)&Prd2[x]);
        OrgBlock           = _mm_slli_epi16(OrgBlock, 1);
        __m128i TmpSub     = _mm_subs_epi16(OrgBlock, Prd0Block);
        __m128i TmpSum     = _mm_adds_epi16(Prd1Block, Prd2Block);
        __m128i Sub        = _mm_subs_epi16(TmpSub, TmpSum);
        _mm_storeu_si128((__m128i*)&Dst[x], Sub);
      }
      Org  += OrgStride;
      Prd0 += Prd0Stride;
      Prd1 += Prd1Stride;
      Prd2 += Prd2Stride;
      Dst  += DstStride;    
    }
  }
  else
  {
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width8; x+=8)
      {
        __m128i OrgBlock   = _mm_loadu_si128((__m128i*)&Org[x]);
        __m128i Prd0Block  = _mm_loadu_si128((__m128i*)&Prd0[x]);
        __m128i Prd1Block  = _mm_loadu_si128((__m128i*)&Prd1[x]);
        __m128i Prd2Block  = _mm_loadu_si128((__m128i*)&Prd2[x]);
        OrgBlock           = _mm_slli_epi16(OrgBlock, 1);
        __m128i TmpSub     = _mm_subs_epi16(OrgBlock, Prd0Block);
        __m128i TmpSum     = _mm_adds_epi16(Prd1Block, Prd2Block);
        __m128i Sub        = _mm_subs_epi16(TmpSub, TmpSum);
        _mm_storeu_si128((__m128i*)&Dst[x], Sub);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i OrgBlock   = _mm_loadl_epi64((__m128i*)&Org[x]);
        __m128i Prd0Block  = _mm_loadl_epi64((__m128i*)&Prd0[x]);
        __m128i Prd1Block  = _mm_loadl_epi64((__m128i*)&Prd1[x]);
        __m128i Prd2Block  = _mm_loadl_epi64((__m128i*)&Prd2[x]);
        OrgBlock           = _mm_slli_epi16(OrgBlock, 1);
        __m128i TmpSub     = _mm_subs_epi16(OrgBlock, Prd0Block);
        __m128i TmpSum     = _mm_adds_epi16(Prd1Block, Prd2Block);
        __m128i Sub        = _mm_subs_epi16(TmpSub, TmpSum);
        _mm_storel_epi64((__m128i*)&Dst[x], Sub);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = (Org[x]<<2) - Prd0[x] - Prd1[x] - Prd2[x];
      }
      Org  += OrgStride;
      Prd0 += Prd0Stride;
      Prd1 += Prd1Stride;
      Prd2 += Prd2Stride;
      Dst  += DstStride;    
    }
  }
}
void xPixelOpsArithm::xAvg_SSE(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area)
{
  const __m128i OffsetV = _mm_set1_epi16(1);
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);

  for(int32 i=0; i<Area8; i+=8)
  {
    __m128i Src0BlockV  = _mm_loadu_si128((__m128i*)&Src0[i]);
    __m128i Src1BlockV  = _mm_loadu_si128((__m128i*)&Src1[i]);
    __m128i AddV        = _mm_adds_epi16(_mm_adds_epi16(Src0BlockV, Src1BlockV), OffsetV);
    __m128i ShiftV      = _mm_srai_epi16(AddV, 1);
    _mm_storeu_si128((__m128i*)&Dst[i], ShiftV);
  }
  for(int32 i=Area8; i<Area; i++)
  { 
    Dst[i] = (Src0[i] + Src1[i] + 1)>>1;
  }
}
void xPixelOpsArithm::xAvg_SSE(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height)
{
  const __m128i OffsetV = _mm_set1_epi16(1);

  if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=8)
      {
        __m128i Src0BlockV  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1BlockV  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i AddV        = _mm_adds_epi16(_mm_adds_epi16(Src0BlockV, Src1BlockV), OffsetV);
        __m128i ShiftV      = _mm_srai_epi16(AddV, 1);
        _mm_storeu_si128((__m128i*)&Dst[x], ShiftV);
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;    
    }
  }
  else
  {
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width8; x+=8)
      {
        __m128i Src0BlockV  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1BlockV  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i AddV        = _mm_adds_epi16(_mm_adds_epi16(Src0BlockV, Src1BlockV), OffsetV);
        __m128i ShiftV      = _mm_srai_epi16(AddV, 1);
        _mm_storeu_si128((__m128i*)&Dst[x], ShiftV);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i Src0BlockV  = _mm_loadl_epi64((__m128i*)&Src0[x]);
        __m128i Src1BlockV  = _mm_loadl_epi64((__m128i*)&Src1[x]);
        __m128i AddV        = _mm_adds_epi16(_mm_adds_epi16(Src0BlockV, Src1BlockV), OffsetV);
        __m128i ShiftV      = _mm_srai_epi16(AddV, 1);
        _mm_storel_epi64((__m128i*)&Dst[x], ShiftV);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = (Src0[x] + Src1[x] + 1)>>1;
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;    
    }
  }
}
void xPixelOpsArithm::xAvgAndClipU_SSE(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area, int16 ClippingRange)
{
  const __m128i ZeroV   = _mm_setzero_si128();
  const __m128i OffsetV = _mm_set1_epi16(1);
  const __m128i MaxValV = _mm_set1_epi16(ClippingRange);
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);

  for(int32 i=0; i<Area8; i+=8)
  {
    __m128i Src0BlockV  = _mm_loadu_si128((__m128i*)&Src0[i]);
    __m128i Src1BlockV  = _mm_loadu_si128((__m128i*)&Src1[i]);
    __m128i AddV        = _mm_adds_epi16(_mm_adds_epi16(Src0BlockV, Src1BlockV), OffsetV);
    __m128i ShiftV      = _mm_srai_epi16(AddV, 1);
    __m128i ClipV       = _mm_max_epi16(_mm_min_epi16(ShiftV, MaxValV),ZeroV);
    _mm_storeu_si128((__m128i*)&Dst[i], ClipV);
  }
  for(int32 i=Area8; i<Area; i++)
  { 
    Dst[i] = xClipU<int16>(((Src0[i] + Src1[i] + 1)>>1), ClippingRange);
  }
}
void xPixelOpsArithm::xAvgAndClipU_SSE(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange)
{
  const __m128i ZeroV   = _mm_setzero_si128();
  const __m128i OffsetV = _mm_set1_epi16(1);
  const __m128i MaxValV = _mm_set1_epi16(ClippingRange);

  if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=8)
      {
        __m128i Src0BlockV  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1BlockV  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i AddV        = _mm_adds_epi16(_mm_adds_epi16(Src0BlockV, Src1BlockV), OffsetV);
        __m128i ShiftV      = _mm_srai_epi16(AddV, 1);
        __m128i ClipV       = _mm_max_epi16(_mm_min_epi16(ShiftV, MaxValV), ZeroV);
        _mm_storeu_si128((__m128i*)&Dst[x], ClipV);
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;    
    }
  }
  else
  {
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width8; x+=8)
      {
        __m128i Src0BlockV  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1BlockV  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i AddV        = _mm_adds_epi16(_mm_adds_epi16(Src0BlockV, Src1BlockV), OffsetV);
        __m128i ShiftV      = _mm_srai_epi16(AddV, 1);
        __m128i ClipV       = _mm_max_epi16(_mm_min_epi16(ShiftV, MaxValV), ZeroV);
        _mm_storeu_si128((__m128i*)&Dst[x], ClipV);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i Src0BlockV  = _mm_loadl_epi64((__m128i*)&Src0[x]);
        __m128i Src1BlockV  = _mm_loadl_epi64((__m128i*)&Src1[x]);
        __m128i AddV        = _mm_adds_epi16(_mm_adds_epi16(Src0BlockV, Src1BlockV), OffsetV);
        __m128i ShiftV      = _mm_srai_epi16(AddV, 1);
        __m128i ClipV       = _mm_max_epi16(_mm_min_epi16(ShiftV, MaxValV),ZeroV);
        _mm_storel_epi64((__m128i*)&Dst[x], ClipV);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = xClipU<int16>(((Src0[x] + Src1[x] + 1)>>1), ClippingRange);
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;    
    }
  }
}
void xPixelOpsArithm::xClip_SSE(int16* restrict Dst, int16* restrict Src, int32 Area, int16 ClippingRangeLow, int16 ClippingRangeHigh)
{
  const __m128i RangeLowV  = _mm_set1_epi16(ClippingRangeLow );
  const __m128i RangeHighV = _mm_set1_epi16(ClippingRangeHigh);
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);

  for(int32 i=0; i<Area8; i+=8)
  {
    __m128i SrcBlockV  = _mm_loadu_si128((__m128i*)&Src[i]);
    __m128i ClipV      = _mm_max_epi16(_mm_min_epi16(SrcBlockV, RangeHighV), RangeLowV);
    _mm_storeu_si128((__m128i*)&Dst[i], ClipV);
  }
  for(int32 i=Area8; i<Area; i++)
  { 
    Dst[i] = xClip<int16>(Src[i], ClippingRangeLow, ClippingRangeHigh);
  }
}
void xPixelOpsArithm::xClip_SSE(int16* restrict Dst, int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int16 ClippingRangeLow, int16 ClippingRangeHigh)
{
  const __m128i RangeLowV  = _mm_set1_epi16(ClippingRangeLow );
  const __m128i RangeHighV = _mm_set1_epi16(ClippingRangeHigh);

  if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=8)
      {
        __m128i SrcBlockV  = _mm_loadu_si128((__m128i*)&Src[x]);
        __m128i ClipV      = _mm_max_epi16(_mm_min_epi16(SrcBlockV, RangeHighV), RangeLowV);
        _mm_storeu_si128((__m128i*)&Dst[x], ClipV);
      }
      Src += SrcStride;
      Dst += DstStride;    
    }
  }
  else
  {
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width8; x+=8)
      {
        __m128i SrcBlockV  = _mm_loadu_si128((__m128i*)&Src[x]);
        __m128i ClipV      = _mm_max_epi16(_mm_min_epi16(SrcBlockV, RangeHighV), RangeLowV);
        _mm_storeu_si128((__m128i*)&Dst[x], ClipV);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i SrcBlockV  = _mm_loadl_epi64((__m128i*)&Src[x]);
        __m128i ClipV      = _mm_max_epi16(_mm_min_epi16(SrcBlockV, RangeHighV), RangeLowV);
        _mm_storel_epi64((__m128i*)&Dst[x], ClipV);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = xClip<int16>(Src[x], ClippingRangeLow, ClippingRangeHigh);
      }
      Src += SrcStride;
      Dst += DstStride;    
    }
  }
}
void xPixelOpsArithm::xThreshold_SSE(int16* restrict Dst, int16* restrict Src, int32 Area, const int16 Threshold)
{
  const __m128i ThresholdV = _mm_set1_epi16(Threshold);
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);

  for(int32 i=0; i<Area8; i+=8)
  {
    __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[i]);
    __m128i CmpV = _mm_cmpgt_epi16(SrcV, ThresholdV); //0 - <=, 0xFFFF - >
    __m128i DstV = _mm_srli_epi16(CmpV, 15);       
    _mm_storeu_si128((__m128i*)&Dst[i], DstV);
  }
  for(int32 i=Area8; i<Area; i++)
  {
    Dst[i] = xThreshold(Src[i], Threshold); 
  }
}
void xPixelOpsArithm::xThreshold_SSE(int16* restrict Dst, int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Threshold)
{
  const __m128i ThresholdV = _mm_set1_epi16(Threshold);

  if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=8)
      {
        __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x]);
        __m128i CmpV = _mm_cmpgt_epi16(SrcV, ThresholdV); //0 - <=, 0xFFFF - >
        __m128i DstV = _mm_srli_epi16(CmpV, 15);       
        _mm_storeu_si128((__m128i*)&Dst[x], DstV);
      }
      Src += SrcStride;
      Dst += DstStride;    
    }
  }
  else
  {
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width8; x+=8)
      {
        __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x]);
        __m128i CmpV = _mm_cmpgt_epi16(SrcV, ThresholdV); //0 - <=, 0xFFFF - >
        __m128i DstV = _mm_srli_epi16(CmpV, 15);       
        _mm_storeu_si128((__m128i*)&Dst[x], DstV);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i SrcV = _mm_loadl_epi64((__m128i*)&Src[x]);
        __m128i CmpV = _mm_cmpgt_epi16(SrcV, ThresholdV); //0 - <=, 0xFFFF - >
        __m128i DstV = _mm_srli_epi16(CmpV, 15);       
        _mm_storel_epi64((__m128i*)&Dst[x], DstV);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = xThreshold(Src[x], Threshold);
      }
      Src += SrcStride;
      Dst += DstStride;    
    }
  }
}
uint32 xPixelOpsArithm::xCountNonZero_SSE(int16* restrict Src, int32 Area)
{
  uint32  NumNonZero = 0;
  __m128i ZeroV = _mm_setzero_si128();
  int32   Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);

  for(int32 i=0; i<Area16; i+=16)
  {
    __m128i CoeffsA = _mm_loadu_si128((__m128i*)&Src[i  ]);
    __m128i CoeffsB = _mm_loadu_si128((__m128i*)&Src[i+8]);
    __m128i MasksA  = _mm_cmpeq_epi16(CoeffsA, ZeroV);
    __m128i MasksB  = _mm_cmpeq_epi16(CoeffsB, ZeroV);
    __m128i Masks   = _mm_packs_epi16(MasksA, MasksB);
    uint32 Mask     = (~_mm_movemask_epi8(Masks)) & 0xFFFF;
    uint32 NumOnes  = _mm_popcnt_u32(Mask);
    NumNonZero += NumOnes;
  }
  for(int32 i=Area16; i<Area; i++)
  {
    if(Src[i]!=0) { NumNonZero++; }
  }
  return NumNonZero;
}
uint32 xPixelOpsArithm::xCountNonZero_SSE(int16* restrict Src, int32 SrcStride, int32 Width, int32 Height)
{
  uint32  NumNonZero = 0;
  __m128i ZeroV = _mm_setzero_si128();

  if(((uint32)Width & (uint32)c_RemainderMask16)==0) //Width%16==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=16)
      {
        __m128i CoeffsA = _mm_loadu_si128((__m128i*)&Src[x  ]);
        __m128i CoeffsB = _mm_loadu_si128((__m128i*)&Src[x+8]);
        __m128i MasksA  = _mm_cmpeq_epi16(CoeffsA, ZeroV);
        __m128i MasksB  = _mm_cmpeq_epi16(CoeffsB, ZeroV);
        __m128i Masks   = _mm_packs_epi16(MasksA, MasksB);
        uint32 Mask     = (~_mm_movemask_epi8(Masks)) & 0xFFFF;
        uint32 NumOnes  = _mm_popcnt_u32(Mask);
        NumNonZero += NumOnes;
      }
      Src += SrcStride;
    }
  }
  else
  {
    int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8 );
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4 );

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width16; x+=16)
      {
        __m128i CoeffsA = _mm_loadu_si128((__m128i*)&Src[x  ]);
        __m128i CoeffsB = _mm_loadu_si128((__m128i*)&Src[x+8]);
        __m128i MasksA  = _mm_cmpeq_epi16(CoeffsA, ZeroV);
        __m128i MasksB  = _mm_cmpeq_epi16(CoeffsB, ZeroV);
        __m128i Masks   = _mm_packs_epi16(MasksA, MasksB);
        uint32 Mask     = (~_mm_movemask_epi8(Masks)) & 0xFFFF;
        uint32 NumOnes  = _mm_popcnt_u32(Mask);
        NumNonZero += NumOnes;
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i Coeffs  = _mm_loadu_si128((__m128i*)&Src[x  ]);
        __m128i MasksA  = _mm_cmpeq_epi16(Coeffs, ZeroV);
        __m128i Masks   = _mm_packs_epi16(MasksA, ZeroV);
        uint32 Mask     = (~_mm_movemask_epi8(Masks)) & 0xFFFF;
        uint32 NumOnes  = _mm_popcnt_u32(Mask);
        NumNonZero += NumOnes;
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i Coeffs  = _mm_loadl_epi64((__m128i*)&Src[x  ]);
        __m128i MasksA  = _mm_cmpeq_epi16(Coeffs, ZeroV);
        __m128i Masks   = _mm_packs_epi16(MasksA, ZeroV);
        uint32 Mask     = (~_mm_movemask_epi8(Masks)) & 0xFFFF;
        uint32 NumOnes  = _mm_popcnt_u32(Mask);
        NumNonZero += NumOnes;
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        if(Src[x]!=0) { NumNonZero++; }
      }
      Src += SrcStride;
    }
  }

  return NumNonZero;
}
int32 xPixelOpsArithm::xSumAll_SSE(int16* restrict Src, int32 Area)
{  
  int32   Sum   = 0;
  __m128i SumV  = _mm_setzero_si128();
  int32   Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);

  for(int32 i=0; i<Area8; i+=8)
  {
    __m128i SrcV  = _mm_loadu_si128((__m128i*)&Src[i]);
    __m128i SrcVA = _mm_cvtepi16_epi32(SrcV);
    __m128i SrcVB = _mm_cvtepi16_epi32(_mm_srli_si128(SrcV,8));
    __m128i TmpV  = _mm_add_epi32(SrcVA, SrcVB);
    SumV = _mm_add_epi32(SumV, TmpV);
  }
  for(int32 i=Area8; i<Area; i++)
  { 
    Sum += Src[i];
  }
  SumV = _mm_hadd_epi32(SumV, SumV);
  SumV = _mm_hadd_epi32(SumV, SumV);
  Sum += _mm_extract_epi32(SumV, 0);
  return Sum;
}
int32 xPixelOpsArithm::xSumAll_SSE(int16* restrict Src, int32 SrcStride, int32 Width, int32 Height)
{
  int32   Sum   = 0;
  __m128i SumV  = _mm_setzero_si128();

  if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=8)
      {
        __m128i SrcV  = _mm_loadu_si128((__m128i*)&Src[x]);
        __m128i SrcVA = _mm_cvtepi16_epi32(SrcV);
        __m128i SrcVB = _mm_cvtepi16_epi32(_mm_srli_si128(SrcV,8));
        __m128i TmpV  = _mm_add_epi32(SrcVA, SrcVB);
        SumV = _mm_add_epi32(SumV, TmpV);
      }
      Src += SrcStride;
    }
  }
  else
  {
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width8; x+=8)
      {
        __m128i SrcV  = _mm_loadu_si128((__m128i*)&Src[x]);
        __m128i SrcVA = _mm_cvtepi16_epi32(SrcV);
        __m128i SrcVB = _mm_cvtepi16_epi32(_mm_srli_si128(SrcV,8));
        __m128i TmpV  = _mm_add_epi32(SrcVA, SrcVB);
        SumV = _mm_add_epi32(SumV, TmpV);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i SrcV  = _mm_loadl_epi64((__m128i*)&Src[x]);
        __m128i SrcVA = _mm_cvtepi16_epi32(SrcV);
        SumV = _mm_add_epi32(SumV, SrcVA);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Sum += Src[x];
      }
      Src += SrcStride;
    }
  }
  SumV = _mm_hadd_epi32(SumV, SumV);
  SumV = _mm_hadd_epi32(SumV, SumV);
  Sum += _mm_extract_epi32(SumV, 0);
  return Sum;
}
#endif //X_SSE4_2

//===============================================================================================================================================================================================================

#if X_AVX1 && X_AVX2
void xPixelOpsArithm::xAddSat_AVX(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  for(int32 i=0; i<Area16; i+=16)
  {
    __m256i Src0Block  = _mm256_loadu_si256((__m256i*)&Src0[i]);
    __m256i Src1Block  = _mm256_loadu_si256((__m256i*)&Src1[i]);
    __m256i Add        = _mm256_adds_epi16(Src0Block, Src1Block);
    _mm256_storeu_si256((__m256i*)&Dst[i], Add);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    Dst[i] = (int16)xClipS16(((int32)Src0[i]) + ((int32)Src1[i]));
  }
}
void xPixelOpsArithm::xAddSat_AVX(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height)
{
  if(((uint32)Width & (uint32)0xF)==0) //Width%16==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=16)
      {
        __m256i Src0Block  = _mm256_loadu_si256((__m256i*)&Src0[x]);
        __m256i Src1Block  = _mm256_loadu_si256((__m256i*)&Src1[x]);
        __m256i Add        = _mm256_adds_epi16(Src0Block, Src1Block);
        _mm256_storeu_si256((__m256i*)&Dst[x], Add);
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;  
    }
  }
  else
  {
    int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width16; x+=16)
      {
        __m256i Src0Block  = _mm256_loadu_si256((__m256i*)&Src0[x]);
        __m256i Src1Block  = _mm256_loadu_si256((__m256i*)&Src1[x]);
        __m256i Add        = _mm256_adds_epi16(Src0Block, Src1Block);
        _mm256_storeu_si256((__m256i*)&Dst[x], Add);
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i Src0Block  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i Add        = _mm_adds_epi16(Src0Block, Src1Block); //
        _mm_storeu_si128((__m128i*)&Dst[x], Add);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i Src0Block  = _mm_loadl_epi64((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadl_epi64((__m128i*)&Src1[x]);
        __m128i Add        = _mm_adds_epi16(Src0Block, Src1Block); //
        _mm_storel_epi64((__m128i*)&Dst[x], Add);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = (int16)xClipS16(((int32)Src0[x]) + ((int32)Src1[x]));
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;  
    }
  }
}
void xPixelOpsArithm::xAddAndClipU_AVX(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area, int16 ClippingRange)
{
  const __m256i Zero   = _mm256_setzero_si256();
  const __m256i MaxVal = _mm256_set1_epi16(ClippingRange);
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);

  for(int32 i=0; i<Area16; i+=16)
  {
    __m256i Src0Block  = _mm256_loadu_si256((__m256i*)&Src0[i]);
    __m256i Src1Block  = _mm256_loadu_si256((__m256i*)&Src1[i]);
    __m256i AddClip    = _mm256_adds_epi16(Src0Block, Src1Block); //
    AddClip = _mm256_min_epi16(AddClip, MaxVal);
    AddClip = _mm256_max_epi16(AddClip, Zero);
    _mm256_storeu_si256((__m256i*)&Dst[i], AddClip);
  }
  for(int32 i=Area16; i<Area; i+=8)
  {
    Dst[i] = (int16)xClipU<int32>((int32)Src0[i] + (int32)Src1[i], (int32)ClippingRange);
  }
}
void xPixelOpsArithm::xAddAndClipU_AVX(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange)
{
  const __m256i Zero   = _mm256_setzero_si256();
  const __m256i MaxVal = _mm256_set1_epi16(ClippingRange);

  if(((uint32)Width & (uint32)0xF)==0) //Width%16==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=16)
      {
        __m256i Src0Block  = _mm256_loadu_si256((__m256i*)&Src0[x]);
        __m256i Src1Block  = _mm256_loadu_si256((__m256i*)&Src1[x]);
        __m256i AddClip    = _mm256_adds_epi16(Src0Block, Src1Block); //
        AddClip = _mm256_min_epi16(AddClip, MaxVal);
        AddClip = _mm256_max_epi16(AddClip, Zero);
        _mm256_storeu_si256((__m256i*)&Dst[x], AddClip);
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;  
    }
  }
  else
  {
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width8; x+=8)
      {
        __m128i Src0Block  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i AddClip    = _mm_adds_epi16(Src0Block, Src1Block); //
        AddClip = _mm_min_epi16(AddClip, _mm256_castsi256_si128(MaxVal));
        AddClip = _mm_max_epi16(AddClip, _mm256_castsi256_si128(Zero));
        _mm_storeu_si128((__m128i*)&Dst[x], AddClip);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i Src0Block  = _mm_loadl_epi64((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadl_epi64((__m128i*)&Src1[x]);
        __m128i AddClip    = _mm_adds_epi16(Src0Block, Src1Block); //
        AddClip = _mm_min_epi16(AddClip, _mm256_castsi256_si128(MaxVal));
        AddClip = _mm_max_epi16(AddClip, _mm256_castsi256_si128(Zero));
        _mm_storel_epi64((__m128i*)&Dst[x], AddClip);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = xClipU<int16>(Src0[x] + Src1[x], ClippingRange);
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;  
    }
  }
}
void xPixelOpsArithm::xSubSat_AVX(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);

  for(int32 i=0; i<Area16; i+=16)
  {
    __m256i Src0Block  = _mm256_loadu_si256((__m256i*)&Src0[i]);
    __m256i Src1Block  = _mm256_loadu_si256((__m256i*)&Src1[i]);
    __m256i Sub        = _mm256_subs_epi16(Src0Block, Src1Block);
    _mm256_storeu_si256((__m256i*)&Dst[i], Sub);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    Dst[i] = (int16)xClipS16(((int32)Src0[i]) - ((int32)Src1[i]));   
  }
}
void xPixelOpsArithm::xSubSat_AVX(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height)
{
  if(((uint32)Width & (uint32)0xF)==0) //Width%16==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=16)
      {
        __m256i Src0Block  = _mm256_loadu_si256((__m256i*)&Src0[x]);
        __m256i Src1Block  = _mm256_loadu_si256((__m256i*)&Src1[x]);
        __m256i Sub        = _mm256_subs_epi16(Src0Block, Src1Block);
        _mm256_storeu_si256((__m256i*)&Dst[x], Sub);
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;  
    }
  }
  else
  {
    int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width16; x+=16)
      {
        __m256i Src0Block  = _mm256_loadu_si256((__m256i*)&Src0[x]);
        __m256i Src1Block  = _mm256_loadu_si256((__m256i*)&Src1[x]);
        __m256i Sub        = _mm256_subs_epi16(Src0Block, Src1Block);
        _mm256_storeu_si256((__m256i*)&Dst[x], Sub);
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i Src0Block  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i Sub        = _mm_subs_epi16(Src0Block, Src1Block); //
        _mm_storeu_si128((__m128i*)&Dst[x], Sub);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i Src0Block  = _mm_loadl_epi64((__m128i*)&Src0[x]);
        __m128i Src1Block  = _mm_loadl_epi64((__m128i*)&Src1[x]);
        __m128i Sub        = _mm_subs_epi16(Src0Block, Src1Block); //
        _mm_storel_epi64((__m128i*)&Dst[x], Sub);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = (int16)xClipS16(((int32)Src0[x]) - ((int32)Src1[x]));
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;  
    }
  }
}
void xPixelOpsArithm::xMulConst_AVX(int16* restrict Dst, int16* restrict Src, int32 Area, const int16 Multiplier)
{
  __m256i MultiplierV = _mm256_set1_epi16(Multiplier);
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  for(int32 i=0; i<Area16; i+=16)
  {
    __m256i SrcV  = _mm256_loadu_si256((__m256i*)&Src[i]);
    __m256i MulV   = _mm256_mullo_epi16(SrcV, MultiplierV);
    _mm256_storeu_si256((__m256i*)&Dst[i], MulV);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    Dst[i] = Src[i] * Multiplier; 
  }
}
void xPixelOpsArithm::xMulConst_AVX(int16* restrict Dst, int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Multiplier)
{
  __m256i MultiplierV = _mm256_set1_epi16(Multiplier);

  if(((uint32)Width & (uint32)0xF)==0) //Width%8==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=16)
      {
        __m256i SrcV  = _mm256_loadu_si256((__m256i*)&Src[x]);
        __m256i MulV   = _mm256_mullo_epi16(SrcV, MultiplierV);
        _mm256_storeu_si256((__m256i*)&Dst[x], MulV);
      }
      Src += SrcStride;
      Dst += DstStride;  
    }
  }
  else
  {
    int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width16; x+=16)
      {
        __m256i SrcV  = _mm256_loadu_si256((__m256i*)&Src[x]);
        __m256i MulV   = _mm256_mullo_epi16(SrcV, MultiplierV);
        _mm256_storeu_si256((__m256i*)&Dst[x], MulV);
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x]);
        __m128i MulV = _mm_mullo_epi16(SrcV, _mm256_castsi256_si128(MultiplierV)); //
        _mm_storeu_si128((__m128i*)&Dst[x], MulV);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i SrcV  = _mm_loadl_epi64((__m128i*)&Src[x]);
        __m128i MulV = _mm_mullo_epi16(SrcV, _mm256_castsi256_si128(MultiplierV)); //
        _mm_storel_epi64((__m128i*)&Dst[x], MulV);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = Src[x] * Multiplier;
      }
      Src += SrcStride;
      Dst += DstStride;  
    }
  }
}
void xPixelOpsArithm::xBiPredRefEst_AVX(int16* restrict Dst, int16* restrict Org, int16* restrict Prd0, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);

  for(int32 i=0; i<Area16; i+=16)
  {
    __m256i OrgBlock   = _mm256_loadu_si256((__m256i*)&Org[i]);
    __m256i Prd0Block  = _mm256_loadu_si256((__m256i*)&Prd0[i]);
    OrgBlock           = _mm256_slli_epi16(OrgBlock, 1);
    __m256i Sub        = _mm256_subs_epi16(OrgBlock, Prd0Block);
    _mm256_storeu_si256((__m256i*)&Dst[i], Sub);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    Dst[i] = (Org[i]<<1) - Prd0[i];
  }
}
void xPixelOpsArithm::xBiPredRefEst_AVX(int16* restrict Dst, int16* restrict Org, int16* restrict Prd0, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Width, int32 Height)
{
  if(((uint32)Width & (uint32)0xF)==0) //Width%16==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=16)
      {
        __m256i OrgBlock   = _mm256_loadu_si256((__m256i*)&Org[x]);
        __m256i Prd0Block  = _mm256_loadu_si256((__m256i*)&Prd0[x]);
        OrgBlock           = _mm256_slli_epi16(OrgBlock, 1);
        __m256i Sub        = _mm256_subs_epi16(OrgBlock, Prd0Block);
        _mm256_storeu_si256((__m256i*)&Dst[x], Sub);
      }
      Org  += OrgStride;
      Prd0 += Prd0Stride;
      Dst  += DstStride;    
    }
  }
  else
  {
    int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width16; x+=16)
      {
        __m256i OrgBlock   = _mm256_loadu_si256((__m256i*)&Org[x]);
        __m256i Prd0Block  = _mm256_loadu_si256((__m256i*)&Prd0[x]);
        OrgBlock           = _mm256_slli_epi16(OrgBlock, 1);
        __m256i Sub        = _mm256_subs_epi16(OrgBlock, Prd0Block);
        _mm256_storeu_si256((__m256i*)&Dst[x], Sub);
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i OrgBlock   = _mm_loadu_si128((__m128i*)&Org[x]);
        __m128i Prd0Block  = _mm_loadu_si128((__m128i*)&Prd0[x]);
        OrgBlock           = _mm_slli_epi16(OrgBlock, 1);
        __m128i Sub        = _mm_subs_epi16(OrgBlock, Prd0Block);
        _mm_storeu_si128((__m128i*)&Dst[x], Sub);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i OrgBlock   = _mm_loadl_epi64((__m128i*)&Org[x]);
        __m128i Prd0Block  = _mm_loadl_epi64((__m128i*)&Prd0[x]);
        OrgBlock           = _mm_slli_epi16(OrgBlock, 1);
        __m128i Sub        = _mm_subs_epi16(OrgBlock, Prd0Block);
        _mm_storel_epi64((__m128i*)&Dst[x], Sub);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = (Org[x]<<1) - Prd0[x];
      }
      Org  += OrgStride;
      Prd0 += Prd0Stride;
      Dst  += DstStride;    
    }
  }
}
void xPixelOpsArithm::xQuadPredRefEst_AVX(int16* restrict Dst, int16* restrict Org, int16* restrict Prd0, int16* restrict Prd1, int16* restrict Prd2, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);

  for(int32 i=0; i<Area16; i+=16)
  {
    __m256i OrgBlock   = _mm256_loadu_si256((__m256i*)&Org[i]);
    __m256i Prd0Block  = _mm256_loadu_si256((__m256i*)&Prd0[i]);
    __m256i Prd1Block  = _mm256_loadu_si256((__m256i*)&Prd1[i]);
    __m256i Prd2Block  = _mm256_loadu_si256((__m256i*)&Prd2[i]);
    OrgBlock           = _mm256_slli_epi16(OrgBlock, 1);
    __m256i TmpSub     = _mm256_subs_epi16(OrgBlock, Prd0Block);
    __m256i TmpSum     = _mm256_adds_epi16(Prd1Block, Prd2Block);
    __m256i Sub        = _mm256_subs_epi16(TmpSub, TmpSum);
    _mm256_storeu_si256((__m256i*)&Dst[i], Sub);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    Dst[i] = (Org[i]<<1) - Prd0[i];
  }
}
void xPixelOpsArithm::xQuadPredRefEst_AVX(int16* restrict Dst, int16* restrict Org, int16* restrict Prd0, int16* restrict Prd1, int16* restrict Prd2, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Prd1Stride, int32 Prd2Stride, int32 Width, int32 Height)
{
  if(((uint32)Width & (uint32)0xF)==0) //Width%16==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=16)
      {
        __m256i OrgBlock   = _mm256_loadu_si256((__m256i*)&Org[x]);
        __m256i Prd0Block  = _mm256_loadu_si256((__m256i*)&Prd0[x]);
        __m256i Prd1Block  = _mm256_loadu_si256((__m256i*)&Prd1[x]);
        __m256i Prd2Block  = _mm256_loadu_si256((__m256i*)&Prd2[x]);
        OrgBlock           = _mm256_slli_epi16(OrgBlock, 1);
        __m256i TmpSub     = _mm256_subs_epi16(OrgBlock, Prd0Block);
        __m256i TmpSum     = _mm256_adds_epi16(Prd1Block, Prd2Block);
        __m256i Sub        = _mm256_subs_epi16(TmpSub, TmpSum);
        _mm256_storeu_si256((__m256i*)&Dst[x], Sub);
      }
      Org  += OrgStride;
      Prd0 += Prd0Stride;
      Prd1 += Prd1Stride;
      Prd2 += Prd2Stride;
      Dst  += DstStride;    
    }
  }
  else
  {
    int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width16; x+=16)
      {
        __m256i OrgBlock   = _mm256_loadu_si256((__m256i*)&Org[x]);
        __m256i Prd0Block  = _mm256_loadu_si256((__m256i*)&Prd0[x]);
        __m256i Prd1Block  = _mm256_loadu_si256((__m256i*)&Prd1[x]);
        __m256i Prd2Block  = _mm256_loadu_si256((__m256i*)&Prd2[x]);
        OrgBlock           = _mm256_slli_epi16(OrgBlock, 1);
        __m256i TmpSub     = _mm256_subs_epi16(OrgBlock, Prd0Block);
        __m256i TmpSum     = _mm256_adds_epi16(Prd1Block, Prd2Block);
        __m256i Sub        = _mm256_subs_epi16(TmpSub, TmpSum);
        _mm256_storeu_si256((__m256i*)&Dst[x], Sub);
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i OrgBlock   = _mm_loadu_si128((__m128i*)&Org[x]);
        __m128i Prd0Block  = _mm_loadu_si128((__m128i*)&Prd0[x]);
        __m128i Prd1Block  = _mm_loadu_si128((__m128i*)&Prd1[x]);
        __m128i Prd2Block  = _mm_loadu_si128((__m128i*)&Prd2[x]);
        OrgBlock           = _mm_slli_epi16(OrgBlock, 1);
        __m128i TmpSub     = _mm_subs_epi16(OrgBlock, Prd0Block);
        __m128i TmpSum     = _mm_adds_epi16(Prd1Block, Prd2Block);
        __m128i Sub        = _mm_subs_epi16(TmpSub, TmpSum);
        _mm_storeu_si128((__m128i*)&Dst[x], Sub);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i OrgBlock   = _mm_loadl_epi64((__m128i*)&Org[x]);
        __m128i Prd0Block  = _mm_loadl_epi64((__m128i*)&Prd0[x]);
        __m128i Prd1Block  = _mm_loadl_epi64((__m128i*)&Prd1[x]);
        __m128i Prd2Block  = _mm_loadl_epi64((__m128i*)&Prd2[x]);
        OrgBlock           = _mm_slli_epi16(OrgBlock, 1);
        __m128i TmpSub     = _mm_subs_epi16(OrgBlock, Prd0Block);
        __m128i TmpSum     = _mm_adds_epi16(Prd1Block, Prd2Block);
        __m128i Sub        = _mm_subs_epi16(TmpSub, TmpSum);
        _mm_storel_epi64((__m128i*)&Dst[x], Sub);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = (Org[x]<<2) - Prd0[x] - Prd1[x] - Prd2[x];
      }
      Org  += OrgStride;
      Prd0 += Prd0Stride;
      Prd1 += Prd1Stride;
      Prd2 += Prd2Stride;
      Dst  += DstStride;    
    }
  }
}
void xPixelOpsArithm::xAvg_AVX(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area)
{
  const __m256i OffsetV = _mm256_set1_epi16(1);
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);

  for(int32 i=0; i<Area16; i+=16)
  {
    __m256i Src0BlockV  = _mm256_loadu_si256((__m256i*)&Src0[i]);
    __m256i Src1BlockV  = _mm256_loadu_si256((__m256i*)&Src1[i]);
    __m256i AddV        = _mm256_adds_epi16(_mm256_adds_epi16(Src0BlockV, Src1BlockV), OffsetV);
    __m256i ShiftV      = _mm256_srai_epi16(AddV, 1);
    _mm256_storeu_si256((__m256i*)&Dst[i], ShiftV);
  }
  for(int32 i=Area16; i<Area; i++)
  { 
    Dst[i] = (Src0[i] + Src1[i] + 1)>>1;
  }
}
void xPixelOpsArithm::xAvg_AVX(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height)
{
  const __m256i OffsetV = _mm256_set1_epi16(1);

  if(((uint32)Width & (uint32)0xF)==0) //Width%16==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=16)
      {
        __m256i Src0BlockV  = _mm256_loadu_si256((__m256i*)&Src0[x]);
        __m256i Src1BlockV  = _mm256_loadu_si256((__m256i*)&Src1[x]);
        __m256i AddV        = _mm256_adds_epi16(_mm256_adds_epi16(Src0BlockV, Src1BlockV), OffsetV);
        __m256i ShiftV      = _mm256_srai_epi16(AddV, 1);
        _mm256_storeu_si256((__m256i*)&Dst[x], ShiftV);
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;    
    }
  }
  else
  {
    int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width16; x+=8)
      {
        __m256i Src0BlockV  = _mm256_loadu_si256((__m256i*)&Src0[x]);
        __m256i Src1BlockV  = _mm256_loadu_si256((__m256i*)&Src1[x]);
        __m256i AddV        = _mm256_adds_epi16(_mm256_adds_epi16(Src0BlockV, Src1BlockV), OffsetV);
        __m256i ShiftV      = _mm256_srai_epi16(AddV, 1);
        _mm256_storeu_si256((__m256i*)&Dst[x], ShiftV);
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i Src0BlockV  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1BlockV  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i AddV        = _mm_adds_epi16(_mm_adds_epi16(Src0BlockV, Src1BlockV), _mm256_castsi256_si128(OffsetV));
        __m128i ShiftV      = _mm_srai_epi16(AddV, 1);
        _mm_storeu_si128((__m128i*)&Dst[x], ShiftV);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i Src0BlockV  = _mm_loadl_epi64((__m128i*)&Src0[x]);
        __m128i Src1BlockV  = _mm_loadl_epi64((__m128i*)&Src1[x]);
        __m128i AddV        = _mm_adds_epi16(_mm_adds_epi16(Src0BlockV, Src1BlockV), _mm256_castsi256_si128(OffsetV));
        __m128i ShiftV      = _mm_srai_epi16(AddV, 1);
        _mm_storel_epi64((__m128i*)&Dst[x], ShiftV);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = (Src0[x] + Src1[x] + 1)>>1;
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;    
    }
  }
}
void xPixelOpsArithm::xAvgAndClipU_AVX(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 Area, int16 ClippingRange)
{
  const __m256i ZeroV   = _mm256_setzero_si256();
  const __m256i OffsetV = _mm256_set1_epi16(1);
  const __m256i MaxValV = _mm256_set1_epi16(ClippingRange);
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);

  for(int32 i=0; i<Area16; i+=16)
  {
    __m256i Src0BlockV  = _mm256_loadu_si256((__m256i*)&Src0[i]);
    __m256i Src1BlockV  = _mm256_loadu_si256((__m256i*)&Src1[i]);
    __m256i AddV        = _mm256_adds_epi16(_mm256_adds_epi16(Src0BlockV, Src1BlockV), OffsetV);
    __m256i ShiftV      = _mm256_srai_epi16(AddV, 1);
    __m256i ClipV       = _mm256_max_epi16(_mm256_min_epi16(ShiftV, MaxValV),ZeroV);
    _mm256_storeu_si256((__m256i*)&Dst[i], ClipV);
  }
  for(int32 i=Area16; i<Area; i++)
  { 
    Dst[i] = xClipU<int16>(((Src0[i] + Src1[i] + 1)>>1), ClippingRange);
  }
}
void xPixelOpsArithm::xAvgAndClipU_AVX(int16* restrict Dst, int16* restrict Src0, int16* restrict Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange)
{
  const __m256i ZeroV   = _mm256_setzero_si256();
  const __m256i OffsetV = _mm256_set1_epi16(1);
  const __m256i MaxValV = _mm256_set1_epi16(ClippingRange);

  if(((uint32)Width & (uint32)0xF)==0) //Width%16==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=16)
      {
        __m256i Src0BlockV  = _mm256_loadu_si256((__m256i*)&Src0[x]);
        __m256i Src1BlockV  = _mm256_loadu_si256((__m256i*)&Src1[x]);
        __m256i AddV        = _mm256_adds_epi16(_mm256_adds_epi16(Src0BlockV, Src1BlockV), OffsetV);
        __m256i ShiftV      = _mm256_srai_epi16(AddV, 1);
        __m256i ClipV       = _mm256_max_epi16(_mm256_min_epi16(ShiftV, MaxValV),ZeroV);
        _mm256_storeu_si256((__m256i*)&Dst[x], ClipV);
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;    
    }
  }
  else
  {
    int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width16; x+=8)
      {
        __m256i Src0BlockV  = _mm256_loadu_si256((__m256i*)&Src0[x]);
        __m256i Src1BlockV  = _mm256_loadu_si256((__m256i*)&Src1[x]);
        __m256i AddV        = _mm256_adds_epi16(_mm256_adds_epi16(Src0BlockV, Src1BlockV), OffsetV);
        __m256i ShiftV      = _mm256_srai_epi16(AddV, 1);
        __m256i ClipV       = _mm256_max_epi16(_mm256_min_epi16(ShiftV, MaxValV),ZeroV);
        _mm256_storeu_si256((__m256i*)&Dst[x], ClipV);
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i Src0BlockV  = _mm_loadu_si128((__m128i*)&Src0[x]);
        __m128i Src1BlockV  = _mm_loadu_si128((__m128i*)&Src1[x]);
        __m128i AddV        = _mm_adds_epi16(_mm_adds_epi16(Src0BlockV, Src1BlockV), _mm256_castsi256_si128(OffsetV));
        __m128i ShiftV      = _mm_srai_epi16(AddV, 1);
        __m128i ClipV       = _mm_max_epi16(_mm_min_epi16(ShiftV, _mm256_castsi256_si128(MaxValV)), _mm256_castsi256_si128(ZeroV));
        _mm_storeu_si128((__m128i*)&Dst[x], ClipV);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i Src0BlockV  = _mm_loadl_epi64((__m128i*)&Src0[x]);
        __m128i Src1BlockV  = _mm_loadl_epi64((__m128i*)&Src1[x]);
        __m128i AddV        = _mm_adds_epi16(_mm_adds_epi16(Src0BlockV, Src1BlockV), _mm256_castsi256_si128(OffsetV));
        __m128i ShiftV      = _mm_srai_epi16(AddV, 1);
        __m128i ClipV       = _mm_max_epi16(_mm_min_epi16(ShiftV, _mm256_castsi256_si128(MaxValV)), _mm256_castsi256_si128(ZeroV));
        _mm_storel_epi64((__m128i*)&Dst[x], ClipV);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = xClipU<int16>(((Src0[x] + Src1[x] + 1)>>1), ClippingRange);
      }
      Src0 += Src0Stride;
      Src1 += Src1Stride;
      Dst  += DstStride;    
    }
  }
}
void xPixelOpsArithm::xClip_AVX(int16* restrict Dst, int16* restrict Src, int32 Area, int16 ClippingRangeLow, int16 ClippingRangeHigh)
{
  const __m256i RangeLowV  = _mm256_set1_epi16(ClippingRangeLow );
  const __m256i RangeHighV = _mm256_set1_epi16(ClippingRangeHigh);
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);

  for(int32 i=0; i<Area16; i+=16)
  {
    __m256i SrcBlockV  = _mm256_loadu_si256((__m256i*)&Src[i]);
    __m256i ClipV      = _mm256_max_epi16(_mm256_min_epi16(SrcBlockV, RangeHighV), RangeLowV);
    _mm256_storeu_si256((__m256i*)&Dst[i], ClipV);
  }
  for(int32 i=Area16; i<Area; i++)
  { 
    Dst[i] = xClip<int16>(Src[i], ClippingRangeLow, ClippingRangeHigh);
  }
}
void xPixelOpsArithm::xClip_AVX(int16* restrict Dst, int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int16 ClippingRangeLow, int16 ClippingRangeHigh)
{
  const __m256i RangeLowV  = _mm256_set1_epi16(ClippingRangeLow );
  const __m256i RangeHighV = _mm256_set1_epi16(ClippingRangeHigh);

  if(((uint32)Width & (uint32)0xF)==0) //Width%16==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=16)
      {
        __m256i SrcBlockV  = _mm256_loadu_si256((__m256i*)&Src[x]);
        __m256i ClipV      = _mm256_max_epi16(_mm256_min_epi16(SrcBlockV, RangeHighV), RangeLowV);
        _mm256_storeu_si256((__m256i*)&Dst[x], ClipV);
      }
      Src += SrcStride;
      Dst += DstStride;    
    }
  }
  else
  {
    int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width16; x+=16)
      {
        __m256i SrcBlockV  = _mm256_loadu_si256((__m256i*)&Src[x]);
        __m256i ClipV      = _mm256_max_epi16(_mm256_min_epi16(SrcBlockV, RangeHighV), RangeLowV);
        _mm256_storeu_si256((__m256i*)&Dst[x], ClipV);
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i SrcBlockV  = _mm_loadu_si128((__m128i*)&Src[x]);
        __m128i ClipV      = _mm_max_epi16(_mm_min_epi16(SrcBlockV, _mm256_castsi256_si128(RangeHighV)), _mm256_castsi256_si128(RangeLowV));
        _mm_storeu_si128((__m128i*)&Dst[x], ClipV);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i SrcBlockV  = _mm_loadl_epi64((__m128i*)&Src[x]);
        __m128i ClipV      = _mm_max_epi16(_mm_min_epi16(SrcBlockV, _mm256_castsi256_si128(RangeHighV)), _mm256_castsi256_si128(RangeLowV));
        _mm_storel_epi64((__m128i*)&Dst[x], ClipV);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = xClip<int16>(Src[x], ClippingRangeLow, ClippingRangeHigh);
      }
      Src += SrcStride;
      Dst += DstStride;    
    }
  }
}
void xPixelOpsArithm::xThreshold_AVX(int16* restrict Dst, int16* restrict Src, int32 Area, const int16 Threshold)
{  
  const __m256i ThresholdV = _mm256_set1_epi16(Threshold);
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);

  for(int32 i=0; i<Area16; i+=16)
  {
    __m256i SrcV = _mm256_loadu_si256((__m256i*)&Src[i]);
    __m256i CmpV = _mm256_cmpgt_epi16(SrcV, ThresholdV); //0 - <=, 0xFFFF - >
    __m256i DstV = _mm256_srli_epi16(CmpV, 15);       
    _mm256_storeu_si256((__m256i*)&Dst[i], DstV);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    Dst[i] = xThreshold(Src[i], Threshold); 
  }
}
void xPixelOpsArithm::xThreshold_AVX(int16* restrict Dst, int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Threshold)
{
  const __m256i ThresholdV = _mm256_set1_epi16(Threshold);

  if(((uint32)Width & (uint32)0xF)==0) //Width%16==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=16)
      {
        __m256i SrcV = _mm256_loadu_si256((__m256i*)&Src[x]);
        __m256i CmpV = _mm256_cmpgt_epi16(SrcV, ThresholdV); //0 - <=, 0xFFFF - >
        __m256i DstV = _mm256_srli_epi16(CmpV, 15);       
        _mm256_storeu_si256((__m256i*)&Dst[x], DstV);
      }
      Src += SrcStride;
      Dst += DstStride;    
    }
  }
  else
  {
    int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width16; x+=16)
      {
        __m256i SrcV = _mm256_loadu_si256((__m256i*)&Src[x]);
        __m256i CmpV = _mm256_cmpgt_epi16(SrcV, ThresholdV); //0 - <=, 0xFFFF - >
        __m256i DstV = _mm256_srli_epi16(CmpV, 15);       
        _mm256_storeu_si256((__m256i*)&Dst[x], DstV);
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x]);
        __m128i CmpV = _mm_cmpgt_epi16(SrcV, _mm256_castsi256_si128(ThresholdV)); //0 - <=, 0xFFFF - >
        __m128i DstV = _mm_srli_epi16(CmpV, 15);       
        _mm_storeu_si128((__m128i*)&Dst[x], DstV);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i SrcV = _mm_loadl_epi64((__m128i*)&Src[x]);
        __m128i CmpV = _mm_cmpgt_epi16(SrcV, _mm256_castsi256_si128(ThresholdV)); //0 - <=, 0xFFFF - >
        __m128i DstV = _mm_srli_epi16(CmpV, 15);       
        _mm_storel_epi64((__m128i*)&Dst[x], DstV);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        Dst[x] = xThreshold(Src[x], Threshold); 
      }
      Src += SrcStride;
      Dst += DstStride;    
    }
  }
}
uint32 xPixelOpsArithm::xCountNonZero_AVX(int16* restrict Src, int32 Area)
{
  uint32  NumNonZero = 0;
  __m256i ZeroV = _mm256_setzero_si256();
  int32   Area32 = (int32)((uint32)Area & (uint32)c_MultipleMask32);

  for(int32 i=0; i<Area32; i+=32)
  {
    __m256i CoeffsA = _mm256_loadu_si256((__m256i*)&Src[i  ]);
    __m256i CoeffsB = _mm256_loadu_si256((__m256i*)&Src[i+8]);
    __m256i MasksA  = _mm256_cmpeq_epi16(CoeffsA, ZeroV);
    __m256i MasksB  = _mm256_cmpeq_epi16(CoeffsB, ZeroV);
    __m256i Masks   = _mm256_packs_epi16(MasksA, MasksB);
    uint32 Mask     = (~_mm256_movemask_epi8(Masks)) & 0xFFFFFFFF;
    uint32 NumOnes  = _mm_popcnt_u32(Mask);
    NumNonZero += NumOnes;
  }
  for(int32 i=Area32; i<Area; i++)
  {
    if(Src[i]!=0) { NumNonZero++; }
  }
  return NumNonZero;
}
uint32 xPixelOpsArithm::xCountNonZero_AVX(int16* restrict Src, int32 SrcStride, int32 Width, int32 Height)
{
  uint32  NumNonZero = 0;
  __m256i ZeroV = _mm256_setzero_si256();

  if(((uint32)Width & (uint32)c_RemainderMask32)==0) //Width%32==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=32)
      {
        __m256i CoeffsA = _mm256_loadu_si256((__m256i*)&Src[x  ]);
        __m256i CoeffsB = _mm256_loadu_si256((__m256i*)&Src[x+8]);
        __m256i MasksA  = _mm256_cmpeq_epi16(CoeffsA, ZeroV);
        __m256i MasksB  = _mm256_cmpeq_epi16(CoeffsB, ZeroV);
        __m256i Masks   = _mm256_packs_epi16(MasksA, MasksB);
        uint32 Mask     = (~_mm256_movemask_epi8(Masks)) & 0xFFFFFFFF;
        uint32 NumOnes  = _mm_popcnt_u32(Mask);
        NumNonZero += NumOnes;
      }
      Src += SrcStride;
    }
  }
  else
  {
    int32 Width32 = (int32)((uint32)Width & (uint32)c_MultipleMask32);
    int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);

    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width32; x+=32)
      {
        __m256i CoeffsA = _mm256_loadu_si256((__m256i*)&Src[x  ]);
        __m256i CoeffsB = _mm256_loadu_si256((__m256i*)&Src[x+8]);
        __m256i MasksA  = _mm256_cmpeq_epi16(CoeffsA, ZeroV);
        __m256i MasksB  = _mm256_cmpeq_epi16(CoeffsB, ZeroV);
        __m256i Masks   = _mm256_packs_epi16(MasksA, MasksB);
        uint32 Mask     = (~_mm256_movemask_epi8(Masks)) & 0xFFFFFFFF;
        uint32 NumOnes  = _mm_popcnt_u32(Mask);
        NumNonZero += NumOnes;
      }
      for(int32 x=Width32; x<Width16; x+=16)
      {
        __m256i Coeffs  = _mm256_loadu_si256((__m256i*)&Src[x  ]);
        __m256i MasksA  = _mm256_cmpeq_epi16(Coeffs, ZeroV);
        __m256i Masks   = _mm256_packs_epi16(MasksA, ZeroV);
        uint32 Mask     = (~_mm256_movemask_epi8(Masks)) & 0xFFFFFFFF;
        uint32 NumOnes  = _mm_popcnt_u32(Mask);
        NumNonZero += NumOnes;
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i Coeffs  = _mm_loadu_si128((__m128i*)&Src[x  ]);
        __m128i MasksA  = _mm_cmpeq_epi16(Coeffs, _mm256_castsi256_si128(ZeroV));
        __m128i Masks   = _mm_packs_epi16(MasksA, _mm256_castsi256_si128(ZeroV));
        uint32 Mask     = (~_mm_movemask_epi8(Masks)) & 0xFFFF;
        uint32 NumOnes  = _mm_popcnt_u32(Mask);
        NumNonZero += NumOnes;      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        __m128i Coeffs  = _mm_loadl_epi64((__m128i*)&Src[x  ]);
        __m128i MasksA  = _mm_cmpeq_epi16(Coeffs, _mm256_castsi256_si128(ZeroV));
        __m128i Masks   = _mm_packs_epi16(MasksA, _mm256_castsi256_si128(ZeroV));
        uint32 Mask     = (~_mm_movemask_epi8(Masks)) & 0xFFFF;
        uint32 NumOnes  = _mm_popcnt_u32(Mask);
        NumNonZero += NumOnes;
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        if(Src[x]!=0) { NumNonZero++; }
      }
      Src += SrcStride;
    }
  }

  return NumNonZero;
}
int32 xPixelOpsArithm::xSumAll_AVX(int16* restrict Src, int32 Area)
{
  int32   Sum    = 0;
  __m256i SumV   = _mm256_setzero_si256();
  int32   Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);

  for(int32 i=0; i<Area16; i+=8)
  {
    __m256i SrcV  = _mm256_loadu_si256((__m256i*)&Src[i]);
    __m256i SrcV0 = _mm256_cvtepi16_epi32(_mm256_castsi256_si128(SrcV));
    __m256i SrcV1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(SrcV, 1));
    __m256i TmpV  = _mm256_add_epi32(SrcV0, SrcV1);
    SumV = _mm256_add_epi32(SumV, TmpV);
  }
  for(int32 i=Area16; i<Area; i++)
  { 
    Sum += Src[i];
  }
  __m128i SumV0 = _mm256_extractf128_si256(SumV, 1);
  __m128i SumV1 = _mm256_castsi256_si128(SumV);
  __m128i SumVA = _mm_add_epi32(SumV0, SumV1);
  SumVA = _mm_hadd_epi32(SumVA, SumVA);
  SumVA = _mm_hadd_epi32(SumVA, SumVA); 
  Sum  += _mm_extract_epi32(SumVA, 0);
  return Sum;
}
int32 xPixelOpsArithm::xSumAll_AVX(int16* restrict Src, int32 SrcStride, int32 Width, int32 Height)
{
  xAssert(0); return NOT_VALID;
}
#endif //X_AVX1 && X_AVX2

//===============================================================================================================================================================================================================

} //end of namespace AVLib

  //void xPixelOpsArithm::xShift_STD(int16* restrict Dst, int16* restrict Src, int32 Area, int32 Shift)
  //{
  //  if(Shift==0) //no operation
  //  {
  //    if(Dst!=Src) ::memcpy(Dst, Src, Area*sizeof(int16));
  //  }
  //  else if(Shift>0) //bit depth increase
  //  {
  //    const int32 DeltaBits = Shift;
  //    for(int32 i=0; i<Area; i++)
  //    {
  //      Dst[i] = Src[i] << DeltaBits;
  //    }
  //  }
  //  else //Shift<0  //bit depth decrease
  //  {
  //    const int32 DeltaBits = -Shift;
  //    uint32 Offset = 1<<(DeltaBits-1);
  //    for(int32 i=0; i<Area; i++)
  //    {
  //      Dst[i] = (Src[i] + Offset) >> DeltaBits;
  //    }
  //  }
  //}
  //void xPixelOpsArithm::xShift_SSE(int16* restrict Dst, int16* restrict Src, int32 Area, int32 Shift)
  //{
  //  if(Shift==0) //no operation
  //  {
  //    if(Dst!=Src) ::memcpy(Dst, Src, Area*sizeof(int16));
  //  }
  //  else if(Shift>0) //bit depth increase
  //  {
  //    const int32 DeltaBits = Shift;
  //    int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);
  //    for(int32 i=0; i<Area8; i+=8)
  //    {
  //      __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[i]);
  //      __m128i DstV = _mm_slli_epi16(SrcV, DeltaBits);
  //      _mm_storeu_si128((__m128i*)&Dst[i], DstV);
  //    }
  //    for(int32 i=Area8; i<Area; i++)
  //    {
  //      Dst[i] = Src[i] << DeltaBits;
  //    }
  //  }
  //  else //Shift<0  //bit depth decrease
  //  {
  //    const int32 DeltaBits = -Shift;
  //    uint32 Offset   = 1<<(DeltaBits-1);
  //    __m128i OffsetV = _mm_set1_epi16(Offset);
  //    int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);
  //    for(int32 i=0; i<Area8; i+=8)
  //    {
  //      __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[i]);
  //      __m128i AddV = _mm_add_epi16(SrcV, OffsetV);
  //      __m128i DstV = _mm_srli_epi16(AddV, DeltaBits);
  //      _mm_storeu_si128((__m128i*)&Dst[i], DstV);
  //    }
  //    for(int32 i=Area8; i<Area; i++)
  //    {
  //      Dst[i] = (Src[i] + Offset) >> DeltaBits;
  //    }
  //  }
  //}
  //void xPixelOpsArithm::xShift_AVX(int16* restrict Dst, int16* restrict Src, int32 Area, int32 Shift)
  //{
  //  if(Shift==0) //no operation
  //  {
  //    if(Dst!=Src) ::memcpy(Dst, Src, Area*sizeof(int16));
  //  }
  //  else if(Shift>0) //bit depth increase
  //  {
  //    const int32 DeltaBits = Shift;
  //    int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  //    for(int32 i=0; i<Area16; i+=16)
  //    {
  //      __m256i SrcV = _mm256_loadu_si256((__m256i*)&Src[i]);
  //      __m256i DstV = _mm256_slli_epi16(SrcV, DeltaBits);
  //      _mm256_storeu_si256((__m256i*)&Dst[i], DstV);
  //    }
  //    for(int32 i=Area16; i<Area; i++)
  //    {
  //      Dst[i] = Src[i] << DeltaBits;
  //    }
  //  }
  //  else //Shift<0  //bit depth decrease
  //  {
  //    const int32 DeltaBits = -Shift;
  //    uint32 Offset   = 1<<(DeltaBits-1);
  //    __m256i OffsetV = _mm256_set1_epi16(Offset);
  //    int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  //    for(int32 i=0; i<Area16; i+=16)
  //    {
  //      __m256i SrcV = _mm256_loadu_si256((__m256i*)&Src[i]);
  //      __m256i AddV = _mm256_add_epi16(SrcV, OffsetV);
  //      __m256i DstV = _mm256_srli_epi16(AddV, DeltaBits);
  //      _mm256_storeu_si256((__m256i*)&Dst[i], DstV);
  //    }
  //    for(int32 i=Area16; i<Area; i++)
  //    {
  //      Dst[i] = (Src[i] + Offset) >> DeltaBits;
  //    }
  //  }
  //}
  //void xPixelOpsArithm::xShift_STD(int16* restrict Dst, int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift)
  //{
  //  if(Shift==0) //no operation
  //  {
  //    if(Dst!=Src)
  //    {
  //      for(int32 y=0; y<Height; y++)
  //      {
  //        ::memcpy(Dst, Src, Width*sizeof(int16));
  //        Src += SrcStride;
  //        Dst += DstStride;  
  //      }
  //    }
  //  }
  //  else if(Shift>0) //bit depth increase
  //  {
  //    const int32 DeltaBits = Shift;
  //    for(int32 y=0; y<Height; y++)
  //    {
  //      for(int32 x=0; x<Width; x++)
  //      {      
  //        Dst[x] = Src[x] << DeltaBits;
  //      }
  //      Src += SrcStride;
  //      Dst += DstStride;  
  //    }
  //  }
  //  else //Shift<0  //bit depth decrease
  //  {
  //    const int32 DeltaBits = -Shift;
  //    uint32 Offset = 1<<(DeltaBits-1);
  //    for(int32 y=0; y<Height; y++)
  //    {
  //      for(int32 x=0; x<Width; x++)
  //      {      
  //        Dst[x] = (Src[x] + Offset) >> DeltaBits;
  //      }
  //      Src += SrcStride;
  //      Dst += DstStride;  
  //    }
  //  }
  //}
  //void xPixelOpsArithm::xShift_SSE(int16* restrict Dst, int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift)
  //{
  //  if(Shift==0) //no operation
  //  {
  //    if(Dst!=Src)
  //    {
  //      for(int32 y=0; y<Height; y++)
  //      {
  //        ::memcpy(Dst, Src, Width*sizeof(int16));
  //        Src += SrcStride;
  //        Dst += DstStride;  
  //      }
  //    }
  //  }
  //  else if(Shift>0) //bit depth increase
  //  {
  //    const int32 DeltaBits = Shift;
  //    if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
  //    {
  //      for(int32 y=0; y<Height; y++)
  //      {
  //        for(int32 x=0; x<Width; x+=8)
  //        {
  //          __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x]);
  //          __m128i DstV = _mm_slli_epi16(SrcV, DeltaBits);
  //          _mm_storeu_si128((__m128i*)&Dst[x], DstV);
  //        }
  //        Src += SrcStride;
  //        Dst += DstStride;  
  //      }
  //    }
  //    else
  //    {
  //      int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
  //      int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);
  //
  //      for(int32 y=0; y<Height; y++)
  //      {
  //        for(int32 x=0; x<Width8; x+=8)
  //        {
  //          __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x]);
  //          __m128i DstV = _mm_slli_epi16(SrcV, DeltaBits);
  //          _mm_storeu_si128((__m128i*)&Dst[x], DstV);
  //        }
  //        for(int32 x=Width8; x<Width4; x+=4)
  //        {
  //          __m128i SrcV  = _mm_loadl_epi64((__m128i*)&Src[x]);
  //          __m128i DstV = _mm_slli_epi16(SrcV, DeltaBits);
  //          _mm_storel_epi64((__m128i*)&Dst[x], DstV);
  //        }
  //        for(int32 x=Width4; x<Width; x++)
  //        {      
  //          Dst[x] = Src[x] << DeltaBits;
  //        }
  //        Src += SrcStride;
  //        Dst += DstStride;  
  //      }
  //    }
  //  }
  //  else //Shift<0  //bit depth decrease
  //  {
  //    const int32 DeltaBits = -Shift;
  //    uint32 Offset   = 1<<(DeltaBits-1);
  //    __m128i OffsetV = _mm_set1_epi16(Offset);
  //    if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
  //    {
  //      for(int32 y=0; y<Height; y++)
  //      {
  //        for(int32 x=0; x<Width; x+=8)
  //        {
  //          __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x]);
  //          __m128i AddV = _mm_add_epi16(SrcV, OffsetV);
  //          __m128i DstV = _mm_srli_epi16(AddV, DeltaBits);
  //          _mm_storeu_si128((__m128i*)&Dst[x], DstV);
  //        }
  //        Src += SrcStride;
  //        Dst += DstStride;  
  //      }
  //    }
  //    else
  //    {
  //      int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
  //      int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);
  //
  //      for(int32 y=0; y<Height; y++)
  //      {
  //        for(int32 x=0; x<Width8; x+=8)
  //        {
  //          __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x]);
  //          __m128i AddV = _mm_add_epi16(SrcV, OffsetV);
  //          __m128i DstV = _mm_srli_epi16(AddV, DeltaBits);
  //          _mm_storeu_si128((__m128i*)&Dst[x], DstV);
  //        }
  //        for(int32 x=Width8; x<Width4; x+=4)
  //        {
  //          __m128i SrcV = _mm_loadl_epi64((__m128i*)&Src[x]);
  //          __m128i AddV = _mm_add_epi16(SrcV, OffsetV);
  //          __m128i DstV = _mm_srli_epi16(AddV, DeltaBits);
  //          _mm_storel_epi64((__m128i*)&Dst[x], DstV);
  //        }
  //        for(int32 x=Width4; x<Width; x++)
  //        {      
  //          Dst[x] = (Src[x] + Offset) >> DeltaBits;
  //        }
  //        Src += SrcStride;
  //        Dst += DstStride;  
  //      }
  //    }
  //  }
  //}
  //void xPixelOpsArithm::xShift_AVX(int16* restrict Dst, int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift)
  //{
  //  if(Shift==0) //no operation
  //  {
  //    if(Dst!=Src)
  //    {
  //      for(int32 y=0; y<Height; y++)
  //      {
  //        ::memcpy(Dst, Src, Width*sizeof(int16));
  //        Src += SrcStride;
  //        Dst += DstStride;  
  //      }
  //    }
  //  }
  //  else if(Shift>0) //bit depth increase
  //  {
  //    const int32 DeltaBits = Shift;
  //    if(((uint32)Width & (uint32)0xF)==0) //Width%16==0
  //    {
  //      for(int32 y=0; y<Height; y++)
  //      {
  //        for(int32 x=0; x<Width; x+=16)
  //        {
  //          __m256i SrcV = _mm256_loadu_si256((__m256i*)&Src[x]);
  //          __m256i DstV = _mm256_slli_epi16(SrcV, DeltaBits);
  //          _mm256_storeu_si256((__m256i*)&Dst[x], DstV);
  //        }
  //        Src += SrcStride;
  //        Dst += DstStride;  
  //      }
  //    }
  //    else
  //    {
  //      int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
  //      int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
  //      int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);
  //
  //      for(int32 y=0; y<Height; y++)
  //      {
  //        for(int32 x=0; x<Width16; x+=16)
  //        {
  //          __m256i SrcV = _mm256_loadu_si256((__m256i*)&Src[x]);
  //          __m256i DstV = _mm256_slli_epi16(SrcV, DeltaBits);
  //          _mm256_storeu_si256((__m256i*)&Dst[x], DstV);
  //        }
  //        for(int32 x=Width16; x<Width8; x+=8)
  //        {
  //          __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x]);
  //          __m128i DstV = _mm_slli_epi16(SrcV, DeltaBits);
  //          _mm_storeu_si128((__m128i*)&Dst[x], DstV);
  //        }
  //        for(int32 x=Width8; x<Width4; x+=4)
  //        {
  //          __m128i SrcV  = _mm_loadl_epi64((__m128i*)&Src[x]);
  //          __m128i DstV = _mm_slli_epi16(SrcV, DeltaBits);
  //          _mm_storel_epi64((__m128i*)&Dst[x], DstV);
  //        }
  //        for(int32 x=Width4; x<Width; x++)
  //        {      
  //          Dst[x] = Src[x] << DeltaBits;
  //        }
  //        Src += SrcStride;
  //        Dst += DstStride;  
  //      }
  //    }
  //  }
  //  else //Shift<0  //bit depth decrease
  //  {
  //    const int32 DeltaBits = -Shift;
  //    uint32 Offset   = 1<<(DeltaBits-1);
  //    __m256i OffsetV = _mm256_set1_epi16(Offset);
  //    if(((uint32)Width & (uint32)0xF)==0) //Width%16==0
  //    {
  //      for(int32 y=0; y<Height; y++)
  //      {
  //        for(int32 x=0; x<Width; x+=8)
  //        {
  //          __m256i SrcV = _mm256_loadu_si256((__m256i*)&Src[x]);
  //          __m256i AddV = _mm256_add_epi16(SrcV, OffsetV);
  //          __m256i DstV = _mm256_srli_epi16(AddV, DeltaBits);
  //          _mm256_storeu_si256((__m256i*)&Dst[x], DstV);
  //        }
  //        Src += SrcStride;
  //        Dst += DstStride;  
  //      }
  //    }
  //    else
  //    {
  //      int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
  //      int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
  //      int32 Width4  = (int32)((uint32)Width & (uint32)c_MultipleMask4);
  //
  //      for(int32 y=0; y<Height; y++)
  //      {
  //        for(int32 x=0; x<Width16; x+=16)
  //        {
  //          __m256i SrcV = _mm256_loadu_si256((__m256i*)&Src[x]);
  //          __m256i AddV = _mm256_add_epi16(SrcV, OffsetV);
  //          __m256i DstV = _mm256_srli_epi16(AddV, DeltaBits);
  //          _mm256_storeu_si256((__m256i*)&Dst[x], DstV);
  //        }
  //        for(int32 x=Width16; x<Width8; x+=8)
  //        {
  //          __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x]);
  //          __m128i AddV = _mm_add_epi16(SrcV, _mm256_castsi256_si128(OffsetV));
  //          __m128i DstV = _mm_srli_epi16(AddV, DeltaBits);
  //          _mm_storeu_si128((__m128i*)&Dst[x], DstV);
  //        }
  //        for(int32 x=Width8; x<Width4; x+=4)
  //        {
  //          __m128i SrcV = _mm_loadl_epi64((__m128i*)&Src[x]);
  //          __m128i AddV = _mm_add_epi16(SrcV, _mm256_castsi256_si128(OffsetV));
  //          __m128i DstV = _mm_srli_epi16(AddV, DeltaBits);
  //          _mm_storel_epi64((__m128i*)&Dst[x], DstV);
  //        }
  //        for(int32 x=Width4; x<Width; x++)
  //        {      
  //          Dst[x] = (Src[x] + Offset) >> DeltaBits;
  //        }
  //        Src += SrcStride;
  //        Dst += DstStride;  
  //      }
  //    }
  //  }
  //}
