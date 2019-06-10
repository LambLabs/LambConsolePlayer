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

#include "xPixelOps.h"

namespace AVlib {

//===============================================================================================================================================================================================================

void xPixelOps::xConvertI16ToFloat_STD(float* restrict Dst, int16* restrict Src, int32 Area)
{
  for(int32 i=0; i<Area; i++)
  { 
    Dst[i] = (float)(Src[i]);   
  }
}
void xPixelOps::xConvertFloatToI16_STD(int16* restrict Dst, float* restrict Src, int32 Area)
{
  for(int32 i=0; i<Area; i++)
  { 
    Dst[i] = (int16)(xClipS16(Src[i]));   
  }
}

//===============================================================================================================================================================================================================

#if X_SSE4_2
void xPixelOps::xConvertI16ToFloat_SSE(float* restrict Dst, int16* restrict Src, int32 Area)
{
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);
  
  for(int32 i=0; i<Area8; i+=8)
  { 
    __m128i In = _mm_loadu_si128((__m128i*)&(Src[i]));
    __m128i In32V0 = _mm_cvtepi16_epi32(In);
    __m128i In32V1 = _mm_cvtepi16_epi32(_mm_srli_si128(In,8));
    __m128  OutFloatV0 = _mm_cvtepi32_ps(In32V0);
    __m128  OutFloatV1 = _mm_cvtepi32_ps(In32V1);
    _mm_store_ps(&(Dst[i  ]), OutFloatV0);
    _mm_store_ps(&(Dst[i+4]), OutFloatV1);
  }
  for(int32 i=Area8; i<Area; i++)
  {
    Dst[i] = (float)(Src[i]);
  }
}
void xPixelOps::xConvertFloatToI16_SSE(int16* restrict Dst, float* restrict Src, int32 Area)
{
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);
  __m128 Int16Min = _mm_set1_ps(-32768.0);
  __m128 Int16Max = _mm_set1_ps( 32767.0);

  for(int32 i=0; i<Area8; i+=8)
  { 
    __m128 InV0 = _mm_load_ps(&(Src[i  ]));
    __m128 InV1 = _mm_load_ps(&(Src[i+4]));
    InV0 = _mm_max_ps(_mm_min_ps(InV0, Int16Max), Int16Min);
    InV1 = _mm_max_ps(_mm_min_ps(InV1, Int16Max), Int16Min);
    __m128i Out32V0 = _mm_cvtps_epi32(InV0);
    __m128i Out32V1 = _mm_cvtps_epi32(InV1);
    __m128i Out = _mm_packs_epi32(Out32V0, Out32V1);
    _mm_storeu_si128((__m128i*)(&(Dst[i  ])), Out);
  }
  for(int32 i=Area8; i<Area; i++)
  {
    Dst[i] = (int16)(xClipS16(Src[i]));   
  }
}
#endif //X_SSE4_2

//===============================================================================================================================================================================================================

#if X_AVX1 && X_AVX2
void xPixelOps::xConvertI16ToFloat_AVX(float* restrict Dst, int16* restrict Src, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  
  for(int32 i=0; i<Area16; i+=16)
  { 
    __m256i In = _mm256_loadu_si256((__m256i*)&(Src[i]));
    __m256i In32V0  = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(In,0));
    __m256i In32V1  = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(In,1));
    __m256  OutFloatV0 = _mm256_cvtepi32_ps(In32V0);
    __m256  OutFloatV1 = _mm256_cvtepi32_ps(In32V1);
    _mm256_store_ps(&(Dst[i  ]), OutFloatV0);
    _mm256_store_ps(&(Dst[i+8]), OutFloatV1);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    Dst[i] = (float)(Src[i]);
  }
}
void xPixelOps::xConvertFloatToI16_AVX(int16* restrict Dst, float* restrict Src, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  __m256 Int16Min = _mm256_set1_ps(-32768.0);
  __m256 Int16Max = _mm256_set1_ps( 32767.0);

  for(int32 i=0; i<Area16; i+=16)
  { 
    __m256 InV0 = _mm256_load_ps(&(Src[i  ]));
    __m256 InV1 = _mm256_load_ps(&(Src[i+4]));
    InV0 = _mm256_max_ps(_mm256_min_ps(InV0, Int16Max), Int16Min);
    InV1 = _mm256_max_ps(_mm256_min_ps(InV1, Int16Max), Int16Min);
    __m256i Out32V0 = _mm256_cvtps_epi32(InV0);
    __m256i Out32V1 = _mm256_cvtps_epi32(InV1);
    __m256i Out = _mm256_packs_epi32(Out32V0, Out32V1);
    Out = _mm256_permute4x64_epi64(Out, 0xD8);
    _mm256_storeu_si256((__m256i*)(&(Dst[i  ])), Out);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    Dst[i] = (int16)(xClipS16(Src[i]));   
  }
}
#endif //X_AVX1 && X_AVX2

//===============================================================================================================================================================================================================

} //end of namespace AVLib


