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
#include <cstdint>
#include <cinttypes>
#include <cfloat>
#include <climits>
#include <cstddef>
//#include "half/half.hpp"

namespace AVlib {

//=============================================================================================================================================================================
// Integers
//=============================================================================================================================================================================
typedef  int64_t   int64;
typedef  int32_t   int32;
typedef  int16_t   int16;
typedef  int8_t    int8 ;
                   
typedef  uint64_t  uint64;
typedef  uint32_t  uint32;
typedef  uint16_t  uint16;
typedef  uint8_t   uint8 ;

typedef  uint8     byte  ;

static const int64  int64_max  = INT64_MAX;
static const int64  int64_min  = INT64_MIN;
static const int32  int32_max  = INT32_MAX;
static const int32  int32_min  = INT32_MIN;
static const int16  int16_max  = INT16_MAX;
static const int16  int16_min  = INT16_MIN;
static const int8   int8_max   = INT8_MAX ;
static const int8   int8_min   = INT8_MIN ;

static const uint64 uint64_max = UINT64_MAX;
static const uint64 uint64_min = 0         ;
static const uint32 uint32_max = UINT32_MAX;
static const uint32 uint32_min = 0         ;
static const uint16 uint16_max = UINT16_MAX;
static const uint16 uint16_min = 0         ;
static const uint8  uint8_max  = UINT8_MAX ;
static const uint8  uint8_min  = 0         ;

//=============================================================================================================================================================================
// Special integers
//=============================================================================================================================================================================
typedef  uintptr_t  uintPtr;
typedef  size_t     uintSize;

static const size_t uintSize_max = SIZE_MAX;
static const size_t uintSize_min = 0       ;

//=============================================================================================================================================================================
// Floats
//=============================================================================================================================================================================
//typedef  half_float::half flt16;
typedef  float            flt32;
typedef  double           flt64;

//static const flt16 flt16_max = std::numeric_limits<flt16>::max();
//static const flt16 flt16_min = std::numeric_limits<flt16>::min();
//static const flt16 flt16_eps = std::numeric_limits<flt16>::epsilon();

static const flt32 flt32_max = FLT_MAX;
static const flt32 flt32_min = FLT_MIN;
static const flt32 flt32_eps = FLT_EPSILON;

static const flt64 flt64_max = DBL_MAX;
static const flt64 flt64_min = DBL_MIN;
static const flt64 flt64_eps = DBL_EPSILON;

//=============================================================================================================================================================================

} //end of namespace AVLib
