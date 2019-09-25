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
#include <cstring>
#include <array>
#include <vector>

//TODO - check "std::reduce, std::transform_reduce"

namespace AVlib {

//=============================================================================================================================================================================

template <uint32 First, uint32 Last> struct xcStaticFor { template <typename Lambda> static inline constexpr void apply(Lambda const& f) { if constexpr(First < Last) { f(std::integral_constant<uint32, First>{}); xcStaticFor<First + 1, Last>::apply(f); } } };
template <typename XXX, std::size_t Size> static inline constexpr XXX xcStaticMax    (std::array<XXX, Size> const& Arr) { XXX Result(      Arr[0] ); for(std::size_t i = 1; i < Size; i++) { Result = std::max(Result, Arr[i]); } return Result; }
template <typename XXX, std::size_t Size> static inline constexpr XXX xcStaticMin    (std::array<XXX, Size> const& Arr) { XXX Result(      Arr[0] ); for(std::size_t i = 1; i < Size; i++) { Result = std::min(Result, Arr[i]); } return Result; }
template <typename XXX, std::size_t Size> static inline constexpr XXX xcStaticMaxAbs (std::array<XXX, Size> const& Arr) { XXX Result(xAbs (Arr[0])); for(std::size_t i = 1; i < Size; i++) { Result = std::max(Result, xAbs(Arr[i])); } return Result; }
template <typename XXX, std::size_t Size> static inline constexpr XXX xcStaticMinAbs (std::array<XXX, Size> const& Arr) { XXX Result(xAbs (Arr[0])); for(std::size_t i = 1; i < Size; i++) { Result = std::min(Result, xAbs(Arr[i])); } return Result; }
template <typename XXX, std::size_t Size> static inline constexpr XXX xcStaticSum    (std::array<XXX, Size> const& Arr) { XXX Result(      Arr[0] ); for(std::size_t i = 1; i < Size; i++) { Result += Arr[i]; } return Result; }
template <typename XXX, std::size_t Size> static inline constexpr XXX xcStaticSumAbs (std::array<XXX, Size> const& Arr) { XXX Result(xAbs (Arr[0])); for(std::size_t i = 1; i < Size; i++) { Result += xAbs (Arr[i]); } return Result; }
template <typename XXX, std::size_t Size> static inline constexpr XXX xcStaticSumPow2(std::array<XXX, Size> const& Arr) { XXX Result(xPow2(Arr[0])); for(std::size_t i = 1; i < Size; i++) { Result += xPow2(Arr[i]); } return Result; }
template <typename YYY, typename XXX, std::size_t Size> static inline constexpr XXX xcStaticSumCvtPow2(std::array<XXX, Size> const& Arr) { YYY Result(xPow2((YYY)(Arr[0]))); for(std::size_t i = 1; i < Size; i++) { Result += xPow2((YYY)(Arr[i])); } return Result;}

//=============================================================================================================================================================================

template<typename XXX, uint32 Size> class xVecX
{
protected:
  std::array<XXX, Size> m_V;

public:
  inline xVecX(                                ) {                                                            }
  inline xVecX(const XXX v                     ) { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i] = v; }); }
  inline xVecX(const XXX*                   Ptr) { std::memcpy(m_V, Ptr       , sizeof(m_V));                 }
  inline xVecX(const std::vector<XXX>&      Vec) { std::memcpy(m_V, Vec.data(), sizeof(m_V));                 }
  inline xVecX(const std::array<XXX, Size>& Arr) { std::memcpy(m_V, Arr.data(), sizeof(m_V));                 }
  template <uint32 S = Size> inline xVecX(XXX x, XXX y,               typename std::enable_if<S == 2, void>::type* = 0) { m_V[0] = x; m_V[1] = y;                         }
  template <uint32 S = Size> inline xVecX(XXX x, XXX y, XXX z,        typename std::enable_if<S == 3, void>::type* = 0) { m_V[0] = x; m_V[1] = y; m_V[2] = z;             }
  template <uint32 S = Size> inline xVecX(XXX x, XXX y, XXX z, XXX a, typename std::enable_if<S == 4, void>::type* = 0) { m_V[0] = x; m_V[1] = y; m_V[2] = z; m_V[3] = a; }

  template <uint32 S = Size, typename YYY> explicit operator typename std::enable_if<S == 2, xVecX<YYY, Size>>::type() const { return xVecX<YYY, Size>((YYY)m_V[0], (YYY)m_V[1]                          ); }
  template <uint32 S = Size, typename YYY> explicit operator typename std::enable_if<S == 3, xVecX<YYY, Size>>::type() const { return xVecX<YYY, Size>((YYY)m_V[0], (YYY)m_V[1], (YYY)m_V[2]             ); }
  template <uint32 S = Size, typename YYY> explicit operator typename std::enable_if<S == 4, xVecX<YYY, Size>>::type() const { return xVecX<YYY, Size>((YYY)m_V[0], (YYY)m_V[1], (YYY)m_V[2], (YYY)m_V[3]); }

  inline void  set    (XXX v                           ) { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i] = v; }); }
  inline void  set    (const XXX*                   Ptr) { std::memcpy(m_V, Ptr       , sizeof(m_V));                 }
  inline void  set    (const std::vector<XXX>&      Vec) { std::memcpy(m_V, Vec.data(), sizeof(m_V));                 }
  inline void  set    (const std::array<XXX, Size>& Arr) { std::memcpy(m_V, Arr.data(), sizeof(m_V));                 }
  inline void  setIdx (const XXX v, const int32 Idx    ) { m_V[Idx] = v;                                              }
  inline void  setZero(                                ) { std::memset(m_V, 0, sizeof(m_V));                          }

  template <uint32 S = Size> inline void  set (const XXX x, const XXX y                          , typename std::enable_if<S == 2, void>::type* = 0) { m_V[0] = x; m_V[1] = y;                         }
  template <uint32 S = Size> inline void  set (const XXX x, const XXX y, const XXX z             , typename std::enable_if<S == 3, void>::type* = 0) { m_V[0] = x; m_V[1] = y; m_V[2] = z;             }
  template <uint32 S = Size> inline void  set (const XXX x, const XXX y, const XXX z, const XXX a, typename std::enable_if<S == 4, void>::type* = 0) { m_V[0] = x; m_V[1] = y; m_V[2] = z; m_V[3] = a; }
  template <uint32 S = Size> inline void  setX(const XXX x,                                        typename std::enable_if<S >= 1, void>::type* = 0) { m_V[0] = x;                                     }
  template <uint32 S = Size> inline void  setY(const XXX y,                                        typename std::enable_if<S >= 2, void>::type* = 0) { m_V[1] = y;                                     }
  template <uint32 S = Size> inline void  setZ(const XXX z,                                        typename std::enable_if<S >= 3, void>::type* = 0) { m_V[2] = z;                                     }
  template <uint32 S = Size> inline void  setA(const XXX a,                                        typename std::enable_if<S >= 4, void>::type* = 0) { m_V[3] = a;                                     }

  inline void  get    (XXX*                   Ptr) const { std::memcpy(Ptr       , m_V, sizeof(m_V)); }
  inline void  get    (std::vector<XXX>&      Vec) const { std::memcpy(Vec.data(), m_V, sizeof(m_V)); }
  inline void  get    (std::array<XXX, Size>& Arr) const { std::memcpy(Arr.data(), m_V, sizeof(m_V)); }
  inline XXX   getIdx (const int32 Idx           ) const { return m_V[Idx];                           }
  
  template <uint32 S = Size> inline XXX   getX   (typename std::enable_if<S >= 1, void>::type* = 0) const { return m_V[0];      }
  template <uint32 S = Size> inline XXX   getY   (typename std::enable_if<S >= 2, void>::type* = 0) const { return m_V[1];      }
  template <uint32 S = Size> inline XXX   getZ   (typename std::enable_if<S >= 3, void>::type* = 0) const { return m_V[2];      }
  template <uint32 S = Size> inline XXX   getA   (typename std::enable_if<S >= 4, void>::type* = 0) const { return m_V[3];      }
  template <uint32 S = Size> inline XXX   getAbsX(typename std::enable_if<S >= 1, void>::type* = 0) const { return xAbs(m_V[0]);}
  template <uint32 S = Size> inline XXX   getAbsY(typename std::enable_if<S >= 2, void>::type* = 0) const { return xAbs(m_V[1]);}
  template <uint32 S = Size> inline XXX   getAbsZ(typename std::enable_if<S >= 3, void>::type* = 0) const { return xAbs(m_V[2]);}
  template <uint32 S = Size> inline XXX   getAbsA(typename std::enable_if<S >= 4, void>::type* = 0) const { return xAbs(m_V[3]);}

  inline XXX   getMax    () const { return xcStaticMax    <XXX, Size>(m_V); }
  inline XXX   getMin    () const { return xcStaticMin    <XXX, Size>(m_V); }
  inline XXX   getMaxAbs () const { return xcStaticMaxAbs <XXX, Size>(m_V); }
  inline XXX   getMinAbs () const { return xcStaticMinAbs <XXX, Size>(m_V); }
  inline XXX   getSum    () const { return xcStaticSum    <XXX, Size>(m_V); }
  inline XXX   getSumSqrs() const { return xcStaticSumAbs <XXX, Size>(m_V); }
  inline XXX   getSumAbs () const { return xcStaticSumPow2<XXX, Size>(m_V); }
  template <typename YYY> inline YYY getSumCvtSqrs() const { return xcStaticSumCvtPow2<YYY, XXX, Size>(m_V); }

  inline const xVecX<XXX, Size>& operator  += (const xVecX<XXX, Size>& Vec)       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i]  += Vec.m_V[i]; }); return *this;}  
  inline const xVecX<XXX, Size>& operator  -= (const xVecX<XXX, Size>& Vec)       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i]  -= Vec.m_V[i]; }); return *this;}
  inline const xVecX<XXX, Size>& operator  *= (const xVecX<XXX, Size>& Vec)       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i]  *= Vec.m_V[i]; }); return *this;}
  inline const xVecX<XXX, Size>& operator  /= (const xVecX<XXX, Size>& Vec)       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i]  /= Vec.m_V[i]; }); return *this;}
  inline const xVecX<XXX, Size>& operator  %= (const xVecX<XXX, Size>& Vec)       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i]  %= Vec.m_V[i]; }); return *this;}
  inline const xVecX<XXX, Size>& operator >>= (const xVecX<XXX, Size>& Vec)       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i] >>= Vec.m_V[i]; }); return *this;}
  inline const xVecX<XXX, Size>& operator <<= (const xVecX<XXX, Size>& Vec)       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i] <<= Vec.m_V[i]; }); return *this;}

  inline const xVecX<XXX, Size>& operator  += (const XXX Scalar           )       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i]  += Scalar; }); return *this;}  
  inline const xVecX<XXX, Size>& operator  -= (const XXX Scalar           )       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i]  -= Scalar; }); return *this;}
  inline const xVecX<XXX, Size>& operator  *= (const XXX Scalar           )       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i]  *= Scalar; }); return *this;}
  inline const xVecX<XXX, Size>& operator  /= (const XXX Scalar           )       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i]  /= Scalar; }); return *this;}
  inline const xVecX<XXX, Size>& operator  %= (const XXX Scalar           )       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i]  %= Scalar; }); return *this;}
  inline const xVecX<XXX, Size>& operator >>= (const XXX Scalar           )       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i] >>= Scalar; }); return *this;}
  inline const xVecX<XXX, Size>& operator <<= (const XXX Scalar           )       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i] <<= Scalar; }); return *this;}
  inline const xVecX<XXX, Size>& operator  &= (const XXX Scalar           )       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i]  &= Scalar; }); return *this;}
  inline const xVecX<XXX, Size>& operator  |= (const XXX Scalar           )       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i]  |= Scalar; }); return *this;}
  inline const xVecX<XXX, Size>& operator  ^= (const XXX Scalar           )       { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i]  ^= Scalar; }); return *this;}

  inline const xVecX<XXX, Size>  operator  +  (const xVecX<XXX, Size>& Vec) const { return (xVecX(*this)  += Vec); }  
  inline const xVecX<XXX, Size>  operator  -  (const xVecX<XXX, Size>& Vec) const { return (xVecX(*this)  -= Vec); }
  inline const xVecX<XXX, Size>  operator  *  (const xVecX<XXX, Size>& Vec) const { return (xVecX(*this)  *= Vec); }
  inline const xVecX<XXX, Size>  operator  /  (const xVecX<XXX, Size>& Vec) const { return (xVecX(*this)  /= Vec); }
  inline const xVecX<XXX, Size>  operator  %  (const xVecX<XXX, Size>& Vec) const { return (xVecX(*this)  %= Vec); }
  inline const xVecX<XXX, Size>  operator >>  (const xVecX<XXX, Size>& Vec) const { return (xVecX(*this) >>= Vec); }
  inline const xVecX<XXX, Size>  operator <<  (const xVecX<XXX, Size>& Vec) const { return (xVecX(*this) <<= Vec); }
                                    
  inline const xVecX<XXX, Size>  operator  +  (const XXX Scalar           ) const { return (xVecX(*this)  += Scalar); }
  inline const xVecX<XXX, Size>  operator  -  (const XXX Scalar           ) const { return (xVecX(*this)  -= Scalar); }
  inline const xVecX<XXX, Size>  operator  *  (const XXX Scalar           ) const { return (xVecX(*this)  *= Scalar); }
  inline const xVecX<XXX, Size>  operator  /  (const XXX Scalar           ) const { return (xVecX(*this)  /= Scalar); }
  inline const xVecX<XXX, Size>  operator  %  (const XXX Scalar           ) const { return (xVecX(*this)  %= Scalar); }
  inline const xVecX<XXX, Size>  operator <<  (const XXX Scalar           ) const { return (xVecX(*this) <<= Scalar); }
  inline const xVecX<XXX, Size>  operator >>  (const XXX Scalar           ) const { return (xVecX(*this) >>= Scalar); }
  inline const xVecX<XXX, Size>  operator  &  (const XXX Scalar           ) const { return (xVecX(*this)  &= Scalar); }
  inline const xVecX<XXX, Size>  operator  |  (const XXX Scalar           ) const { return (xVecX(*this)  |= Scalar); }
  inline const xVecX<XXX, Size>  operator  ^  (const XXX Scalar           ) const { return (xVecX(*this)  ^= Scalar); }

  inline bool              operator ==  (const xVecX<XXX, Size>& Vec) const { return (m_V == Vec.m_V); }
  inline bool              operator !=  (const xVecX<XXX, Size>& Vec) const { return (m_V != Vec.m_V); }

  inline bool              operator ==  (const XXX Scalar           ) const { return *this == xVecX(Scalar); }
  inline bool              operator !=  (const XXX Scalar           ) const { return *this != xVecX(Scalar); }

  inline       XXX&        operator[]   (const int32 Idx)       {return m_V[Idx];}
  inline const XXX&        operator[]   (const int32 Idx) const {return m_V[Idx];}
   
  //inline bool  isInsideSize  (xVecX<XXX> Size) const { return (m_V[0] >= 0 && m_V[0] < Size[0] && m_V[1] >= 0 && m_V[1] < Size[1]); }
  //inline bool  isInsideSize  (xVecX<XXX> Size) const { return (m_V[0] >= 0 && m_V[0] < Size[0] && m_V[1] >= 0 && m_V[1] < Size[1] && m_V[2] >= 0 && m_V[2] < Size[2]); }

  inline void  clip (const xVecX<XXX, Size>& Min, const xVecX<XXX, Size>& Max) { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i] = xClip (m_V[i], Min.m_V[i], Max.m_V[i]); }); }
  inline void  clip (const XXX               Min, const XXX Max              ) { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[i] = xClip (m_V[i], Min       , Max       ); }); }
  inline void  clipU(                             const xVecX<XXX, Size>& Max) { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[0] = xClipU(m_V[i],             Max.m_V[i]); }); }
  inline void  clipU(                             const XXX Max              ) { xcStaticFor<0, Size>::apply([&](uint32 i){ m_V[0] = xClipU(m_V[i],             Max       ); }); }
};

//=============================================================================================================================================================================

template<typename XXX> class xVec2
{
protected:
  xAligned(2 * sizeof(XXX)) XXX m_V[2];

public:
  xVec2() {}
  xVec2(XXX v)                       { m_V[0] = v; m_V[1] = v; }
  xVec2(XXX x, XXX y)                { m_V[0] = x; m_V[1] = y; }
  xVec2(XXX V[])                     { std::memcpy(m_V, V, sizeof(m_V)); }
  xVec2(const std::vector<XXX>&   V) { std::memcpy(m_V, V.data(), sizeof(m_V)); }
  xVec2(const std::array<XXX, 2>& A) { std::memcpy(m_V, A.data(), sizeof(m_V)); }

  template<typename YYY> explicit operator xVec2<YYY>() const { return xVec2<YYY>((YYY)m_V[0], (YYY)m_V[1]); }

  inline void  set     (const XXX x, const XXX y)     {m_V[0] = x; m_V[1] = y;}
  inline void  setIdx  (const XXX v, const int32 Idx) {m_V[Idx] = v; }
  inline void  setX    (const XXX x)                  {m_V[0] = x;}
  inline void  setY    (const XXX y)                  {m_V[1] = y;}
  inline void  setZero ()                             {std::memset(m_V, 0, sizeof(m_V));}

  inline XXX   getIdx    (const int32 Idx) const {return m_V[Idx]; }
  inline XXX   getX      ()                const {return m_V[0];}
  inline XXX   getY      ()                const {return m_V[1];}
  inline XXX   getAbsX   ()                const {return xAbs(m_V[0]);}
  inline XXX   getAbsY   ()                const {return xAbs(m_V[1]);}
  inline XXX   getMax    ()                const {return xMax(m_V[0], m_V[1]);}
  inline XXX   getMin    ()                const {return xMin(m_V[0], m_V[1]);}
  inline XXX   getMaxAbs ()                const {return xMax(xAbs(m_V[0]), xAbs(m_V[1]));}
  inline XXX   getMinAbs ()                const {return xMin(xAbs(m_V[0]), xAbs(m_V[1]));}
  inline XXX   getSum    ()                const {return (m_V[0] + m_V[1]);}
  inline XXX   getSumPow2()                const {return (xPow2(m_V[0]) + xPow2(m_V[1]));}
  inline XXX   getSumAbs ()                const {return (xAbs(m_V[0]) + xAbs(m_V[1]));}
  inline XXX   getMul    ()                const {return m_V[0] * m_V[1];}

  template <typename YYY> inline YYY getSumCvtSqrs() const {return (xPow2((YYY)m_V[0]) + xPow2((YYY)m_V[1]));}

  inline const xVec2<XXX>& operator += (const xVec2<XXX>& Vec2)       {m_V[0]  += Vec2.m_V[0]; m_V[1]  += Vec2.m_V[1]; return *this;}  
  inline const xVec2<XXX>& operator -= (const xVec2<XXX>& Vec2)       {m_V[0]  -= Vec2.m_V[0]; m_V[1]  -= Vec2.m_V[1]; return *this;}
  inline const xVec2<XXX>& operator *= (const xVec2<XXX>& Vec2)       {m_V[0]  *= Vec2.m_V[0]; m_V[1]  *= Vec2.m_V[1]; return *this;}
  inline const xVec2<XXX>& operator /= (const xVec2<XXX>& Vec2)       {m_V[0]  /= Vec2.m_V[0]; m_V[1]  /= Vec2.m_V[1]; return *this;}
  inline const xVec2<XXX>& operator %= (const xVec2<XXX>& Vec2)       {m_V[0]  %= Vec2.m_V[0]; m_V[1]  %= Vec2.m_V[1]; return *this;}
  inline const xVec2<XXX>& operator>>= (const xVec2<XXX>& Vec2)       {m_V[0] >>= Vec2.m_V[0]; m_V[1] >>= Vec2.m_V[1]; return *this;}
  inline const xVec2<XXX>& operator<<= (const xVec2<XXX>& Vec2)       {m_V[0] <<= Vec2.m_V[0]; m_V[1] <<= Vec2.m_V[1]; return *this;}
               
  inline const xVec2<XXX>& operator += (const XXX i)                  {m_V[0]  += i;           m_V[1]  += i;           return *this;}
  inline const xVec2<XXX>& operator -= (const XXX i)                  {m_V[0]  -= i;           m_V[1]  -= i;           return *this;}
  inline const xVec2<XXX>& operator *= (const XXX i)                  {m_V[0]  *= i;           m_V[1]  *= i;           return *this;}
  inline const xVec2<XXX>& operator /= (const XXX i)                  {m_V[0]  /= i;           m_V[1]  /= i;           return *this;}
  inline const xVec2<XXX>& operator %= (const XXX i)                  {m_V[0]  %= i;           m_V[1]  %= i;           return *this;}
  inline const xVec2<XXX>& operator>>= (const XXX i)                  {m_V[0] >>= i;           m_V[1] >>= i;           return *this;}
  inline const xVec2<XXX>& operator<<= (const XXX i)                  {m_V[0] <<= i;           m_V[1] <<= i;           return *this;}
  inline const xVec2<XXX>& operator &= (const XXX i)                  {m_V[0]  &= i;           m_V[1]  &= i;           return *this;}
  inline const xVec2<XXX>& operator |= (const XXX i)                  {m_V[0]  |= i;           m_V[1]  |= i;           return *this;}
  inline const xVec2<XXX>& operator ^= (const XXX i)                  {m_V[0]  ^= i;           m_V[1]  ^= i;           return *this;}

  inline const xVec2<XXX>  operator -  (const xVec2<XXX>& Vec2) const {return xVec2(m_V[0]  - Vec2.m_V[0], m_V[1]  - Vec2.m_V[1]);}
  inline const xVec2<XXX>  operator +  (const xVec2<XXX>& Vec2) const {return xVec2(m_V[0]  + Vec2.m_V[0], m_V[1]  + Vec2.m_V[1]);}  
  inline const xVec2<XXX>  operator *  (const xVec2<XXX>& Vec2) const {return xVec2(m_V[0]  * Vec2.m_V[0], m_V[1]  * Vec2.m_V[1]);}
  inline const xVec2<XXX>  operator /  (const xVec2<XXX>& Vec2) const {return xVec2(m_V[0]  / Vec2.m_V[0], m_V[1]  / Vec2.m_V[1]);}
  inline const xVec2<XXX>  operator %  (const xVec2<XXX>& Vec2) const {return xVec2(m_V[0]  % Vec2.m_V[0], m_V[1]  % Vec2.m_V[1]);}
  inline const xVec2<XXX>  operator>>  (const xVec2<XXX>& Vec2) const {return xVec2(m_V[0] >> Vec2.m_V[0], m_V[1] >> Vec2.m_V[1]);}
  inline const xVec2<XXX>  operator<<  (const xVec2<XXX>& Vec2) const {return xVec2(m_V[0] << Vec2.m_V[0], m_V[1] << Vec2.m_V[1]);}

  inline const xVec2<XXX>  operator +  (const XXX i) const            {return xVec2(m_V[0]  + i,           m_V[1]  + i          );}
  inline const xVec2<XXX>  operator -  (const XXX i) const            {return xVec2(m_V[0]  - i,           m_V[1]  - i          );}
  inline const xVec2<XXX>  operator *  (const XXX i) const            {return xVec2(m_V[0]  * i,           m_V[1]  * i          );}
  inline const xVec2<XXX>  operator /  (const XXX i) const            {return xVec2(m_V[0]  / i,           m_V[1]  / i          );}
  inline const xVec2<XXX>  operator %  (const XXX i) const            {return xVec2(m_V[0]  % i,           m_V[1]  % i          );}
  inline const xVec2<XXX>  operator<<  (const XXX i) const            {return xVec2(m_V[0] << i,           m_V[1] << i          );}
  inline const xVec2<XXX>  operator>>  (const XXX i) const            {return xVec2(m_V[0] >> i,           m_V[1] >> i          );}
  inline const xVec2<XXX>  operator &  (const XXX i) const            {return xVec2(m_V[0]  & i,           m_V[1]  & i          );}
  inline const xVec2<XXX>  operator |  (const XXX i) const            {return xVec2(m_V[0]  | i,           m_V[1]  | i          );}
  inline const xVec2<XXX>  operator ^  (const XXX i) const            {return xVec2(m_V[0]  ^ i,           m_V[1]  ^ i          );}

  inline bool              operator==  (const xVec2<XXX>& Vec2) const {return (m_V[0]==Vec2.m_V[0] && m_V[1]==Vec2.m_V[1]);}
  inline bool              operator!=  (const xVec2<XXX>& Vec2) const {return (m_V[0]!=Vec2.m_V[0] || m_V[1]!=Vec2.m_V[1]);}
  inline bool              operator<   (const xVec2<XXX>& Vec2) const {return (m_V[0]< Vec2.m_V[0] && m_V[1]< Vec2.m_V[1]);}
  inline bool              operator>   (const xVec2<XXX>& Vec2) const {return (m_V[0]> Vec2.m_V[0] && m_V[1]> Vec2.m_V[1]);}
  inline bool              operator<=  (const xVec2<XXX>& Vec2) const {return (m_V[0]<=Vec2.m_V[0] && m_V[1]<=Vec2.m_V[1]);}
  inline bool              operator>=  (const xVec2<XXX>& Vec2) const {return (m_V[0]>=Vec2.m_V[0] && m_V[1]>=Vec2.m_V[1]);}

  inline bool              operator==  (const XXX i) const            {return (m_V[0]==i           && m_V[1]==i          );}
  inline bool              operator!=  (const XXX i) const            {return (m_V[0]!=i           || m_V[1]!=i          );}
  inline bool              operator<   (const XXX i) const            {return (m_V[0]< i           && m_V[1]< i          );}
  inline bool              operator>   (const XXX i) const            {return (m_V[0]> i           && m_V[1]> i          );}
  inline bool              operator<=  (const XXX i) const            {return (m_V[0]<=i           && m_V[1]<=i          );}
  inline bool              operator>=  (const XXX i) const            {return (m_V[0]>=i           && m_V[1]>=i          );}

  inline       XXX&        operator[]  (int32 Idx)       {return m_V[Idx];}
  inline const XXX&        operator[]  (int32 Idx) const {return m_V[Idx];}

  inline bool  isZero()    const { return (m_V[0]==0 && m_V[1]==0); }
  inline bool  isNonZero() const { return (m_V[0]!=0 && m_V[1]!=0); } 

  inline bool  isInsideSize  (xVec2<XXX> Size) const { return (m_V[0] >= 0 && m_V[0] < Size[0] && m_V[1] >= 0 && m_V[1] < Size[1]); }
  inline bool  isInsideRange (XXX MinX, XXX MaxX, XXX MinY, XXX MaxY) const {return (m_V[0]>=MinX && m_V[0]<=MaxX && m_V[1]>=MinY && m_V[1]<=MaxY);}
  inline bool  isOutsideRange(XXX MinX, XXX MaxX, XXX MinY, XXX MaxY) const {return (m_V[0]< MinX || m_V[0]> MaxX || m_V[1]< MinY || m_V[1]> MaxY);}

  inline void  clip (xVec2<XXX>& Min, xVec2<XXX>& Max) { m_V[0] = xClip (m_V[0], Min.m_V[0], Max.m_V[0]); m_V[1] = xClip (m_V[1], Min.m_V[1], Max.m_V[1]); }
  inline void  clipU(                 xVec2<XXX>& Max) { m_V[0] = xClipU(m_V[0],             Max.m_V[0]); m_V[1] = xClipU(m_V[1],             Max.m_V[1]); }
};

//=============================================================================================================================================================================

template<typename XXX> class xVec3
{
protected:
  xAligned(sizeof(XXX)) XXX m_V[3];

public:
  xVec3() {}
  xVec3(XXX v)                       { m_V[0] = v; m_V[1] = v; m_V[2] = v;}
  xVec3(XXX x, XXX y, XXX z)         { m_V[0] = x; m_V[1] = y; m_V[2] = z;}
  xVec3(const XXX* V)                { std::memcpy(m_V, V, sizeof(m_V)); }
  xVec3(const std::vector<XXX>&   V) { std::memcpy(m_V, V.data(), sizeof(m_V)); }
  xVec3(const std::array<XXX, 3>& A) { std::memcpy(m_V, A.data(), sizeof(m_V)); }
  xVec3(const xVec2<XXX>& Vec2, XXX z) { m_V[0] = Vec2[0]; m_V[1] = Vec2[1]; m_V[2] = z; }
  xVec3(XXX x, const xVec2<XXX>& Vec2) { m_V[0] = x; m_V[1] = Vec2[0]; m_V[2] = Vec2[1]; }

  template<typename YYY> explicit operator xVec3<YYY>() const { return xVec3<YYY>((YYY)m_V[0], (YYY)m_V[1], (YYY)m_V[2]); }

  inline void  set     (const XXX x, const XXX y, const XXX z) {m_V[0] = x; m_V[1] = y; m_V[2] = z; }
  inline void  set     (const XXX* V)                          { std::memcpy(m_V, V, sizeof(m_V)); }
  inline void  setIdx  (const XXX v, const int32 Idx)          {m_V[Idx] = v; }
  inline void  setX    (const XXX x)                           {m_V[0] = x;}
  inline void  setY    (const XXX y)                           {m_V[1] = y;}
  inline void  setZ    (const XXX z)                           {m_V[2] = z;}
  inline void  setZero ()                                      {std::memset(m_V, 0, sizeof(m_V));}

  inline XXX   getIdx    (const int32 Idx) const {return m_V[Idx]; }
  inline void  get       (XXX* V         ) const { std::memcpy(V, m_V, sizeof(m_V)); }
  inline XXX   getX      (               ) const {return m_V[0];}
  inline XXX   getY      (               ) const {return m_V[1];}
  inline XXX   getZ      (               ) const {return m_V[2];}
  inline XXX   getAbsX   (               ) const {return xAbs(m_V[0]);}
  inline XXX   getAbsY   (               ) const {return xAbs(m_V[1]);}
  inline XXX   getAbsZ   (               ) const {return xAbs(m_V[2]);}
  inline XXX   getMax    (               ) const {return xMax(xMax(m_V[0], m_V[1]), m_V[2]);}
  inline XXX   getMin    (               ) const {return xMin(xMin(m_V[0], m_V[1]), m_V[2]);}
  inline XXX   getMaxAbs (               ) const {return xMax(xMax(xAbs(m_V[0]), xAbs(m_V[1])), xAbs(m_V[2]));}
  inline XXX   getMinAbs (               ) const {return xMin(xMin(xAbs(m_V[0]), xAbs(m_V[1])), xAbs(m_V[2]));}
  inline XXX   getSum    (               ) const {return (m_V[0] + m_V[1] + m_V[2]);}
  inline XXX   getSumSqrs(               ) const {return (xPow2(m_V[0]) + xPow2(m_V[1]) + xPow2(m_V[2]));}
  inline XXX   getSumAbs (               ) const {return (xAbs(m_V[0]) + xAbs(m_V[1]) + xAbs(m_V[2]));}
  inline XXX   getMul    ()                const {return m_V[0] * m_V[1] * m_V[2];}

  inline xVec2<XXX> SubsetToVec2() const { return xVec2<XXX>(m_V[0], m_V[1]); }

  template <typename YYY> inline YYY getSumCvtSqrs() const {return (xPow2((YYY)m_V[0]) + xPow2((YYY)m_V[1]) + xPow2((YYY)m_V[2]));}

  inline const xVec3<XXX>& operator += (const xVec3<XXX>& Vec3)       {m_V[0]  += Vec3.m_V[0]; m_V[1]  += Vec3.m_V[1]; m_V[2]  += Vec3.m_V[2]; return *this;}  
  inline const xVec3<XXX>& operator -= (const xVec3<XXX>& Vec3)       {m_V[0]  -= Vec3.m_V[0]; m_V[1]  -= Vec3.m_V[1]; m_V[2]  -= Vec3.m_V[2]; return *this;}
  inline const xVec3<XXX>& operator *= (const xVec3<XXX>& Vec3)       {m_V[0]  *= Vec3.m_V[0]; m_V[1]  *= Vec3.m_V[1]; m_V[2]  *= Vec3.m_V[2]; return *this;}
  inline const xVec3<XXX>& operator /= (const xVec3<XXX>& Vec3)       {m_V[0]  /= Vec3.m_V[0]; m_V[1]  /= Vec3.m_V[1]; m_V[2]  /= Vec3.m_V[2]; return *this;}
  inline const xVec3<XXX>& operator %= (const xVec3<XXX>& Vec3)       {m_V[0]  %= Vec3.m_V[0]; m_V[1]  %= Vec3.m_V[1]; m_V[2]  %= Vec3.m_V[2]; return *this;}
  inline const xVec3<XXX>& operator>>= (const xVec3<XXX>& Vec3)       {m_V[0] >>= Vec3.m_V[0]; m_V[1] >>= Vec3.m_V[1]; m_V[2] >>= Vec3.m_V[2]; return *this;}
  inline const xVec3<XXX>& operator<<= (const xVec3<XXX>& Vec3)       {m_V[0] <<= Vec3.m_V[0]; m_V[1] <<= Vec3.m_V[1]; m_V[2] <<= Vec3.m_V[2]; return *this;}

  inline const xVec3<XXX>& operator += (const XXX i)                  {m_V[0]  += i;           m_V[1]  += i;           m_V[2]  += i;           return *this;}
  inline const xVec3<XXX>& operator -= (const XXX i)                  {m_V[0]  -= i;           m_V[1]  -= i;           m_V[2]  -= i;           return *this;}
  inline const xVec3<XXX>& operator *= (const XXX i)                  {m_V[0]  *= i;           m_V[1]  *= i;           m_V[2]  *= i;           return *this;}
  inline const xVec3<XXX>& operator /= (const XXX i)                  {m_V[0]  /= i;           m_V[1]  /= i;           m_V[2]  /= i;           return *this;}
  inline const xVec3<XXX>& operator %= (const XXX i)                  {m_V[0]  %= i;           m_V[1]  %= i;           m_V[2]  %= i;           return *this;}
  inline const xVec3<XXX>& operator>>= (const XXX i)                  {m_V[0] >>= i;           m_V[1] >>= i;           m_V[2] >>= i;           return *this;}  
  inline const xVec3<XXX>& operator<<= (const XXX i)                  {m_V[0] <<= i;           m_V[1] <<= i;           m_V[2] <<= i;           return *this;}  
  inline const xVec3<XXX>& operator &= (const XXX i)                  {m_V[0]  &= i;           m_V[1]  &= i;           m_V[2]  &= i;           return *this;}
  inline const xVec3<XXX>& operator |= (const XXX i)                  {m_V[0]  |= i;           m_V[1]  |= i;           m_V[2]  |= i;           return *this;}
  inline const xVec3<XXX>& operator ^= (const XXX i)                  {m_V[0]  ^= i;           m_V[1]  ^= i;           m_V[2]  ^= i;           return *this;}

  inline const xVec3<XXX>  operator +  (const xVec3<XXX>& Vec3) const {return xVec3(m_V[0]  + Vec3.m_V[0], m_V[1]  + Vec3.m_V[1], m_V[2]  + Vec3.m_V[2]);}  
  inline const xVec3<XXX>  operator -  (const xVec3<XXX>& Vec3) const {return xVec3(m_V[0]  - Vec3.m_V[0], m_V[1]  - Vec3.m_V[1], m_V[2]  - Vec3.m_V[2]);}
  inline const xVec3<XXX>  operator *  (const xVec3<XXX>& Vec3) const {return xVec3(m_V[0]  * Vec3.m_V[0], m_V[1]  * Vec3.m_V[1], m_V[2]  * Vec3.m_V[2]);}
  inline const xVec3<XXX>  operator /  (const xVec3<XXX>& Vec3) const {return xVec3(m_V[0]  / Vec3.m_V[0], m_V[1]  / Vec3.m_V[1], m_V[2]  / Vec3.m_V[2]);}
  inline const xVec3<XXX>  operator %  (const xVec3<XXX>& Vec3) const {return xVec3(m_V[0]  % Vec3.m_V[0], m_V[1]  % Vec3.m_V[1], m_V[2]  % Vec3.m_V[2]);}
  inline const xVec3<XXX>  operator>>  (const xVec3<XXX>& Vec3) const {return xVec3(m_V[0] >> Vec3.m_V[0], m_V[1] >> Vec3.m_V[1], m_V[2] >> Vec3.m_V[2]);}
  inline const xVec3<XXX>  operator<<  (const xVec3<XXX>& Vec3) const {return xVec3(m_V[0] << Vec3.m_V[0], m_V[1] << Vec3.m_V[1], m_V[2] << Vec3.m_V[2]);}

  inline const xVec3<XXX>  operator +  (const XXX i) const            {return xVec3(m_V[0]  + i,           m_V[1]  + i,           m_V[2]  + i          );}
  inline const xVec3<XXX>  operator -  (const XXX i) const            {return xVec3(m_V[0]  - i,           m_V[1]  - i,           m_V[2]  - i          );}
  inline const xVec3<XXX>  operator *  (const XXX i) const            {return xVec3(m_V[0]  * i,           m_V[1]  * i,           m_V[2]  * i          );}
  inline const xVec3<XXX>  operator /  (const XXX i) const            {return xVec3(m_V[0]  / i,           m_V[1]  / i,           m_V[2]  / i          );}
  inline const xVec3<XXX>  operator<<  (const XXX i) const            {return xVec3(m_V[0] << i,           m_V[1] << i,           m_V[2] << i          );}
  inline const xVec3<XXX>  operator>>  (const XXX i) const            {return xVec3(m_V[0] >> i,           m_V[1] >> i,           m_V[2] >> i          );}
  inline const xVec3<XXX>  operator &  (const XXX i) const            {return xVec3(m_V[0]  & i,           m_V[1]  & i,           m_V[2]  & i          );}
  inline const xVec3<XXX>  operator |  (const XXX i) const            {return xVec3(m_V[0]  | i,           m_V[1]  | i,           m_V[2]  | i          );}
  inline const xVec3<XXX>  operator ^  (const XXX i) const            {return xVec3(m_V[0]  ^ i,           m_V[1]  ^ i,           m_V[2]  ^ i          );}

  inline bool              operator==  (const xVec3<XXX>& Vec3) const {return (m_V[0]==Vec3.m_V[0] && m_V[1]==Vec3.m_V[1] && m_V[2]==Vec3.m_V[2]);}
  inline bool              operator!=  (const xVec3<XXX>& Vec3) const {return (m_V[0]!=Vec3.m_V[0] || m_V[1]!=Vec3.m_V[1] || m_V[2]!=Vec3.m_V[2]);}
  inline bool              operator==  (const XXX i) const            {return (m_V[0]==i           && m_V[1]==i           && m_V[2]==i          );}
  inline bool              operator!=  (const XXX i) const            {return (m_V[0]!=i           || m_V[1]!=i           || m_V[2]!=i          );}

  inline XXX&              operator[]  (int32 Idx)       {return m_V[Idx];}
  inline const XXX&        operator[]  (int32 Idx) const {return m_V[Idx];}

  inline bool  isZero()    const { return (m_V[0]==0 && m_V[1]==0 && m_V[2]==0); }
  inline bool  isNonZero() const { return (m_V[0]!=0 && m_V[1]!=0 && m_V[2]!=0); } 

  inline bool  isInsideSize  (xVec3<XXX> Size) const { return (m_V[0] >= 0 && m_V[0] < Size[0] && m_V[1] >= 0 && m_V[1] < Size[1] && m_V[2] >= 0 && m_V[2] < Size[2]); }
  inline bool  isInsideRange (XXX MinX, XXX MaxX, XXX MinY, XXX MaxY, XXX MinZ, XXX MaxZ) const {return (m_V[0]>=MinX && m_V[0]<=MaxX && m_V[1]>=MinY && m_V[1]<=MaxY && m_V[2]>=MinZ && m_V[2]<=MaxZ);}
  inline bool  isOutsideRange(XXX MinX, XXX MaxX, XXX MinY, XXX MaxY, XXX MinZ, XXX MaxZ) const {return (m_V[0]< MinX || m_V[0]> MaxX || m_V[1]< MinY || m_V[1]> MaxY || m_V[2]< MinZ || m_V[2]> MaxZ);}

  inline void  clip (xVec3<XXX>& Min, xVec3<XXX>& Max) { m_V[0] = xClip (m_V[0], Min.m_V[0], Max.m_V[0]); m_V[1] = xClip (m_V[1], Min.m_V[1], Max.m_V[1]); m_V[2] = xClip (m_V[2], Min.m_V[2], Max.m_V[2]); }
  inline void  clipU(                 xVec3<XXX>& Max) { m_V[0] = xClipU(m_V[0],             Max.m_V[0]); m_V[1] = xClipU(m_V[1],             Max.m_V[1]); m_V[2] = xClipU(m_V[2],             Max.m_V[2]); }
};

//=============================================================================================================================================================================

template<typename XXX> class xVec4
{
protected:
  xAligned(4 * sizeof(XXX)) XXX m_V[4];

public:
  inline xVec4() {}
  inline xVec4(const XXX v)                 { m_V[0] = v; m_V[1] = v; m_V[2] = v; m_V[3] = v;}
  inline xVec4(const XXX x, const XXX y, const XXX z, const XXX a)  { m_V[0] = x; m_V[1] = y; m_V[2] = z; m_V[3] = a;}
  inline xVec4(const XXX V[])               { std::memcpy(m_V, V, sizeof(m_V)); }
  inline xVec4(const std::vector<XXX>&   V) { std::memcpy(m_V, V.data(), sizeof(m_V)); }
  inline xVec4(const std::array<XXX, 4>& A) { std::memcpy(m_V, A.data(), sizeof(m_V)); }

  template<typename YYY> explicit operator xVec4<YYY>() const { return xVec4<YYY>((YYY)m_V[0], (YYY)m_V[1], (YYY)m_V[2], (YYY)m_V[3]); }

  inline void  set     (const XXX x, const XXX y, const XXX z, const XXX a) {m_V[0] = x; m_V[1] = y; m_V[2] = z; m_V[3] = a; }
  inline void  setIdx  (const XXX v, const int32 Idx)                       {m_V[Idx] = v; }
  inline void  setX    (const XXX x)                                        {m_V[0] = x;}
  inline void  setY    (const XXX y)                                        {m_V[1] = y;}
  inline void  setZ    (const XXX z)                                        {m_V[2] = z;}
  inline void  setA    (const XXX a)                                        {m_V[3] = a;}
  inline void  setZero ()                                                   {std::memset(m_V, 0, sizeof(m_V));}

  inline XXX   getIdx    (const int32 Idx) const {return m_V[Idx]; }
  inline void  get       (XXX* V         ) const { std::memcpy(V, m_V, sizeof(m_V)); }
  inline XXX   getX      ()                const {return m_V[0];}
  inline XXX   getY      ()                const {return m_V[1];}
  inline XXX   getZ      ()                const {return m_V[2];}
  inline XXX   getA      ()                const {return m_V[3];}
  inline xVec4<XXX> getAbs() const { return xVec4<XXX>(xAbs(m_V[0]), xAbs(m_V[1]), xAbs(m_V[2]), xAbs(m_V[3])); }
  inline XXX   getAbsX   ()                const {return xAbs(m_V[0]);}
  inline XXX   getAbsY   ()                const {return xAbs(m_V[1]);}
  inline XXX   getAbsZ   ()                const {return xAbs(m_V[2]);}
  inline XXX   getAbsA   ()                const {return xAbs(m_V[3]);}
  inline XXX   getMax    ()                const {return xMax(xMax(m_V[0], m_V[1]), xMax(m_V[2], m_V[3]));}
  inline XXX   getMin    ()                const {return xMin(xMin(m_V[0], m_V[1]), xMin(m_V[2], m_V[3]));}
  inline XXX   getMaxAbs ()                const {return xMax(xMax(xAbs(m_V[0]), xAbs(m_V[1])), xMax(xAbs(m_V[2]), xAbs(m_V[3])));}
  inline XXX   getMinAbs ()                const {return xMin(xMin(xAbs(m_V[0]), xAbs(m_V[1])), xMin(xAbs(m_V[2]), xAbs(m_V[3])));}
  inline XXX   getSum    ()                const {return (m_V[0] + m_V[1] + m_V[2] + m_V[3]);}
  inline XXX   getSumSqrs()                const {return (xPow2(m_V[0]) + xPow2(m_V[1]) + xPow2(m_V[2]) + zPow2(m_V[3]));}
  inline XXX   getSumAbs ()                const {return (xAbs(m_V[0]) + xAbs(m_V[1]) + xAbs(m_V[2]) + xAbs(m_V[3]));}
  inline XXX   getMul    ()                const {return m_V[0] * m_V[1] * m_V[2] * m_V[3];}

  inline const xVec4<XXX>& operator += (const xVec4<XXX>& Vec4)       {m_V[0]  += Vec4.m_V[0]; m_V[1]  += Vec4.m_V[1]; m_V[2]  += Vec4.m_V[2]; m_V[3]  += Vec4.m_V[3]; return *this;}  
  inline const xVec4<XXX>& operator -= (const xVec4<XXX>& Vec4)       {m_V[0]  -= Vec4.m_V[0]; m_V[1]  -= Vec4.m_V[1]; m_V[2]  -= Vec4.m_V[2]; m_V[3]  -= Vec4.m_V[3]; return *this;}
  inline const xVec4<XXX>& operator *= (const xVec4<XXX>& Vec4)       {m_V[0]  *= Vec4.m_V[0]; m_V[1]  *= Vec4.m_V[1]; m_V[2]  *= Vec4.m_V[2]; m_V[3]  *= Vec4.m_V[3]; return *this;}
  inline const xVec4<XXX>& operator /= (const xVec4<XXX>& Vec4)       {m_V[0]  /= Vec4.m_V[0]; m_V[1]  /= Vec4.m_V[1]; m_V[2]  /= Vec4.m_V[2]; m_V[3]  /= Vec4.m_V[3]; return *this;}
  inline const xVec4<XXX>& operator %= (const xVec4<XXX>& Vec4)       {m_V[0]  %= Vec4.m_V[0]; m_V[1]  %= Vec4.m_V[1]; m_V[2]  %= Vec4.m_V[2]; m_V[3]  %= Vec4.m_V[3]; return *this;}
  inline const xVec4<XXX>& operator>>= (const xVec4<XXX>& Vec4)       {m_V[0] >>= Vec4.m_V[0]; m_V[1] >>= Vec4.m_V[1]; m_V[2] >>= Vec4.m_V[2]; m_V[3] >>= Vec4.m_V[3]; return *this;}
  inline const xVec4<XXX>& operator<<= (const xVec4<XXX>& Vec4)       {m_V[0] <<= Vec4.m_V[0]; m_V[1] <<= Vec4.m_V[1]; m_V[2] <<= Vec4.m_V[2]; m_V[3] <<= Vec4.m_V[3]; return *this;}

  inline const xVec4<XXX>& operator += (const XXX i)                  {m_V[0]  += i;           m_V[1]  += i;           m_V[2]  += i;           m_V[3]  += i;           return *this;}
  inline const xVec4<XXX>& operator -= (const XXX i)                  {m_V[0]  -= i;           m_V[1]  -= i;           m_V[2]  -= i;           m_V[3]  -= i;           return *this;}
  inline const xVec4<XXX>& operator *= (const XXX i)                  {m_V[0]  *= i;           m_V[1]  *= i;           m_V[2]  *= i;           m_V[3]  *= i;           return *this;}
  inline const xVec4<XXX>& operator /= (const XXX i)                  {m_V[0]  /= i;           m_V[1]  /= i;           m_V[2]  /= i;           m_V[3]  /= i;           return *this;}
  inline const xVec4<XXX>& operator %= (const XXX i)                  {m_V[0]  %= i;           m_V[1]  %= i;           m_V[2]  %= i;           m_V[3]  %= i;           return *this;}
  inline const xVec4<XXX>& operator>>= (const XXX i)                  {m_V[0] >>= i;           m_V[1] >>= i;           m_V[2] >>= i;           m_V[3] >>= i;           return *this;}  
  inline const xVec4<XXX>& operator<<= (const XXX i)                  {m_V[0] <<= i;           m_V[1] <<= i;           m_V[2] <<= i;           m_V[3] <<= i;           return *this;}  
  inline const xVec4<XXX>& operator &= (const XXX i)                  {m_V[0]  &= i;           m_V[1]  &= i;           m_V[2]  &= i;           m_V[3]  &= i;           return *this;}
  inline const xVec4<XXX>& operator |= (const XXX i)                  {m_V[0]  |= i;           m_V[1]  |= i;           m_V[2]  |= i;           m_V[3]  |= i;           return *this;}
  inline const xVec4<XXX>& operator ^= (const XXX i)                  {m_V[0]  ^= i;           m_V[1]  ^= i;           m_V[2]  ^= i;           m_V[3]  ^= i;           return *this;}

  inline const xVec4<XXX>  operator +  (const xVec4<XXX>& Vec4) const {return xVec4(m_V[0]  + Vec4.m_V[0], m_V[1]  + Vec4.m_V[1], m_V[2]  + Vec4.m_V[2], m_V[3]  + Vec4.m_V[3]);}  
  inline const xVec4<XXX>  operator -  (const xVec4<XXX>& Vec4) const {return xVec4(m_V[0]  - Vec4.m_V[0], m_V[1]  - Vec4.m_V[1], m_V[2]  - Vec4.m_V[2], m_V[3]  - Vec4.m_V[3]);}
  inline const xVec4<XXX>  operator *  (const xVec4<XXX>& Vec4) const {return xVec4(m_V[0]  * Vec4.m_V[0], m_V[1]  * Vec4.m_V[1], m_V[2]  * Vec4.m_V[2], m_V[3]  * Vec4.m_V[3]);}
  inline const xVec4<XXX>  operator /  (const xVec4<XXX>& Vec4) const {return xVec4(m_V[0]  / Vec4.m_V[0], m_V[1]  / Vec4.m_V[1], m_V[2]  / Vec4.m_V[2], m_V[3]  / Vec4.m_V[3]);}
  inline const xVec4<XXX>  operator %  (const xVec4<XXX>& Vec4) const {return xVec4(m_V[0]  % Vec4.m_V[0], m_V[1]  % Vec4.m_V[1], m_V[2]  % Vec4.m_V[2], m_V[3]  % Vec4.m_V[3]);}
  inline const xVec4<XXX>  operator>>  (const xVec4<XXX>& Vec4) const {return xVec4(m_V[0] >> Vec4.m_V[0], m_V[1] >> Vec4.m_V[1], m_V[2] >> Vec4.m_V[2], m_V[3] >> Vec4.m_V[3]);}
  inline const xVec4<XXX>  operator<<  (const xVec4<XXX>& Vec4) const {return xVec4(m_V[0] << Vec4.m_V[0], m_V[1] << Vec4.m_V[1], m_V[2] << Vec4.m_V[2], m_V[3] << Vec4.m_V[3]);}

  inline const xVec4<XXX>  operator +  (const XXX i) const            {return xVec4(m_V[0]  + i,           m_V[1]  + i,           m_V[2]  + i,           m_V[3]  + i          );}
  inline const xVec4<XXX>  operator -  (const XXX i) const            {return xVec4(m_V[0]  - i,           m_V[1]  - i,           m_V[2]  - i,           m_V[3]  - i          );}
  inline const xVec4<XXX>  operator *  (const XXX i) const            {return xVec4(m_V[0]  * i,           m_V[1]  * i,           m_V[2]  * i,           m_V[3]  * i          );}
  inline const xVec4<XXX>  operator /  (const XXX i) const            {return xVec4(m_V[0]  / i,           m_V[1]  / i,           m_V[2]  / i,           m_V[3]  / i          );}
  inline const xVec4<XXX>  operator %  (const XXX i) const            {return xVec4(m_V[0]  % i,           m_V[1]  % i,           m_V[2]  % i,           m_V[3]  % i          );}
  inline const xVec4<XXX>  operator<<  (const XXX i) const            {return xVec4(m_V[0] << i,           m_V[1] << i,           m_V[2] << i,           m_V[3] << i          );}
  inline const xVec4<XXX>  operator>>  (const XXX i) const            {return xVec4(m_V[0] >> i,           m_V[1] >> i,           m_V[2] >> i,           m_V[3] >> i          );}
  inline const xVec4<XXX>  operator &  (const XXX i) const            {return xVec4(m_V[0]  & i,           m_V[1]  & i,           m_V[2]  & i,           m_V[3]  & i          );}
  inline const xVec4<XXX>  operator |  (const XXX i) const            {return xVec4(m_V[0]  | i,           m_V[1]  | i,           m_V[2]  | i,           m_V[3]  | i          );}
  inline const xVec4<XXX>  operator ^  (const XXX i) const            {return xVec4(m_V[0]  ^ i,           m_V[1]  ^ i,           m_V[2]  ^ i,           m_V[3]  ^ i          );}

  inline bool              operator==  (const xVec4<XXX>& Vec4) const {return (m_V[0]==Vec4.m_V[0] && m_V[1]==Vec4.m_V[1] && m_V[2]==Vec4.m_V[2] && m_V[3]==Vec4.m_V[3]);}
  inline bool              operator!=  (const xVec4<XXX>& Vec4) const {return (m_V[0]!=Vec4.m_V[0] || m_V[1]!=Vec4.m_V[1] || m_V[2]!=Vec4.m_V[2] || m_V[3]!=Vec4.m_V[3]);}
  inline bool              operator==  (const XXX i) const            {return (m_V[0]==i           && m_V[1]==i           && m_V[2]==i           && m_V[3]==i          );}
  inline bool              operator!=  (const XXX i) const            {return (m_V[0]!=i           || m_V[1]!=i           || m_V[2]!=i           || m_V[3]==i          );}

  inline XXX&              operator[]  (int32 Idx)       {return m_V[Idx];}
  inline const XXX&        operator[]  (int32 Idx) const {return m_V[Idx];}

  inline bool  isZero()    const { return (m_V[0]==0 && m_V[1]==0 && m_V[2]==0 && m_V[3]==0); }
  inline bool  isNonZero() const { return (m_V[0]!=0 && m_V[1]!=0 && m_V[2]!=0 && m_V[3]!=0); } 

  inline bool  isInsideRange (XXX MinX, XXX MaxX, XXX MinY, XXX MaxY, XXX MinZ, XXX MaxZ, XXX MinA, XXX MaxA) const {return (m_V[0]>=MinX && m_V[0]<=MaxX && m_V[1]>=MinY && m_V[1]<=MaxY && m_V[2]>=MinZ && m_V[2]<=MaxZ && m_V[3]>=MinA && m_V[3]<=MaxA);}
  inline bool  isOutsideRange(XXX MinX, XXX MaxX, XXX MinY, XXX MaxY, XXX MinZ, XXX MaxZ, XXX MinA, XXX MaxA) const {return (m_V[0]< MinX || m_V[0]> MaxX || m_V[1]< MinY || m_V[1]> MaxY || m_V[2]< MinZ || m_V[2]> MaxZ || m_V[3]< MinA || m_V[3]> MaxA);}

  inline void  clip (xVec4<XXX>& Min, xVec4<XXX>& Max) { m_V[0] = xClip (m_V[0], Min.m_V[0], Max.m_V[0]); m_V[1] = xClip (m_V[1], Min.m_V[1], Max.m_V[1]); m_V[2] = xClip (m_V[2], Min.m_V[2], Max.m_V[2]); m_V[3] = xClip (m_V[3], Min.m_V[3], Max.m_V[3]); }
  inline void  clipU(                 xVec4<XXX>& Max) { m_V[0] = xClipU(m_V[0],             Max.m_V[0]); m_V[1] = xClipU(m_V[1],             Max.m_V[1]); m_V[2] = xClipU(m_V[2],             Max.m_V[2]); m_V[3] = xClipU(m_V[3],             Max.m_V[3]); }
};

//=============================================================================================================================================================================

typedef xVec2<  bool>   boolV2;
typedef xVec2<  int8>   int8V2;
typedef xVec2< uint8>  uint8V2;
typedef xVec2< int16>  int16V2;
typedef xVec2<uint16> uint16V2;
typedef xVec2< int32>  int32V2;
typedef xVec2<uint32> uint32V2;
typedef xVec2< int64>  int64V2;
typedef xVec2<uint64> uint64V2;
typedef xVec2< float>  floatV2;
typedef xVec2< flt32>  flt32V2;
typedef xVec2<double> doubleV2;
typedef xVec2< flt64>  flt64V2;

typedef xVec3<  bool>   boolV3;
typedef xVec3<  int8>   int8V3;
typedef xVec3< uint8>  uint8V3;
typedef xVec3< int16>  int16V3;
typedef xVec3<uint16> uint16V3;
typedef xVec3< int32>  int32V3;
typedef xVec3<uint32> uint32V3;
typedef xVec3< int64>  int64V3;
typedef xVec3<uint64> uint64V3;
typedef xVec3< float>  floatV3;
typedef xVec3< flt32>  flt32V3;
typedef xVec3<double> doubleV3;
typedef xVec3< flt64>  flt64V3;

typedef xVec4<  bool>   boolV4;
typedef xVec4<  int8>   int8V4;
typedef xVec4< uint8>  uint8V4;
typedef xVec4< int16>  int16V4;
typedef xVec4<uint16> uint16V4;
typedef xVec4< int32>  int32V4;
typedef xVec4<uint32> uint32V4;
typedef xVec4< int64>  int64V4;
typedef xVec4<uint64> uint64V4;
typedef xVec4< float>  floatV4;
typedef xVec4< flt32>  flt32V4;
typedef xVec4<double> doubleV4;
typedef xVec4< flt64>  flt64V4;

//=============================================================================================================================================================================

template <class XXX> static inline int32V2 xRoundFltToInt32(xVec2<XXX> FltV);
template <> inline int32V2 xRoundFltToInt32(xVec2<flt32> FltV) { return { xRoundFloatToInt32 (FltV[0]), xRoundFloatToInt32 (FltV[1])}; }
template <> inline int32V2 xRoundFltToInt32(xVec2<flt64> FltV) { return { xRoundDoubleToInt32(FltV[0]), xRoundDoubleToInt32(FltV[1])}; }

template <class XXX> static inline int32V3 xRoundFltToInt32(xVec3<XXX> FltV);
template <> inline int32V3 xRoundFltToInt32(xVec3<flt32> FltV) { return { xRoundFloatToInt32 (FltV[0]), xRoundFloatToInt32 (FltV[1]), xRoundFloatToInt32 (FltV[2])}; }
template <> inline int32V3 xRoundFltToInt32(xVec3<flt64> FltV) { return { xRoundDoubleToInt32(FltV[0]), xRoundDoubleToInt32(FltV[1]), xRoundDoubleToInt32(FltV[2])}; }

template <class XXX> static inline int32V4 xRoundFltToInt32(xVec4<XXX> FltV);
template <> inline int32V4 xRoundFltToInt32(xVec4<flt32> FltV) { return { xRoundFloatToInt32 (FltV[0]), xRoundFloatToInt32 (FltV[1]), xRoundFloatToInt32 (FltV[2]), xRoundFloatToInt32 (FltV[3])}; }
template <> inline int32V4 xRoundFltToInt32(xVec4<flt64> FltV) { return { xRoundDoubleToInt32(FltV[0]), xRoundDoubleToInt32(FltV[1]), xRoundDoubleToInt32(FltV[2]), xRoundDoubleToInt32(FltV[3])}; }

//=============================================================================================================================================================================

} //end of namespace AVLib

