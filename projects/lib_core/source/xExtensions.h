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

#include "xCompiler.h"

//SSE, AVX intrinsics headers
#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || defined(_M_IX86))
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
//#include <xmmintrin.h> //SSE
//#include <emmintrin.h> //SSE2
//#include <pmmintrin.h> //SSE3
//#include <tmmintrin.h> //SSSE3
//#include <ammintrin.h> //SSE4A
//#include <smmintrin.h> //SSE4.1
//#include <nmmintrin.h> //SSE4.2
//#include <immintrin.h> //AVX & AVX2
#endif

//================================================================================================
// SSE & AVX SIMD section - set your processor capabilities
//================================================================================================

//MSVC does not define __SSEn__ macros. Assuming all extensions present.
#if X_COMPILER == X_COMPILER_MSVC
#define __SSE__    1
#define __SSE2__   1
#define __SSE3__   1
#define __SSSE3__  1
#define __SSE4_1__ 1
#define __SSE4_2__ 1
#endif

#define X_USE_SSE 1
#if X_USE_SSE
#define X_SSE1   __SSE__    //Pentium III
#define X_SSE2   __SSE2__   //Pentium 4    (Willamette, Northwood, Gallatin)
#define X_SSE3   __SSE3__   //Pentium 4    (Prescott, Cedar Mill)
#define X_SSSE3  __SSSE3__  //Core 2       (Conroe, Merom)
#define X_SSE4_1 __SSE4_1__ //Core 2       (Penryn, Wolfdale)
#define X_SSE4_2 __SSE4_2__ //Core iX nnn  (Nehalem, Westmere)
#endif

#define X_USE_AVX 1 
#if X_USE_AVX
#if defined (__AVX2__)
#define X_AVX1 1    //Core iX nnnn (Sandy Bridge, Ivy Bridge)
#define X_AVX2 1    //Core iX nnnn (Haswell, Broadwell, Skylake, ZenV1)
#elif defined (__AVX__)
#define X_AVX1 1    //Core iX nnnn (Sandy Bridge, Ivy Bridge)
#endif
#endif

#define X_USE_AVX512 0   //Skylake XEON

//================================================================================================
// 
//================================================================================================
#define _MM_SHUFFLE_IMM8_CTRL(v3, v2, v1, v0) (((v3) << 6) | ((v2) << 4) | ((v1) << 2) | ((v0)))
#define _MM_SHUFLELO_EPI16_CTRL(v3, v2, v1, v0) _MM_SHUFFLE_IMM8_CTRL(v3, v2, v1, v0)
#define _MM_SHUFLEHI_EPI16_CTRL(v3, v2, v1, v0) _MM_SHUFFLE_IMM8_CTRL(v3, v2, v1, v0)
#define _MM_SHUFFLE_PS_CTRL(v3, v2, v1, v0)     _MM_SHUFFLE_IMM8_CTRL(v3, v2, v1, v0)

#define _MM_BLEND_EPI16_CTRL(v7, v6, v5, v4, v3, v2, v1, v0) (((v7) << 7) | ((v6) << 6) | ((v5) << 5) | ((v4) << 4) | ((v3) << 3) | ((v2) << 2) | ((v1) << 1) | ((v0)))
#define _MM256_BLEND_EPI16_CTRL(v15, v14, v13, v12, v11, v10, v9, v8, v7, v6, v5, v4, v3, v2, v1, v0) (((v15) << 15) | ((v14) << 14) | ((v13) << 13) | ((v12) << 12) | ((v11) << 11) | ((v10) << 10) | ((v9) << 9) | ((v8) << 8) | ((v7) << 7) | ((v6) << 6) | ((v5) << 5) | ((v4) << 4) | ((v3) << 3) | ((v2) << 2) | ((v1) << 1) | ((v0)))

//================================================================================================
// Some compilers don't have _mm256_set_m128i
//================================================================================================
#ifndef _mm256_set_m128i
#define _mm256_set_m128i(/* __m128i */ hi, /* __m128i */ lo) _mm256_insertf128_si256(_mm256_castsi128_si256(lo), (hi), 0x1)
#endif
