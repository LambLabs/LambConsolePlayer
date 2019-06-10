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

class xPixelOpsSTD
{
public:
  //===============================================================================================================================================================================================================
  // Copy
  //===============================================================================================================================================================================================================
  template <typename PelType> static inline void Copy(PelType* restrict Dst, const PelType* restrict Src, int32 Area)
  {
    ::memcpy(Dst, Src, Area * sizeof(PelType));
  }
  template <typename PelType> static inline void Copy(PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height)
  {
    for(int32 y=0; y < Height; y++)
    {
      ::memcpy(Dst, Src, Width * sizeof(PelType));
      Src += SrcStride; Dst += DstStride;
    }
  }
  template <typename PelType> static inline void CopyPart(PelType* restrict Dst, const PelType* restrict Src, int32 DstStride, int32 SrcStride, int32V2 DstCoord, int32V2 SrcCoord, int32V2 Size)
  {
    for(int32 y=0; y < Size.getY(); y++)
    {
      const PelType* restrict SrcLine = Src + (SrcCoord.getY() + y) * SrcStride + SrcCoord.getX();
      PelType* restrict DstLine = Dst + (DstCoord.getY() + y) * DstStride + DstCoord.getX();
      ::memcpy(DstLine, SrcLine, Size.getX() * sizeof(PelType));
    }
  }

  //===============================================================================================================================================================================================================
  // Convert = Pack (with saturation) or Unpack
  //===============================================================================================================================================================================================================
  template<typename DstPelType, typename SrcPelType> static void Convert(DstPelType* restrict Dst, const SrcPelType* restrict Src, int32 Area);
  template<typename DstPelType, typename SrcPelType> static void Convert(DstPelType* restrict Dst, const SrcPelType* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height);

  //===============================================================================================================================================================================================================
  // Arrangement
  //===============================================================================================================================================================================================================
  // 2 components
  template <typename PelType> static void SOA2toAOS2(PelType* restrict DstAB, const PelType* restrict SrcA, const PelType* restrict SrcB, int32 Area);
  template <typename PelType> static void AOS2toSOA2(PelType* restrict DstA, PelType* restrict DstB, const PelType* restrict SrcAB, int32 Area);
  // 3 components
  template <typename PelType> static void SOA3toAOS3(PelType* restrict DstABC, const PelType* restrict SrcA, const PelType* restrict SrcB, const int16* restrict SrcC, int32 Area);
  template <typename PelType> static void AOS3toSOA3(PelType* restrict DstA, PelType* restrict DstB, PelType* restrict DstC, const int16* restrict SrcABC, int32 Area);
  // 4 components
  template <typename PelType> static void SOA4toAOS4(PelType* restrict DstABCD, const PelType* restrict SrcA, const PelType* restrict SrcB, const PelType* restrict SrcC, const PelType* restrict SrcD, int32 Area);
  template <typename PelType> static void SOA4toAOS4(PelType* restrict DstABCD, const PelType* restrict SrcA, const PelType* restrict SrcB, const PelType* restrict SrcC, const PelType* restrict SrcD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height);
  template <typename PelType> static void AOS4toSOA4(PelType* restrict DstA, PelType* restrict DstB, PelType* restrict DstC, PelType* restrict DstD, const PelType* restrict SrcABCD, int32 Area);
  template <typename PelType> static void AOS4toSOA4(PelType* restrict DstA, PelType* restrict DstB, PelType* restrict DstC, PelType* restrict DstD, const PelType* restrict SrcABCD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height);
  // partial 4 components
  template <typename PelType> static void SOA3toAOS4(PelType* restrict DstABCD, const PelType* restrict SrcA, const PelType* restrict SrcB, const PelType* restrict SrcC, PelType ValueD, int32 Area);
  template <typename PelType> static void SOA3toAOS4(PelType* restrict DstABCD, const PelType* restrict SrcA, const PelType* restrict SrcB, const PelType* restrict SrcC, PelType ValueD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height);
  template <typename PelType> static void AOS4toSOA3(PelType* restrict DstA, PelType* restrict DstB, PelType* restrict DstC, const PelType* restrict SrcABCD, int32 Area);
  template <typename PelType> static void AOS4toSOA3(PelType* restrict DstA, PelType* restrict DstB, PelType* restrict DstC, const PelType* restrict SrcABCD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height);

  //===============================================================================================================================================================================================================
  // Arrangement (crazy)
  //===============================================================================================================================================================================================================
  // 3(4) components with repetition (ABAC)
  template <typename PelType> static void SOA3toAOS4_ABAC(PelType* restrict DstABAC, const PelType* restrict SrcA, const PelType* restrict SrcB, const PelType* restrict SrcC, int32 Area);
  template <typename PelType> static void AOS4toSOA3_ABAC(PelType* restrict DstA, PelType* restrict DstB, PelType* restrict DstC, const PelType* restrict SrcABAC, int32 Area);
  // 3(4) components with repetition (BACA)
  template <typename PelType> static void SOA3toAOS4_BACA(PelType* restrict DstBACA, const PelType* restrict SrcA, const PelType* restrict SrcB, const PelType* restrict SrcC, int32 Area); 
  template <typename PelType> static void AOS4toSOA3_BACA(PelType* restrict DstA, PelType* restrict DstB, PelType* restrict DstC, const PelType* restrict SrcBACA, int32 Area);

  //===============================================================================================================================================================================================================
  // Arrangement + convertion
  //===============================================================================================================================================================================================================
  // 4 components
  template<typename DstPelType, typename SrcPelType> static void ConvertSOA4toAOS4(DstPelType* restrict DstABCD, const SrcPelType* restrict SrcA, const SrcPelType* restrict SrcB, const SrcPelType* restrict SrcC, const SrcPelType* restrict SrcD, int32 Area);
  template<typename DstPelType, typename SrcPelType> static void ConvertSOA4toAOS4(DstPelType* restrict DstABCD, const SrcPelType* restrict SrcA, const SrcPelType* restrict SrcB, const SrcPelType* restrict SrcC, const SrcPelType* restrict SrcD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height);
  template<typename DstPelType, typename SrcPelType> static void ConvertAOS4toSOA4(DstPelType* restrict DstA, DstPelType* restrict DstB, DstPelType* restrict DstC, DstPelType* restrict DstD, const SrcPelType* restrict SrcABCD, int32 Area);
  template<typename DstPelType, typename SrcPelType> static void ConvertAOS4toSOA4(DstPelType* restrict DstA, DstPelType* restrict DstB, DstPelType* restrict DstC, DstPelType* restrict DstD, const SrcPelType* restrict SrcABCD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height);
  // partial 4 components  
  template<typename DstPelType, typename SrcPelType> static void ConvertSOA3toAOS4(DstPelType* restrict DstABCD, const SrcPelType* restrict SrcA, const SrcPelType* restrict SrcB, const SrcPelType* restrict SrcC, SrcPelType ValueD, int32 Area);
  template<typename DstPelType, typename SrcPelType> static void ConvertSOA3toAOS4(DstPelType* restrict DstABCD, const SrcPelType* restrict SrcA, const SrcPelType* restrict SrcB, const SrcPelType* restrict SrcC, SrcPelType ValueD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height);
  template<typename DstPelType, typename SrcPelType> static void ConvertAOS4toSOA3(DstPelType* restrict DstA, DstPelType* restrict DstB, DstPelType* restrict DstC, const SrcPelType* restrict SrcABCD, int32 Area);
  template<typename DstPelType, typename SrcPelType> static void ConvertAOS4toSOA3(DstPelType* restrict DstA, DstPelType* restrict DstB, DstPelType* restrict DstC, const SrcPelType* restrict SrcABCD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height);

  //===============================================================================================================================================================================================================
  // Arithmetic
  //===============================================================================================================================================================================================================
  //template <typename PelType> static void   AddSat        (PelType* Dst, PelType* Src0, PelType* Src1, int32 Area);
  //template <typename PelType> static void   AddSat        (PelType* Dst, PelType* Src0, PelType* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height);
  //template <typename PelType> static void   AddAndClipU   (PelType* Dst, PelType* Src0, PelType* Src1, int32 Area, PelType ClippingRange);
  //template <typename PelType> static void   AddAndClipU   (PelType* Dst, PelType* Src0, PelType* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, PelType ClippingRange);
  //template <typename PelType> static void   SubSat        (PelType* Dst, PelType* Src0, PelType* Src1, int32 Area);
  //template <typename PelType> static void   SubSat        (PelType* Dst, PelType* Src0, PelType* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height);
  //template <typename PelType> static void   MulConst      (PelType* Dst, PelType* Src, int32 Area, const PelType Multiplier);
  //template <typename PelType> static void   MulConst      (PelType* Dst, PelType* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const PelType Multiplier);
  //template <typename PelType> static void   BiPredRefEst  (PelType* Dst, PelType* Org, PelType* Prd0, int32 Area);
  //template <typename PelType> static void   BiPredRefEst  (PelType* Dst, PelType* Org, PelType* Prd0, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Width, int32 Height);
  //template <typename PelType> static void   QuadPredRefEst(PelType* Dst, PelType* Org, PelType* Prd0, PelType* Prd1, PelType* Prd2, int32 Area);
  //template <typename PelType> static void   QuadPredRefEst(PelType* Dst, PelType* Org, PelType* Prd0, PelType* Prd1, PelType* Prd2, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Prd1Stride, int32 Prd2Stride, int32 Width, int32 Height);
  //template <typename PelType> static void   Avg           (PelType* Dst, PelType* Src0, PelType* Src1, int32 Area);
  //template <typename PelType> static void   Avg           (PelType* Dst, PelType* Src0, PelType* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height);
  //template <typename PelType> static void   AvgAndClipU   (PelType* Dst, PelType* Src0, PelType* Src1, int32 Area, PelType ClippingRange);
  //template <typename PelType> static void   AvgAndClipU   (PelType* Dst, PelType* Src0, PelType* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, PelType ClippingRange);
  //template <typename PelType> static void   Clip          (PelType* Dst, PelType* Src, int32 Area, PelType ClippingRangeLow, PelType ClippingRangeHigh);
  //template <typename PelType> static void   Clip          (PelType* Dst, PelType* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, PelType ClippingRangeLow, PelType ClippingRangeHigh);
  template <typename PelType> static void   Shift         (PelType* Dst, PelType* Src, int32 Area, int32 Shift);
  template <typename PelType> static void   Shift         (PelType* Dst, PelType* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift);
  //template <typename PelType> static void   Threshold     (PelType* Dst, PelType* Src, int32 Area, const PelType Threshold);
  //template <typename PelType> static uint32 CountNonZero  (PelType* Src, int32 Area);
  //template <typename PelType> static int32  SumAll        (PelType* Src, int32 Area);
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Pack (with saturation) + Unpack
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename DstPelType, typename SrcPelType> void xPixelOpsSTD::Convert(DstPelType* restrict Dst, const SrcPelType* restrict Src, int32 Area)
{
  if constexpr(std::is_same_v<DstPelType, SrcPelType>) { Copy(Dst, Src, Area); return; }

  for(int32 i=0; i < Area; i++)
  {
    Dst[i] = xConvertSaturateRoundType<DstPelType, SrcPelType>(Src[i]);
  }
}
template<typename DstPelType, typename SrcPelType> void xPixelOpsSTD::Convert(DstPelType* restrict Dst, const SrcPelType* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height)
{
  if constexpr(std::is_same_v<DstPelType, SrcPelType>) { Copy(Dst, Src, DstStride, SrcStride, Width, Height); return; }

  for(int32 y=0; y < Height; y++) 
  { 
    for(int32 x=0; x < Width; x++)
    {
      Dst[x] = xConvertSaturateRoundType<DstPelType, SrcPelType>(Src[x]); 
    }
    Src += SrcStride;
    Dst += DstStride;
  }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Arrangement
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename PelType> void xPixelOpsSTD::SOA2toAOS2(PelType* restrict DstAB, const PelType* restrict SrcA, const PelType* restrict SrcB, int32 Area)
{
  for(int32 i=0; i<Area; i++)
  {
    PelType a = SrcA[i];
    PelType b = SrcB[i];
    DstAB[(i<<1)+0] = a;
    DstAB[(i<<1)+1] = b;
  }
}
template <typename PelType> void xPixelOpsSTD::AOS2toSOA2(PelType* restrict DstA, PelType* restrict DstB, const PelType* restrict SrcAB, int32 Area)
{
  for(int32 i=0; i<Area; i++)
  {
    PelType a = SrcAB[(i<<1)+0];
    PelType b = SrcAB[(i<<1)+1];
    DstA[i] = a;
    DstB[i] = b;
  }
}
template <typename PelType> void xPixelOpsSTD::SOA3toAOS3(PelType* restrict DstABC, const PelType* restrict SrcA, const PelType* restrict SrcB, const int16* restrict SrcC, int32 Area)
{
  for(int32 i=0, j=0; i<Area; i++, j+=3)
  {
    PelType a = SrcA[i];
    PelType b = SrcB[i];
    PelType c = SrcC[i];
    DstABC[j+0] = a;
    DstABC[j+1] = b;
    DstABC[j+2] = c;
  }
}
template <typename PelType> void xPixelOpsSTD::AOS3toSOA3(PelType* restrict DstA, PelType* restrict DstB, PelType* restrict DstC, const int16* restrict SrcABC, int32 Area)
{
  for(int32 i=0, j=0; i<Area; i++, j+=3)
  {
    PelType a = SrcABC[j+0];
    PelType b = SrcABC[j+1];
    PelType c = SrcABC[j+2];
    DstA[i] = a;
    DstB[i] = b;
    DstC[i] = c;
  }
}
template <typename PelType> void xPixelOpsSTD::SOA4toAOS4(PelType* restrict DstABCD, const PelType* restrict SrcA, const PelType* restrict SrcB, const PelType* restrict SrcC, const PelType* restrict SrcD, int32 Area)
{
  for(int32 i=0; i<Area; i++)
  {
    PelType a = SrcA[i];
    PelType b = SrcB[i];
    PelType c = SrcC[i];
    PelType d = SrcD[i];
    DstABCD[(i<<2)+0] = a;
    DstABCD[(i<<2)+1] = b;
    DstABCD[(i<<2)+2] = c;
    DstABCD[(i<<2)+3] = d;
  }

}
template <typename PelType> void xPixelOpsSTD::SOA4toAOS4(PelType* restrict DstABCD, const PelType* restrict SrcA, const PelType* restrict SrcB, const PelType* restrict SrcC, const PelType* restrict SrcD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height)
{
  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {      
      PelType a = SrcA[x];
      PelType b = SrcB[x];
      PelType c = SrcC[x];
      PelType d = SrcD[x];
      DstABCD[(x<<2)+0] = a;
      DstABCD[(x<<2)+1] = b;
      DstABCD[(x<<2)+2] = c;
      DstABCD[(x<<2)+3] = d;
    }
    SrcA    += SrcStride;
    SrcB    += SrcStride;
    SrcC    += SrcStride;
    SrcD    += SrcStride;
    DstABCD += DstStride;  
  }
}
template <typename PelType> void xPixelOpsSTD::AOS4toSOA4(PelType* restrict DstA, PelType* restrict DstB, PelType* restrict DstC, PelType* restrict DstD, const PelType* restrict SrcABCD, int32 Area)
{
  for(int32 i=0; i<Area; i++)
  {
    PelType a = SrcABCD[(i<<2)+0];
    PelType b = SrcABCD[(i<<2)+1];
    PelType c = SrcABCD[(i<<2)+2];
    PelType d = SrcABCD[(i<<2)+3];
    DstA[i] = a;
    DstB[i] = b;
    DstC[i] = c;
    DstD[i] = d;
  }
}
template <typename PelType> void xPixelOpsSTD::AOS4toSOA4(PelType* restrict DstA, PelType* restrict DstB, PelType* restrict DstC, PelType* restrict DstD, const PelType* restrict SrcABCD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height)
{
  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {      
      PelType a = SrcABCD[(x<<2)+0];
      PelType b = SrcABCD[(x<<2)+1];
      PelType c = SrcABCD[(x<<2)+2];
      PelType d = SrcABCD[(x<<2)+3];
      DstA[x] = a;
      DstB[x] = b;
      DstC[x] = c;
      DstD[x] = d;
    }
    SrcABCD += SrcStride;
    DstA    += DstStride;
    DstB    += DstStride;
    DstC    += DstStride;
    DstD    += DstStride;
  }
}
template <typename PelType> void xPixelOpsSTD::SOA3toAOS4(PelType* restrict DstABCD, const PelType* restrict SrcA, const PelType* restrict SrcB, const PelType* restrict SrcC, PelType ValueD, int32 Area)
{
  for(int32 i=0; i<Area; i++)
  {
    PelType a = SrcA[i];
    PelType b = SrcB[i];
    PelType c = SrcC[i];
    DstABCD[(i<<2)+0] = a;
    DstABCD[(i<<2)+1] = b;
    DstABCD[(i<<2)+2] = c;
    DstABCD[(i<<2)+3] = ValueD;
  }
}
template <typename PelType> void xPixelOpsSTD::SOA3toAOS4(PelType* restrict DstABCD, const PelType* restrict SrcA, const PelType* restrict SrcB, const PelType* restrict SrcC, PelType ValueD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height)
{
  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {      
      PelType a = SrcA[x];
      PelType b = SrcB[x];
      PelType c = SrcC[x];
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
template <typename PelType> void xPixelOpsSTD::AOS4toSOA3(PelType* restrict DstA, PelType* restrict DstB, PelType* restrict DstC, const PelType* restrict SrcABCD, int32 Area)
{
  for(int32 i=0; i<Area; i++)
  {
    PelType a = SrcABCD[(i<<2)+0];
    PelType b = SrcABCD[(i<<2)+1];
    PelType c = SrcABCD[(i<<2)+2];
    DstA[i] = a;
    DstB[i] = b;
    DstC[i] = c;
  }
}
template <typename PelType> void xPixelOpsSTD::AOS4toSOA3(PelType* restrict DstA, PelType* restrict DstB, PelType* restrict DstC, const PelType* restrict SrcABCD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height)
{
  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {      
      PelType a = SrcABCD[(x<<2)+0];
      PelType b = SrcABCD[(x<<2)+1];
      PelType c = SrcABCD[(x<<2)+2];
      DstA[x] = a;
      DstB[x] = b;
      DstC[x] = c;
    }
    SrcABCD += SrcStride;
    DstA    += DstStride;
    DstB    += DstStride;
    DstC    += DstStride;
  }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Arrangement (crazy)
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename PelType> void xPixelOpsSTD::SOA3toAOS4_ABAC(PelType* restrict DstABAC, const PelType* restrict SrcA, const PelType* restrict SrcB, const PelType* restrict SrcC, int32 Area)
{
  for (int32 i = 0; i < Area; i++)
  {
    PelType a1 = SrcA[i<<1      ];
    PelType b  = SrcB[i         ];
    PelType a2 = SrcA[(i<<1) + 1];
    PelType c  = SrcC[i         ];
    DstABAC[(i<<2) + 0] = a1;
    DstABAC[(i<<2) + 1] = b;
    DstABAC[(i<<2) + 2] = a2;
    DstABAC[(i<<2) + 3] = c;
  }
}
template <typename PelType> void xPixelOpsSTD::AOS4toSOA3_ABAC(PelType* restrict DstA, PelType* restrict DstB, PelType* restrict DstC, const PelType* restrict SrcABAC, int32 Area)
{
  for (int32 i = 0; i < Area; i++)
  {
    PelType a1 = SrcABAC[(i << 2) + 0];
    PelType b  = SrcABAC[(i << 2) + 1];
    PelType a2 = SrcABAC[(i << 2) + 2];
    PelType c  = SrcABAC[(i << 2) + 3];
    DstA[i<<1      ] = a1;
    DstB[i         ] = b;
    DstA[(i<<1) + 1] = a2;
    DstC[i         ] = c;
  }
}
template <typename PelType> void xPixelOpsSTD::SOA3toAOS4_BACA(PelType* restrict DstBACA, const PelType* restrict SrcA, const PelType* restrict SrcB, const PelType* restrict SrcC, int32 Area)
{
  for (int32 i = 0; i < Area; i++)
  {
    PelType b  = SrcB[i         ];
    PelType a1 = SrcA[i<<1      ];
    PelType c  = SrcC[i         ];
    PelType a2 = SrcA[(i<<1) + 1];    
    DstBACA[(i<<2) + 0] = b;
    DstBACA[(i<<2) + 1] = a1;
    DstBACA[(i<<2) + 2] = c;
    DstBACA[(i<<2) + 3] = a2;
  }
}
template <typename PelType> void xPixelOpsSTD::AOS4toSOA3_BACA(PelType* restrict DstA, PelType* restrict DstB, PelType* restrict DstC, const PelType* restrict SrcBACA, int32 Area)
{
  for (int32 i = 0; i < Area; i++)
  {
    PelType b  = SrcBACA[(i << 2) + 0];
    PelType a1 = SrcBACA[(i << 2) + 1];
    PelType c  = SrcBACA[(i << 2) + 2];
    PelType a2 = SrcBACA[(i << 2) + 3];
    DstB[i         ] = b;
    DstA[i<<1      ] = a1;
    DstC[i         ] = c;
    DstA[(i<<1) + 1] = a2;    
  }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Arrangement combined with (Pack (with saturation) + Unpack)
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename DstPelType, typename SrcPelType> void xPixelOpsSTD::ConvertSOA4toAOS4(DstPelType* restrict DstABCD, const SrcPelType* restrict SrcA, const SrcPelType* restrict SrcB, const SrcPelType* restrict SrcC, const SrcPelType* restrict SrcD, int32 Area)
{
  if constexpr(std::is_same_v<DstPelType, SrcPelType>) { SOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); return; }

  for(int32 i=0; i<Area; i++)
  {
    DstPelType a = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcA[i]);
    DstPelType b = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcB[i]);
    DstPelType c = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcC[i]);
    DstPelType d = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcD[i]);
    DstABCD[(i<<2)+0] = a;
    DstABCD[(i<<2)+1] = b;
    DstABCD[(i<<2)+2] = c;
    DstABCD[(i<<2)+3] = d;
  }
}
template<typename DstPelType, typename SrcPelType> void xPixelOpsSTD::ConvertSOA4toAOS4(DstPelType* restrict DstABCD, const SrcPelType* restrict SrcA, const SrcPelType* restrict SrcB, const SrcPelType* restrict SrcC, const SrcPelType* restrict SrcD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height)
{
  if constexpr(std::is_same_v<DstPelType, SrcPelType>) { SOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, DstStride, SrcStride, Width, Height); return; }

  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {      
      DstPelType a = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcA[x]);
      DstPelType b = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcB[x]);
      DstPelType c = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcC[x]);
      DstPelType d = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcD[x]);
      DstABCD[(x<<2)+0] = a;
      DstABCD[(x<<2)+1] = b;
      DstABCD[(x<<2)+2] = c;
      DstABCD[(x<<2)+3] = d;
    }
    SrcA    += SrcStride;
    SrcB    += SrcStride;
    SrcC    += SrcStride;
    SrcD    += SrcStride;
    DstABCD += DstStride;  
  }
}
template<typename DstPelType, typename SrcPelType> void xPixelOpsSTD::ConvertAOS4toSOA4(DstPelType* restrict DstA, DstPelType* restrict DstB, DstPelType* restrict DstC, DstPelType* restrict DstD, const SrcPelType* restrict SrcABCD, int32 Area)
{
  if constexpr(std::is_same_v<DstPelType, SrcPelType>) { AOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); return; }

  for(int32 i=0; i<Area; i++)
  {
    DstPelType a = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcABCD[(i<<2)+0]);
    DstPelType b = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcABCD[(i<<2)+1]);
    DstPelType c = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcABCD[(i<<2)+2]);
    DstPelType d = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcABCD[(i<<2)+3]);
    DstA[i] = a;
    DstB[i] = b;
    DstC[i] = c;
    DstD[i] = d;
  }
}
template<typename DstPelType, typename SrcPelType> void xPixelOpsSTD::ConvertAOS4toSOA4(DstPelType* restrict DstA, DstPelType* restrict DstB, DstPelType* restrict DstC, DstPelType* restrict DstD, const SrcPelType* restrict SrcABCD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height)
{
  if constexpr(std::is_same_v<DstPelType, SrcPelType>) { AOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, DstStride, SrcStride, Width, Height); return; }

  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {      
      DstPelType a = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcABCD[(x<<2)+0]);
      DstPelType b = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcABCD[(x<<2)+1]);
      DstPelType c = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcABCD[(x<<2)+2]);
      DstPelType d = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcABCD[(x<<2)+3]);
      DstA[x] = a;
      DstB[x] = b;
      DstC[x] = c;
      DstD[x] = d;
    }
    SrcABCD += SrcStride;
    DstA    += DstStride;
    DstB    += DstStride;
    DstC    += DstStride;
    DstD    += DstStride;
  }
}
template<typename DstPelType, typename SrcPelType> void xPixelOpsSTD::ConvertSOA3toAOS4(DstPelType* restrict DstABCD, const SrcPelType* restrict SrcA, const SrcPelType* restrict SrcB, const SrcPelType* restrict SrcC, SrcPelType ValueD, int32 Area)
{
  if constexpr(std::is_same_v<DstPelType, SrcPelType>) { SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); return; }

  DstPelType d = xConvertSaturateRoundType<DstPelType, SrcPelType>(ValueD);

  for(int32 i=0; i<Area; i++)
  {
    DstPelType a = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcA[i]);
    DstPelType b = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcB[i]);
    DstPelType c = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcC[i]);
    DstABCD[(i<<2)+0] = a;
    DstABCD[(i<<2)+1] = b;
    DstABCD[(i<<2)+2] = c;
    DstABCD[(i<<2)+3] = d;
  }
}
template<typename DstPelType, typename SrcPelType> void xPixelOpsSTD::ConvertSOA3toAOS4(DstPelType* restrict DstABCD, const SrcPelType* restrict SrcA, const SrcPelType* restrict SrcB, const SrcPelType* restrict SrcC, SrcPelType ValueD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height)
{
  if constexpr(std::is_same_v<DstPelType, SrcPelType>) { SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, DstStride, SrcStride, Width, Height); return; }

  DstPelType d = xConvertSaturateRoundType<DstPelType, SrcPelType>(ValueD);

  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {      
      DstPelType a = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcA[x]);
      DstPelType b = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcB[x]);
      DstPelType c = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcC[x]);
      DstABCD[(x<<2)+0] = a;
      DstABCD[(x<<2)+1] = b;
      DstABCD[(x<<2)+2] = c;
      DstABCD[(x<<2)+3] = d;
    }
    SrcA    += SrcStride;
    SrcB    += SrcStride;
    SrcC    += SrcStride;
    DstABCD += DstStride;  
  }
}
template<typename DstPelType, typename SrcPelType> void xPixelOpsSTD::ConvertAOS4toSOA3(DstPelType* restrict DstA, DstPelType* restrict DstB, DstPelType* restrict DstC, const SrcPelType* restrict SrcABCD, int32 Area)
{
  if constexpr(std::is_same_v<DstPelType, SrcPelType>) { AOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); return; }

  for(int32 i=0; i<Area; i++)
  {
    DstPelType a = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcABCD[(i<<2)+0]);
    DstPelType b = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcABCD[(i<<2)+1]);
    DstPelType c = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcABCD[(i<<2)+2]);
    DstA[i] = a;
    DstB[i] = b;
    DstC[i] = c;
  }
}
template<typename DstPelType, typename SrcPelType> void xPixelOpsSTD::ConvertAOS4toSOA3(DstPelType* restrict DstA, DstPelType* restrict DstB, DstPelType* restrict DstC, const SrcPelType* restrict SrcABCD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height)
{
  if constexpr(std::is_same_v<DstPelType, SrcPelType>) { AOS4toSOA3(DstA, DstB, DstC, SrcABCD, DstStride, SrcStride, Width, Height); return; }

  for(int32 y=0; y<Height; y++)
  {
    for(int32 x=0; x<Width; x++)
    {      
      DstPelType a = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcABCD[(x<<2)+0]);
      DstPelType b = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcABCD[(x<<2)+1]);
      DstPelType c = xConvertSaturateRoundType<DstPelType, SrcPelType>(SrcABCD[(x<<2)+2]);
      DstA[x] = a;
      DstB[x] = b;
      DstC[x] = c;
    }
    SrcABCD += SrcStride;
    DstA    += DstStride;
    DstB    += DstStride;
    DstC    += DstStride;
  }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Shift
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename PelType> void xPixelOpsSTD::Shift(PelType* Dst, PelType* Src, int32 Area, int32 Shift)
{
  if(Shift==0) //no operation
  {
    if(Dst!=Src) ::memcpy(Dst, Src, Area*sizeof(int16));
  }
  else if(Shift>0) //bit depth increase
  {
    const int32 DeltaBits = Shift;
    for(int32 i=0; i<Area; i++)
    {
      Dst[i] = Src[i] << DeltaBits;
    }
  }
  else //Shift<0  //bit depth decrease
  {
    const int32 DeltaBits = -Shift;
    uint32 Offset = 1<<(DeltaBits-1);
    for(int32 i=0; i<Area; i++)
    {
      Dst[i] = (Src[i] + Offset) >> DeltaBits;
    }
  }
}
template <typename PelType> void xPixelOpsSTD::Shift(PelType* Dst, PelType* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift) 
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
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x++)
      {      
        Dst[x] = Src[x] << DeltaBits;
      }
      Src += SrcStride;
      Dst += DstStride;  
    }
  }
  else //Shift<0  //bit depth decrease
  {
    const int32 DeltaBits = -Shift;
    uint32 Offset = 1<<(DeltaBits-1);
    for(int32 y=0; y<Height; y++)
    {
      for(int32 x=0; x<Width; x++)
      {      
        Dst[x] = (Src[x] + Offset) >> DeltaBits;
      }
      Src += SrcStride;
      Dst += DstStride;  
    }
  }
}


//===============================================================================================================================================================================================================

} //end of namespace AVLib
