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
#include "xCompiler.h"
#include <cstdlib>

namespace AVlib {

//=============================================================================================================================================================================

//================================================================================================
// Memmory and alignment section
//================================================================================================
#define X_Log2MemSizeSSE    4                 //SSE    = 128 bit
#define X_Log2MemSizeAVX    5                 //AVX    = 256 bit
#define X_Log2MemSizeAVX512 6                 //AVX512 = 512 bit
#define X_Log2MemSizeReg    X_Log2MemSizeAVX  //current register size selection
#define X_Log2MemSizeCache  6                 //Cache line size = 64B
#define X_Log2MemSizePage   12                //Memmory page size = 4kB

#define X_MemSizeSSE        (1<<X_Log2MemSizeSSE   )
#define X_MemSizeAVX        (1<<X_Log2MemSizeAVX   )
#define X_MemSizeAVX512     (1<<X_Log2MemSizeAVX512)
#define X_MemSizeReg        (1<<X_Log2MemSizeReg   )
#define X_MemSizeCache      (1<<X_Log2MemSizeCache )
#define X_MemSizePage       (1<<X_Log2MemSizePage  )

#define X_AlignmentPel      X_MemSizePage      //pel origin alignment
#define X_AlignmentDef      X_MemSizeReg

//Allocation with explicit alignment
#if defined(X_SYSTEM_WINDOWS)
#define xAlignedMalloc(size, alignment) _aligned_malloc((size), (alignment))
#define xAlignedFree(memmory)           _aligned_free((memmory))
#elif defined(X_SYSTEM_LINUX)
#define xAlignedMalloc(size, alignment) aligned_alloc((alignment), (size))
#define xAlignedFree(memmory)           free((memmory))
#endif

//Allocation with explicit alignment and total size that is a multiple of RequiredSizeMultiple
static inline void* xAlignedMallocP(size_t Size, size_t Alignment, size_t RequiredSizeMultiple)
{
  Size += (RequiredSizeMultiple >> 1);
  Size -= (Size % RequiredSizeMultiple);
  return xAlignedMalloc(Size, Alignment);
}

//Allocation with implicit alignment
#define xMalloc(size)  xAlignedMalloc(size, X_AlignmentDef)
#define xFree(memmory) xAlignedFree(memmory)

//Aligned variables/objects
#if X_COMPILER == X_COMPILER_MSVC
#define xAligned(x) __declspec(align(x))
#elif (X_COMPILER == X_COMPILER_GCC || X_COMPILER == X_COMPILER_CLANG)
#define xAligned(x) __attribute__ ((aligned(x)))
#else
#error "Unknown implementation"
#endif

//=============================================================================================================================================================================

template <typename XXX> class xMemMap1D
{
protected:
  XXX* const  m_Origin;

public:
  xMemMap1D(XXX* const Origin) : m_Origin(Origin) {}
  inline const XXX& operator[] (const int32 x) const { return m_Origin[x]; }
  inline       XXX& operator[] (const int32 x)       { return m_Origin[x]; }
};

template <typename XXX> class xMemMap2D
{
protected:
  XXX* const  m_Origin;
  const int32 m_StrideY;

public:
  xMemMap2D(XXX* const Origin, const int32 StrideY) : m_Origin(Origin), m_StrideY(StrideY) {}
  inline const xMemMap1D<XXX> operator[] (const int32 y) const { return xMemMap1D<XXX>(m_Origin + y * m_StrideY); }
  inline       xMemMap1D<XXX> operator[] (const int32 y)       { return xMemMap1D<XXX>(m_Origin + y * m_StrideY); }
};

template <typename XXX> class xMemMap3D
{
protected:
  XXX* const  m_Origin;
  const int32 m_StrideZ;
  const int32 m_StrideY;

public:
  xMemMap3D(XXX* const Origin, const int32 StrideZ, const int32 StrideY) : m_Origin(Origin), m_StrideZ(StrideZ), m_StrideY(StrideY) {}
  inline const xMemMap2D<XXX> operator[] (const int32 z) const { return xMemMap2D<XXX>(m_Origin + z * m_StrideZ, m_StrideY); }
  inline       xMemMap2D<XXX> operator[] (const int32 z)       { return xMemMap2D<XXX>(m_Origin + z * m_StrideZ, m_StrideY); }
};

template <typename XXX> class xMemMap4D
{
protected:
  XXX* const  m_Origin;
  const int32 m_StrideW;
  const int32 m_StrideZ;
  const int32 m_StrideY;

public:
  xMemMap4D(XXX* const Origin, const int32 StrideW, const int32 StrideZ, const int32 StrideY) : m_Origin(Origin), m_StrideW(StrideW), m_StrideZ(StrideZ), m_StrideY(StrideY) {}
  inline const xMemMap3D<XXX> operator[] (const int32 w) const { return xMemMap3D<XXX>(m_Origin + w * m_StrideW, m_StrideZ, m_StrideY); }
  inline       xMemMap3D<XXX> operator[] (const int32 w)       { return xMemMap3D<XXX>(m_Origin + w * m_StrideW, m_StrideZ, m_StrideY); }
};

//=============================================================================================================================================================================

} //end of namespace AVLib

