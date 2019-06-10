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

#include "xResample.h"

namespace AVlib {

//===============================================================================================================================================================================================================

#if X_SSE4_2
void xResampleSSE::ResampleU2HVNearest(int16* restrict Dst, const int16* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight)
{
  int16 *restrict DstL0 = Dst;
  int16 *restrict DstL1 = Dst + DstStride;
  int32 DstStrideMul2   = DstStride<<1;

  if(((uint32)DstWidth & (uint32)0xF)==0) //Width%16==0
  {
    for(int32 y=0; y<DstHeight; y+=2)
    {
      for(int32 x=0; x<DstWidth; x+=16)
      {
        __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x>>1]);
        __m128i LeftV  = _mm_unpacklo_epi16(SrcV, SrcV);
        __m128i RightV = _mm_unpackhi_epi16(SrcV, SrcV);
        _mm_storeu_si128((__m128i*)&DstL0[x  ], LeftV );
        _mm_storeu_si128((__m128i*)&DstL0[x+8], RightV);
        _mm_storeu_si128((__m128i*)&DstL1[x  ], LeftV );
        _mm_storeu_si128((__m128i*)&DstL1[x+8], RightV);
      }
      Src   += SrcStride;
      DstL0 += DstStrideMul2;
      DstL1 += DstStrideMul2;
    }
  }
  else
  {
    int32 Width16 = (int32)((uint32)DstWidth & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)DstWidth & (uint32)c_MultipleMask8);

    for(int32 y=0; y<DstHeight; y+=2)
    {
      for(int32 x=0; x<Width16; x+=16)
      {
        __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x>>1]);
        __m128i LeftV  = _mm_unpacklo_epi16(SrcV, SrcV);
        __m128i RightV = _mm_unpackhi_epi16(SrcV, SrcV);
        _mm_storeu_si128((__m128i*)&DstL0[x  ], LeftV );
        _mm_storeu_si128((__m128i*)&DstL0[x+8], RightV);
        _mm_storeu_si128((__m128i*)&DstL1[x  ], LeftV );
        _mm_storeu_si128((__m128i*)&DstL1[x+8], RightV);
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i SrcV = _mm_loadl_epi64((__m128i*)&Src[x>>1]);
        __m128i LeftV  = _mm_unpacklo_epi16(SrcV, SrcV);
        _mm_storeu_si128((__m128i*)&DstL0[x  ], LeftV );
        _mm_storeu_si128((__m128i*)&DstL1[x  ], LeftV );
      }
      for(int32 x=Width8; x<DstWidth; x++)
      {
        int16 S = Src[x>>1];
        DstL0[x  ] = S;
        DstL0[x+1] = S;
        DstL1[x  ] = S;
        DstL1[x+1] = S;
      }
      Src   += SrcStride;
      DstL0 += DstStrideMul2;
      DstL1 += DstStrideMul2;
    }
  }
}
void xResampleSSE::ResampleD2HVAverage(int16* restrict Dst, const int16* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight)
{
  __m128i TwoV = _mm_set1_epi16((int16)2);

  const int16 *restrict SrcL0 = Src;
  const int16 *restrict SrcL1 = Src + SrcStride;
  int32 SrcStrideMul2   = SrcStride<<1;

  if(((uint32)DstWidth & (uint32)0x7)==0) //Width%8==0
  {
    for(int32 y=0; y<DstHeight; y++)
    {
      for(int32 x=0; x<DstWidth; x+=8)
      {
        int32 SrcX = x<<1;
        __m128i TopLeftV     = _mm_loadu_si128((__m128i*)&SrcL0[SrcX  ]);
        __m128i TopRightV    = _mm_loadu_si128((__m128i*)&SrcL0[SrcX+8]);        
        __m128i BottomLeftV  = _mm_loadu_si128((__m128i*)&SrcL1[SrcX  ]);
        __m128i BottomRightV = _mm_loadu_si128((__m128i*)&SrcL1[SrcX+8]);
        __m128i TopV         = _mm_hadd_epi16(TopLeftV, TopRightV);
        __m128i BottomV      = _mm_hadd_epi16(BottomLeftV, BottomRightV);
        __m128i OutV         = _mm_srai_epi16(_mm_add_epi16(_mm_add_epi16(TopV, BottomV), TwoV), 2);
        _mm_storeu_si128 ((__m128i*)&Dst[x], OutV);
      }
      SrcL0 += SrcStrideMul2;
      SrcL1 += SrcStrideMul2;
      Dst += DstStride;
    }
  }  
  else
  {
    int32 Width8  = (int32)((uint32)DstWidth & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)DstWidth & (uint32)c_MultipleMask4);

    for(int32 y=0; y<DstHeight; y++)
    {
      for(int32 x=0; x<Width8; x+=8)
      {
        int32 SrcX = x<<1;
        __m128i TopLeftV     = _mm_loadu_si128((__m128i*)&SrcL0[SrcX  ]);
        __m128i TopRightV    = _mm_loadu_si128((__m128i*)&SrcL0[SrcX+8]);        
        __m128i BottomLeftV  = _mm_loadu_si128((__m128i*)&SrcL1[SrcX  ]);
        __m128i BottomRightV = _mm_loadu_si128((__m128i*)&SrcL1[SrcX+8]);
        __m128i TopV         = _mm_hadd_epi16(TopLeftV, TopRightV);
        __m128i BottomV      = _mm_hadd_epi16(BottomLeftV, BottomRightV);
        __m128i OutV         = _mm_srai_epi16(_mm_add_epi16(_mm_add_epi16(TopV, BottomV), TwoV), 2);
        _mm_storeu_si128 ((__m128i*)&Dst[x], OutV);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        int32 SrcX = x<<1;
        __m128i TopLeftV     = _mm_loadu_si128((__m128i*)&SrcL0[SrcX  ]);
        __m128i BottomLeftV  = _mm_loadu_si128((__m128i*)&SrcL1[SrcX  ]);
        __m128i TopV         = _mm_hadd_epi16(TopLeftV, TopLeftV);
        __m128i BottomV      = _mm_hadd_epi16(BottomLeftV, BottomLeftV);
        __m128i OutV         = _mm_srai_epi16(_mm_add_epi16(_mm_add_epi16(TopV, BottomV), TwoV), 2);
        _mm_storel_epi64 ((__m128i*)&Dst[x], OutV);      
      }
      for(int32 x=Width4; x<DstWidth; x++)
      {      
        int32 SrcX = x<<1;
        int16 D = ((int32)SrcL0[SrcX  ] + (int32)SrcL0[SrcX+1] + (int32)SrcL1[SrcX  ] + (int32)SrcL1[SrcX+1] + 2)>>2;
        Dst[x] = D;
      }
      SrcL0 += SrcStrideMul2;
      SrcL1 += SrcStrideMul2;
      Dst += DstStride;
    }
  }
}
#endif //X_SSE4_2

//===============================================================================================================================================================================================================

#if X_AVX1 && X_AVX2
void xResampleAVX::ResampleU2HVNearest(int16* restrict Dst, const int16* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight)
{
  int16 *restrict DstL0 = Dst;
  int16 *restrict DstL1 = Dst + DstStride;
  int32 DstStrideMul2   = DstStride<<1;

  if(((uint32)DstWidth & (uint32)0x01F)==0) //Width%32==0
  {
    for(int32 y=0; y<DstHeight; y+=2)
    {
      for(int32 x=0; x<DstWidth; x+=32)
      {
        __m256i SrcV = _mm256_loadu_si256((__m256i*)&Src[x>>1]);
        SrcV = _mm256_permute4x64_epi64(SrcV, 0xD8);
        __m256i LeftV  = _mm256_unpacklo_epi16(SrcV, SrcV);
        __m256i RightV = _mm256_unpackhi_epi16(SrcV, SrcV);        
        _mm256_storeu_si256((__m256i*)&DstL0[x   ], LeftV );
        _mm256_storeu_si256((__m256i*)&DstL0[x+16], RightV);
        _mm256_storeu_si256((__m256i*)&DstL1[x   ], LeftV );
        _mm256_storeu_si256((__m256i*)&DstL1[x+16], RightV);
      }
      Src   += SrcStride;
      DstL0 += DstStrideMul2;
      DstL1 += DstStrideMul2;
    }
  }
  else
  {
    int32 Width32 = (int32)((uint32)DstWidth & (uint32)c_MultipleMask32);
    int32 Width16 = (int32)((uint32)DstWidth & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)DstWidth & (uint32)c_MultipleMask8);

    for(int32 y=0; y<DstHeight; y+=2)
    {
      for(int32 x=0; x<Width32; x+=32)
      {
        __m256i SrcV = _mm256_loadu_si256((__m256i*)&Src[x>>1]);
        SrcV = _mm256_permute4x64_epi64(SrcV, 0xD8);
        __m256i LeftV  = _mm256_unpacklo_epi16(SrcV, SrcV);
        __m256i RightV = _mm256_unpackhi_epi16(SrcV, SrcV);
        _mm256_storeu_si256((__m256i*)&DstL0[x   ], LeftV );
        _mm256_storeu_si256((__m256i*)&DstL0[x+16], RightV);
        _mm256_storeu_si256((__m256i*)&DstL1[x   ], LeftV );
        _mm256_storeu_si256((__m256i*)&DstL1[x+16], RightV);
      }
      for(int32 x=Width32; x<Width16; x+=16)
      {
        __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x>>1]);
        __m128i LeftV  = _mm_unpacklo_epi16(SrcV, SrcV);
        __m128i RightV = _mm_unpackhi_epi16(SrcV, SrcV);
        _mm_storeu_si128((__m128i*)&DstL0[x  ], LeftV );
        _mm_storeu_si128((__m128i*)&DstL0[x+8], RightV);
        _mm_storeu_si128((__m128i*)&DstL1[x  ], LeftV );
        _mm_storeu_si128((__m128i*)&DstL1[x+8], RightV);
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i SrcV = _mm_loadl_epi64((__m128i*)&Src[x>>1]);
        __m128i LeftV  = _mm_unpacklo_epi16(SrcV, SrcV);
        _mm_storeu_si128((__m128i*)&DstL0[x  ], LeftV );
        _mm_storeu_si128((__m128i*)&DstL1[x  ], LeftV );
      }
      for(int32 x=Width8; x<DstWidth; x++)
      {
        int16 S = Src[x>>1];
        DstL0[x  ] = S;
        DstL0[x+1] = S;
        DstL1[x  ] = S;
        DstL1[x+1] = S;
      }
      Src   += SrcStride;
      DstL0 += DstStrideMul2;
      DstL1 += DstStrideMul2;
    }
  }
}
void xResampleAVX::ResampleD2HVAverage(int16* restrict Dst, const int16* restrict Src, int32 DstStride, int32 SrcStride, int32 DstWidth, int32 DstHeight)
{
  __m256i TwoV = _mm256_set1_epi16((int16)2);

  const int16 *restrict SrcL0 = Src;
  const int16 *restrict SrcL1 = Src + SrcStride;
  int32 SrcStrideMul2   = SrcStride<<1;

  if(((uint32)DstWidth & (uint32)0xF)==0) //Width%8==0
  {
    for(int32 y=0; y<DstHeight; y++)
    {
      for(int32 x=0; x<DstWidth; x+=16)
      {
        int32 SrcX = x<<1;
        __m256i TopLeftV     = _mm256_loadu_si256((__m256i*)&SrcL0[SrcX   ]);
        __m256i TopRightV    = _mm256_loadu_si256((__m256i*)&SrcL0[SrcX+16]);        
        __m256i BottomLeftV  = _mm256_loadu_si256((__m256i*)&SrcL1[SrcX   ]);
        __m256i BottomRightV = _mm256_loadu_si256((__m256i*)&SrcL1[SrcX+16]);
        __m256i TopV         = _mm256_hadd_epi16(TopLeftV, TopRightV);
        __m256i BottomV      = _mm256_hadd_epi16(BottomLeftV, BottomRightV);
        __m256i OutV         = _mm256_srai_epi16(_mm256_add_epi16(_mm256_add_epi16(TopV, BottomV), TwoV), 2);
        OutV = _mm256_permute4x64_epi64(OutV, 0xD8);
        _mm256_storeu_si256((__m256i*)&Dst[x], OutV);
      }
      SrcL0 += SrcStrideMul2;
      SrcL1 += SrcStrideMul2;
      Dst += DstStride;
    }
  }  
  else
  {
    int32 Width16 = (int32)((uint32)DstWidth & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)DstWidth & (uint32)c_MultipleMask8);
    int32 Width4  = (int32)((uint32)DstWidth & (uint32)c_MultipleMask4);

    for(int32 y=0; y<DstHeight; y++)
    {
      for(int32 x=0; x<Width16; x+=8)
      {
        int32 SrcX = x<<1;
        __m256i TopLeftV     = _mm256_loadu_si256((__m256i*)&SrcL0[SrcX   ]);
        __m256i TopRightV    = _mm256_loadu_si256((__m256i*)&SrcL0[SrcX+16]);        
        __m256i BottomLeftV  = _mm256_loadu_si256((__m256i*)&SrcL1[SrcX   ]);
        __m256i BottomRightV = _mm256_loadu_si256((__m256i*)&SrcL1[SrcX+16]);
        __m256i TopV         = _mm256_hadd_epi16(TopLeftV, TopRightV);
        __m256i BottomV      = _mm256_hadd_epi16(BottomLeftV, BottomRightV);
        __m256i OutV         = _mm256_srai_epi16(_mm256_add_epi16(_mm256_add_epi16(TopV, BottomV), TwoV), 2);
        OutV = _mm256_permute4x64_epi64(OutV, 0xD8);
        _mm256_storeu_si256((__m256i*)&Dst[x], OutV);
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        int32 SrcX = x<<1;
        __m128i TopLeftV     = _mm_loadu_si128((__m128i*)&SrcL0[SrcX  ]);
        __m128i TopRightV    = _mm_loadu_si128((__m128i*)&SrcL0[SrcX+8]);        
        __m128i BottomLeftV  = _mm_loadu_si128((__m128i*)&SrcL1[SrcX  ]);
        __m128i BottomRightV = _mm_loadu_si128((__m128i*)&SrcL1[SrcX+8]);
        __m128i TopV         = _mm_hadd_epi16(TopLeftV, TopRightV);
        __m128i BottomV      = _mm_hadd_epi16(BottomLeftV, BottomRightV);
        __m128i OutV         = _mm_srai_epi16(_mm_add_epi16(_mm_add_epi16(TopV, BottomV), _mm256_castsi256_si128(TwoV)), 2);
        _mm_storeu_si128 ((__m128i*)&Dst[x], OutV);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        int32 SrcX = x<<1;
        __m128i TopLeftV     = _mm_loadu_si128((__m128i*)&SrcL0[SrcX  ]);
        __m128i BottomLeftV  = _mm_loadu_si128((__m128i*)&SrcL1[SrcX  ]);
        __m128i TopV         = _mm_hadd_epi16(TopLeftV, TopLeftV);
        __m128i BottomV      = _mm_hadd_epi16(BottomLeftV, BottomLeftV);
        __m128i OutV         = _mm_srai_epi16(_mm_add_epi16(_mm_add_epi16(TopV, BottomV), _mm256_castsi256_si128(TwoV)), 2);
        _mm_storel_epi64 ((__m128i*)&Dst[x], OutV);      
      }
      for(int32 x=Width4; x<DstWidth; x++)
      {      
        int32 SrcX = x<<1;
        int16 D = ((int32)SrcL0[SrcX  ] + (int32)SrcL0[SrcX+1] + (int32)SrcL1[SrcX  ] + (int32)SrcL1[SrcX+1] + 2)>>2;
        Dst[x] = D;
      }
      SrcL0 += SrcStrideMul2;
      SrcL1 += SrcStrideMul2;
      Dst += DstStride;
    }
  }
}
#endif //X_AVX1 && X_AVX2

//===============================================================================================================================================================================================================

} //end of namespace AVLib
