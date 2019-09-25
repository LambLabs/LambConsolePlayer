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
#include "xPixelOpsArithm.h"

#include "xPixelOpsSTD.h"
#if X_USE_SSE && X_SSE4_2
#include "xPixelOpsSSE.h"
#endif
#if X_USE_AVX && X_AVX1 && X_AVX2
#include "xPixelOpsAVX.h"
#endif

namespace AVlib {

//===============================================================================================================================================================================================================

class xPixelOps : public xPixelOpsArithm
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
    for(int32 y=0; y<Height; y++)
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
  // Margin
  //===============================================================================================================================================================================================================
  template <typename PelType> static inline void ExtendMarginSolid (PelType* restrict Addr, int32 Stride, int32 Width, int32 Height, int32 Margin, PelType Color);
  template <typename PelType> static inline void ExtendMarginEdge  (PelType* restrict Addr, int32 Stride, int32 Width, int32 Height, int32 Margin);
  template <typename PelType> static inline void ExtendMarginMirror(PelType* restrict Addr, int32 Stride, int32 Width, int32 Height, int32 Margin);
  template <typename PelType> static inline void ExtendMarginBayer (PelType* restrict Addr, int32 Stride, int32 Width, int32 Height, int32 Margin);
  template <typename PelType> static inline void ClearMargin       (PelType* restrict Addr, int32 Stride, int32 Width, int32 Height, int32 Margin);

  //===============================================================================================================================================================================================================
  // Convert = Pack (with saturation) or Unpack
  //===============================================================================================================================================================================================================
  template<typename DstPelType, typename SrcPelType> static inline void Convert(DstPelType* Dst, const SrcPelType* Src, int32 Area) { xPixelOpsSTD::Convert(Dst, Src, Area); }
  template<typename DstPelType, typename SrcPelType> static inline void Convert(DstPelType* Dst, const SrcPelType* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSTD::Convert(Dst, Src, DstStride, SrcStride, Width, Height); }

// moved outside class due to bug 85282 in gcc (CWG 727)
//#if X_USE_AVX && X_AVX2
//  template <> inline void Convert(uint8* Dst, const int16* Src, int32 Area) { xPixelOpsAVX::Convert(Dst, Src, Area); }
//  template <> inline void Convert(int16* Dst, const uint8* Src, int32 Area) { xPixelOpsAVX::Convert(Dst, Src, Area); }
//  template <> inline void Convert(uint8* Dst, const int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsAVX::Convert(Dst, Src, DstStride, SrcStride, Width, Height); }
//  template <> inline void Convert(int16* Dst, const uint8* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsAVX::Convert(Dst, Src, DstStride, SrcStride, Width, Height); }
//#elif X_USE_SSE && X_SSE4_2
//  template <> inline void Convert(uint8* Dst, const int16* Src, int32 Area) { xPixelOpsSSE::Convert(Dst, Src, Area); }
//  template <> inline void Convert(int16* Dst, const uint8* Src, int32 Area) { xPixelOpsSSE::Convert(Dst, Src, Area); }
//  template <> inline void Convert(uint8* Dst, const int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSSE::Convert(Dst, Src, DstStride, SrcStride, Width, Height); }
//  template <> inline void Convert(int16* Dst, const uint8* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSSE::Convert(Dst, Src, DstStride, SrcStride, Width, Height); }
//#endif

  //===============================================================================================================================================================================================================
  // Arrangement
  //===============================================================================================================================================================================================================
  // 2 components
  template <typename PelType> static inline void SOA2toAOS2(PelType* DstAB, const PelType* SrcA, const PelType* SrcB, int32 Area) { xPixelOpsSTD::SOA2toAOS2(DstAB, SrcA, SrcB, Area); }
  template <typename PelType> static inline void AOS2toSOA2(PelType* DstA, PelType* DstB, const PelType* SrcAB, int32 Area) { xPixelOpsSTD::AOS2toSOA2(DstA, DstB, SrcAB, Area); }
  // 3 components
  template <typename PelType> static inline void SOA3toAOS3(PelType* DstABC, const PelType* SrcA, const PelType* SrcB, const int16* SrcC, int32 Area) { xPixelOpsSTD::SOA2toAOS2(DstABC, SrcA, SrcB, SrcC, Area); }
  template <typename PelType> static inline void AOS3toSOA3(PelType* DstA, PelType* DstB, PelType* DstC, const int16* SrcABC, int32 Area) { xPixelOpsSTD::AOS2toSOA2(DstA, DstB, DstC, SrcABC, Area); }
  // 4 components
  template <typename PelType> static inline void SOA4toAOS4(PelType* DstABCD, const PelType* SrcA, const PelType* SrcB, const PelType* SrcC, const PelType* SrcD, int32 Area) { xPixelOpsSTD::SOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); }
  template <typename PelType> static inline void SOA4toAOS4(PelType* DstABCD, const PelType* SrcA, const PelType* SrcB, const PelType* SrcC, const PelType* SrcD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSTD::SOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, DstStride, SrcStride, Width, Height); }
  template <typename PelType> static inline void AOS4toSOA4(PelType* DstA, PelType* DstB, PelType* DstC, PelType* DstD, const PelType* SrcABCD, int32 Area) { xPixelOpsSTD::AOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); }
  template <typename PelType> static inline void AOS4toSOA4(PelType* DstA, PelType* DstB, PelType* DstC, PelType* DstD, const PelType* SrcABCD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSTD::AOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, DstStride, SrcStride, Width, Height); }
  // partial 4 components
  template <typename PelType> static inline void SOA3toAOS4(PelType* DstABCD, const PelType* SrcA, const PelType* SrcB, const PelType* SrcC, PelType ValueD, int32 Area) { xPixelOpsSTD::SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); }
  template <typename PelType> static inline void SOA3toAOS4(PelType* DstABCD, const PelType* SrcA, const PelType* SrcB, const PelType* SrcC, PelType ValueD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSTD::SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, DstStride, SrcStride, Width, Height); }
  template <typename PelType> static inline void AOS4toSOA3(PelType* DstA, PelType* DstB, PelType* DstC, const PelType* SrcABCD, int32 Area) { xPixelOpsSTD::AOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); }
  template <typename PelType> static inline void AOS4toSOA3(PelType* DstA, PelType* DstB, PelType* DstC, const PelType* SrcABCD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSTD::AOS4toSOA3(DstA, DstB, DstC, SrcABCD, DstStride, SrcStride, Width, Height); }

// moved outside class due to bug 85282 in gcc (CWG 727)
//#if X_USE_AVX && X_AVX1 && X_AVX2
//  template <> inline void SOA2toAOS2(uint8* restrict DstAB, const uint8* restrict SrcA, const uint8* restrict SrcB, int32 Area) { xPixelOpsAVX::SOA2toAOS2(DstAB, SrcA, SrcB, Area); }
//  template <> inline void AOS2toSOA2(uint8* restrict DstA, uint8* restrict DstB, const uint8* restrict SrcAB, int32 Area) { xPixelOpsAVX::AOS2toSOA2(DstA, DstB, SrcAB, Area); }
//  template <> inline void SOA2toAOS2(int16* restrict DstAB, const int16* restrict SrcA, const int16* restrict SrcB, int32 Area) { xPixelOpsAVX::SOA2toAOS2(DstAB, SrcA, SrcB, Area); }
//  template <> inline void AOS2toSOA2(int16* restrict DstA, int16* restrict DstB, const int16* restrict SrcAB, int32 Area) { xPixelOpsAVX::AOS2toSOA2(DstA, DstB, SrcAB, Area); }  
//
//  template <> inline void SOA4toAOS4(uint8* restrict DstABCD, const uint8* restrict SrcA, const uint8* restrict SrcB, const uint8* restrict SrcC, const uint8* restrict SrcD, int32 Area) { xPixelOpsAVX::SOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); }
//  template <> inline void AOS4toSOA4(uint8* restrict DstA, uint8* restrict DstB, uint8* restrict DstC, uint8* restrict DstD, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsAVX::AOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); }
//  template <> inline void SOA4toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16* restrict SrcD, int32 Area) { xPixelOpsAVX::SOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); }
//  template <> inline void AOS4toSOA4(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, int16* restrict DstD, const int16* restrict SrcABCD, int32 Area) { xPixelOpsAVX::AOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); }
//
//  template <> inline void SOA3toAOS4(uint8* restrict DstABCD, const uint8* restrict SrcA, const uint8* restrict SrcB, const uint8* restrict SrcC, uint8 ValueD, int32 Area) { xPixelOpsAVX::SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); }
//  template <> inline void AOS4toSOA3(uint8* restrict DstA, uint8* restrict DstB, uint8* restrict DstC, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsAVX::AOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); }
//  template <> inline void SOA3toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 Area) { xPixelOpsAVX::SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); }
//  template <> inline void SOA3toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsAVX::SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, DstStride, SrcStride, Width, Height); }
//  template <> inline void AOS4toSOA3(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, const int16* restrict SrcABCD, int32 Area) { xPixelOpsAVX::AOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); }
//#elif X_USE_SSE && X_SSE4_2
//  template <> inline void SOA2toAOS2(uint8* restrict DstAB, const uint8* restrict SrcA, const uint8* restrict SrcB, int32 Area) { xPixelOpsSSE::SOA2toAOS2(DstAB, SrcA, SrcB, Area); }
//  template <> inline void AOS2toSOA2(uint8* restrict DstA, uint8* restrict DstB, const uint8* restrict SrcAB, int32 Area) { xPixelOpsSSE::AOS2toSOA2(DstA, DstB, SrcAB, Area); }
//  template <> inline void SOA2toAOS2(int16* restrict DstAB, const int16* restrict SrcA, const int16* restrict SrcB, int32 Area) { xPixelOpsSSE::SOA2toAOS2(DstAB, SrcA, SrcB, Area); }
//  template <> inline void AOS2toSOA2(int16* restrict DstA, int16* restrict DstB, const int16* restrict SrcAB, int32 Area) { xPixelOpsSSE::AOS2toSOA2(DstA, DstB, SrcAB, Area); }  
//
//  template <> inline void SOA4toAOS4(uint8* restrict DstABCD, const uint8* restrict SrcA, const uint8* restrict SrcB, const uint8* restrict SrcC, const uint8* restrict SrcD, int32 Area) { xPixelOpsSSE::SOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); }
//  template <> inline void AOS4toSOA4(uint8* restrict DstA, uint8* restrict DstB, uint8* restrict DstC, uint8* restrict DstD, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsSSE::AOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); }
//  template <> inline void SOA4toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16* restrict SrcD, int32 Area) { xPixelOpsSSE::SOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); }
//  template <> inline void AOS4toSOA4(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, int16* restrict DstD, const int16* restrict SrcABCD, int32 Area) { xPixelOpsSSE::AOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); }
//
//  template <> inline void SOA3toAOS4(uint8* restrict DstABCD, const uint8* restrict SrcA, const uint8* restrict SrcB, const uint8* restrict SrcC, uint8 ValueD, int32 Area) { xPixelOpsSSE::SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); }
//  template <> inline void AOS4toSOA3(uint8* restrict DstA, uint8* restrict DstB, uint8* restrict DstC, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsSSE::AOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); }
//  template <> inline void SOA3toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 Area) { xPixelOpsSSE::SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); }
//  template <> inline void SOA3toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSSE::SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, DstStride, SrcStride, Width, Height); }
//  template <> inline void AOS4toSOA3(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, const int16* restrict SrcABCD, int32 Area) { xPixelOpsSSE::AOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); }
//#endif

  //===============================================================================================================================================================================================================
  // Arrangement (crazy)
  //===============================================================================================================================================================================================================
  // 3(4) components with repetition (ABAC)
  template <typename PelType> static inline void SOA3toAOS4_ABAC(PelType* DstABAC, const PelType* SrcA, const PelType* SrcB, const PelType* SrcC, int32 Area) { xPixelOpsSTD::SOA3toAOS4_ABAC(DstABAC, SrcA, SrcB, SrcC, Area); }
  template <typename PelType> static inline void AOS4toSOA3_ABAC(PelType* DstA, PelType* DstB, PelType* DstC, const PelType* SrcABAC, int32 Area) { xPixelOpsSTD::AOS4toSOA3_ABAC(DstA, DstB, DstC, SrcABAC, Area); }
  // 3(4) components with repetition (BACA)
  template <typename PelType> static inline void SOA3toAOS4_BACA(PelType* DstBACA, const PelType* SrcA, const PelType* SrcB, const PelType* SrcC, int32 Area) { xPixelOpsSTD::SOA3toAOS4_BACA(DstBACA, SrcA, SrcB, SrcC, Area); }
  template <typename PelType> static inline void AOS4toSOA3_BACA(PelType* DstA, PelType* DstB, PelType* DstC, const PelType* SrcBACA, int32 Area) { xPixelOpsSTD::AOS4toSOA3_BACA(DstA, DstB, DstC, SrcBACA, Area); }

  //===============================================================================================================================================================================================================
  // Arrangement + convertion
  //===============================================================================================================================================================================================================
  // 4 components
  template<typename DstPelType, typename SrcPelType> static inline void ConvertSOA4toAOS4(DstPelType* DstABCD, const SrcPelType* SrcA, const SrcPelType* SrcB, const SrcPelType* SrcC, const SrcPelType* SrcD, int32 Area) { xPixelOpsSTD::ConvertSOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); }
  template<typename DstPelType, typename SrcPelType> static inline void ConvertSOA4toAOS4(DstPelType* DstABCD, const SrcPelType* SrcA, const SrcPelType* SrcB, const SrcPelType* SrcC, const SrcPelType* SrcD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSTD::ConvertSOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, DstStride, SrcStride, Width, Height); }
  template<typename DstPelType, typename SrcPelType> static inline void ConvertAOS4toSOA4(DstPelType* DstA, DstPelType* DstB, DstPelType* DstC, DstPelType* DstD, const SrcPelType* SrcABCD, int32 Area) { xPixelOpsSTD::ConvertAOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); }
  template<typename DstPelType, typename SrcPelType> static inline void ConvertAOS4toSOA4(DstPelType* DstA, DstPelType* DstB, DstPelType* DstC, DstPelType* DstD, const SrcPelType* SrcABCD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSTD::ConvertAOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, DstStride, SrcStride, Width, Height); }
  // partial 4 components  
  template<typename DstPelType, typename SrcPelType> static inline void ConvertSOA3toAOS4(DstPelType* DstABCD, const SrcPelType* SrcA, const SrcPelType* SrcB, const SrcPelType* SrcC, SrcPelType ValueD, int32 Area) { xPixelOpsSTD::ConvertSOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); }
  template<typename DstPelType, typename SrcPelType> static inline void ConvertSOA3toAOS4(DstPelType* DstABCD, const SrcPelType* SrcA, const SrcPelType* SrcB, const SrcPelType* SrcC, SrcPelType ValueD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSTD::ConvertSOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, DstStride, SrcStride, Width, Height); }
  template<typename DstPelType, typename SrcPelType> static inline void ConvertAOS4toSOA3(DstPelType* DstA, DstPelType* DstB, DstPelType* DstC, const SrcPelType* SrcABCD, int32 Area) { xPixelOpsSTD::ConvertAOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); }
  template<typename DstPelType, typename SrcPelType> static inline void ConvertAOS4toSOA3(DstPelType* DstA, DstPelType* DstB, DstPelType* DstC, const SrcPelType* SrcABCD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSTD::ConvertAOS4toSOA3(DstA, DstB, DstC, SrcABCD, DstStride, SrcStride, Width, Height); }

// moved outside class due to bug 85282 in gcc (CWG 727)
//#if X_USE_AVX && X_AVX1 && X_AVX2
//  template <> inline void ConvertSOA4toAOS4(uint8* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16* restrict SrcD, int32 Area) { xPixelOpsAVX::ConvertSOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); }
//  template <> inline void ConvertAOS4toSOA4(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, int16* restrict DstD, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsAVX::ConvertAOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); }
//  template <> inline void ConvertSOA3toAOS4(uint8* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 Area) { xPixelOpsAVX::ConvertSOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); }
//  template <> inline void ConvertAOS4toSOA3(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsAVX::ConvertAOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); }
//#elif X_USE_SSE && X_SSE4_2
//  template <> inline void ConvertSOA4toAOS4(uint8* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16* restrict SrcD, int32 Area) { xPixelOpsSSE::ConvertSOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); }
//  template <> inline void ConvertAOS4toSOA4(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, int16* restrict DstD, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsSSE::ConvertAOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); }
//  template <> inline void ConvertSOA3toAOS4(uint8* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 Area) { xPixelOpsSSE::ConvertSOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); }
//  template <> inline void ConvertAOS4toSOA3(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsSSE::ConvertAOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); }
//#endif

  //===============================================================================================================================================================================================================
  // Arrangement  + convertion (crazy)
  //===============================================================================================================================================================================================================
  // 3(4) components with repetition (ABAC)
  template<typename DstPelType, typename SrcPelType> static inline void ConvertSOA3toAOS4_ABAC(DstPelType* DstABAC, const SrcPelType* SrcA, const SrcPelType* SrcB, const SrcPelType* SrcC, int32 Area) { xPixelOpsSTD::ConvertSOA3toAOS4_ABAC(DstABAC, SrcA, SrcB, SrcC, Area); }
  template<typename DstPelType, typename SrcPelType> static inline void ConvertAOS4toSOA3_ABAC(DstPelType* DstA, DstPelType* DstB, DstPelType* DstC, const SrcPelType* SrcABAC, int32 Area) { xPixelOpsSTD::ConvertAOS4toSOA3_ABAC(DstA, DstB, DstC, SrcABAC, Area); }
  // 3(4) components with repetition (BACA)
  template<typename DstPelType, typename SrcPelType> static inline void ConvertSOA3toAOS4_BACA(DstPelType* DstBACA, const SrcPelType* SrcA, const SrcPelType* SrcB, const SrcPelType* SrcC, int32 Area) { xPixelOpsSTD::ConvertSOA3toAOS4_BACA(DstBACA, SrcA, SrcB, SrcC, Area); }
  template<typename DstPelType, typename SrcPelType> static inline void ConvertAOS4toSOA3_BACA(DstPelType* DstA, DstPelType* DstB, DstPelType* DstC, const SrcPelType* SrcBACA, int32 Area) { xPixelOpsSTD::ConvertAOS4toSOA3_BACA(DstA, DstB, DstC, SrcBACA, Area); }
  
  //===============================================================================================================================================================================================================
  // Arithmetic
  //===============================================================================================================================================================================================================
  //Arithmetic - shift
  template <typename PelType> static inline void Shift(PelType* Dst, PelType* Src, int32 Area, int32 Shift) { xPixelOpsSTD::Shift(Dst, Src, Area, Shift); }
  template <typename PelType> static inline void Shift(PelType* Dst, PelType* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift) { xPixelOpsSTD::Shift(Dst, Src, DstStride, SrcStride, Width, Height, Shift); }

// moved outside class due to bug 85282 in gcc (CWG 727)
//#if X_USE_AVX && X_AVX1 && X_AVX2
//  template <> inline void Shift(int16* Dst, int16* Src, int32 Area, int32 Shift) { xPixelOpsAVX::Shift(Dst, Src, Area, Shift); }
//  template <> inline void Shift(int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift) { xPixelOpsAVX::Shift(Dst, Src, DstStride, SrcStride, Width, Height, Shift); }
//#elif X_USE_SSE && X_SSE4_2
//  template <> inline void Shift(int16* Dst, int16* Src, int32 Area, int32 Shift) { xPixelOpsSSE::Shift(Dst, Src, Area, Shift); }
//  template <> inline void Shift(int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift) { xPixelOpsSSE::Shift(Dst, Src, DstStride, SrcStride, Width, Height, Shift); }
//#endif

  //===============================================================================================================================================================================================================
  // Some unused functions
  //===============================================================================================================================================================================================================
  static void     xConvertI16ToFloat_STD       (float* restrict Dst, int16* restrict Src, int32 Area);
  static void     xConvertI16ToFloat_SSE       (float* restrict Dst, int16* restrict Src, int32 Area);
  static void     xConvertI16ToFloat_AVX       (float* restrict Dst, int16* restrict Src, int32 Area);                                                                             
  static void     xConvertFloatToI16_STD       (int16* restrict Dst, float* restrict Src, int32 Area);
  static void     xConvertFloatToI16_SSE       (int16* restrict Dst, float* restrict Src, int32 Area);
  static void     xConvertFloatToI16_AVX       (int16* restrict Dst, float* restrict Src, int32 Area);    
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Margin
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename PelType> void xPixelOps::ExtendMarginSolid(PelType* restrict Addr, int32 Stride, int32 Width, int32 Height, int32 Margin, PelType Color)
{
  //left/right
  for(int32 y=0; y<Height; y++)
  {
    xMemsetX<PelType>(Addr - Margin, Color, Margin);
    xMemsetX<PelType>(Addr + Width,  Color, Margin);
    Addr += Stride;
  }  
  //below
  Addr -= (Stride + Margin);
  for(int32 y=0; y<Margin; y++)
  {
    xMemsetX<PelType>(Addr + (y+1)*Stride, Color, (Width + (Margin<<1)));
  } 
  //above
  Addr -= ((Height-1) * Stride);
  for(int32 y=0; y<Margin; y++)
  {
    xMemsetX<PelType>(Addr - (y+1)*Stride, Color, (Width + (Margin<<1)));
  }
}
template<typename PelType> void xPixelOps::ExtendMarginEdge(PelType* restrict Addr, int32 Stride, int32 Width, int32 Height, int32 Margin)
{
  //left/right
  for(int32 y=0; y<Height; y++)
  {
    PelType Left  = Addr[0];
    PelType Right = Addr[Width-1];
    for (int32 x=0; x<Margin; x++)
    {      
      Addr[x - Margin] = Left;
      Addr[x + Width ] = Right;
    }
    Addr += Stride;
  }
  //below
  Addr -= (Stride + Margin);
  for(int32 y=0; y<Margin; y++)
  {
    ::memcpy(Addr + (y+1)*Stride, Addr, sizeof(PelType)*(Width + (Margin<<1)) );
  }
  //above
  Addr -= ((Height-1) * Stride);
  for(int32 y=0; y<Margin; y++)
  {
    ::memcpy(Addr - (y+1)*Stride, Addr, sizeof(PelType)*(Width + (Margin<<1)) );
  }
}
template<typename PelType> void xPixelOps::ExtendMarginMirror(PelType* restrict Addr, int32 Stride, int32 Width, int32 Height, int32 Margin)
{
  //left/right
  for(int32 y=0; y<Height; y++)
  {
    for (int32 x=0; x<Margin; x++)
    {      
      Addr[x - Margin] = Addr[x];
      Addr[x + Width ] = Addr[Width-1-x];
    }
    Addr += Stride;
  }
  //below
  Addr -= (Stride + Margin);
  for(int32 y=0; y<Margin; y++)
  {
    ::memcpy(Addr + (y+1)*Stride, Addr - (y*Stride), sizeof(PelType)*(Width + (Margin<<1)) );
  }
  //above
  Addr -= ((Height-1) * Stride);
  for(int32 y=0; y<Margin; y++)
  {
    ::memcpy(Addr - (y+1)*Stride, Addr + (y*Stride), sizeof(PelType)*(Width + (Margin<<1)) );
  }
}
template<typename PelType> void xPixelOps::ExtendMarginBayer(PelType* restrict Addr, int32 Stride, int32 Width, int32 Height, int32 Margin)
{
  //left/right
  for(int32 y=0; y<Height; y++)
  {
    PelType LeftA  = Addr[0];
    PelType LeftB  = Addr[1];
    PelType RightA = Addr[Width-2];
    PelType RightB = Addr[Width-1];
    for (int32 x=0; x<Margin; x+=2)
    {      
      Addr[x - Margin    ] = LeftA;
      Addr[x - Margin + 1] = LeftB;
      Addr[x + Width     ] = RightA;
      Addr[x + Width  + 1] = RightB;
    }
    Addr += Stride;
  }
  //below
  Addr -= (Stride + Margin);
  for(int32 y=0; y<Margin; y+=2)
  {
    ::memcpy(Addr + (y+1)*Stride, Addr,          sizeof(PelType)*(Width + (Margin<<1)) );
    ::memcpy(Addr + (y+2)*Stride, Addr + Stride, sizeof(PelType)*(Width + (Margin<<1)) );
  }
  //above
  Addr -= ((Height-1) * Stride);
  for(int32 y=0; y<Margin; y+=2)
  {
    ::memcpy(Addr - (y+1)*Stride, Addr,          sizeof(PelType)*(Width + (Margin<<1)) );
    ::memcpy(Addr - (y+2)*Stride, Addr - Stride, sizeof(PelType)*(Width + (Margin<<1)) );
  }
}
template<typename PelType> void xPixelOps::ClearMargin(PelType* restrict Addr, int32 Stride, int32 Width, int32 Height, int32 Margin)
{
  //left and right
  for (int32 y=0; y<Height; y++)
  {
    ::memset((void*)(Addr - Margin), 0, Margin*sizeof(PelType));
    ::memset((void*)(Addr + Width ), 0, Margin*sizeof(PelType));
    Addr += Stride;
  }
  //below
  Addr -= (Stride + Margin);
  for (int32 y=0; y<Margin; y++)
  {
    ::memset((void*)(Addr + (y+1)*Stride), 0, (Width + (Margin<<1))*sizeof(PelType));
  }
  //above
  Addr -= ((Height-1) * Stride);
  for (int32 y=0; y<Margin; y++)
  {
    ::memset((void*)(Addr - (y+1)*Stride), 0, (Width + (Margin<<1))*sizeof(PelType));
  }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Convert = Pack (with saturation) or Unpack
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if X_USE_AVX && X_AVX2
template <> inline void xPixelOps::Convert<uint8, int16>(uint8* Dst, const int16* Src, int32 Area) { xPixelOpsAVX::Convert(Dst, Src, Area); }
template <> inline void xPixelOps::Convert<int16, uint8>(int16* Dst, const uint8* Src, int32 Area) { xPixelOpsAVX::Convert(Dst, Src, Area); }
template <> inline void xPixelOps::Convert<uint8, int16>(uint8* Dst, const int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsAVX::Convert(Dst, Src, DstStride, SrcStride, Width, Height); }
template <> inline void xPixelOps::Convert<int16, uint8>(int16* Dst, const uint8* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsAVX::Convert(Dst, Src, DstStride, SrcStride, Width, Height); }
#elif X_USE_SSE && X_SSE4_2
template <> inline void xPixelOps::Convert(uint8* Dst, const int16* Src, int32 Area) { xPixelOpsSSE::Convert(Dst, Src, Area); }
template <> inline void xPixelOps::Convert(int16* Dst, const uint8* Src, int32 Area) { xPixelOpsSSE::Convert(Dst, Src, Area); }
template <> inline void xPixelOps::Convert(uint8* Dst, const int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSSE::Convert(Dst, Src, DstStride, SrcStride, Width, Height); }
template <> inline void xPixelOps::Convert(int16* Dst, const uint8* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSSE::Convert(Dst, Src, DstStride, SrcStride, Width, Height); }
#endif

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Arrangement
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if X_USE_AVX && X_AVX1 && X_AVX2
template <> inline void xPixelOps::SOA2toAOS2(uint8* restrict DstAB, const uint8* restrict SrcA, const uint8* restrict SrcB, int32 Area) { xPixelOpsAVX::SOA2toAOS2(DstAB, SrcA, SrcB, Area); }
template <> inline void xPixelOps::AOS2toSOA2(uint8* restrict DstA, uint8* restrict DstB, const uint8* restrict SrcAB, int32 Area) { xPixelOpsAVX::AOS2toSOA2(DstA, DstB, SrcAB, Area); }
template <> inline void xPixelOps::SOA2toAOS2(int16* restrict DstAB, const int16* restrict SrcA, const int16* restrict SrcB, int32 Area) { xPixelOpsAVX::SOA2toAOS2(DstAB, SrcA, SrcB, Area); }
template <> inline void xPixelOps::AOS2toSOA2(int16* restrict DstA, int16* restrict DstB, const int16* restrict SrcAB, int32 Area) { xPixelOpsAVX::AOS2toSOA2(DstA, DstB, SrcAB, Area); }  

template <> inline void xPixelOps::SOA4toAOS4(uint8* restrict DstABCD, const uint8* restrict SrcA, const uint8* restrict SrcB, const uint8* restrict SrcC, const uint8* restrict SrcD, int32 Area) { xPixelOpsAVX::SOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); }
template <> inline void xPixelOps::AOS4toSOA4(uint8* restrict DstA, uint8* restrict DstB, uint8* restrict DstC, uint8* restrict DstD, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsAVX::AOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); }
template <> inline void xPixelOps::SOA4toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16* restrict SrcD, int32 Area) { xPixelOpsAVX::SOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); }
template <> inline void xPixelOps::AOS4toSOA4(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, int16* restrict DstD, const int16* restrict SrcABCD, int32 Area) { xPixelOpsAVX::AOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); }

template <> inline void xPixelOps::SOA3toAOS4(uint8* restrict DstABCD, const uint8* restrict SrcA, const uint8* restrict SrcB, const uint8* restrict SrcC, uint8 ValueD, int32 Area) { xPixelOpsAVX::SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); }
template <> inline void xPixelOps::AOS4toSOA3(uint8* restrict DstA, uint8* restrict DstB, uint8* restrict DstC, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsAVX::AOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); }
template <> inline void xPixelOps::SOA3toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 Area) { xPixelOpsAVX::SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); }
template <> inline void xPixelOps::SOA3toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsAVX::SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, DstStride, SrcStride, Width, Height); }
template <> inline void xPixelOps::AOS4toSOA3(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, const int16* restrict SrcABCD, int32 Area) { xPixelOpsAVX::AOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); }
#elif X_USE_SSE && X_SSE4_2
template <> inline void xPixelOps::SOA2toAOS2(uint8* restrict DstAB, const uint8* restrict SrcA, const uint8* restrict SrcB, int32 Area) { xPixelOpsSSE::SOA2toAOS2(DstAB, SrcA, SrcB, Area); }
template <> inline void xPixelOps::AOS2toSOA2(uint8* restrict DstA, uint8* restrict DstB, const uint8* restrict SrcAB, int32 Area) { xPixelOpsSSE::AOS2toSOA2(DstA, DstB, SrcAB, Area); }
template <> inline void xPixelOps::SOA2toAOS2(int16* restrict DstAB, const int16* restrict SrcA, const int16* restrict SrcB, int32 Area) { xPixelOpsSSE::SOA2toAOS2(DstAB, SrcA, SrcB, Area); }
template <> inline void xPixelOps::AOS2toSOA2(int16* restrict DstA, int16* restrict DstB, const int16* restrict SrcAB, int32 Area) { xPixelOpsSSE::AOS2toSOA2(DstA, DstB, SrcAB, Area); }  

template <> inline void xPixelOps::SOA4toAOS4(uint8* restrict DstABCD, const uint8* restrict SrcA, const uint8* restrict SrcB, const uint8* restrict SrcC, const uint8* restrict SrcD, int32 Area) { xPixelOpsSSE::SOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); }
template <> inline void xPixelOps::AOS4toSOA4(uint8* restrict DstA, uint8* restrict DstB, uint8* restrict DstC, uint8* restrict DstD, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsSSE::AOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); }
template <> inline void xPixelOps::SOA4toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16* restrict SrcD, int32 Area) { xPixelOpsSSE::SOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); }
template <> inline void xPixelOps::AOS4toSOA4(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, int16* restrict DstD, const int16* restrict SrcABCD, int32 Area) { xPixelOpsSSE::AOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); }

template <> inline void xPixelOps::SOA3toAOS4(uint8* restrict DstABCD, const uint8* restrict SrcA, const uint8* restrict SrcB, const uint8* restrict SrcC, uint8 ValueD, int32 Area) { xPixelOpsSSE::SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); }
template <> inline void xPixelOps::AOS4toSOA3(uint8* restrict DstA, uint8* restrict DstB, uint8* restrict DstC, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsSSE::AOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); }
template <> inline void xPixelOps::SOA3toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 Area) { xPixelOpsSSE::SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); }
template <> inline void xPixelOps::SOA3toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height) { xPixelOpsSSE::SOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, DstStride, SrcStride, Width, Height); }
template <> inline void xPixelOps::AOS4toSOA3(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, const int16* restrict SrcABCD, int32 Area) { xPixelOpsSSE::AOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); }
#endif

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Arrangement + convertion
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if X_USE_AVX && X_AVX1 && X_AVX2
template <> inline void xPixelOps::ConvertSOA4toAOS4(uint8* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16* restrict SrcD, int32 Area) { xPixelOpsAVX::ConvertSOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); }
template <> inline void xPixelOps::ConvertAOS4toSOA4(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, int16* restrict DstD, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsAVX::ConvertAOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); }
template <> inline void xPixelOps::ConvertSOA3toAOS4(uint8* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 Area) { xPixelOpsAVX::ConvertSOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); }
template <> inline void xPixelOps::ConvertAOS4toSOA3(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsAVX::ConvertAOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); }
#elif X_USE_SSE && X_SSE4_2
template <> inline void xPixelOps::ConvertSOA4toAOS4(uint8* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16* restrict SrcD, int32 Area) { xPixelOpsSSE::ConvertSOA4toAOS4(DstABCD, SrcA, SrcB, SrcC, SrcD, Area); }
template <> inline void xPixelOps::ConvertAOS4toSOA4(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, int16* restrict DstD, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsSSE::ConvertAOS4toSOA4(DstA, DstB, DstC, DstD, SrcABCD, Area); }
template <> inline void xPixelOps::ConvertSOA3toAOS4(uint8* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 Area) { xPixelOpsSSE::ConvertSOA3toAOS4(DstABCD, SrcA, SrcB, SrcC, ValueD, Area); }
template <> inline void xPixelOps::ConvertAOS4toSOA3(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, const uint8* restrict SrcABCD, int32 Area) { xPixelOpsSSE::ConvertAOS4toSOA3(DstA, DstB, DstC, SrcABCD, Area); }
#endif

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Arithmetic
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if X_USE_AVX && X_AVX1 && X_AVX2
template <> inline void xPixelOps::Shift(int16* Dst, int16* Src, int32 Area, int32 Shift) { xPixelOpsAVX::Shift(Dst, Src, Area, Shift); }
template <> inline void xPixelOps::Shift(int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift) { xPixelOpsAVX::Shift(Dst, Src, DstStride, SrcStride, Width, Height, Shift); }
#elif X_USE_SSE && X_SSE4_2
template <> inline void xPixelOps::Shift(int16* Dst, int16* Src, int32 Area, int32 Shift) { xPixelOpsSSE::Shift(Dst, Src, Area, Shift); }
template <> inline void xPixelOps::Shift(int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift) { xPixelOpsSSE::Shift(Dst, Src, DstStride, SrcStride, Width, Height, Shift); }
#endif

//===============================================================================================================================================================================================================

} //end of namespace AVLib
