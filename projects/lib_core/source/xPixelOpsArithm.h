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

class xPixelOpsArithm
{
public:
  //arithmetic - addition with saturation
  static void     xAddSat_STD                  (int16* Dst, int16* Src0, int16* Src1, int32 Area);
  static void     xAddSat_SSE                  (int16* Dst, int16* Src0, int16* Src1, int32 Area);
  static void     xAddSat_AVX                  (int16* Dst, int16* Src0, int16* Src1, int32 Area);
  static void     xAddSat_STD                  (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height);
  static void     xAddSat_SSE                  (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height);
  static void     xAddSat_AVX                  (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height);
  //arithmetic - addition with clipping
  static void     xAddAndClipU_STD             (int16* Dst, int16* Src0, int16* Src1, int32 Area, int16 ClippingRange);
  static void     xAddAndClipU_SSE             (int16* Dst, int16* Src0, int16* Src1, int32 Area, int16 ClippingRange);
  static void     xAddAndClipU_AVX             (int16* Dst, int16* Src0, int16* Src1, int32 Area, int16 ClippingRange);
  static void     xAddAndClipU_STD             (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange);
  static void     xAddAndClipU_SSE             (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange);
  static void     xAddAndClipU_AVX             (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange);
  //arithmetic - substraction with saturation
  static void     xSubSat_STD                  (int16* Dst, int16* Src0, int16* Src1, int32 Area);
  static void     xSubSat_SSE                  (int16* Dst, int16* Src0, int16* Src1, int32 Area);
  static void     xSubSat_AVX                  (int16* Dst, int16* Src0, int16* Src1, int32 Area);
  static void     xSubSat_STD                  (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height);
  static void     xSubSat_SSE                  (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height);
  static void     xSubSat_AVX                  (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height);
  //arithmetic - multiplication by constant
  static void     xMulConst_STD                (int16* Dst, int16* Src, int32 Area, const int16 Multiplier);
  static void     xMulConst_SSE                (int16* Dst, int16* Src, int32 Area, const int16 Multiplier);
  static void     xMulConst_AVX                (int16* Dst, int16* Src, int32 Area, const int16 Multiplier);
  static void     xMulConst_STD                (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Multiplier);
  static void     xMulConst_SSE                (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Multiplier);
  static void     xMulConst_AVX                (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Multiplier);
  //inter prediction - calculation of bi pred search signal
  static void     xBiPredRefEst_STD            (int16* Dst, int16* Org, int16* Prd0, int32 Area);
  static void     xBiPredRefEst_SSE            (int16* Dst, int16* Org, int16* Prd0, int32 Area);
  static void     xBiPredRefEst_AVX            (int16* Dst, int16* Org, int16* Prd0, int32 Area);
  static void     xBiPredRefEst_STD            (int16* Dst, int16* Org, int16* Prd0, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Width, int32 Height);  
  static void     xBiPredRefEst_SSE            (int16* Dst, int16* Org, int16* Prd0, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Width, int32 Height);
  static void     xBiPredRefEst_AVX            (int16* Dst, int16* Org, int16* Prd0, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Width, int32 Height);
  //inter prediction - calculation of quad pred search signal
  static void     xQuadPredRefEst_STD          (int16* Dst, int16* Org, int16* Prd0, int16* Prd1, int16* Prd2, int32 Area);
  static void     xQuadPredRefEst_SSE          (int16* Dst, int16* Org, int16* Prd0, int16* Prd1, int16* Prd2, int32 Area);
  static void     xQuadPredRefEst_AVX          (int16* Dst, int16* Org, int16* Prd0, int16* Prd1, int16* Prd2, int32 Area);
  static void     xQuadPredRefEst_STD          (int16* Dst, int16* Org, int16* Prd0, int16* Prd1, int16* Prd2, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Prd1Stride, int32 Prd2Stride, int32 Width, int32 Height);  
  static void     xQuadPredRefEst_SSE          (int16* Dst, int16* Org, int16* Prd0, int16* Prd1, int16* Prd2, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Prd1Stride, int32 Prd2Stride, int32 Width, int32 Height);
  static void     xQuadPredRefEst_AVX          (int16* Dst, int16* Org, int16* Prd0, int16* Prd1, int16* Prd2, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Prd1Stride, int32 Prd2Stride, int32 Width, int32 Height);
  //arithmetic - averageing
  static void     xAvg_STD                     (int16* Dst, int16* Src0, int16* Src1, int32 Area);
  static void     xAvg_SSE                     (int16* Dst, int16* Src0, int16* Src1, int32 Area);
  static void     xAvg_AVX                     (int16* Dst, int16* Src0, int16* Src1, int32 Area);
  static void     xAvg_STD                     (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height);
  static void     xAvg_SSE                     (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height);
  static void     xAvg_AVX                     (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height);
  //arithmetic - averageing with clipping
  static void     xAvgAndClipU_STD             (int16* Dst, int16* Src0, int16* Src1, int32 Area, int16 ClippingRange);
  static void     xAvgAndClipU_SSE             (int16* Dst, int16* Src0, int16* Src1, int32 Area, int16 ClippingRange);
  static void     xAvgAndClipU_AVX             (int16* Dst, int16* Src0, int16* Src1, int32 Area, int16 ClippingRange);
  static void     xAvgAndClipU_STD             (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange);
  static void     xAvgAndClipU_SSE             (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange);
  static void     xAvgAndClipU_AVX             (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange);
  //arithmetic - clipping
  static void     xClip_STD                    (int16* Dst, int16* Src, int32 Area, int16 ClippingRangeLow, int16 ClippingRangeHigh);
  static void     xClip_SSE                    (int16* Dst, int16* Src, int32 Area, int16 ClippingRangeLow, int16 ClippingRangeHigh);
  static void     xClip_AVX                    (int16* Dst, int16* Src, int32 Area, int16 ClippingRangeLow, int16 ClippingRangeHigh);
  static void     xClip_STD                    (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int16 ClippingRangeLow, int16 ClippingRangeHigh);
  static void     xClip_SSE                    (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int16 ClippingRangeLow, int16 ClippingRangeHigh);
  static void     xClip_AVX                    (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int16 ClippingRangeLow, int16 ClippingRangeHigh);
  ////arithmetic - schift
  //static void     xShift_STD                   (int16* Dst, int16* Src, int32 Area, int32 Shift);
  //static void     xShift_SSE                   (int16* Dst, int16* Src, int32 Area, int32 Shift);
  //static void     xShift_AVX                   (int16* Dst, int16* Src, int32 Area, int32 Shift);
  //static void     xShift_STD                   (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift);
  //static void     xShift_SSE                   (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift);
  //static void     xShift_AVX                   (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift);
  //thresholding
  static void     xThreshold_STD               (int16* Dst, int16* Src, int32 Area, const int16 Threshold);
  static void     xThreshold_SSE               (int16* Dst, int16* Src, int32 Area, const int16 Threshold);
  static void     xThreshold_AVX               (int16* Dst, int16* Src, int32 Area, const int16 Threshold);
  static void     xThreshold_STD               (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Threshold);
  static void     xThreshold_SSE               (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Threshold);
  static void     xThreshold_AVX               (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Threshold);
  //count number of non-zero elements
  static uint32   xCountNonZero_STD            (int16* Src, int32 Area);
  static uint32   xCountNonZero_SSE            (int16* Src, int32 Area);
  static uint32   xCountNonZero_AVX            (int16* Src, int32 Area);
  static uint32   xCountNonZero_STD            (int16* Src, int32 SrcStride, int32 Width, int32 Height);
  static uint32   xCountNonZero_SSE            (int16* Src, int32 SrcStride, int32 Width, int32 Height);
  static uint32   xCountNonZero_AVX            (int16* Src, int32 SrcStride, int32 Width, int32 Height);
  //sum of all elements
  static int32    xSumAll_STD                  (int16* Src, int32 Area);
  static int32    xSumAll_SSE                  (int16* Src, int32 Area);
  static int32    xSumAll_AVX                  (int16* Src, int32 Area);
  static int32    xSumAll_STD                  (int16* Src, int32 SrcStride, int32 Width, int32 Height);
  static int32    xSumAll_SSE                  (int16* Src, int32 SrcStride, int32 Width, int32 Height);
  static int32    xSumAll_AVX                  (int16* Src, int32 SrcStride, int32 Width, int32 Height);

//=========================================================================================================================================

public:
#if X_USE_AVX && X_AVX2
  static void     AddSat                       (int16* Dst, int16* Src0, int16* Src1, int32 Area) { xAddSat_AVX(Dst, Src0, Src1, Area); }
  static void     AddSat                       (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height) { xAddSat_AVX(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height); }
  static void     AddAndClipU                  (int16* Dst, int16* Src0, int16* Src1, int32 Area, int16 ClippingRange) { xAddAndClipU_AVX(Dst, Src0, Src1, Area, ClippingRange); }
  static void     AddAndClipU                  (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange) { xAddAndClipU_AVX(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height, ClippingRange); }
  static void     SubSat                       (int16* Dst, int16* Src0, int16* Src1, int32 Area) { xSubSat_AVX(Dst, Src0, Src1, Area); }
  static void     SubSat                       (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height) { xSubSat_AVX(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height); }
  static void     MulConst                     (int16* Dst, int16* Src, int32 Area, const int16 Multiplier) { xMulConst_AVX(Dst, Src, Area, Multiplier); }
  static void     MulConst                     (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Multiplier) { xMulConst_AVX(Dst, Src, DstStride, SrcStride, Width, Height, Multiplier); }
  static void     BiPredRefEst                 (int16* Dst, int16* Org, int16* Prd0, int32 Area) { xBiPredRefEst_AVX(Dst, Org, Prd0, Area); }
  static void     BiPredRefEst                 (int16* Dst, int16* Org, int16* Prd0, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Width, int32 Height) { xBiPredRefEst_AVX(Dst, Org, Prd0, DstStride, OrgStride, Prd0Stride, Width, Height); }
  static void     QuadPredRefEst               (int16* Dst, int16* Org, int16* Prd0, int16* Prd1, int16* Prd2, int32 Area) { xQuadPredRefEst_AVX(Dst, Org, Prd0, Prd1, Prd2, Area); }
  static void     QuadPredRefEst               (int16* Dst, int16* Org, int16* Prd0, int16* Prd1, int16* Prd2, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Prd1Stride, int32 Prd2Stride, int32 Width, int32 Height) { xQuadPredRefEst_AVX(Dst, Org, Prd0, Prd1, Prd2, DstStride, OrgStride, Prd0Stride, Prd1Stride, Prd2Stride, Width, Height); }
  static void     Avg                          (int16* Dst, int16* Src0, int16* Src1, int32 Area) { xAvg_AVX(Dst, Src0, Src1, Area); }
  static void     Avg                          (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height) { xAvg_AVX(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height); }
  static void     AvgAndClipU                  (int16* Dst, int16* Src0, int16* Src1, int32 Area, int16 ClippingRange) { xAvgAndClipU_AVX(Dst, Src0, Src1, Area, ClippingRange); }
  static void     AvgAndClipU                  (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange) { xAvgAndClipU_AVX(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height, ClippingRange); }
  static void     Clip                         (int16* Dst, int16* Src, int32 Area, int16 ClippingRangeLow, int16 ClippingRangeHigh) { xClip_AVX(Dst, Src, Area, ClippingRangeLow, ClippingRangeHigh); }
  static void     Clip                         (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int16 ClippingRangeLow, int16 ClippingRangeHigh) { xClip_AVX(Dst, Src, DstStride, SrcStride, Width, Height, ClippingRangeLow, ClippingRangeHigh); }
  //static void     Shift                        (int16* Dst, int16* Src, int32 Area, int32 Shift) { xShift_AVX(Dst, Src, Area, Shift); }
  //static void     Shift                        (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift) { xShift_AVX(Dst, Src, DstStride, SrcStride, Width, Height, Shift); }
  static void     Threshold                    (int16* Dst, int16* Src, int32 Area, const int16 Threshold) { xThreshold_AVX(Dst, Src, Area, Threshold); }
  static uint32   CountNonZero                 (int16* Src, int32 Area) { return xCountNonZero_AVX(Src, Area); }
  static int32    SumAll                       (int16* Src, int32 Area) { return xSumAll_AVX(Src, Area); }  
#elif X_USE_SSE && X_SSE3
  static void     AddSat                       (int16* Dst, int16* Src0, int16* Src1, int32 Area) { xAddSat_SSE(Dst, Src0, Src1, Area); }
  static void     AddSat                       (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height) { xAddSat_SSE(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height); }
  static void     AddAndClipU                  (int16* Dst, int16* Src0, int16* Src1, int32 Area, int16 ClippingRange) { xAddAndClipU_SSE(Dst, Src0, Src1, Area, ClippingRange); }
  static void     AddAndClipU                  (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange) { xAddAndClipU_SSE(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height, ClippingRange); }
  static void     SubSat                       (int16* Dst, int16* Src0, int16* Src1, int32 Area) { xSubSat_SSE(Dst, Src0, Src1, Area); }
  static void     SubSat                       (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height) { xSubSat_SSE(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height); }
  static void     MulConst                     (int16* Dst, int16* Src, int32 Area, const int16 Multiplier) { xMulConst_SSE(Dst, Src, Area, Multiplier); }
  static void     MulConst                     (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Multiplier) { xMulConst_SSE(Dst, Src, DstStride, SrcStride, Width, Height, Multiplier); }
  static void     BiPredRefEst                 (int16* Dst, int16* Org, int16* Prd0, int32 Area) { xBiPredRefEst_SSE(Dst, Org, Prd0, Area); }
  static void     BiPredRefEst                 (int16* Dst, int16* Org, int16* Prd0, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Width, int32 Height) { xBiPredRefEst_SSE(Dst, Org, Prd0, DstStride, OrgStride, Prd0Stride, Width, Height); }
  static void     QuadPredRefEst               (int16* Dst, int16* Org, int16* Prd0, int16* Prd1, int16* Prd2, int32 Area) { xQuadPredRefEst_SSE(Dst, Org, Prd0, Prd1, Prd2, Area); }
  static void     QuadPredRefEst               (int16* Dst, int16* Org, int16* Prd0, int16* Prd1, int16* Prd2, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Prd1Stride, int32 Prd2Stride, int32 Width, int32 Height) { xQuadPredRefEst_SSE(Dst, Org, Prd0, Prd1, Prd2, DstStride, OrgStride, Prd0Stride, Prd1Stride, Prd2Stride, Width, Height); }
  static void     Avg                          (int16* Dst, int16* Src0, int16* Src1, int32 Area) { xAvg_SSE(Dst, Src0, Src1, Area); }
  static void     Avg                          (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height) { xAvg_SSE(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height); }
  static void     AvgAndClipU                  (int16* Dst, int16* Src0, int16* Src1, int32 Area, int16 ClippingRange) { xAvgAndClipU_SSE(Dst, Src0, Src1, Area, ClippingRange); }
  static void     AvgAndClipU                  (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange) { xAvgAndClipU_SSE(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height, ClippingRange); }
  static void     Clip                         (int16* Dst, int16* Src, int32 Area, int16 ClippingRangeLow, int16 ClippingRangeHigh) { xClip_SSE(Dst, Src, Area, ClippingRangeLow, ClippingRangeHigh); }
  static void     Clip                         (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int16 ClippingRangeLow, int16 ClippingRangeHigh) { xClip_SSE(Dst, Src, DstStride, SrcStride, Width, Height, ClippingRangeLow, ClippingRangeHigh); }
  //static void     Shift                        (int16* Dst, int16* Src, int32 Area, int32 Shift) { xShift_SSE(Dst, Src, Area, Shift); }
  //static void     Shift                        (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift) { xShift_SSE(Dst, Src, DstStride, SrcStride, Width, Height, Shift); }
  static void     Threshold                    (int16* Dst, int16* Src, int32 Area, const int16 Threshold) { xThreshold_SSE(Dst, Src, Area, Threshold); }
  static uint32   CountNonZero                 (int16* Src, int32 Area) { return xCountNonZero_SSE(Src, Area); }
  static int32    SumAll                       (int16* Src, int32 Area) { return xSumAll_SSE(Src, Area); }
#else
  static void     AddSat                       (int16* Dst, int16* Src0, int16* Src1, int32 Area) { xAddSat_STD(Dst, Src0, Src1, Area); }
  static void     AddSat                       (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height) { xAddSat_STD(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height); }
  static void     AddAndClipU                  (int16* Dst, int16* Src0, int16* Src1, int32 Area, int16 ClippingRange) { xAddAndClipU_STD(Dst, Src0, Src1, Area, ClippingRange); }
  static void     AddAndClipU                  (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange) { xAddAndClipU_STD(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height, ClippingRange); }
  static void     SubSat                       (int16* Dst, int16* Src0, int16* Src1, int32 Area) { xSubSat_STD(Dst, Src0, Src1, Area); }
  static void     SubSat                       (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height) { xSubSat_STD(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height); }
  static void     MulConst                     (int16* Dst, int16* Src, int32 Area, const int16 Multiplier) { xMulConst_STD(Dst, Src, Area, Multiplier); }
  static void     MulConst                     (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, const int16 Multiplier) { xMulConst_STD(Dst, Src, DstStride, SrcStride, Width, Height, Multiplier); }
  static void     BiPredRefEst                 (int16* Dst, int16* Org, int16* Prd0, int32 Area) { xBiPredRefEst_STD(Dst, Org, Prd0, Area); }
  static void     BiPredRefEst                 (int16* Dst, int16* Org, int16* Prd0, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Width, int32 Height) { xBiPredRefEst_STD(Dst, Org, Prd0, DstStride, OrgStride, Prd0Stride, Width, Height); }
  static void     QuadPredRefEst               (int16* Dst, int16* Org, int16* Prd0, int16* Prd1, int16* Prd2, int32 Area) { xQuadPredRefEst_STD(Dst, Org, Prd0, Prd1, Prd2, Area); }
  static void     QuadPredRefEst               (int16* Dst, int16* Org, int16* Prd0, int16* Prd1, int16* Prd2, int32 DstStride, int32 OrgStride, int32 Prd0Stride, int32 Prd1Stride, int32 Prd2Stride, int32 Width, int32 Height) { xQuadPredRefEst_STD(Dst, Org, Prd0, Prd1, Prd2, DstStride, OrgStride, Prd0Stride, Prd1Stride, Prd2Stride, Width, Height); }
  static void     Avg                          (int16* Dst, int16* Src0, int16* Src1, int32 Area) { xAvg_STD(Dst, Src0, Src1, Area); }
  static void     Avg                          (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height) { xAvg_STD(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height); }
  static void     AvgAndClipU                  (int16* Dst, int16* Src0, int16* Src1, int32 Area, int16 ClippingRange) { xAvgAndClipU_STD(Dst, Src0, Src1, Area, ClippingRange); }
  static void     AvgAndClipU                  (int16* Dst, int16* Src0, int16* Src1, int32 DstStride, int32 Src0Stride, int32 Src1Stride, int32 Width, int32 Height, int16 ClippingRange) { xAvgAndClipU_STD(Dst, Src0, Src1, DstStride, Src0Stride, Src1Stride, Width, Height, ClippingRange); }
  static void     Clip                         (int16* Dst, int16* Src, int32 Area, int16 ClippingRangeLow, int16 ClippingRangeHigh) { xClip_STD(Dst, Src, Area, ClippingRangeLow, ClippingRangeHigh); }
  static void     Clip                         (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int16 ClippingRangeLow, int16 ClippingRangeHigh) { xClip_STD(Dst, Src, DstStride, SrcStride, Width, Height, ClippingRangeLow, ClippingRangeHigh); }
  //static void     Shift                        (int16* Dst, int16* Src, int32 Area, int32 Shift) { xShift_STD(Dst, Src, Area, Shift); }
  //static void     Shift                        (int16* Dst, int16* Src, int32 DstStride, int32 SrcStride, int32 Width, int32 Height, int32 Shift) { xShift_STD(Dst, Src, DstStride, SrcStride, Width, Height, Shift); }
  static void     Threshold                    (int16* Dst, int16* Src, int32 Area, const int16 Threshold) { xThreshold_STD(Dst, Src, Area, Threshold); }
  static uint32   CountNonZero                 (int16* Src, int32 Area) { return xCountNonZero_STD(Src, Area); }
  static int32    SumAll                       (int16* Src, int32 Area) { return xSumAll_STD(Src, Area); }
#endif
};

} //end of namespace AVLib
