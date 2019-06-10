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

#include "xPixelOpsSSE.h"

#if X_USE_SSE && X_SSE4_2

namespace AVlib {

//=============================================================================================================================================================================
// Convert = Pack (with saturation) or Unpack
//=============================================================================================================================================================================
void xPixelOpsSSE::Convert(uint8* restrict Dst, const int16* restrict Src, int32 Area)
{ 
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  for(int32 i=0; i<Area16; i+=16)
  { 
    __m128i In1 = _mm_loadu_si128((__m128i*)&(Src[i  ]));
    __m128i In2 = _mm_loadu_si128((__m128i*)&(Src[i+8]));
    __m128i Out = _mm_packus_epi16(In1, In2);
    _mm_storeu_si128 ((__m128i*)(&(Dst[i])), Out);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    Dst[i] = (uint8)xClipU8(Src[i]);
  }
}
void xPixelOpsSSE::Convert(uint8* restrict Dst, const int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height)
{
  if(((uint32)Width & (uint32)0xF)==0) //Width%16==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=16)
      {
        __m128i In1 = _mm_loadu_si128((__m128i*)&(Src[x  ]));
        __m128i In2 = _mm_loadu_si128((__m128i*)&(Src[x+8]));
        __m128i Out = _mm_packus_epi16(In1, In2);
        _mm_storeu_si128 ((__m128i*)(&(Dst[x])), Out);
      }
      Src += SrcStride;
      Dst += DstStride;    
    }
  }
  else
  {
    int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width16; x+=16)
      {
        __m128i In1 = _mm_loadu_si128((__m128i*)&(Src[x  ]));
        __m128i In2 = _mm_loadu_si128((__m128i*)&(Src[x+8]));
        __m128i Out = _mm_packus_epi16(In1, In2);
        _mm_storeu_si128 ((__m128i*)(&(Dst[x])), Out);
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i In = _mm_loadu_si128((__m128i*)&(Src[x  ]));
        __m128i Out = _mm_packus_epi16(In, In);
        _mm_storel_epi64 ((__m128i*)(&(Dst[x])), Out);
      }
      for(int32 x=Width8 ; x<Width; x++)
      {
        Dst[x] = (uint8)xClipU8(Src[x]);
      }
      Src += SrcStride;
      Dst += DstStride;    
    }
  }
}
void xPixelOpsSSE::Convert(int16* restrict Dst, const uint8* restrict Src, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  __m128i Zero = _mm_setzero_si128();

  for(int32 i=0; i<Area16; i+=16)
  { 
    __m128i In = _mm_loadu_si128((__m128i*)&(Src[i]));
    __m128i Out1 = _mm_unpacklo_epi8(In, Zero);
    __m128i Out2 = _mm_unpackhi_epi8(In, Zero);
    _mm_storeu_si128 ((__m128i*)(&(Dst[i  ])), Out1);
    _mm_storeu_si128 ((__m128i*)(&(Dst[i+8])), Out2);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    Dst[i] = (int16)(Src[i]);
  }
}
void xPixelOpsSSE::Convert(int16* restrict Dst, const uint8* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height)
{
  __m128i Zero = _mm_setzero_si128();

  if(((uint32)Width & (uint32)0xF)==0) //Width%16==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=16)
      {
        __m128i In = _mm_loadu_si128((__m128i*)&(Src[x]));
        __m128i Out1 = _mm_unpacklo_epi8(In, Zero);
        __m128i Out2 = _mm_unpackhi_epi8(In, Zero);
        _mm_storeu_si128 ((__m128i*)(&(Dst[x  ])), Out1);
        _mm_storeu_si128 ((__m128i*)(&(Dst[x+8])), Out2);
      }
      Src += SrcStride;
      Dst += DstStride;    
    }
  }
  else
  {
    int32 Width16 = (int32)((uint32)Width & (uint32)c_MultipleMask16);
    int32 Width8  = (int32)((uint32)Width & (uint32)c_MultipleMask8);
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width16; x+=16)
      {
        __m128i In = _mm_loadu_si128((__m128i*)&(Src[x]));
        __m128i Out1 = _mm_unpacklo_epi8(In, Zero);
        __m128i Out2 = _mm_unpackhi_epi8(In, Zero);
        _mm_storeu_si128 ((__m128i*)(&(Dst[x  ])), Out1);
        _mm_storeu_si128 ((__m128i*)(&(Dst[x+8])), Out2);
      }
      for(int32 x=Width16; x<Width8; x+=8)
      {
        __m128i In = _mm_loadl_epi64((__m128i*)&(Src[x]));
        __m128i Out1 = _mm_unpacklo_epi8(In, Zero);
        _mm_storeu_si128 ((__m128i*)(&(Dst[x  ])), Out1);
      }
      for(int32 x=Width8 ; x<Width; x++)
      {
        Dst[x] = (int16)(Src[x]);
      }
      Src += SrcStride;
      Dst += DstStride;    
    }
  }
}

//=============================================================================================================================================================================
// Arrangement ---> 2 components
//=============================================================================================================================================================================
void xPixelOpsSSE::SOA2toAOS2(uint8* restrict DstAB, const uint8* restrict SrcA, const uint8* restrict SrcB, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m128i a = _mm_loadu_si128((__m128i*)&SrcA[i]); //load A0-A7
    __m128i b = _mm_loadu_si128((__m128i*)&SrcB[i]); //load B0-B7

                                                     //transpose
    __m128i ab_0 = _mm_unpacklo_epi8(a, b);
    __m128i ab_1 = _mm_unpackhi_epi8(a, b);

    //save
    _mm_storeu_si128((__m128i*)&DstAB[(i<<1)+  0], ab_0);
    _mm_storeu_si128((__m128i*)&DstAB[(i<<1)+ 16], ab_1);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    uint8 a = SrcA[i];
    uint8 b = SrcB[i];
    DstAB[(i<<1)+0] = a;
    DstAB[(i<<1)+1] = b;
  }
}
void xPixelOpsSSE::SOA2toAOS2(int16* restrict DstAB, const int16* restrict SrcA, const int16* restrict SrcB, int32 Area)
{
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);
  for(int32 i=0; i<Area8; i+=8)
  {
    //load
    __m128i a = _mm_loadu_si128((__m128i*)&SrcA[i]); //load A0-A7
    __m128i b = _mm_loadu_si128((__m128i*)&SrcB[i]); //load B0-B7

                                                     //transpose
    __m128i ab_0 = _mm_unpacklo_epi16(a, b);
    __m128i ab_1 = _mm_unpackhi_epi16(a, b);

    //save
    _mm_storeu_si128((__m128i*)&DstAB[(i<<1)+ 0], ab_0);
    _mm_storeu_si128((__m128i*)&DstAB[(i<<1)+ 8], ab_1);
  }
  for(int32 i=Area8; i<Area; i++)
  {
    int16 a = SrcA[i];
    int16 b = SrcB[i];
    DstAB[(i<<1)+0] = a;
    DstAB[(i<<1)+1] = b;
  }
}
void xPixelOpsSSE::AOS2toSOA2(uint8* restrict DstA, uint8* restrict DstB, const uint8* restrict SrcAB, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m128i ab_0 = _mm_loadu_si128((__m128i*)&SrcAB[(i<<1)+ 0]);
    __m128i ab_1 = _mm_loadu_si128((__m128i*)&SrcAB[(i<<1)+16]);

    //transpose
    __m128i aabb_0 = _mm_unpacklo_epi8(ab_0, ab_1);
    __m128i aabb_1 = _mm_unpackhi_epi8(ab_0, ab_1);

    __m128i aaaabbbb_0 = _mm_unpacklo_epi8(aabb_0, aabb_1);
    __m128i aaaabbbb_1 = _mm_unpackhi_epi8(aabb_0, aabb_1);

    __m128i aaaaaaaabbbbbbbb_0 = _mm_unpacklo_epi8(aaaabbbb_0, aaaabbbb_1);
    __m128i aaaaaaaabbbbbbbb_1 = _mm_unpackhi_epi8(aaaabbbb_0, aaaabbbb_1);

    __m128i a = _mm_unpacklo_epi8(aaaaaaaabbbbbbbb_0, aaaaaaaabbbbbbbb_1);
    __m128i b = _mm_unpackhi_epi8(aaaaaaaabbbbbbbb_0, aaaaaaaabbbbbbbb_1);

    //save
    _mm_storeu_si128((__m128i*)&DstA[i], a);
    _mm_storeu_si128((__m128i*)&DstB[i], b);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    uint8 a = SrcAB[(i<<1)+0];
    uint8 b = SrcAB[(i<<1)+1];
    DstA[i] = a;
    DstB[i] = b;
  }
}
void xPixelOpsSSE::AOS2toSOA2(int16* restrict DstA, int16* restrict DstB, const int16* restrict SrcAB, int32 Area)
{
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);
  for(int32 i=0; i<Area8; i+=8)
  {
    //load
    __m128i ab_0 = _mm_loadu_si128((__m128i*)&SrcAB[(i<<1)+ 0]);
    __m128i ab_1 = _mm_loadu_si128((__m128i*)&SrcAB[(i<<1)+ 8]);

    //transpose
    __m128i aabb_0 = _mm_unpacklo_epi16(ab_0, ab_1);
    __m128i aabb_1 = _mm_unpackhi_epi16(ab_0, ab_1);

    __m128i aaaabbbb_0 = _mm_unpacklo_epi16(aabb_0, aabb_1);
    __m128i aaaabbbb_1 = _mm_unpackhi_epi16(aabb_0, aabb_1);

    __m128i a = _mm_unpacklo_epi16(aaaabbbb_0, aaaabbbb_1);
    __m128i b = _mm_unpackhi_epi16(aaaabbbb_0, aaaabbbb_1);

    //save
    _mm_storeu_si128((__m128i*)&DstA[i], a);
    _mm_storeu_si128((__m128i*)&DstB[i], b);

  }
  for(int32 i=Area8; i<Area; i++)
  {
    int16 a = SrcAB[(i<<1)+0];
    int16 b = SrcAB[(i<<1)+1];
    DstA[i] = a;
    DstB[i] = b;
  }
}

//=============================================================================================================================================================================
// Arrangement ---> 4 components
//=============================================================================================================================================================================
void xPixelOpsSSE::SOA4toAOS4(uint8* restrict DstABCD, const uint8* restrict SrcA, const uint8* restrict SrcB, const uint8* restrict SrcC, const uint8* restrict SrcD, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m128i a = _mm_loadu_si128((__m128i*)&SrcA[i]);
    __m128i b = _mm_loadu_si128((__m128i*)&SrcB[i]);
    __m128i c = _mm_loadu_si128((__m128i*)&SrcC[i]);
    __m128i d = _mm_loadu_si128((__m128i*)&SrcD[i]);

    //transpose
    __m128i ac_0 = _mm_unpacklo_epi8(a, c);
    __m128i ac_1 = _mm_unpackhi_epi8(a, c);
    __m128i bd_0 = _mm_unpacklo_epi8(b, d);
    __m128i bd_1 = _mm_unpackhi_epi8(b, d);

    __m128i abcd_0 = _mm_unpacklo_epi8(ac_0, bd_0);
    __m128i abcd_1 = _mm_unpackhi_epi8(ac_0, bd_0);
    __m128i abcd_2 = _mm_unpacklo_epi8(ac_1, bd_1);
    __m128i abcd_3 = _mm_unpackhi_epi8(ac_1, bd_1);

    //save
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+ 0], abcd_0);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+16], abcd_1);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+32], abcd_2);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+48], abcd_3);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    uint8 a = SrcA[i];
    uint8 b = SrcB[i];
    uint8 c = SrcC[i];
    uint8 d = SrcD[i];
    DstABCD[(i<<2)+0] = a;
    DstABCD[(i<<2)+1] = b;
    DstABCD[(i<<2)+2] = c;
    DstABCD[(i<<2)+3] = d;
  }
}
void xPixelOpsSSE::AOS4toSOA4(uint8* restrict DstA, uint8* restrict DstB, uint8* restrict DstC, uint8* restrict DstD, const uint8* restrict SrcABCD, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m128i abcd_0 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+ 0]);
    __m128i abcd_1 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+16]);
    __m128i abcd_2 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+32]);
    __m128i abcd_3 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+48]);

    //transpose
    __m128i aabbccdd_0 = _mm_unpacklo_epi8(abcd_0, abcd_1);
    __m128i aabbccdd_1 = _mm_unpackhi_epi8(abcd_0, abcd_1);
    __m128i aabbccdd_2 = _mm_unpacklo_epi8(abcd_2, abcd_3);
    __m128i aabbccdd_3 = _mm_unpackhi_epi8(abcd_2, abcd_3);

    __m128i a4b4c4d4_0 = _mm_unpacklo_epi8(aabbccdd_0, aabbccdd_1);
    __m128i a4b4c4d4_1 = _mm_unpackhi_epi8(aabbccdd_0, aabbccdd_1);
    __m128i a4b4c4d4_2 = _mm_unpacklo_epi8(aabbccdd_2, aabbccdd_3);
    __m128i a4b4c4d4_3 = _mm_unpackhi_epi8(aabbccdd_2, aabbccdd_3);

    __m128i a8b8_0 = _mm_unpacklo_epi8(a4b4c4d4_0, a4b4c4d4_1);
    __m128i c8d8_0 = _mm_unpackhi_epi8(a4b4c4d4_0, a4b4c4d4_1);
    __m128i a8b8_1 = _mm_unpacklo_epi8(a4b4c4d4_2, a4b4c4d4_3);
    __m128i c8d8_1 = _mm_unpackhi_epi8(a4b4c4d4_2, a4b4c4d4_3);

    __m128i a = _mm_unpacklo_epi64(a8b8_0, a8b8_1);
    __m128i b = _mm_unpackhi_epi64(a8b8_0, a8b8_1);
    __m128i c = _mm_unpacklo_epi64(c8d8_0, c8d8_1);
    __m128i d = _mm_unpackhi_epi64(c8d8_0, c8d8_1);

    //save
    _mm_storeu_si128((__m128i*)&DstA[i], a);
    _mm_storeu_si128((__m128i*)&DstB[i], b);
    _mm_storeu_si128((__m128i*)&DstC[i], c);
    _mm_storeu_si128((__m128i*)&DstD[i], d);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    uint8 a = SrcABCD[(i<<2)+0];
    uint8 b = SrcABCD[(i<<2)+1];
    uint8 c = SrcABCD[(i<<2)+2];
    uint8 d = SrcABCD[(i<<2)+3];
    DstA[i] = a;
    DstB[i] = b;
    DstC[i] = c;
    DstD[i] = d;
  }
}
void xPixelOpsSSE::SOA4toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16* restrict SrcD, int32 Area)
{
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);
  for(int32 i=0; i<Area8; i+=8)
  {
    //load
    __m128i a = _mm_loadu_si128((__m128i*)&SrcA[i]); //load R0-R7
    __m128i b = _mm_loadu_si128((__m128i*)&SrcB[i]); //load G0-G7
    __m128i c = _mm_loadu_si128((__m128i*)&SrcC[i]); //load B0-B7
    __m128i d = _mm_loadu_si128((__m128i*)&SrcD[i]); //load A0-A7

                                                     //transpose
    __m128i ac_0 = _mm_unpacklo_epi16(a, c);
    __m128i ac_1 = _mm_unpackhi_epi16(a, c);
    __m128i bd_0 = _mm_unpacklo_epi16(b, d);
    __m128i bd_1 = _mm_unpackhi_epi16(b, d);

    __m128i abcd_0 = _mm_unpacklo_epi16(ac_0, bd_0);
    __m128i abcd_1 = _mm_unpackhi_epi16(ac_0, bd_0);
    __m128i abcd_2 = _mm_unpacklo_epi16(ac_1, bd_1);
    __m128i abcd_3 = _mm_unpackhi_epi16(ac_1, bd_1);

    //save
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+ 0], abcd_0);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+ 8], abcd_1);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+16], abcd_2);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+24], abcd_3);
  }
  for(int32 i=Area8; i<Area; i++)
  {
    int16 a = SrcA[i];
    int16 b = SrcB[i];
    int16 c = SrcC[i];
    int16 d = SrcD[i];
    DstABCD[(i<<2)+0] = a;
    DstABCD[(i<<2)+1] = b;
    DstABCD[(i<<2)+2] = c;
    DstABCD[(i<<2)+3] = d;
  }
}
void xPixelOpsSSE::AOS4toSOA4(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, int16* restrict DstD, const int16* restrict SrcABCD, int32 Area)
{
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);
  for(int32 i=0; i<Area8; i+=8)
  {
    //load
    __m128i abcd_0 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+ 0]);
    __m128i abcd_1 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+ 8]);
    __m128i abcd_2 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+16]);
    __m128i abcd_3 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+24]);

    //transpose
    __m128i ac_0 = _mm_unpacklo_epi16(abcd_0, abcd_1);
    __m128i ac_1 = _mm_unpackhi_epi16(abcd_0, abcd_1);
    __m128i bd_0 = _mm_unpacklo_epi16(abcd_2, abcd_3);
    __m128i bd_1 = _mm_unpackhi_epi16(abcd_2, abcd_3);

    __m128i a_0 = _mm_unpacklo_epi16(ac_0, ac_1);
    __m128i b_0 = _mm_unpackhi_epi16(ac_0, ac_1);
    __m128i c_0 = _mm_unpacklo_epi16(bd_0, bd_1);
    __m128i d_0 = _mm_unpackhi_epi16(bd_0, bd_1);

    __m128i a = _mm_unpacklo_epi64(a_0, c_0);
    __m128i b = _mm_unpackhi_epi64(a_0, c_0);
    __m128i c = _mm_unpacklo_epi64(b_0, d_0);
    __m128i d = _mm_unpackhi_epi64(b_0, d_0);

    //save
    _mm_storeu_si128((__m128i*)&DstA[i], a);
    _mm_storeu_si128((__m128i*)&DstB[i], b);
    _mm_storeu_si128((__m128i*)&DstC[i], c);
    _mm_storeu_si128((__m128i*)&DstD[i], d);

  }
  for(int32 i=Area8; i<Area; i++)
  {
    int16 a = SrcABCD[(i<<2)+0];
    int16 b = SrcABCD[(i<<2)+1];
    int16 c = SrcABCD[(i<<2)+2];
    int16 d = SrcABCD[(i<<2)+3];
    DstA[i] = a;
    DstB[i] = b;
    DstC[i] = c;
    DstD[i] = d;
  }
}

//=============================================================================================================================================================================
// Arrangement ---> partial 4 components
//=============================================================================================================================================================================
void xPixelOpsSSE::SOA3toAOS4(uint8* restrict DstABCD, const uint8* restrict SrcA, const uint8* restrict SrcB, const uint8* restrict SrcC, const uint8 ValueD, int32 Area)
{
  __m128i d = _mm_set1_epi8(ValueD);

  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m128i a = _mm_loadu_si128((__m128i*)&SrcA[i]);
    __m128i b = _mm_loadu_si128((__m128i*)&SrcB[i]);
    __m128i c = _mm_loadu_si128((__m128i*)&SrcC[i]);

    //transpose
    __m128i ac_0 = _mm_unpacklo_epi8(a, c);
    __m128i ac_1 = _mm_unpackhi_epi8(a, c);
    __m128i bd_0 = _mm_unpacklo_epi8(b, d);
    __m128i bd_1 = _mm_unpackhi_epi8(b, d);

    __m128i abcd_0 = _mm_unpacklo_epi8(ac_0, bd_0);
    __m128i abcd_1 = _mm_unpackhi_epi8(ac_0, bd_0);
    __m128i abcd_2 = _mm_unpacklo_epi8(ac_1, bd_1);
    __m128i abcd_3 = _mm_unpackhi_epi8(ac_1, bd_1);

    //save
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+ 0], abcd_0);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+16], abcd_1);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+32], abcd_2);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+48], abcd_3);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    uint8 a = SrcA[i];
    uint8 b = SrcB[i];
    uint8 c = SrcC[i];
    DstABCD[(i<<2)+0] = a;
    DstABCD[(i<<2)+1] = b;
    DstABCD[(i<<2)+2] = c;
    DstABCD[(i<<2)+3] = ValueD;
  }
}
void xPixelOpsSSE::AOS4toSOA3(uint8* restrict DstA, uint8* restrict DstB, uint8* restrict DstC, const uint8* restrict SrcABCD, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m128i abcd_0 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+ 0]);
    __m128i abcd_1 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+16]);
    __m128i abcd_2 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+32]);
    __m128i abcd_3 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+48]);

    //transpose
    __m128i aabbccdd_0 = _mm_unpacklo_epi8(abcd_0, abcd_1);
    __m128i aabbccdd_1 = _mm_unpackhi_epi8(abcd_0, abcd_1);
    __m128i aabbccdd_2 = _mm_unpacklo_epi8(abcd_2, abcd_3);
    __m128i aabbccdd_3 = _mm_unpackhi_epi8(abcd_2, abcd_3);

    __m128i a4b4c4d4_0 = _mm_unpacklo_epi8(aabbccdd_0, aabbccdd_1);
    __m128i a4b4c4d4_1 = _mm_unpackhi_epi8(aabbccdd_0, aabbccdd_1);
    __m128i a4b4c4d4_2 = _mm_unpacklo_epi8(aabbccdd_2, aabbccdd_3);
    __m128i a4b4c4d4_3 = _mm_unpackhi_epi8(aabbccdd_2, aabbccdd_3);

    __m128i a8b8_0 = _mm_unpacklo_epi8(a4b4c4d4_0, a4b4c4d4_1);
    __m128i c8d8_0 = _mm_unpackhi_epi8(a4b4c4d4_0, a4b4c4d4_1);
    __m128i a8b8_1 = _mm_unpacklo_epi8(a4b4c4d4_2, a4b4c4d4_3);
    __m128i c8d8_1 = _mm_unpackhi_epi8(a4b4c4d4_2, a4b4c4d4_3);

    __m128i a = _mm_unpacklo_epi64(a8b8_0, a8b8_1);
    __m128i b = _mm_unpackhi_epi64(a8b8_0, a8b8_1);
    __m128i c = _mm_unpacklo_epi64(c8d8_0, c8d8_1);

    //save
    _mm_storeu_si128((__m128i*)&DstA[i], a);
    _mm_storeu_si128((__m128i*)&DstB[i], b);
    _mm_storeu_si128((__m128i*)&DstC[i], c);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    uint8 a = SrcABCD[(i<<2)+0];
    uint8 b = SrcABCD[(i<<2)+1];
    uint8 c = SrcABCD[(i<<2)+2];
    DstA[i] = a;
    DstB[i] = b;
    DstC[i] = c;
  }
}
void xPixelOpsSSE::SOA3toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16 ValueD, int32 Area)
{
  __m128i d = _mm_set1_epi16(ValueD);

  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);
  for(int32 i=0; i<Area8; i+=8)
  {
    //load
    __m128i a = _mm_loadu_si128((__m128i*)&SrcA[i]); //load R0-R7
    __m128i b = _mm_loadu_si128((__m128i*)&SrcB[i]); //load G0-G7
    __m128i c = _mm_loadu_si128((__m128i*)&SrcC[i]); //load B0-B7

                                                     //transpose
    __m128i ac_0 = _mm_unpacklo_epi16(a, c);
    __m128i ac_1 = _mm_unpackhi_epi16(a, c);
    __m128i bd_0 = _mm_unpacklo_epi16(b, d);
    __m128i bd_1 = _mm_unpackhi_epi16(b, d);

    __m128i abcd_0 = _mm_unpacklo_epi16(ac_0, bd_0);
    __m128i abcd_1 = _mm_unpackhi_epi16(ac_0, bd_0);
    __m128i abcd_2 = _mm_unpacklo_epi16(ac_1, bd_1);
    __m128i abcd_3 = _mm_unpackhi_epi16(ac_1, bd_1);

    //save
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+ 0], abcd_0);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+ 8], abcd_1);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+16], abcd_2);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+24], abcd_3);
  }
  for(int32 i=Area8; i<Area; i++)
  {
    int16 a = SrcA[i];
    int16 b = SrcB[i];
    int16 c = SrcC[i];
    DstABCD[(i<<2)+0] = a;
    DstABCD[(i<<2)+1] = b;
    DstABCD[(i<<2)+2] = c;
    DstABCD[(i<<2)+3] = ValueD;
  }
}
void xPixelOpsSSE::SOA3toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16 ValueD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height)
{
  __m128i d = _mm_set1_epi16(ValueD);

  if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
  {
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x+=8)
      {
        //load
        __m128i a = _mm_loadu_si128((__m128i*)&SrcA[x]); //load R0-R7
        __m128i b = _mm_loadu_si128((__m128i*)&SrcB[x]); //load G0-G7
        __m128i c = _mm_loadu_si128((__m128i*)&SrcC[x]); //load B0-B7

                                                         //transpose
        __m128i ac_0 = _mm_unpacklo_epi16(a, c);
        __m128i ac_1 = _mm_unpackhi_epi16(a, c);
        __m128i bd_0 = _mm_unpacklo_epi16(b, d);
        __m128i bd_1 = _mm_unpackhi_epi16(b, d);

        __m128i abcd_0 = _mm_unpacklo_epi16(ac_0, bd_0);
        __m128i abcd_1 = _mm_unpackhi_epi16(ac_0, bd_0);
        __m128i abcd_2 = _mm_unpacklo_epi16(ac_1, bd_1);
        __m128i abcd_3 = _mm_unpackhi_epi16(ac_1, bd_1);

        //save
        _mm_storeu_si128((__m128i*)&DstABCD[(x<<2)+ 0], abcd_0);
        _mm_storeu_si128((__m128i*)&DstABCD[(x<<2)+ 8], abcd_1);
        _mm_storeu_si128((__m128i*)&DstABCD[(x<<2)+16], abcd_2);
        _mm_storeu_si128((__m128i*)&DstABCD[(x<<2)+24], abcd_3);
      }
      SrcA    += SrcStride;
      SrcB    += SrcStride;
      SrcC    += SrcStride;
      DstABCD += DstStride;  
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
        //load
        __m128i a = _mm_loadu_si128((__m128i*)&SrcA[x]); //load R0-R7
        __m128i b = _mm_loadu_si128((__m128i*)&SrcB[x]); //load G0-G7
        __m128i c = _mm_loadu_si128((__m128i*)&SrcC[x]); //load B0-B7

                                                         //transpose
        __m128i ac_0 = _mm_unpacklo_epi16(a, c);
        __m128i ac_1 = _mm_unpackhi_epi16(a, c);
        __m128i bd_0 = _mm_unpacklo_epi16(b, d);
        __m128i bd_1 = _mm_unpackhi_epi16(b, d);

        __m128i abcd_0 = _mm_unpacklo_epi16(ac_0, bd_0);
        __m128i abcd_1 = _mm_unpackhi_epi16(ac_0, bd_0);
        __m128i abcd_2 = _mm_unpacklo_epi16(ac_1, bd_1);
        __m128i abcd_3 = _mm_unpackhi_epi16(ac_1, bd_1);

        //save
        _mm_storeu_si128((__m128i*)&DstABCD[(x<<2)+ 0], abcd_0);
        _mm_storeu_si128((__m128i*)&DstABCD[(x<<2)+ 8], abcd_1);
        _mm_storeu_si128((__m128i*)&DstABCD[(x<<2)+16], abcd_2);
        _mm_storeu_si128((__m128i*)&DstABCD[(x<<2)+24], abcd_3);
      }
      for(int32 x=Width8; x<Width4; x+=4)
      {
        //load
        __m128i a = _mm_loadl_epi64((__m128i*)&SrcA[x]); //load R0-R7
        __m128i b = _mm_loadl_epi64((__m128i*)&SrcB[x]); //load G0-G7
        __m128i c = _mm_loadl_epi64((__m128i*)&SrcC[x]); //load B0-B7

                                                         //transpose
        __m128i ac_0 = _mm_unpacklo_epi16(a, c);
        __m128i bd_0 = _mm_unpacklo_epi16(b, d);

        __m128i abcd_0 = _mm_unpacklo_epi16(ac_0, bd_0);
        __m128i abcd_1 = _mm_unpackhi_epi16(ac_0, bd_0);

        //save
        _mm_storeu_si128((__m128i*)&DstABCD[(x<<2)+ 0], abcd_0);
        _mm_storeu_si128((__m128i*)&DstABCD[(x<<2)+ 8], abcd_1);
      }
      for(int32 x=Width4; x<Width; x++)
      {      
        int16 a = SrcA[x];
        int16 b = SrcB[x];
        int16 c = SrcC[x];
        DstABCD[(x<<2)+0] = a;
        DstABCD[(x<<2)+1] = b;
        DstABCD[(x<<2)+2] = c;
        DstABCD[(x<<2)+3] = ValueD;
      }
      SrcA    += SrcStride;
      SrcB    += SrcStride;
      SrcC    += SrcStride;
      DstABCD += DstStride;  
    }
  }
}
void xPixelOpsSSE::AOS4toSOA3(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, const int16* restrict SrcABCD, int32 Area)
{
  int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);
  for(int32 i=0; i<Area8; i+=8)
  {
    //load
    __m128i abcd_0 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+ 0]);
    __m128i abcd_1 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+ 8]);
    __m128i abcd_2 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+16]);
    __m128i abcd_3 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+24]);

    //transpose
    __m128i ac_0 = _mm_unpacklo_epi16(abcd_0, abcd_1);
    __m128i ac_1 = _mm_unpackhi_epi16(abcd_0, abcd_1);
    __m128i bd_0 = _mm_unpacklo_epi16(abcd_2, abcd_3);
    __m128i bd_1 = _mm_unpackhi_epi16(abcd_2, abcd_3);

    __m128i a_0 = _mm_unpacklo_epi16(ac_0, ac_1);
    __m128i b_0 = _mm_unpackhi_epi16(ac_0, ac_1);
    __m128i c_0 = _mm_unpacklo_epi16(bd_0, bd_1);
    __m128i d_0 = _mm_unpackhi_epi16(bd_0, bd_1);

    __m128i a = _mm_unpacklo_epi64(a_0, c_0);
    __m128i b = _mm_unpackhi_epi64(a_0, c_0);
    __m128i c = _mm_unpacklo_epi64(b_0, d_0);

    //save
    _mm_storeu_si128((__m128i*)&DstA[i], a);
    _mm_storeu_si128((__m128i*)&DstB[i], b);
    _mm_storeu_si128((__m128i*)&DstC[i], c);
  }
  for(int32 i=Area8; i<Area; i++)
  {
    int16 a = SrcABCD[(i<<2)+0];
    int16 b = SrcABCD[(i<<2)+1];
    int16 c = SrcABCD[(i<<2)+2];
    DstA[i] = a;
    DstB[i] = b;
    DstC[i] = c;
  }
}

//=============================================================================================================================================================================
// Arrangement + convertion ---> 4 components
//=============================================================================================================================================================================
void xPixelOpsSSE::ConvertSOA4toAOS4(uint8* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16* restrict SrcD, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m128i a_0 = _mm_loadu_si128((__m128i*)&SrcA[i  ]);
    __m128i a_1 = _mm_loadu_si128((__m128i*)&SrcA[i+8]);
    __m128i b_0 = _mm_loadu_si128((__m128i*)&SrcB[i  ]);
    __m128i b_1 = _mm_loadu_si128((__m128i*)&SrcB[i+8]);
    __m128i c_0 = _mm_loadu_si128((__m128i*)&SrcC[i  ]);
    __m128i c_1 = _mm_loadu_si128((__m128i*)&SrcC[i+8]);
    __m128i d_0 = _mm_loadu_si128((__m128i*)&SrcD[i  ]);
    __m128i d_1 = _mm_loadu_si128((__m128i*)&SrcD[i+8]);

    //pack
    __m128i a = _mm_packus_epi16(a_0, a_1);
    __m128i b = _mm_packus_epi16(b_0, b_1);
    __m128i c = _mm_packus_epi16(c_0, c_1);
    __m128i d = _mm_packus_epi16(d_0, d_1);

    //transpose
    __m128i ac_0 = _mm_unpacklo_epi8(a, c);
    __m128i ac_1 = _mm_unpackhi_epi8(a, c);
    __m128i bd_0 = _mm_unpacklo_epi8(b, d);
    __m128i bd_1 = _mm_unpackhi_epi8(b, d);

    __m128i abcd_0 = _mm_unpacklo_epi8(ac_0, bd_0);
    __m128i abcd_1 = _mm_unpackhi_epi8(ac_0, bd_0);
    __m128i abcd_2 = _mm_unpacklo_epi8(ac_1, bd_1);
    __m128i abcd_3 = _mm_unpackhi_epi8(ac_1, bd_1);

    //save
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+ 0], abcd_0);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+16], abcd_1);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+32], abcd_2);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+48], abcd_3);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    uint8 a = (uint8)xClipU8(SrcA[i]);
    uint8 b = (uint8)xClipU8(SrcB[i]);
    uint8 c = (uint8)xClipU8(SrcC[i]);
    uint8 d = (uint8)xClipU8(SrcD[i]);
    DstABCD[(i<<2)+0] = a;
    DstABCD[(i<<2)+1] = b;
    DstABCD[(i<<2)+2] = c;
    DstABCD[(i<<2)+3] = d;
  }
}
void xPixelOpsSSE::ConvertAOS4toSOA4(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, int16* restrict DstD, const uint8* restrict SrcABCD, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  __m128i Zero = _mm_setzero_si128();

  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m128i abcd_0 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+ 0]);
    __m128i abcd_1 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+16]);
    __m128i abcd_2 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+32]);
    __m128i abcd_3 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+48]);

    //transpose
    __m128i aabbccdd_0 = _mm_unpacklo_epi8(abcd_0, abcd_1);
    __m128i aabbccdd_1 = _mm_unpackhi_epi8(abcd_0, abcd_1);
    __m128i aabbccdd_2 = _mm_unpacklo_epi8(abcd_2, abcd_3);
    __m128i aabbccdd_3 = _mm_unpackhi_epi8(abcd_2, abcd_3);

    __m128i a4b4c4d4_0 = _mm_unpacklo_epi8(aabbccdd_0, aabbccdd_1);
    __m128i a4b4c4d4_1 = _mm_unpackhi_epi8(aabbccdd_0, aabbccdd_1);
    __m128i a4b4c4d4_2 = _mm_unpacklo_epi8(aabbccdd_2, aabbccdd_3);
    __m128i a4b4c4d4_3 = _mm_unpackhi_epi8(aabbccdd_2, aabbccdd_3);

    __m128i a8b8_0 = _mm_unpacklo_epi8(a4b4c4d4_0, a4b4c4d4_1);
    __m128i c8d8_0 = _mm_unpackhi_epi8(a4b4c4d4_0, a4b4c4d4_1);
    __m128i a8b8_1 = _mm_unpacklo_epi8(a4b4c4d4_2, a4b4c4d4_3);
    __m128i c8d8_1 = _mm_unpackhi_epi8(a4b4c4d4_2, a4b4c4d4_3);

    __m128i a = _mm_unpacklo_epi64(a8b8_0, a8b8_1);
    __m128i b = _mm_unpackhi_epi64(a8b8_0, a8b8_1);
    __m128i c = _mm_unpacklo_epi64(c8d8_0, c8d8_1);
    __m128i d = _mm_unpackhi_epi64(c8d8_0, c8d8_1);

    //unpack
    __m128i a_0 = _mm_unpacklo_epi8(a, Zero);
    __m128i a_1 = _mm_unpackhi_epi8(a, Zero);
    __m128i b_0 = _mm_unpacklo_epi8(b, Zero);
    __m128i b_1 = _mm_unpackhi_epi8(b, Zero);
    __m128i c_0 = _mm_unpacklo_epi8(c, Zero);
    __m128i c_1 = _mm_unpackhi_epi8(c, Zero);
    __m128i d_0 = _mm_unpacklo_epi8(d, Zero);
    __m128i d_1 = _mm_unpackhi_epi8(d, Zero);

    //save
    _mm_storeu_si128((__m128i*)&DstA[i  ], a_0);
    _mm_storeu_si128((__m128i*)&DstA[i+8], a_1);
    _mm_storeu_si128((__m128i*)&DstB[i  ], b_0);
    _mm_storeu_si128((__m128i*)&DstB[i+8], b_1);
    _mm_storeu_si128((__m128i*)&DstC[i  ], c_0);
    _mm_storeu_si128((__m128i*)&DstC[i+8], c_1);
    _mm_storeu_si128((__m128i*)&DstD[i  ], d_0);
    _mm_storeu_si128((__m128i*)&DstD[i+8], d_1);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    int16 a = SrcABCD[(i<<2)+0];
    int16 b = SrcABCD[(i<<2)+1];
    int16 c = SrcABCD[(i<<2)+2];
    int16 d = SrcABCD[(i<<2)+3];
    DstA[i] = a;
    DstB[i] = b;
    DstC[i] = c;
    DstD[i] = d;
  }
}

//=============================================================================================================================================================================
// Arrangement + convertion ---> partial 4 components
//=============================================================================================================================================================================
void xPixelOpsSSE::ConvertSOA3toAOS4(uint8* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16 ValueD, int32 Area)
{
  uint8   dd = (uint8)xClipU8(ValueD);
  __m128i d  = _mm_set1_epi8(dd);

  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m128i a_0 = _mm_loadu_si128((__m128i*)&SrcA[i  ]);
    __m128i a_1 = _mm_loadu_si128((__m128i*)&SrcA[i+8]);
    __m128i b_0 = _mm_loadu_si128((__m128i*)&SrcB[i  ]);
    __m128i b_1 = _mm_loadu_si128((__m128i*)&SrcB[i+8]);
    __m128i c_0 = _mm_loadu_si128((__m128i*)&SrcC[i  ]);
    __m128i c_1 = _mm_loadu_si128((__m128i*)&SrcC[i+8]);

    //pack
    __m128i a = _mm_packus_epi16(a_0, a_1);
    __m128i b = _mm_packus_epi16(b_0, b_1);
    __m128i c = _mm_packus_epi16(c_0, c_1);

    //transpose
    __m128i ac_0 = _mm_unpacklo_epi8(a, c);
    __m128i ac_1 = _mm_unpackhi_epi8(a, c);
    __m128i bd_0 = _mm_unpacklo_epi8(b, d);
    __m128i bd_1 = _mm_unpackhi_epi8(b, d);

    __m128i abcd_0 = _mm_unpacklo_epi8(ac_0, bd_0);
    __m128i abcd_1 = _mm_unpackhi_epi8(ac_0, bd_0);
    __m128i abcd_2 = _mm_unpacklo_epi8(ac_1, bd_1);
    __m128i abcd_3 = _mm_unpackhi_epi8(ac_1, bd_1);

    //save
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+ 0], abcd_0);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+16], abcd_1);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+32], abcd_2);
    _mm_storeu_si128((__m128i*)&DstABCD[(i<<2)+48], abcd_3);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    uint8 a = (uint8)xClipU8(SrcA[i]);
    uint8 b = (uint8)xClipU8(SrcB[i]);
    uint8 c = (uint8)xClipU8(SrcC[i]);
    DstABCD[(i<<2)+0] = a;
    DstABCD[(i<<2)+1] = b;
    DstABCD[(i<<2)+2] = c;
    DstABCD[(i<<2)+3] = dd;
  }
}
void xPixelOpsSSE::ConvertAOS4toSOA3(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, const uint8* restrict SrcABCD, int32 Area)
{
  int32 Area16 = (int32)((uint32)Area & (uint32)c_MultipleMask16);
  __m128i Zero = _mm_setzero_si128();

  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m128i abcd_0 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+ 0]);
    __m128i abcd_1 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+16]);
    __m128i abcd_2 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+32]);
    __m128i abcd_3 = _mm_loadu_si128((__m128i*)&SrcABCD[(i<<2)+48]);

    //transpose
    __m128i aabbccdd_0 = _mm_unpacklo_epi8(abcd_0, abcd_1);
    __m128i aabbccdd_1 = _mm_unpackhi_epi8(abcd_0, abcd_1);
    __m128i aabbccdd_2 = _mm_unpacklo_epi8(abcd_2, abcd_3);
    __m128i aabbccdd_3 = _mm_unpackhi_epi8(abcd_2, abcd_3);

    __m128i a4b4c4d4_0 = _mm_unpacklo_epi8(aabbccdd_0, aabbccdd_1);
    __m128i a4b4c4d4_1 = _mm_unpackhi_epi8(aabbccdd_0, aabbccdd_1);
    __m128i a4b4c4d4_2 = _mm_unpacklo_epi8(aabbccdd_2, aabbccdd_3);
    __m128i a4b4c4d4_3 = _mm_unpackhi_epi8(aabbccdd_2, aabbccdd_3);

    __m128i a8b8_0 = _mm_unpacklo_epi8(a4b4c4d4_0, a4b4c4d4_1);
    __m128i c8d8_0 = _mm_unpackhi_epi8(a4b4c4d4_0, a4b4c4d4_1);
    __m128i a8b8_1 = _mm_unpacklo_epi8(a4b4c4d4_2, a4b4c4d4_3);
    __m128i c8d8_1 = _mm_unpackhi_epi8(a4b4c4d4_2, a4b4c4d4_3);

    __m128i a = _mm_unpacklo_epi64(a8b8_0, a8b8_1);
    __m128i b = _mm_unpackhi_epi64(a8b8_0, a8b8_1);
    __m128i c = _mm_unpacklo_epi64(c8d8_0, c8d8_1);

    //unpack
    __m128i a_0 = _mm_unpacklo_epi8(a, Zero);
    __m128i a_1 = _mm_unpackhi_epi8(a, Zero);
    __m128i b_0 = _mm_unpacklo_epi8(b, Zero);
    __m128i b_1 = _mm_unpackhi_epi8(b, Zero);
    __m128i c_0 = _mm_unpacklo_epi8(c, Zero);
    __m128i c_1 = _mm_unpackhi_epi8(c, Zero);

    //save
    _mm_storeu_si128((__m128i*)&DstA[i  ], a_0);
    _mm_storeu_si128((__m128i*)&DstA[i+8], a_1);
    _mm_storeu_si128((__m128i*)&DstB[i  ], b_0);
    _mm_storeu_si128((__m128i*)&DstB[i+8], b_1);
    _mm_storeu_si128((__m128i*)&DstC[i  ], c_0);
    _mm_storeu_si128((__m128i*)&DstC[i+8], c_1);
  }
  for(int32 i=Area16; i<Area; i++)
  {
    int16 a = SrcABCD[(i<<2)+0];
    int16 b = SrcABCD[(i<<2)+1];
    int16 c = SrcABCD[(i<<2)+2];
    DstA[i] = a;
    DstB[i] = b;
    DstC[i] = c;
  }
}

//=============================================================================================================================================================================
// Arithmetic --> shift
//=============================================================================================================================================================================
void xPixelOpsSSE::Shift(int16* restrict Dst, int16* restrict Src, int32 Area, int32 Shift)
{
  if(Shift==0) //no operation
  {
    if(Dst!=Src) ::memcpy(Dst, Src, Area*sizeof(int16));
  }
  else if(Shift>0) //bit depth increase
  {
    const int32 DeltaBits = Shift;
    int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);
    for(int32 i=0; i<Area8; i+=8)
    {
      __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[i]);
      __m128i DstV = _mm_slli_epi16(SrcV, DeltaBits);
      _mm_storeu_si128((__m128i*)&Dst[i], DstV);
    }
    for(int32 i=Area8; i<Area; i++)
    {
      Dst[i] = Src[i] << DeltaBits;
    }
  }
  else //Shift<0  //bit depth decrease
  {
    const int32 DeltaBits = -Shift;
    uint32 Offset   = 1<<(DeltaBits-1);
    __m128i OffsetV = _mm_set1_epi16(Offset);
    int32 Area8 = (int32)((uint32)Area & (uint32)c_MultipleMask8);
    for(int32 i=0; i<Area8; i+=8)
    {
      __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[i]);
      __m128i AddV = _mm_add_epi16(SrcV, OffsetV);
      __m128i DstV = _mm_srli_epi16(AddV, DeltaBits);
      _mm_storeu_si128((__m128i*)&Dst[i], DstV);
    }
    for(int32 i=Area8; i<Area; i++)
    {
      Dst[i] = (Src[i] + Offset) >> DeltaBits;
    }
  }
}
void xPixelOpsSSE::Shift(int16* restrict Dst, int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift)
{
  if(Shift==0) //no operation
  {
    if(Dst!=Src)
    {
      for(int32 y=0; y<Height; y++)
      {
        ::memcpy(Dst, Src, Width*sizeof(int16));
        Src += SrcStride;
        Dst += DstStride;  
      }
    }
  }
  else if(Shift>0) //bit depth increase
  {
    const int32 DeltaBits = Shift;
    if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
    {
      for(int32 y=0; y<Height; y++)
      {
        for(int32 x=0; x<Width; x+=8)
        {
          __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x]);
          __m128i DstV = _mm_slli_epi16(SrcV, DeltaBits);
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
          __m128i DstV = _mm_slli_epi16(SrcV, DeltaBits);
          _mm_storeu_si128((__m128i*)&Dst[x], DstV);
        }
        for(int32 x=Width8; x<Width4; x+=4)
        {
          __m128i SrcV  = _mm_loadl_epi64((__m128i*)&Src[x]);
          __m128i DstV = _mm_slli_epi16(SrcV, DeltaBits);
          _mm_storel_epi64((__m128i*)&Dst[x], DstV);
        }
        for(int32 x=Width4; x<Width; x++)
        {      
          Dst[x] = Src[x] << DeltaBits;
        }
        Src += SrcStride;
        Dst += DstStride;  
      }
    }
  }
  else //Shift<0  //bit depth decrease
  {
    const int32 DeltaBits = -Shift;
    uint32 Offset   = 1<<(DeltaBits-1);
    __m128i OffsetV = _mm_set1_epi16(Offset);
    if(((uint32)Width & (uint32)0x7)==0) //Width%8==0
    {
      for(int32 y=0; y<Height; y++)
      {
        for(int32 x=0; x<Width; x+=8)
        {
          __m128i SrcV = _mm_loadu_si128((__m128i*)&Src[x]);
          __m128i AddV = _mm_add_epi16(SrcV, OffsetV);
          __m128i DstV = _mm_srli_epi16(AddV, DeltaBits);
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
          __m128i AddV = _mm_add_epi16(SrcV, OffsetV);
          __m128i DstV = _mm_srli_epi16(AddV, DeltaBits);
          _mm_storeu_si128((__m128i*)&Dst[x], DstV);
        }
        for(int32 x=Width8; x<Width4; x+=4)
        {
          __m128i SrcV = _mm_loadl_epi64((__m128i*)&Src[x]);
          __m128i AddV = _mm_add_epi16(SrcV, OffsetV);
          __m128i DstV = _mm_srli_epi16(AddV, DeltaBits);
          _mm_storel_epi64((__m128i*)&Dst[x], DstV);
        }
        for(int32 x=Width4; x<Width; x++)
        {      
          Dst[x] = (Src[x] + Offset) >> DeltaBits;
        }
        Src += SrcStride;
        Dst += DstStride;  
      }
    }
  }
}

//=============================================================================================================================================================================

} //end of namespace AVLib

#endif //X_USE_SSE && X_SSE4_2
