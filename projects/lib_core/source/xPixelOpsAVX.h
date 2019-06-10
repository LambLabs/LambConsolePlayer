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

#if X_USE_AVX && X_AVX1 && X_AVX2

namespace AVlib {

//=============================================================================================================================================================================

class xPixelOpsAVX
{
public:
  // Convert = Pack (with saturation) or Unpack
  static void Convert(uint8* restrict Dst, const int16* restrict Src, int32 Area);
  static void Convert(uint8* restrict Dst, const int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height);
  static void Convert(int16* restrict Dst, const uint8* restrict Src, int32 Area);
  static void Convert(int16* restrict Dst, const uint8* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height);

  // Arrangement ---> 2 components
  static void SOA2toAOS2(uint8* restrict DstAB, const uint8* restrict SrcA, const uint8* restrict SrcB, int32 Area);
  static void AOS2toSOA2(uint8* restrict DstA, uint8* restrict DstB, const uint8* restrict SrcAB, int32 Area);
  static void SOA2toAOS2(int16* restrict DstAB, const int16* restrict SrcA, const int16* restrict SrcB, int32 Area);
  static void AOS2toSOA2(int16* restrict DstA, int16* restrict DstB, const int16* restrict SrcAB, int32 Area);
  // Arrangement ---> 4 components
  static void SOA4toAOS4(uint8* restrict DstABCD, const uint8* restrict SrcA, const uint8* restrict SrcB, const uint8* restrict SrcC, const uint8* restrict SrcD, int32 Area);
  static void AOS4toSOA4(uint8* restrict DstA, uint8* restrict DstB, uint8* restrict DstC, uint8* restrict DstD, const uint8* restrict SrcABCD, int32 Area);
  static void SOA4toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16* restrict SrcD, int32 Area);
  static void AOS4toSOA4(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, int16* restrict DstD, const int16* restrict SrcABCD, int32 Area);
  // Arrangement ---> partial 4 components
  static void SOA3toAOS4(uint8* restrict DstABCD, const uint8* restrict SrcA, const uint8* restrict SrcB, const uint8* restrict SrcC, uint8 ValueD, int32 Area);
  static void AOS4toSOA3(uint8* restrict DstA, uint8* restrict DstB, uint8* restrict DstC, const uint8* restrict SrcABCD, int32 Area);
  static void SOA3toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 Area);
  static void SOA3toAOS4(int16* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 DstStride, int32 SrcStride, int32 Width, int32 Height);
  static void AOS4toSOA3(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, const int16* restrict SrcABCD, int32 Area);
  // Arrangement + convertion ---> 4 components
  static void ConvertSOA4toAOS4(uint8* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, const int16* restrict SrcD, int32 Area);
  static void ConvertAOS4toSOA4(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, int16* restrict DstD, const uint8* restrict SrcABCD, int32 Area);
  // Arrangement + convertion ---> partial 4 components
  static void ConvertSOA3toAOS4(uint8* restrict DstABCD, const int16* restrict SrcA, const int16* restrict SrcB, const int16* restrict SrcC, int16 ValueD, int32 Area);
  static void ConvertAOS4toSOA3(int16* restrict DstA, int16* restrict DstB, int16* restrict DstC, const uint8* restrict SrcABCD, int32 Area);

  // Arithmetic --> shift
  static void Shift(int16* restrict Dst, int16* restrict Src, int32 Area, int32 Shift);
  static void Shift(int16* restrict Dst, int16* restrict Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift);

};

//=============================================================================================================================================================================

} //end of namespace AVLib

#endif //X_USE_AVX && X_AVX1 && X_AVX2