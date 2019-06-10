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
#include "xType.h"
#include "xExtensions.h"
#include <cstring>

namespace AVlib {

//=============================================================================================================================================================================
// Small ops
//=============================================================================================================================================================================
template <class XXX> static inline XXX  xAbs (XXX a){return (XXX)std::abs(a);}
template <class XXX> static inline XXX  xSign(XXX a){return a>=0 ? 1 : -1;}

template <class XXX> static inline XXX  xMin(XXX a, XXX b) {return a<b ? a : b;}
template <class XXX> static inline XXX  xMax(XXX a, XXX b) {return a>b ? a : b;}
template <class XXX> static inline XXX  xMin(XXX a, XXX b, XXX c) {return xMin(xMin(a,b),c);}
template <class XXX> static inline XXX  xMax(XXX a, XXX b, XXX c) {return xMax(xMax(a,b),c);}
template <class XXX> static inline XXX  xMin(XXX a, XXX b, XXX c, XXX d) {return xMin(xMin(a,b),xMin(c,d));}
template <class XXX> static inline XXX  xMax(XXX a, XXX b, XXX c, XXX d) {return xMax(xMax(a,b),xMax(c,d));}

template <class XXX> static inline XXX  xMedian3 (XXX a, XXX b, XXX c) {return(a>b?a>c?b>c?b:c:a:b>c?a>c?a:c:b);} //3-point median

template <class XXX> static inline XXX  xClip    (XXX x, XXX min, XXX max) {return xMax(min,xMin(x,max));}
template <class XXX> static inline XXX  xClipU   (XXX x, XXX max) {return xMax((XXX)0, xMin(x,max));}
template <class XXX> static inline XXX  xClipU8  (XXX x) {return xMax((XXX)0,xMin(x,(XXX)255));}
template <class XXX> static inline XXX  xClipS8  (XXX x) {return xMax((XXX)-128,xMin(x,(XXX)127));}
template <class XXX> static inline XXX  xClipU16 (XXX x) {return xMax((XXX)0,xMin(x,(XXX)65536));}
template <class XXX> static inline XXX  xClipS16 (XXX x) {return xMax((XXX)-32768,xMin(x,(XXX)32767));}
template <class XXX> static inline XXX  xAutoClip(XXX x, XXX a, XXX b) {return (a<b ? xClip(x,a,b) : xClip(x,b,a));}
template <class XXX> static inline XXX  xClipMV  (XXX mv) {return xMax(-32768,xMin(mv,32767));}

template <class XXX> static inline bool xInRange (XXX x, XXX min, XXX max) {return ((x <= max) && (x >= min));}

template <class XXX> static inline XXX  xPow2(XXX x) {return x*x;}
template <class XXX> static inline XXX  xPow3(XXX x) {return x*x*x;}

template <class XXX> static inline XXX  xThreshold(XXX x, XXX t) {return (x)>(t) ? 1 : 0;}

template <class XXX> static inline bool xIsValidFlag(XXX x) { return(x==1 || x==0); }

template <class XXX> static inline XXX  xBitDepth2MidValue(XXX BitDepth) { return (1<<(BitDepth-1)); }
template <class XXX> static inline XXX  xBitDepth2MaxValue(XXX BitDepth) { return ((1<<BitDepth)-1); }
                                       
template <class XXX> static inline XXX  xLog2SizeToSize(XXX Log2Size) { return (1<<Log2Size); } 
template <class XXX> static inline XXX  xLog2SizeToArea(XXX Log2Size) { return (1<<((Log2Size)<<1)); } 

template <class XXX> static inline XXX  xNumUnitsCoveringLength(XXX Length, XXX Log2UnitSzize) { return ((Length - 1) >> Log2UnitSzize) + 1; }

template <class XXX> static inline XXX  xRoundCntrToNearestMultiple(XXX Value, XXX Log2Multiple) { return (((Value + ((1 << Log2Multiple) >> 1)) >> Log2Multiple) << Log2Multiple); } //positive integer only
template <class XXX> static inline XXX  xRoundUpToNearestMultiple  (XXX Value, XXX Log2Multiple) { return (((Value + ((1 << Log2Multiple) -  1)) >> Log2Multiple) << Log2Multiple); } //positive integer only
template <class XXX> static inline XXX  xRoundDownToNearestMultiple(XXX Value, XXX Log2Multiple) { return (( Value                               >> Log2Multiple) << Log2Multiple); } //positive integer only

//=============================================================================================================================================================================
// Constexpr small ops
//=============================================================================================================================================================================
namespace ConstExpr {
template <class XXX> constexpr XXX  xLog2SizeToSize(XXX Log2Size) { return (1 << Log2Size); }
template <class XXX> constexpr XXX  xLog2SizeToArea(XXX Log2Size) { return (1 << ((Log2Size) << 1)); }
}

//=============================================================================================================================================================================
// Memset
//=============================================================================================================================================================================
template <class XXX> static inline void xMemsetX(XXX*   Dst, XXX   Val, uint32 Count) { if constexpr (sizeof(XXX) == 1) { std::memset(Dst, Val, Count); } else { for(uint32 i=0; i < Count; i++) Dst[i] = Val; } }

static inline void xMemset8 (int8*   Dst, int8   Val, uint32 Count) { xMemsetX<int8  >(Dst, Val, Count); }
static inline void xMemset8 (uint8*  Dst, uint8  Val, uint32 Count) { xMemsetX<uint8 >(Dst, Val, Count); }
static inline void xMemset16(int16*  Dst, int16  Val, uint32 Count) { xMemsetX<int16 >(Dst, Val, Count); }
static inline void xMemset16(uint16* Dst, uint16 Val, uint32 Count) { xMemsetX<uint16>(Dst, Val, Count); }
static inline void xMemset32(int32*  Dst, int32  Val, uint32 Count) { xMemsetX<int32 >(Dst, Val, Count); }
static inline void xMemset32(uint32* Dst, uint32 Val, uint32 Count) { xMemsetX<uint32>(Dst, Val, Count); }
static inline void xMemset64(int64*  Dst, int64  Val, uint32 Count) { xMemsetX<int64 >(Dst, Val, Count); }
static inline void xMemset64(uint64* Dst, uint64 Val, uint32 Count) { xMemsetX<uint64>(Dst, Val, Count); }
static inline void xMemsetF (float*  Dst, float  Val, uint32 Count) { xMemsetX<float >(Dst, Val, Count); }
static inline void xMemsetD (double* Dst, double Val, uint32 Count) { xMemsetX<double>(Dst, Val, Count); }

//=============================================================================================================================================================================
// Memcpy
//=============================================================================================================================================================================
template <class XXX> static inline void xMemcpyX(XXX*   Dst, XXX*   Src, uint32 Count) { std::memcpy(Dst, Src, Count*sizeof(XXX)); }

static inline void xMemcpy8 (int8*   Dst, int8*   Src, uint32 Count) { xMemcpyX<int8  >(Dst, Src, Count); }
static inline void xMemcpy8 (uint8*  Dst, uint8*  Src, uint32 Count) { xMemcpyX<uint8 >(Dst, Src, Count); }
static inline void xMemcpy16(int16*  Dst, int16*  Src, uint32 Count) { xMemcpyX<int16 >(Dst, Src, Count); }
static inline void xMemcpy16(uint16* Dst, uint16* Src, uint32 Count) { xMemcpyX<uint16>(Dst, Src, Count); }
static inline void xMemcpy32(int32*  Dst, int32*  Src, uint32 Count) { xMemcpyX<int32 >(Dst, Src, Count); }
static inline void xMemcpy32(uint32* Dst, uint32* Src, uint32 Count) { xMemcpyX<uint32>(Dst, Src, Count); }
static inline void xMemcpy64(int64*  Dst, int64*  Src, uint32 Count) { xMemcpyX<int64 >(Dst, Src, Count); }
static inline void xMemcpy64(uint64* Dst, uint64* Src, uint32 Count) { xMemcpyX<uint64>(Dst, Src, Count); }
static inline void xMemcpyF (float*  Dst, float*  Src, uint32 Count) { xMemcpyX<float >(Dst, Src, Count); }
static inline void xMemcpyD (double* Dst, double* Src, uint32 Count) { xMemcpyX<double>(Dst, Src, Count); }

//=============================================================================================================================================================================
// Memcmp
//=============================================================================================================================================================================
template <class XXX> static inline int32 xMemcmpX(XXX*   B, XXX*   A, uint32 Count) { return std::memcmp(B, A, Count*sizeof(XXX)); }

static inline int32 xMemcmp8 (int8*   B, int8*   A, uint32 Count) { return xMemcmpX<int8  >(B, A, Count); }
static inline int32 xMemcmp8 (uint8*  B, uint8*  A, uint32 Count) { return xMemcmpX<uint8 >(B, A, Count); }
static inline int32 xMemcmp16(int16*  B, int16*  A, uint32 Count) { return xMemcmpX<int16 >(B, A, Count); }
static inline int32 xMemcmp16(uint16* B, uint16* A, uint32 Count) { return xMemcmpX<uint16>(B, A, Count); }
static inline int32 xMemcmp32(int32*  B, int32*  A, uint32 Count) { return xMemcmpX<int32 >(B, A, Count); }
static inline int32 xMemcmp32(uint32* B, uint32* A, uint32 Count) { return xMemcmpX<uint32>(B, A, Count); }
static inline int32 xMemcmp64(int64*  B, int64*  A, uint32 Count) { return xMemcmpX<int64 >(B, A, Count); }
static inline int32 xMemcmp64(uint64* B, uint64* A, uint32 Count) { return xMemcmpX<uint64>(B, A, Count); }
static inline int32 xMemcmpF (float*  B, float*  A, uint32 Count) { return xMemcmpX<float >(B, A, Count); }
static inline int32 xMemcmpD (double* B, double* A, uint32 Count) { return xMemcmpX<double>(B, A, Count); }

//=============================================================================================================================================================================
// MemOR
//=============================================================================================================================================================================
template <class XXX> static inline void xMemOrX(XXX*   Ptr, XXX   Val, uint32 Count) { for(uint32 i=0; i < Count; i++) { Ptr[i] |= Val; } }

static inline void xMemOr8 (int8*   Ptr, int8   Val, uint32 Count) { xMemOrX<int8  >(Ptr, Val, Count); }
static inline void xMemOr8 (uint8*  Ptr, uint8  Val, uint32 Count) { xMemOrX<uint8 >(Ptr, Val, Count); }
static inline void xMemOr16(int16*  Ptr, int16  Val, uint32 Count) { xMemOrX<int16 >(Ptr, Val, Count); }
static inline void xMemOr16(uint16* Ptr, uint16 Val, uint32 Count) { xMemOrX<uint16>(Ptr, Val, Count); }
static inline void xMemOr32(int32*  Ptr, int32  Val, uint32 Count) { xMemOrX<int32 >(Ptr, Val, Count); }
static inline void xMemOr32(uint32* Ptr, uint32 Val, uint32 Count) { xMemOrX<uint32>(Ptr, Val, Count); }
static inline void xMemOr64(int64*  Ptr, int64  Val, uint32 Count) { xMemOrX<int64 >(Ptr, Val, Count); }
static inline void xMemOr64(uint64* Ptr, uint64 Val, uint32 Count) { xMemOrX<uint64>(Ptr, Val, Count); }

//=============================================================================================================================================================================
// Fast iteger Log2 using bsr (Bit Scan Reverse) x86/x64 instructions, undefined for Value == 0 (same as log2())
//=============================================================================================================================================================================
#if X_COMPILER == X_COMPILER_MSVC
static inline uint32 xLog2(uint32 Value) { unsigned long Log2; _BitScanReverse  (&Log2, (uint32)Value); return Log2; }
static inline uint64 xLog2(uint64 Value) { unsigned long Log2; _BitScanReverse64(&Log2, (uint64)Value); return Log2; }
#elif (X_COMPILER == X_COMPILER_GCC || X_COMPILER == X_COMPILER_CLANG)
static inline uint32 xLog2(uint32 Value) { return 31 - __builtin_clz  (        Value); }
static inline uint64 xLog2(uint64 Value) { return 63 - __builtin_clzll(        Value); }
#else
#error Unrecognized compiler
#endif

//=============================================================================================================================================================================
// Leading zero count
//=============================================================================================================================================================================
#if defined __AVX2__
#if X_COMPILER == X_COMPILER_MSVC
static inline uint16 xLZCNT(uint16 Val) { return __lzcnt16(Val); }
static inline uint32 xLZCNT(uint32 Val) { return __lzcnt  (Val); }
static inline uint64 xLZCNT(uint64 Val) { return __lzcnt64(Val); }
#elif X_COMPILER == X_COMPILER_GCC
static inline uint16 xLZCNT(uint16 Val) { return __builtin_ia32_lzcnt_u16(Val); }
static inline uint32 xLZCNT(uint32 Val) { return __builtin_ia32_lzcnt_u32(Val); }
static inline uint64 xLZCNT(uint64 Val) { return __builtin_ia32_lzcnt_u64(Val); }
#elif X_COMPILER == X_COMPILER_CLANG
static inline uint16 xLZCNT(uint16 Val) { return __lzcnt16(Val); }
static inline uint32 xLZCNT(uint32 Val) { return __lzcnt32(Val); }
static inline uint64 xLZCNT(uint64 Val) { return __lzcnt64(Val); }
#else
#error Unrecognized compiler
#endif
#else //__AVX2__
#if X_COMPILER == X_COMPILER_MSVC
static inline uint16 xLZCNT(uint16 Val) { unsigned long Index; return (uint16)(_BitScanReverse  (&Index, Val) ? (31 - Index) : 16); }
static inline uint32 xLZCNT(uint32 Val) { unsigned long Index; return (uint32)(_BitScanReverse  (&Index, Val) ? (31 - Index) : 32); }
static inline uint64 xLZCNT(uint64 Val) { unsigned long Index; return (uint64)(_BitScanReverse64(&Index, Val) ? (63 - Index) : 64); }
#elif (X_COMPILER == X_COMPILER_GCC || X_COMPILER == X_COMPILER_CLANG)
static inline uint16 xLZCNT(uint16 Val) { return Val ? (uint16)__builtin_clz  (Val) : 16; }
static inline uint32 xLZCNT(uint32 Val) { return Val ? (uint32)__builtin_clz  (Val) : 32; }
static inline uint64 xLZCNT(uint64 Val) { return Val ? (uint64)__builtin_clzll(Val) : 64; }
#else
#error Unrecognized compiler
#endif
#endif //__AVX2__

//=============================================================================================================================================================================
// Num significant bits (similar to xLog2, but returns 0 for Val==0, uses faster (i.e. on Zen) lzcnt
//=============================================================================================================================================================================
static inline uint16 xNumSignificantBits(uint16 Val) { return 16 - xLZCNT(Val); }
static inline uint32 xNumSignificantBits(uint32 Val) { return 32 - xLZCNT(Val); }
static inline uint64 xNumSignificantBits(uint64 Val) { return 32 - xLZCNT(Val); }

//uint16 xNumSignificantBits(uint16 Val)
//{
//  if(!Val) { return 0; }
//
//  int32 NumSigBits = 16;
//  if (!(Val & 0xff00)) { NumSigBits -= 8; Val <<= 8; }
//  if (!(Val & 0xf000)) { NumSigBits -= 4; Val <<= 4; }
//  if (!(Val & 0xc000)) { NumSigBits -= 2; Val <<= 2; }
//  if (!(Val & 0x8000)) { NumSigBits -= 1; Val <<= 1; }
//
//  return NumSigBits;
//}
//=============================================================================================================================================================================
// Byte swap
//=============================================================================================================================================================================
#if X_COMPILER == X_COMPILER_MSVC
static inline uint16 xSwapBytes16(uint16 Value) { return _byteswap_ushort(Value); }
static inline  int16 xSwapBytes16( int16 Value) { return _byteswap_ushort(Value); }
static inline uint32 xSwapBytes32(uint32 Value) { return _byteswap_ulong (Value); }
static inline  int32 xSwapBytes32( int32 Value) { return _byteswap_ulong (Value); }
static inline uint64 xSwapBytes64(uint64 Value) { return _byteswap_uint64(Value); }
static inline  int64 xSwapBytes64( int64 Value) { return _byteswap_uint64(Value); }
#elif (X_COMPILER == X_COMPILER_GCC || X_COMPILER == X_COMPILER_CLANG)
static inline uint16 xSwapBytes16(uint16 Value) { return __builtin_bswap16(Value); }
static inline  int16 xSwapBytes16( int16 Value) { return __builtin_bswap16(Value); }
static inline uint32 xSwapBytes32(uint32 Value) { return __builtin_bswap32(Value); }
static inline  int32 xSwapBytes32( int32 Value) { return __builtin_bswap32(Value); }
static inline uint64 xSwapBytes64(uint64 Value) { return __builtin_bswap64(Value); }
static inline  int64 xSwapBytes64( int64 Value) { return __builtin_bswap64(Value); }
#else
#error Unrecognized compiler
#endif

//=============================================================================================================================================================================
// Byte swap
//=============================================================================================================================================================================
#if X_SSE3
static inline int32 xRoundFloatToInt32(float Float) { return _mm_cvt_ss2si(_mm_set_ss(Float)); }
#else
static inline int32 xRoundFloatToInt32(float Float) { return (int32)(round(Float)); }
#endif


//=============================================================================================================================================================================
// FourCC and EightCC
//=============================================================================================================================================================================
constexpr uint32 xMakeFourCC (char A, char B, char C, char D                                ) { return (((uint32)A)) | (((uint32)B) << 8) | (((uint32)C) << 16) | (((uint32)D) << 24); }
constexpr uint64 xMakeEightCC(char A, char B, char C, char D, char E, char F, char G, char H) { return (((uint64)A)) | (((uint64)B) << 8) | (((uint64)C) << 16) | (((uint64)D) << 24) | (((uint64)E) << 32) | (((uint64)F) << 40) | (((uint64)G) << 48) | (((uint64)H) << 56); }
constexpr uint64 xMakeFourCC (const char* T) { return xMakeFourCC (T[0], T[1], T[2], T[3]                        ); }
constexpr uint64 xMakeEightCC(const char* T) { return xMakeEightCC(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7]); }

//=============================================================================================================================================================================
// Slightly bigger ops
//=============================================================================================================================================================================
template <class YYY, class XXX> static inline YYY xClipType(XXX x)
{ 
  if constexpr(std::is_integral_v<YYY>)
  {
    if constexpr(std::is_unsigned_v<XXX>) { return (YYY)xMin<XXX>(x, (XXX)std::numeric_limits<YYY>::max()); }
    else                                  { return (YYY)xMax<XXX>((XXX)std::numeric_limits<YYY>::min(), xMin<XXX>(x, (XXX)std::numeric_limits<YYY>::max())); }
  }
  else
  {
    return (YYY)x;
  }   
}

template <class YYY, class XXX> static inline YYY xConvertSaturateRoundType(XXX x)
{  
  if constexpr(std::is_same_v<YYY, XXX>) { return x; } //nothing to convert
    
  if constexpr(std::is_integral_v<YYY> && std::is_integral_v<XXX>) //integer to integer convertion
  {
    
    if constexpr(std::is_unsigned_v<YYY> && std::is_unsigned_v<XXX>) //unsigned to unsigned
    {
      if constexpr(sizeof(YYY) >= sizeof(XXX)) { return (YYY)x; }
      else { return (YYY)xMin<XXX>(x, (XXX)std::numeric_limits<YYY>::max()); } //clip from the top
    }
    if constexpr(std::is_signed_v<YYY> && std::is_unsigned_v<XXX>) //unsigned to signed
      {
      if constexpr(sizeof(YYY) > sizeof(XXX)) { return (YYY)x; }
      else { return (YYY)xMin<XXX>(x, (XXX)std::numeric_limits<YYY>::max()); } //clip from the top
      }
    if constexpr(std::is_unsigned_v<YYY> && std::is_signed_v<XXX>) //signed to unsigned
      {
      if constexpr(sizeof(YYY) >= sizeof(XXX)) { return (YYY)xMax<XXX>(x, 0); } //clip from the bottom
      else { return (YYY)xMax<XXX>(0, xMin<XXX>(x, (XXX)std::numeric_limits<YYY>::max())); } //clip from the top and bottom
      }
    if constexpr(std::is_signed_v<YYY> && std::is_signed_v<XXX>) //signed to signed
    {
      if constexpr(sizeof(YYY) >= sizeof(XXX)) { return (YYY)x; }
      else { return (YYY)xMax<XXX>((XXX)std::numeric_limits<YYY>::min(), xMin<XXX>(x, (XXX)std::numeric_limits<YYY>::max())); } //clip from the top and bottom
    }
  }
  else if constexpr(!std::is_integral_v<YYY> && std::is_integral_v<XXX>) //integer to floating convertion - no problem
  {
    return (YYY)x;
  }
  else if constexpr(std::is_integral_v<YYY> && !std::is_integral_v<XXX>) //floating to integer - risk of overflow - clip from the top and bottom and round
  {
    return (YYY)(round(xMax<XXX>((XXX)std::numeric_limits<YYY>::min(), xMin<XXX>(x, (XXX)std::numeric_limits<YYY>::max()))));
  }
  else //floating to floating - no problem (???)
  {
    return (YYY)x;
  }
}

//=============================================================================================================================================================================

} //end of namespace AVLib


