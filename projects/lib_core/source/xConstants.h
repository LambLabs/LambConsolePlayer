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

#include "xType.h"

namespace AVlib {

//=============================================================================================================================================================================

#define AVLIB_GENERATION       ("GEN2")
#define AVLIB_VERSION          ("0.3" )

//=====================================================================================================
// Multiple and remainder
//=====================================================================================================
static const uint32 c_MultipleMask4    = 0xFFFFFFFC;
static const uint32 c_MultipleMask8    = 0xFFFFFFF8;
static const uint32 c_MultipleMask16   = 0xFFFFFFF0;
static const uint32 c_MultipleMask32   = 0xFFFFFFE0;
static const uint32 c_MultipleMask64   = 0xFFFFFFC0;
static const uint32 c_MultipleMask128  = 0xFFFFFF80;

static const uint32 c_RemainderMask4   = 0x00000003;
static const uint32 c_RemainderMask8   = 0x00000007;
static const uint32 c_RemainderMask16  = 0x0000000F;
static const uint32 c_RemainderMask32  = 0x0000001F;
static const uint32 c_RemainderMask64  = 0x0000003F;
static const uint32 c_RemainderMask128 = 0x0000007F;

//=====================================================================================================
// Math constants
//=====================================================================================================
#define cM_E          2.71828182845904523536   // e
#define cM_Log2E      1.44269504088896340736   // log2 (e)
#define cM_Log10E     0.434294481903251827651  // log10(e)
#define cM_Ln2        0.693147180559945309417  // ln( 2)
#define cM_Ln10       2.30258509299404568402   // ln(10)
#define cM_Pi         3.14159265358979323846   // pi
#define cM_PiDiv2     1.57079632679489661923   // pi/2
#define cM_PiDiv4     0.785398163397448309616  // pi/4
#define cM_2Pi        6.28318530717958647692   // 2*pi
#define cM_4Pi        12.56637061435917295384  // 4*pi
#define cM_1DivPi     0.318309886183790671538  // 1/pi
#define cM_2DivPi     0.636619772367581343076  // 2/pi
#define cM_1Div2Pi    0.15915494309189533577   // 1/(2*pi)
#define cM_2DivSqrtPi 1.12837916709551257390   // 2/sqrt(pi)
#define cM_Sqrt2      1.41421356237309504880   // sqrt(2)
#define cM_1DivSqrt2  0.707106781186547524401  // 1/sqrt(2)




static const uint32 c_ParserBuffer     = 4194304; //4MB = 1024 pages

//=====================================================================================================
// Hard coded parameters and constrains
//=====================================================================================================
#define ABS_PART   -1
#define NOT_VALID  -1

//motion estimator
#define MAX_NUM_REF_PICS        16 //maximum number of reference pictures per ref pic list
#define MAX_NUM_MV_START_POINT  12
#define MAX_NUM_MV_CANDIDATES   2

//size of smalest possible partition (PU or/and TU)
#define LOG2_PARTITION_SIZE     2
#define LOG2_PARTITION_AREA     (LOG2_PARTITION_SIZE<<1)
#define PARTITION_SIZE          (1<<LOG2_PARTITION_SIZE)
#define PARTITION_AREA          (1<<(LOG2_PARTITION_SIZE<<1))

//maximal size of LCU
#define LOG2_MAX_LCU_SIZE       6
#define MAX_LCU_SIZE            (1<<LOG2_MAX_LCU_SIZE)

//minimal size of CU
#define LOG2_MIN_CU_SIZE        2 
#define MIN_CU_SIZE             (1<<LOG2_MIN_CU_SIZE)

#define MAX_PARTS_IN_LCU_SIDE   (MAX_LCU_SIZE/PARTITION_SIZE)
#define MAX_PARTS_IN_LCU        MAX_PARTS_IN_LCU_SIDE*MAX_PARTS_IN_LCU_SIDE

#define NUM_PIXELS_IN_LCU   MAX_LCU_SIZE*MAX_LCU_SIZE
#define MAX_CU_DEPTH        4   //log2(MAX_LCU_SIZE) - log2(MIN_CU_SIZE) + 1

//component
#define AVLIB_CmpNameLen       32

//cmp extend modes
#define X_CMP_EXTEND_MODE_FILL_GREY  0
#define X_CMP_EXTEND_MODE_EGDE_PIXEL 1
#define X_CMP_EXTEND_MODE_MIRROR     2

//=====================================================================================================
// Constexpr functions
//=====================================================================================================

constexpr int32 xcLog2(int32 Val) { return (Val > 1) ? 1 + xcLog2(Val >> 1) : 0; }
template <class XXX> constexpr static inline XXX  xcLog2SizeToSize(XXX Log2Size) { return (1<<Log2Size); } 
template <class XXX> constexpr static inline XXX  xcLog2SizeToArea(XXX Log2Size) { return (1<<((Log2Size)<<1)); } 
template <class XXX> constexpr static inline XXX  xcLog2SizeToLog2Area(XXX Log2Size) { return (Log2Size<<1); } 

template <class XXX> constexpr static inline XXX  xcMin(XXX a, XXX b) {return a<b ? a : b;}
template <class XXX> constexpr static inline XXX  xcMax(XXX a, XXX b) {return a>b ? a : b;}


//=============================================================================================================================================================================

} //end of namespace AVLib
