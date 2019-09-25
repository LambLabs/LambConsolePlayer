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

#define AVlib_xColorSpace_IMPLEMENTATION
#include "xColorSpace.h"

namespace AVlib {

//===============================================================================================================================================================================================================

template class xColorSpaceCoeff<int32>;
template class xColorSpaceCoeff<int64>;
template class xColorSpaceCoeff<flt32>;
template class xColorSpaceCoeff<flt64>;

//===============================================================================================================================================================================================================

#if X_SSE4_2
void xColorSpaceSSE::ConvertDEF2ABC(int16* restrict A, int16* restrict B, int16* restrict C, const int16* restrict D, const int16* restrict E, const int16* restrict F, uint32 Area, const int32 ConvertMatrix[9])
{
  const __m128i A_D = _mm_set1_epi32(ConvertMatrix[0]);
  const __m128i A_E = _mm_set1_epi32(ConvertMatrix[1]);
  const __m128i A_F = _mm_set1_epi32(ConvertMatrix[2]);
  const __m128i B_D = _mm_set1_epi32(ConvertMatrix[3]);
  const __m128i B_E = _mm_set1_epi32(ConvertMatrix[4]);
  const __m128i B_F = _mm_set1_epi32(ConvertMatrix[5]);
  const __m128i C_D = _mm_set1_epi32(ConvertMatrix[6]);
  const __m128i C_E = _mm_set1_epi32(ConvertMatrix[7]);
  const __m128i C_F = _mm_set1_epi32(ConvertMatrix[8]);
  const __m128i c32768 = _mm_set1_epi32((int32)32768);

  for(uint32 i=0; i<Area; i+=8)
  {
    //load
    __m128i d = _mm_load_si128((__m128i*)(&(D[i])));
    __m128i e = _mm_load_si128((__m128i*)(&(E[i])));
    __m128i f = _mm_load_si128((__m128i*)(&(F[i])));

    //conver to int32
    __m128i d_0 = _mm_cvtepi16_epi32(d);
    __m128i d_1 = _mm_cvtepi16_epi32(_mm_srli_si128(d,8));
    __m128i e_0 = _mm_cvtepi16_epi32(e);
    __m128i e_1 = _mm_cvtepi16_epi32(_mm_srli_si128(e,8));
    __m128i f_0 = _mm_cvtepi16_epi32(f);
    __m128i f_1 = _mm_cvtepi16_epi32(_mm_srli_si128(f,8));

    //calc YUV
    __m128i a_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(A_D, d_0), _mm_mullo_epi32(A_E, e_0)), _mm_add_epi32(_mm_mullo_epi32(A_F, f_0), c32768)), 16);
    __m128i a_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(A_D, d_1), _mm_mullo_epi32(A_E, e_1)), _mm_add_epi32(_mm_mullo_epi32(A_F, f_1), c32768)), 16);
    __m128i b_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(B_D, d_0), _mm_mullo_epi32(B_E, e_0)), _mm_add_epi32(_mm_mullo_epi32(B_F, f_0), c32768)), 16);
    __m128i b_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(B_D, d_1), _mm_mullo_epi32(B_E, e_1)), _mm_add_epi32(_mm_mullo_epi32(B_F, f_1), c32768)), 16);
    __m128i c_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(C_D, d_0), _mm_mullo_epi32(C_E, e_0)), _mm_add_epi32(_mm_mullo_epi32(C_F, f_0), c32768)), 16);
    __m128i c_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(C_D, d_1), _mm_mullo_epi32(C_E, e_1)), _mm_add_epi32(_mm_mullo_epi32(C_F, f_1), c32768)), 16);

    //pack to int16
    __m128i a = _mm_packs_epi32(a_0, a_1);
    __m128i b = _mm_packs_epi32(b_0, b_1);
    __m128i c = _mm_packs_epi32(c_0, c_1);

    //save to output component
    _mm_store_si128 ((__m128i*)(&(A[i])), a);
    _mm_store_si128 ((__m128i*)(&(B[i])), b);
    _mm_store_si128 ((__m128i*)(&(C[i])), c);
  }

  if((uint32)Area & (uint32)0x7)
  {
    const int32 A_Ds = ConvertMatrix[0];
    const int32 A_Es = ConvertMatrix[1];
    const int32 A_Fs = ConvertMatrix[2];
    const int32 B_Ds = ConvertMatrix[3];
    const int32 B_Es = ConvertMatrix[4];
    const int32 B_Fs = ConvertMatrix[5];
    const int32 C_Ds = ConvertMatrix[6];
    const int32 C_Es = ConvertMatrix[7];
    const int32 C_Fs = ConvertMatrix[8];

    for(uint32 i=((uint32)Area & (uint32)0xFFFFFFF8); i<(uint32)Area; i++)
    {
      int32 d = D[i];
      int32 e = E[i];
      int32 f = F[i];
      int32 a = ((A_Ds*d + A_Es*e + A_Fs*f + 32768)>>16);
      int32 b = ((B_Ds*d + B_Es*e + B_Fs*f + 32768)>>16);
      int32 c = ((C_Ds*d + C_Es*e + C_Fs*f + 32768)>>16);
      A[i] = (int16)a;
      B[i] = (int16)b;
      C[i] = (int16)c;
    }
  }
}
void xColorSpaceSSE::ConvertRGB2YUV_BT601(int16* restrict Y, int16* restrict U, int16* restrict V, const int16* restrict R, const int16* restrict G, const int16* restrict B, uint32 Area, uint32 BitDepth)
{
  const __m128i Y_R    = _mm_set1_epi32((int32)(65536 * 0.29900 + 0.5));
  const __m128i Y_G    = _mm_set1_epi32((int32)(65536 * 0.58700 + 0.5));
  const __m128i Y_B    = _mm_set1_epi32((int32)(65536 * 0.11400 + 0.5));
  const __m128i U_R    = _mm_set1_epi32((int32)(65536 *-0.16874 + 0.5));
  const __m128i U_G    = _mm_set1_epi32((int32)(65536 *-0.33126 + 0.5));
  const __m128i U_B_V_R= _mm_set1_epi32((int32)(65536 * 0.50000 + 0.5));
  const __m128i V_G    = _mm_set1_epi32((int32)(65536 *-0.41869 + 0.5));
  const __m128i V_B    = _mm_set1_epi32((int32)(65536 *-0.04585 + 0.5));
  const __m128i c32768 = _mm_set1_epi32((int32)32768);
  const int32   Mid    = (int16)xBitDepth2MidValue(BitDepth);
  const int32   Max    = (int16)xBitDepth2MaxValue(BitDepth);

  int32 Area8 = (int32)((uint32)Area & (uint32)0xFFFFFFF8);
  for(int32 i=0; i<Area8; i+=8)
  {
    //load
    __m128i r = _mm_load_si128((__m128i*)(&(R[i])));
    __m128i g = _mm_load_si128((__m128i*)(&(G[i])));
    __m128i b = _mm_load_si128((__m128i*)(&(B[i])));

    //conver to int32
    __m128i r_0 = _mm_cvtepi16_epi32(r);
    __m128i r_1 = _mm_cvtepi16_epi32(_mm_srli_si128(r,8));
    __m128i g_0 = _mm_cvtepi16_epi32(g);
    __m128i g_1 = _mm_cvtepi16_epi32(_mm_srli_si128(g,8));
    __m128i b_0 = _mm_cvtepi16_epi32(b);
    __m128i b_1 = _mm_cvtepi16_epi32(_mm_srli_si128(b,8));

    //calc YUV
    __m128i y_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(Y_R,     r_0), _mm_mullo_epi32(Y_G, g_0)), _mm_add_epi32(_mm_mullo_epi32(Y_B,     b_0), c32768)), 16);
    __m128i y_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(Y_R,     r_1), _mm_mullo_epi32(Y_G, g_1)), _mm_add_epi32(_mm_mullo_epi32(Y_B,     b_1), c32768)), 16);
    __m128i u_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(U_R,     r_0), _mm_mullo_epi32(U_G, g_0)), _mm_add_epi32(_mm_mullo_epi32(U_B_V_R, b_0), c32768)), 16);
    __m128i u_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(U_R,     r_1), _mm_mullo_epi32(U_G, g_1)), _mm_add_epi32(_mm_mullo_epi32(U_B_V_R, b_1), c32768)), 16);
    __m128i v_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(U_B_V_R, r_0), _mm_mullo_epi32(V_G, g_0)), _mm_add_epi32(_mm_mullo_epi32(V_B,     b_0), c32768)), 16);
    __m128i v_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(U_B_V_R, r_1), _mm_mullo_epi32(V_G, g_1)), _mm_add_epi32(_mm_mullo_epi32(V_B,     b_1), c32768)), 16);

    //pack to int16
    __m128i y = _mm_packs_epi32(y_0, y_1);
    __m128i u = _mm_packs_epi32(u_0, u_1);
    __m128i v = _mm_packs_epi32(v_0, v_1);

    //add half
    const __m128i MidV = _mm_set1_epi16((int16)Mid);
    u = _mm_add_epi16(u, MidV);
    v = _mm_add_epi16(v, MidV);

    //clip
    const __m128i ZeroV = _mm_setzero_si128();
    const __m128i MaxV  = _mm_set1_epi16((int16)Max);
    __m128i cy = _mm_max_epi16(_mm_min_epi16(y, MaxV), ZeroV);
    __m128i cu = _mm_max_epi16(_mm_min_epi16(u, MaxV), ZeroV);
    __m128i cv = _mm_max_epi16(_mm_min_epi16(v, MaxV), ZeroV);

    //save to output component
    _mm_store_si128 ((__m128i*)(&(Y[i])), cy);
    _mm_store_si128 ((__m128i*)(&(U[i])), cu);
    _mm_store_si128 ((__m128i*)(&(V[i])), cv);
  }

  for(uint32 i=Area8; i<Area; i++)
  {
    int32 r = R[i];
    int32 g = G[i];
    int32 b = B[i];
    int32 y = ((((int32)(65536 * 0.29900 + 0.5))*r + ((int32)(65536 * 0.58700 + 0.5))*g + ((int32)(65536 * 0.11400 + 0.5))*b + 32768)>>16);
    int32 u = ((((int32)(65536 *-0.16874 + 0.5))*r + ((int32)(65536 *-0.33126 + 0.5))*g + ((int32)(65536 * 0.50000 + 0.5))*b + 32768)>>16);
    int32 v = ((((int32)(65536 * 0.50000 + 0.5))*r + ((int32)(65536 *-0.41869 + 0.5))*g + ((int32)(65536 *-0.04585 + 0.5))*b + 32768)>>16);    
    int32 cy = xClipU(y      , Max);
    int32 cu = xClipU(u + Mid, Max);
    int32 cv = xClipU(v + Mid, Max);
    Y[i] = (int16)cy;
    U[i] = (int16)cu;
    V[i] = (int16)cv;
  }
}
void xColorSpaceSSE::ConvertYUV2RGB_BT601(int16* restrict R, int16* restrict G, int16* restrict B, const int16* restrict Y, const int16* restrict U, const int16* restrict V, uint32 Area, uint32 BitDepth)
{
  const __m128i R_V    = _mm_set1_epi32(((int32)(65536 * 1.40200 + 0.5)));
  const __m128i G_U    = _mm_set1_epi32(((int32)(65536 *-0.34414 + 0.5)));
  const __m128i G_V    = _mm_set1_epi32(((int32)(65536 *-0.71414 + 0.5)));
  const __m128i B_U    = _mm_set1_epi32(((int32)(65536 * 1.77200 + 0.5)));
  const __m128i c32768 = _mm_set1_epi32((int32)32768);
  const int32   Mid    = (int16)xBitDepth2MidValue(BitDepth);
  const int32   Max    = (int16)xBitDepth2MaxValue(BitDepth);
  const __m128i MidV   = _mm_set1_epi16((int16)Mid);
  const __m128i MaxV   = _mm_set1_epi16((int16)Max);

  int32 Area8 = (int32)((uint32)Area & (uint32)0xFFFFFFF8);
  for(int32 i=0; i<Area8; i+=8)
  {
    //load
    __m128i y = _mm_load_si128((__m128i*)(&(Y[i])));
    __m128i u = _mm_load_si128((__m128i*)(&(U[i])));
    __m128i v = _mm_load_si128((__m128i*)(&(V[i])));

    //sub half
    u = _mm_sub_epi16(u, MidV);
    v = _mm_sub_epi16(v, MidV);

    //conver to int32
    __m128i y_0 = _mm_cvtepi16_epi32(y);
    __m128i y_1 = _mm_cvtepi16_epi32(_mm_srli_si128(y,8));
    __m128i u_0 = _mm_cvtepi16_epi32(u);
    __m128i u_1 = _mm_cvtepi16_epi32(_mm_srli_si128(u,8));
    __m128i v_0 = _mm_cvtepi16_epi32(v);
    __m128i v_1 = _mm_cvtepi16_epi32(_mm_srli_si128(v,8));

    //calc YUV
    y_0 = _mm_add_epi32(_mm_slli_epi32(y_0, 16), c32768);
    y_1 = _mm_add_epi32(_mm_slli_epi32(y_1, 16), c32768);

    __m128i r_0 = _mm_srai_epi32(_mm_add_epi32(y_0, _mm_mullo_epi32(R_V, v_0)), 16);
    __m128i r_1 = _mm_srai_epi32(_mm_add_epi32(y_1, _mm_mullo_epi32(R_V, v_1)), 16);
    __m128i g_0 = _mm_srai_epi32(_mm_add_epi32(y_0, _mm_add_epi32(_mm_mullo_epi32(G_U, u_0), _mm_mullo_epi32(G_V, v_0))), 16);
    __m128i g_1 = _mm_srai_epi32(_mm_add_epi32(y_1, _mm_add_epi32(_mm_mullo_epi32(G_U, u_1), _mm_mullo_epi32(G_V, v_1))), 16);
    __m128i b_0 = _mm_srai_epi32(_mm_add_epi32(y_0, _mm_mullo_epi32(B_U, u_0)), 16);
    __m128i b_1 = _mm_srai_epi32(_mm_add_epi32(y_1, _mm_mullo_epi32(B_U, u_1)), 16);

    //pack to int16
    __m128i r = _mm_packs_epi32(r_0, r_1);
    __m128i g = _mm_packs_epi32(g_0, g_1);
    __m128i b = _mm_packs_epi32(b_0, b_1);

    //clip
    const __m128i ZeroV  = _mm_setzero_si128();
    __m128i cr = _mm_max_epi16(_mm_min_epi16(r, MaxV), ZeroV);
    __m128i cg = _mm_max_epi16(_mm_min_epi16(g, MaxV), ZeroV);
    __m128i cb = _mm_max_epi16(_mm_min_epi16(b, MaxV), ZeroV);

    //save to output component
    _mm_store_si128 ((__m128i*)(&(R[i])), cr);
    _mm_store_si128 ((__m128i*)(&(G[i])), cg);
    _mm_store_si128 ((__m128i*)(&(B[i])), cb);
  }

  for(uint32 i=Area8; i<Area; i++)
  {
    int32 y = (((int32)Y[i])<<16) + 32768;
    int32 u  = (int32)(U[i]) - Mid;
    int32 v  = (int32)(V[i]) - Mid;
    int32 r = (                                      + ((int32)(65536 *  1.40200 + 0.5)) * v + y)>>16;
    int32 g = (((int32)(65536 * -0.34414 + 0.5)) * u + ((int32)(65536 * -0.71414 + 0.5)) * v + y)>>16;
    int32 b = (((int32)(65536 *  1.77200 + 0.5)) * u                                         + y)>>16;    
    int32 cr = xClipU(r, Max);
    int32 cg = xClipU(g, Max);
    int32 cb = xClipU(b, Max);
    R[i] = (int16)cr;
    G[i] = (int16)cg;
    B[i] = (int16)cb;
  }
}
void xColorSpaceSSE::ConvertRGB2YUV_BT709(int16* restrict Y, int16* restrict U, int16* restrict V, const int16* restrict R, const int16* restrict G, const int16* restrict B, uint32 Area, uint32 BitDepth)
{
  const __m128i Y_R    = _mm_set1_epi32(((int32)(65536 * 0.21260 + 0.5)));
  const __m128i Y_G    = _mm_set1_epi32(((int32)(65536 * 0.71520 + 0.5)));
  const __m128i Y_B    = _mm_set1_epi32(((int32)(65536 * 0.07220 + 0.5)));
  const __m128i U_R    = _mm_set1_epi32(((int32)(65536 *-0.11457 + 0.5)));
  const __m128i U_G    = _mm_set1_epi32(((int32)(65536 *-0.38543 + 0.5)));
  const __m128i U_B_V_R= _mm_set1_epi32(((int32)(65536 * 0.50000 + 0.5)));
  const __m128i V_G    = _mm_set1_epi32(((int32)(65536 *-0.45415 + 0.5)));
  const __m128i V_B    = _mm_set1_epi32(((int32)(65536 *-0.04585 + 0.5)));
  const __m128i c32768 = _mm_set1_epi32((int32)32768);
  const int32   Mid    = (int16)xBitDepth2MidValue(BitDepth);
  const int32   Max    = (int16)xBitDepth2MaxValue(BitDepth);

  int32 Area8 = (int32)((uint32)Area & (uint32)0xFFFFFFF8);
  for(int32 i=0; i<Area8; i+=8)
  {
    //load
    __m128i r = _mm_load_si128((__m128i*)(&(R[i])));
    __m128i g = _mm_load_si128((__m128i*)(&(G[i])));
    __m128i b = _mm_load_si128((__m128i*)(&(B[i])));

    //conver to int32
    __m128i r_0 = _mm_cvtepi16_epi32(r);
    __m128i r_1 = _mm_cvtepi16_epi32(_mm_srli_si128(r,8));
    __m128i g_0 = _mm_cvtepi16_epi32(g);
    __m128i g_1 = _mm_cvtepi16_epi32(_mm_srli_si128(g,8));
    __m128i b_0 = _mm_cvtepi16_epi32(b);
    __m128i b_1 = _mm_cvtepi16_epi32(_mm_srli_si128(b,8));

    //calc YUV
    __m128i y_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(Y_R,     r_0), _mm_mullo_epi32(Y_G, g_0)), _mm_add_epi32(_mm_mullo_epi32(Y_B,     b_0), c32768)), 16);
    __m128i y_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(Y_R,     r_1), _mm_mullo_epi32(Y_G, g_1)), _mm_add_epi32(_mm_mullo_epi32(Y_B,     b_1), c32768)), 16);
    __m128i u_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(U_R,     r_0), _mm_mullo_epi32(U_G, g_0)), _mm_add_epi32(_mm_mullo_epi32(U_B_V_R, b_0), c32768)), 16);
    __m128i u_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(U_R,     r_1), _mm_mullo_epi32(U_G, g_1)), _mm_add_epi32(_mm_mullo_epi32(U_B_V_R, b_1), c32768)), 16);
    __m128i v_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(U_B_V_R, r_0), _mm_mullo_epi32(V_G, g_0)), _mm_add_epi32(_mm_mullo_epi32(V_B,     b_0), c32768)), 16);
    __m128i v_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(U_B_V_R, r_1), _mm_mullo_epi32(V_G, g_1)), _mm_add_epi32(_mm_mullo_epi32(V_B,     b_1), c32768)), 16);

    //pack to int16
    __m128i y = _mm_packs_epi32(y_0, y_1);
    __m128i u = _mm_packs_epi32(u_0, u_1);
    __m128i v = _mm_packs_epi32(v_0, v_1);

    //add half
    const __m128i MidV = _mm_set1_epi16((int16)Mid);
    u = _mm_add_epi16(u, MidV);
    v = _mm_add_epi16(v, MidV);

    //clip
    const __m128i ZeroV = _mm_setzero_si128();
    const __m128i MaxV  = _mm_set1_epi16((int16)Max);
    __m128i cy = _mm_max_epi16(_mm_min_epi16(y, MaxV), ZeroV);
    __m128i cu = _mm_max_epi16(_mm_min_epi16(u, MaxV), ZeroV);
    __m128i cv = _mm_max_epi16(_mm_min_epi16(v, MaxV), ZeroV);

    //save to output component
    _mm_store_si128 ((__m128i*)(&(Y[i])), cy);
    _mm_store_si128 ((__m128i*)(&(U[i])), cu);
    _mm_store_si128 ((__m128i*)(&(V[i])), cv);
  }

  for(uint32 i=Area8; i<Area; i++)
  {
    int32 r = R[i];
    int32 g = G[i];
    int32 b = B[i];
    int32 y = ((((int32)(65536 * 0.21260 + 0.5))*r + ((int32)(65536 * 0.71520 + 0.5))*g + ((int32)(65536 * 0.07220 + 0.5))*b + 32768)>>16);
    int32 u = ((((int32)(65536 *-0.11457 + 0.5))*r + ((int32)(65536 *-0.38543 + 0.5))*g + ((int32)(65536 * 0.50000 + 0.5))*b + 32768)>>16);
    int32 v = ((((int32)(65536 * 0.50000 + 0.5))*r + ((int32)(65536 *-0.45415 + 0.5))*g + ((int32)(65536 *-0.04585 + 0.5))*b + 32768)>>16);    
    int32 cy = xClipU(y      , Max);
    int32 cu = xClipU(u + Mid, Max);
    int32 cv = xClipU(v + Mid, Max);
    Y[i] = (int16)cy;
    U[i] = (int16)cu;
    V[i] = (int16)cv;
  }
}
void xColorSpaceSSE::ConvertYUV2RGB_BT709(int16* restrict R, int16* restrict G, int16* restrict B, const int16* restrict Y, const int16* restrict U, const int16* restrict V, uint32 Area, uint32 BitDepth)
{
  const __m128i R_V    = _mm_set1_epi32(((int32)(65536 * 1.57480 + 0.5)));
  const __m128i G_U    = _mm_set1_epi32(((int32)(65536 *-0.18732 + 0.5)));
  const __m128i G_V    = _mm_set1_epi32(((int32)(65536 *-0.46812 + 0.5)));
  const __m128i B_U    = _mm_set1_epi32(((int32)(65536 * 1.85560 + 0.5)));
  const __m128i c32768 = _mm_set1_epi32((int32)32768);
  const int32   Mid    = (int16)xBitDepth2MidValue(BitDepth);
  const int32   Max    = (int16)xBitDepth2MaxValue(BitDepth);
  const __m128i MidV   = _mm_set1_epi16((int16)Mid);
  const __m128i MaxV   = _mm_set1_epi16((int16)Max);

  int32 Area8 = (int32)((uint32)Area & (uint32)0xFFFFFFF8);
  for(int32 i=0; i<Area8; i+=8)
  {
    //load
    __m128i y = _mm_load_si128((__m128i*)(&(Y[i])));
    __m128i u = _mm_load_si128((__m128i*)(&(U[i])));
    __m128i v = _mm_load_si128((__m128i*)(&(V[i])));

    //sub half
    u = _mm_sub_epi16(u, MidV);
    v = _mm_sub_epi16(v, MidV);

    //conver to int32
    __m128i y_0 = _mm_cvtepi16_epi32(y);
    __m128i y_1 = _mm_cvtepi16_epi32(_mm_srli_si128(y,8));
    __m128i u_0 = _mm_cvtepi16_epi32(u);
    __m128i u_1 = _mm_cvtepi16_epi32(_mm_srli_si128(u,8));
    __m128i v_0 = _mm_cvtepi16_epi32(v);
    __m128i v_1 = _mm_cvtepi16_epi32(_mm_srli_si128(v,8));

    //calc YUV
    y_0 = _mm_add_epi32(_mm_slli_epi32(y_0, 16), c32768);
    y_1 = _mm_add_epi32(_mm_slli_epi32(y_1, 16), c32768);

    __m128i r_0 = _mm_srai_epi32(_mm_add_epi32(y_0, _mm_mullo_epi32(R_V, v_0)), 16);
    __m128i r_1 = _mm_srai_epi32(_mm_add_epi32(y_1, _mm_mullo_epi32(R_V, v_1)), 16);
    __m128i g_0 = _mm_srai_epi32(_mm_add_epi32(y_0, _mm_add_epi32(_mm_mullo_epi32(G_U, u_0), _mm_mullo_epi32(G_V, v_0))), 16);
    __m128i g_1 = _mm_srai_epi32(_mm_add_epi32(y_1, _mm_add_epi32(_mm_mullo_epi32(G_U, u_1), _mm_mullo_epi32(G_V, v_1))), 16);
    __m128i b_0 = _mm_srai_epi32(_mm_add_epi32(y_0, _mm_mullo_epi32(B_U, u_0)), 16);
    __m128i b_1 = _mm_srai_epi32(_mm_add_epi32(y_1, _mm_mullo_epi32(B_U, u_1)), 16);

    //pack to int16
    __m128i r = _mm_packs_epi32(r_0, r_1);
    __m128i g = _mm_packs_epi32(g_0, g_1);
    __m128i b = _mm_packs_epi32(b_0, b_1);

    //clip
    const __m128i ZeroV  = _mm_setzero_si128();
    __m128i cr = _mm_max_epi16(_mm_min_epi16(r, MaxV), ZeroV);
    __m128i cg = _mm_max_epi16(_mm_min_epi16(g, MaxV), ZeroV);
    __m128i cb = _mm_max_epi16(_mm_min_epi16(b, MaxV), ZeroV);

    //save to output component
    _mm_store_si128 ((__m128i*)(&(R[i])), cr);
    _mm_store_si128 ((__m128i*)(&(G[i])), cg);
    _mm_store_si128 ((__m128i*)(&(B[i])), cb);
  }

  for(uint32 i=Area8; i<Area; i++)
  {
    int32 y = (((int32)Y[i])<<16) + 32768;
    int32 u  = (int32)(U[i]) - Mid;
    int32 v  = (int32)(V[i]) - Mid;
    int32 r = (                                      + ((int32)(65536 *  1.57480 + 0.5)) * v + y)>>16;
    int32 g = (((int32)(65536 * -0.18732 + 0.5)) * u + ((int32)(65536 * -0.46812 + 0.5)) * v + y)>>16;
    int32 b = (((int32)(65536 *  1.85560 + 0.5)) * u                                         + y)>>16;    
    int32 cr = xClipU(r, Max);
    int32 cg = xClipU(g, Max);
    int32 cb = xClipU(b, Max);
    R[i] = (int16)cr;
    G[i] = (int16)cg;
    B[i] = (int16)cb;
  }

}
void xColorSpaceSSE::ConvertRGB2YUV_ANY(int16* restrict Y, int16* restrict U, int16* restrict V, const int16* restrict R, const int16* restrict G, const int16* restrict B, uint32 Area, uint32 BitDepth, eClrSpc YUVColorSpace)
{
  const int32 Index = (int32)YUVColorSpace;
  const __m128i Y_R    = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][0]);
  const __m128i Y_G    = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][1]);
  const __m128i Y_B    = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][2]);
  const __m128i U_R    = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][3]);
  const __m128i U_G    = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][4]);
  const __m128i U_B    = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][5]);
  const __m128i V_R    = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][6]);
  const __m128i V_G    = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][7]);
  const __m128i V_B    = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][8]);
  const __m128i c32768 = _mm_set1_epi32((int32)32768);
  const int32   Mid    = (int16)xBitDepth2MidValue(BitDepth);
  const int32   Max    = (int16)xBitDepth2MaxValue(BitDepth);

  int32 Area8 = (int32)((uint32)Area & (uint32)0xFFFFFFF8);
  for(int32 i=0; i<Area8; i+=8)
  {
    //load
    __m128i r = _mm_load_si128((__m128i*)(&(R[i])));
    __m128i g = _mm_load_si128((__m128i*)(&(G[i])));
    __m128i b = _mm_load_si128((__m128i*)(&(B[i])));

    //conver to int32
    __m128i r_0 = _mm_cvtepi16_epi32(r);
    __m128i r_1 = _mm_cvtepi16_epi32(_mm_srli_si128(r,8));
    __m128i g_0 = _mm_cvtepi16_epi32(g);
    __m128i g_1 = _mm_cvtepi16_epi32(_mm_srli_si128(g,8));
    __m128i b_0 = _mm_cvtepi16_epi32(b);
    __m128i b_1 = _mm_cvtepi16_epi32(_mm_srli_si128(b,8));

    //calc YUV
    __m128i y_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(Y_R, r_0), _mm_mullo_epi32(Y_G, g_0)), _mm_add_epi32(_mm_mullo_epi32(Y_B, b_0), c32768)), 16);
    __m128i y_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(Y_R, r_1), _mm_mullo_epi32(Y_G, g_1)), _mm_add_epi32(_mm_mullo_epi32(Y_B, b_1), c32768)), 16);
    __m128i u_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(U_R, r_0), _mm_mullo_epi32(U_G, g_0)), _mm_add_epi32(_mm_mullo_epi32(U_B, b_0), c32768)), 16);
    __m128i u_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(U_R, r_1), _mm_mullo_epi32(U_G, g_1)), _mm_add_epi32(_mm_mullo_epi32(U_B, b_1), c32768)), 16);
    __m128i v_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(V_R, r_0), _mm_mullo_epi32(V_G, g_0)), _mm_add_epi32(_mm_mullo_epi32(V_B, b_0), c32768)), 16);
    __m128i v_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(V_R, r_1), _mm_mullo_epi32(V_G, g_1)), _mm_add_epi32(_mm_mullo_epi32(V_B, b_1), c32768)), 16);

    //pack to int16
    __m128i y = _mm_packs_epi32(y_0, y_1);
    __m128i u = _mm_packs_epi32(u_0, u_1);
    __m128i v = _mm_packs_epi32(v_0, v_1);

    //add half
    const __m128i MidV = _mm_set1_epi16((int16)Mid);
    u = _mm_add_epi16(u, MidV);
    v = _mm_add_epi16(v, MidV);

    //clip
    const __m128i ZeroV = _mm_setzero_si128();
    const __m128i MaxV  = _mm_set1_epi16((int16)Max);
    __m128i cy = _mm_max_epi16(_mm_min_epi16(y, MaxV), ZeroV);
    __m128i cu = _mm_max_epi16(_mm_min_epi16(u, MaxV), ZeroV);
    __m128i cv = _mm_max_epi16(_mm_min_epi16(v, MaxV), ZeroV);

    //save to output component
    _mm_store_si128 ((__m128i*)(&(Y[i])), cy);
    _mm_store_si128 ((__m128i*)(&(U[i])), cu);
    _mm_store_si128 ((__m128i*)(&(V[i])), cv);
  }

  if((uint32)Area8 != (uint32)Area)
  {
    const int32 Y_R = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][0];
    const int32 Y_G = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][1];
    const int32 Y_B = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][2];
    const int32 U_R = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][3];
    const int32 U_G = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][4];
    const int32 U_B = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][5];
    const int32 V_R = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][6];
    const int32 V_G = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][7];
    const int32 V_B = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][8];

    for(uint32 i=Area8; i<(uint32)Area; i++)
    {
      int32 r = R[i];
      int32 g = G[i];
      int32 b = B[i];
      int32 y = ((Y_R*r + Y_G*g + Y_B*b + 32768)>>16);
      int32 u = ((U_R*r + U_G*g + U_B*b + 32768)>>16);
      int32 v = ((V_R*r + V_G*g + V_B*b + 32768)>>16);    
      int32 cy = xClipU(y      , Max);
      int32 cu = xClipU(u + Mid, Max);
      int32 cv = xClipU(v + Mid, Max);
      Y[i] = (int16)cy;
      U[i] = (int16)cu;
      V[i] = (int16)cv;
    }
  }
}
void xColorSpaceSSE::ConvertYUV2RGB_ANY(int16* restrict R, int16* restrict G, int16* restrict B, const int16* restrict Y, const int16* restrict U, const int16* restrict V, uint32 Area, uint32 BitDepth, eClrSpc YUVColorSpace)
{
  const int32 Index = (int32)YUVColorSpace;
  const __m128i R_Y = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][0]);
  const __m128i R_U = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][1]);
  const __m128i R_V = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][2]);
  const __m128i G_Y = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][3]);
  const __m128i G_U = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][4]);
  const __m128i G_V = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][5]);
  const __m128i B_Y = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][6]);
  const __m128i B_U = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][7]);
  const __m128i B_V = _mm_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][8]);
  const __m128i c32768 = _mm_set1_epi32((int32)32768);
  const int32   Mid    = (int16)xBitDepth2MidValue(BitDepth);
  const int32   Max    = (int16)xBitDepth2MaxValue(BitDepth);
  const __m128i MidV   = _mm_set1_epi16((int16)Mid);
  const __m128i MaxV   = _mm_set1_epi16((int16)Max);

  int32 Area8 = (int32)((uint32)Area & (uint32)0xFFFFFFF8);
  for(int32 i=0; i<Area8; i+=8)
  {
    //load
    __m128i y = _mm_load_si128((__m128i*)(&(Y[i])));
    __m128i u = _mm_load_si128((__m128i*)(&(U[i])));
    __m128i v = _mm_load_si128((__m128i*)(&(V[i])));

    //sub half
    u = _mm_sub_epi16(u, MidV);
    v = _mm_sub_epi16(v, MidV);

    //conver to int32
    __m128i y_0 = _mm_cvtepi16_epi32(y);
    __m128i y_1 = _mm_cvtepi16_epi32(_mm_srli_si128(y,8));
    __m128i u_0 = _mm_cvtepi16_epi32(u);
    __m128i u_1 = _mm_cvtepi16_epi32(_mm_srli_si128(u,8));
    __m128i v_0 = _mm_cvtepi16_epi32(v);
    __m128i v_1 = _mm_cvtepi16_epi32(_mm_srli_si128(v,8));

    //calc YUV
    __m128i r_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(R_Y, y_0), _mm_mullo_epi32(R_U, u_0)), _mm_add_epi32(_mm_mullo_epi32(R_V, v_0), c32768)), 16);
    __m128i r_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(R_Y, y_1), _mm_mullo_epi32(R_U, u_1)), _mm_add_epi32(_mm_mullo_epi32(R_V, v_1), c32768)), 16);
    __m128i g_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(G_Y, y_0), _mm_mullo_epi32(G_U, u_0)), _mm_add_epi32(_mm_mullo_epi32(G_V, v_0), c32768)), 16);
    __m128i g_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(G_Y, y_1), _mm_mullo_epi32(G_U, u_1)), _mm_add_epi32(_mm_mullo_epi32(G_V, v_1), c32768)), 16);
    __m128i b_0 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(B_Y, y_0), _mm_mullo_epi32(B_U, u_0)), _mm_add_epi32(_mm_mullo_epi32(B_V, v_0), c32768)), 16);
    __m128i b_1 = _mm_srai_epi32(_mm_add_epi32(_mm_add_epi32(_mm_mullo_epi32(B_Y, y_1), _mm_mullo_epi32(B_U, u_1)), _mm_add_epi32(_mm_mullo_epi32(B_V, v_1), c32768)), 16);

    //pack to int16
    __m128i r = _mm_packs_epi32(r_0, r_1);
    __m128i g = _mm_packs_epi32(g_0, g_1);
    __m128i b = _mm_packs_epi32(b_0, b_1);

    //clip
    const __m128i ZeroV  = _mm_setzero_si128();
    __m128i cr = _mm_max_epi16(_mm_min_epi16(r, MaxV), ZeroV);
    __m128i cg = _mm_max_epi16(_mm_min_epi16(g, MaxV), ZeroV);
    __m128i cb = _mm_max_epi16(_mm_min_epi16(b, MaxV), ZeroV);

    //save to output component
    _mm_store_si128 ((__m128i*)(&(R[i])), cr);
    _mm_store_si128 ((__m128i*)(&(G[i])), cg);
    _mm_store_si128 ((__m128i*)(&(B[i])), cb);
  }

  if((uint32)Area8 != (uint32)Area)
  {
    const int32 R_Y = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][0];
    const int32 R_U = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][1];
    const int32 R_V = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][2];
    const int32 G_Y = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][3];
    const int32 G_U = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][4];
    const int32 G_V = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][5];
    const int32 B_Y = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][6];
    const int32 B_U = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][7];
    const int32 B_V = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][8];
    const int32 Half= 1<<(BitDepth-1);

    for(uint32 i=(uint32)Area8; i<(uint32)Area; i++)
    {
      int32 y = Y[i];
      int32 u = U[i] - Half;
      int32 v = V[i] - Half;
      int32 r = ((R_Y*y + R_U*u + R_V*v + 32768)>>16);
      int32 g = ((G_Y*y + G_U*u + G_V*v + 32768)>>16);
      int32 b = ((B_Y*y + B_U*u + B_V*v + 32768)>>16);
      int32 cr = xClipU(r, Max);
      int32 cg = xClipU(g, Max);
      int32 cb = xClipU(b, Max);
      R[i] = (int16)cr;
      G[i] = (int16)cg;
      B[i] = (int16)cb;
    }
  }
}
#endif //X_SSE4_2

//===============================================================================================================================================================================================================

#if X_AVX1 && X_AVX2
void xColorSpaceAVX::ConvertRGB2YUV_BT601(int16* restrict Y, int16* restrict U, int16* restrict V, const int16* restrict R, const int16* restrict G, const int16* restrict B, uint32 Area, uint32 BitDepth)
{
  const __m256i Y_R    = _mm256_set1_epi32(((int32)(65536 * 0.29900 + 0.5)));
  const __m256i Y_G    = _mm256_set1_epi32(((int32)(65536 * 0.58700 + 0.5)));
  const __m256i Y_B    = _mm256_set1_epi32(((int32)(65536 * 0.11400 + 0.5)));
  const __m256i U_R    = _mm256_set1_epi32(((int32)(65536 *-0.16874 + 0.5)));
  const __m256i U_G    = _mm256_set1_epi32(((int32)(65536 *-0.33126 + 0.5)));
  const __m256i U_B_V_R= _mm256_set1_epi32(((int32)(65536 * 0.50000 + 0.5)));
  const __m256i V_G    = _mm256_set1_epi32(((int32)(65536 *-0.41869 + 0.5)));
  const __m256i V_B    = _mm256_set1_epi32(((int32)(65536 *-0.04585 + 0.5)));
  const __m256i c32768 = _mm256_set1_epi32((int32)32768);
  const int32   Mid    = (int16)xBitDepth2MidValue(BitDepth);
  const int32   Max    = (int16)xBitDepth2MaxValue(BitDepth);

  int32 Area16 = (int32)((uint32)Area & (uint32)0xFFFFFFF0);
  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m256i r = _mm256_load_si256((__m256i*)(&(R[i])));
    __m256i g = _mm256_load_si256((__m256i*)(&(G[i])));
    __m256i b = _mm256_load_si256((__m256i*)(&(B[i])));

    //conver to int32
    __m256i r_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(r,0));
    __m256i r_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(r,1));
    __m256i g_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(g,0));
    __m256i g_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(g,1));
    __m256i b_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(b,0));
    __m256i b_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(b,1));

    //calc YUV
    __m256i y_0 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(Y_R,     r_0), _mm256_mullo_epi32(Y_G, g_0)), _mm256_add_epi32(_mm256_mullo_epi32(Y_B,     b_0), c32768)), 16);
    __m256i y_1 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(Y_R,     r_1), _mm256_mullo_epi32(Y_G, g_1)), _mm256_add_epi32(_mm256_mullo_epi32(Y_B,     b_1), c32768)), 16);
    __m256i u_0 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(U_R,     r_0), _mm256_mullo_epi32(U_G, g_0)), _mm256_add_epi32(_mm256_mullo_epi32(U_B_V_R, b_0), c32768)), 16);
    __m256i u_1 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(U_R,     r_1), _mm256_mullo_epi32(U_G, g_1)), _mm256_add_epi32(_mm256_mullo_epi32(U_B_V_R, b_1), c32768)), 16);
    __m256i v_0 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(U_B_V_R, r_0), _mm256_mullo_epi32(V_G, g_0)), _mm256_add_epi32(_mm256_mullo_epi32(V_B,     b_0), c32768)), 16);
    __m256i v_1 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(U_B_V_R, r_1), _mm256_mullo_epi32(V_G, g_1)), _mm256_add_epi32(_mm256_mullo_epi32(V_B,     b_1), c32768)), 16);

    //pack to int16
    __m256i y = _mm256_permute4x64_epi64(_mm256_packs_epi32(y_0, y_1), 0xD8);
    __m256i u = _mm256_permute4x64_epi64(_mm256_packs_epi32(u_0, u_1), 0xD8);
    __m256i v = _mm256_permute4x64_epi64(_mm256_packs_epi32(v_0, v_1), 0xD8);

    //add half
    const __m256i MidV = _mm256_set1_epi16((int16)Mid);
    u = _mm256_add_epi16(u, MidV);
    v = _mm256_add_epi16(v, MidV);

    //clip
    const __m256i ZeroV = _mm256_setzero_si256();
    const __m256i MaxV  = _mm256_set1_epi16((int16)Max);
    __m256i cy = _mm256_max_epi16(_mm256_min_epi16(y, MaxV), ZeroV);
    __m256i cu = _mm256_max_epi16(_mm256_min_epi16(u, MaxV), ZeroV);
    __m256i cv = _mm256_max_epi16(_mm256_min_epi16(v, MaxV), ZeroV);

    //save to output component
    _mm256_store_si256 ((__m256i*)(&(Y[i])), cy);
    _mm256_store_si256 ((__m256i*)(&(U[i])), cu);
    _mm256_store_si256 ((__m256i*)(&(V[i])), cv);
  }

  for(uint32 i=Area16; i<Area; i++)
  {
    int32 r = R[i];
    int32 g = G[i];
    int32 b = B[i];
    int32 y = ((((int32)(65536 * 0.29900 + 0.5))*r + ((int32)(65536 * 0.58700 + 0.5))*g + ((int32)(65536 * 0.11400 + 0.5))*b + 32768)>>16);
    int32 u = ((((int32)(65536 *-0.16874 + 0.5))*r + ((int32)(65536 *-0.33126 + 0.5))*g + ((int32)(65536 * 0.50000 + 0.5))*b + 32768)>>16);
    int32 v = ((((int32)(65536 * 0.50000 + 0.5))*r + ((int32)(65536 *-0.41869 + 0.5))*g + ((int32)(65536 *-0.04585 + 0.5))*b + 32768)>>16);    
    int32 cy = xClipU(y      , Max);
    int32 cu = xClipU(u + Mid, Max);
    int32 cv = xClipU(v + Mid, Max);
    Y[i] = (int16)cy;
    U[i] = (int16)cu;
    V[i] = (int16)cv;
  }
}
void xColorSpaceAVX::ConvertYUV2RGB_BT601(int16* restrict R, int16* restrict G, int16* restrict B, const int16* restrict Y, const int16* restrict U, const int16* restrict V, uint32 Area, uint32 BitDepth)
{
  const __m256i R_V    = _mm256_set1_epi32(((int32)(65536 * 1.40200 + 0.5)));
  const __m256i G_U    = _mm256_set1_epi32(((int32)(65536 *-0.34414 + 0.5)));
  const __m256i G_V    = _mm256_set1_epi32(((int32)(65536 *-0.71414 + 0.5)));
  const __m256i B_U    = _mm256_set1_epi32(((int32)(65536 * 1.77200 + 0.5)));
  const __m256i c32768 = _mm256_set1_epi32((int32)32768);
  const int32   Mid    = (int16)xBitDepth2MidValue(BitDepth);
  const int32   Max    = (int16)xBitDepth2MaxValue(BitDepth);
  const __m256i MidV   = _mm256_set1_epi16((int16)Mid);
  const __m256i MaxV   = _mm256_set1_epi16((int16)Max);

  int32 Area16 = (int32)((uint32)Area & (uint32)0xFFFFFFF0);
  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m256i y = _mm256_load_si256((__m256i*)(&(Y[i])));
    __m256i u = _mm256_load_si256((__m256i*)(&(U[i])));
    __m256i v = _mm256_load_si256((__m256i*)(&(V[i])));

    //sub half
    u = _mm256_sub_epi16(u, MidV);
    v = _mm256_sub_epi16(v, MidV);

    //conver to int32
    __m256i y_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(y,0));
    __m256i y_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(y,1));
    __m256i u_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(u,0));
    __m256i u_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(u,1));
    __m256i v_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(v,0));
    __m256i v_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(v,1));

    //calc YUV
    y_0 = _mm256_add_epi32(_mm256_slli_epi32(y_0, 16), c32768);
    y_1 = _mm256_add_epi32(_mm256_slli_epi32(y_1, 16), c32768);

    __m256i r_0 = _mm256_srai_epi32(_mm256_add_epi32(y_0, _mm256_mullo_epi32(R_V, v_0)), 16);
    __m256i r_1 = _mm256_srai_epi32(_mm256_add_epi32(y_1, _mm256_mullo_epi32(R_V, v_1)), 16);
    __m256i g_0 = _mm256_srai_epi32(_mm256_add_epi32(y_0, _mm256_add_epi32(_mm256_mullo_epi32(G_U, u_0), _mm256_mullo_epi32(G_V, v_0))), 16);
    __m256i g_1 = _mm256_srai_epi32(_mm256_add_epi32(y_1, _mm256_add_epi32(_mm256_mullo_epi32(G_U, u_1), _mm256_mullo_epi32(G_V, v_1))), 16);
    __m256i b_0 = _mm256_srai_epi32(_mm256_add_epi32(y_0, _mm256_mullo_epi32(B_U, u_0)), 16);
    __m256i b_1 = _mm256_srai_epi32(_mm256_add_epi32(y_1, _mm256_mullo_epi32(B_U, u_1)), 16);

    //pack to int16
    __m256i r = _mm256_permute4x64_epi64(_mm256_packs_epi32(r_0, r_1), 0xD8);
    __m256i g = _mm256_permute4x64_epi64(_mm256_packs_epi32(g_0, g_1), 0xD8);
    __m256i b = _mm256_permute4x64_epi64(_mm256_packs_epi32(b_0, b_1), 0xD8);

    //clip
    const __m256i ZeroV  = _mm256_setzero_si256();
    __m256i cr = _mm256_max_epi16(_mm256_min_epi16(r, MaxV), ZeroV);
    __m256i cg = _mm256_max_epi16(_mm256_min_epi16(g, MaxV), ZeroV);
    __m256i cb = _mm256_max_epi16(_mm256_min_epi16(b, MaxV), ZeroV);

    //save to output component
    _mm256_store_si256 ((__m256i*)(&(R[i])), cr);
    _mm256_store_si256 ((__m256i*)(&(G[i])), cg);
    _mm256_store_si256 ((__m256i*)(&(B[i])), cb);
  }

  for(uint32 i=Area16; i<Area; i++)
  {
    int32 y = (((int32)Y[i])<<16) + 32768;
    int32 u  = (int32)(U[i]) - Mid;
    int32 v  = (int32)(V[i]) - Mid;
    int32 r = (                                      + ((int32)(65536 *  1.40200 + 0.5)) * v + y)>>16;
    int32 g = (((int32)(65536 * -0.34414 + 0.5)) * u + ((int32)(65536 * -0.71414 + 0.5)) * v + y)>>16;
    int32 b = (((int32)(65536 *  1.77200 + 0.5)) * u                                         + y)>>16;    
    int32 cr = xClipU(r, Max);
    int32 cg = xClipU(g, Max);
    int32 cb = xClipU(b, Max);
    R[i] = (int16)cr;
    G[i] = (int16)cg;
    B[i] = (int16)cb;
  }
}
void xColorSpaceAVX::ConvertRGB2YUV_BT709(int16* restrict Y, int16* restrict U, int16* restrict V, const int16* restrict R, const int16* restrict G, const int16* restrict B, uint32 Area, uint32 BitDepth)
{
  const __m256i Y_R    = _mm256_set1_epi32(((int32)(65536 * 0.21260 + 0.5)));
  const __m256i Y_G    = _mm256_set1_epi32(((int32)(65536 * 0.71520 + 0.5)));
  const __m256i Y_B    = _mm256_set1_epi32(((int32)(65536 * 0.07220 + 0.5)));
  const __m256i U_R    = _mm256_set1_epi32(((int32)(65536 *-0.11457 + 0.5)));
  const __m256i U_G    = _mm256_set1_epi32(((int32)(65536 *-0.38543 + 0.5)));
  const __m256i U_B_V_R= _mm256_set1_epi32(((int32)(65536 * 0.50000 + 0.5)));
  const __m256i V_G    = _mm256_set1_epi32(((int32)(65536 *-0.45415 + 0.5)));
  const __m256i V_B    = _mm256_set1_epi32(((int32)(65536 *-0.04585 + 0.5)));
  const __m256i c32768 = _mm256_set1_epi32((int32)32768);
  const int32   Mid    = (int16)xBitDepth2MidValue(BitDepth);
  const int32   Max    = (int16)xBitDepth2MaxValue(BitDepth);

  int32 Area16 = (int32)((uint32)Area & (uint32)0xFFFFFFF0);
  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m256i r = _mm256_load_si256((__m256i*)(&(R[i])));
    __m256i g = _mm256_load_si256((__m256i*)(&(G[i])));
    __m256i b = _mm256_load_si256((__m256i*)(&(B[i])));

    //conver to int32
    __m256i r_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(r,0));
    __m256i r_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(r,1));
    __m256i g_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(g,0));
    __m256i g_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(g,1));
    __m256i b_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(b,0));
    __m256i b_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(b,1));

    //calc YUV
    __m256i y_0 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(Y_R,     r_0), _mm256_mullo_epi32(Y_G, g_0)), _mm256_add_epi32(_mm256_mullo_epi32(Y_B,     b_0), c32768)), 16);
    __m256i y_1 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(Y_R,     r_1), _mm256_mullo_epi32(Y_G, g_1)), _mm256_add_epi32(_mm256_mullo_epi32(Y_B,     b_1), c32768)), 16);
    __m256i u_0 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(U_R,     r_0), _mm256_mullo_epi32(U_G, g_0)), _mm256_add_epi32(_mm256_mullo_epi32(U_B_V_R, b_0), c32768)), 16);
    __m256i u_1 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(U_R,     r_1), _mm256_mullo_epi32(U_G, g_1)), _mm256_add_epi32(_mm256_mullo_epi32(U_B_V_R, b_1), c32768)), 16);
    __m256i v_0 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(U_B_V_R, r_0), _mm256_mullo_epi32(V_G, g_0)), _mm256_add_epi32(_mm256_mullo_epi32(V_B,     b_0), c32768)), 16);
    __m256i v_1 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(U_B_V_R, r_1), _mm256_mullo_epi32(V_G, g_1)), _mm256_add_epi32(_mm256_mullo_epi32(V_B,     b_1), c32768)), 16);

    //pack to int16
    __m256i y = _mm256_permute4x64_epi64(_mm256_packs_epi32(y_0, y_1), 0xD8);
    __m256i u = _mm256_permute4x64_epi64(_mm256_packs_epi32(u_0, u_1), 0xD8);
    __m256i v = _mm256_permute4x64_epi64(_mm256_packs_epi32(v_0, v_1), 0xD8);

    //add half
    const __m256i MidV = _mm256_set1_epi16((int16)Mid);
    u = _mm256_add_epi16(u, MidV);
    v = _mm256_add_epi16(v, MidV);

    //clip
    const __m256i ZeroV = _mm256_setzero_si256();
    const __m256i MaxV  = _mm256_set1_epi16((int16)Max);
    __m256i cy = _mm256_max_epi16(_mm256_min_epi16(y, MaxV), ZeroV);
    __m256i cu = _mm256_max_epi16(_mm256_min_epi16(u, MaxV), ZeroV);
    __m256i cv = _mm256_max_epi16(_mm256_min_epi16(v, MaxV), ZeroV);

    //save to output component
    _mm256_store_si256 ((__m256i*)(&(Y[i])), cy);
    _mm256_store_si256 ((__m256i*)(&(U[i])), cu);
    _mm256_store_si256 ((__m256i*)(&(V[i])), cv);
  }

  for(uint32 i=Area16; i<Area; i++)
  {
    int32 r = R[i];
    int32 g = G[i];
    int32 b = B[i];
    int32 y = ((((int32)(65536 * 0.21260 + 0.5))*r + ((int32)(65536 * 0.71520 + 0.5))*g + ((int32)(65536 * 0.07220 + 0.5))*b + 32768)>>16);
    int32 u = ((((int32)(65536 *-0.11457 + 0.5))*r + ((int32)(65536 *-0.38543 + 0.5))*g + ((int32)(65536 * 0.50000 + 0.5))*b + 32768)>>16);
    int32 v = ((((int32)(65536 * 0.50000 + 0.5))*r + ((int32)(65536 *-0.45415 + 0.5))*g + ((int32)(65536 *-0.04585 + 0.5))*b + 32768)>>16);    
    int32 cy = xClipU(y      , Max);
    int32 cu = xClipU(u + Mid, Max);
    int32 cv = xClipU(v + Mid, Max);
    Y[i] = (int16)cy;
    U[i] = (int16)cu;
    V[i] = (int16)cv;
  }
}
void xColorSpaceAVX::ConvertYUV2RGB_BT709(int16* restrict R, int16* restrict G, int16* restrict B, const int16* restrict Y, const int16* restrict U, const int16* restrict V, uint32 Area, uint32 BitDepth)
{
  const __m256i R_V    = _mm256_set1_epi32(((int32)(65536 * 1.57480 + 0.5)));
  const __m256i G_U    = _mm256_set1_epi32(((int32)(65536 *-0.18732 + 0.5)));
  const __m256i G_V    = _mm256_set1_epi32(((int32)(65536 *-0.46812 + 0.5)));
  const __m256i B_U    = _mm256_set1_epi32(((int32)(65536 * 1.85560 + 0.5)));
  const __m256i c32768 = _mm256_set1_epi32((int32)32768);
  const int32   Mid    = (int16)xBitDepth2MidValue(BitDepth);
  const int32   Max    = (int16)xBitDepth2MaxValue(BitDepth);
  const __m256i MidV   = _mm256_set1_epi16((int16)Mid);
  const __m256i MaxV   = _mm256_set1_epi16((int16)Max);

  int32 Area16 = (int32)((uint32)Area & (uint32)0xFFFFFFF0);
  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m256i y = _mm256_load_si256((__m256i*)(&(Y[i])));
    __m256i u = _mm256_load_si256((__m256i*)(&(U[i])));
    __m256i v = _mm256_load_si256((__m256i*)(&(V[i])));

    //sub half
    u = _mm256_sub_epi16(u, MidV);
    v = _mm256_sub_epi16(v, MidV);

    //conver to int32
    __m256i y_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(y,0));
    __m256i y_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(y,1));
    __m256i u_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(u,0));
    __m256i u_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(u,1));
    __m256i v_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(v,0));
    __m256i v_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(v,1));

    //calc YUV
    y_0 = _mm256_add_epi32(_mm256_slli_epi32(y_0, 16), c32768);
    y_1 = _mm256_add_epi32(_mm256_slli_epi32(y_1, 16), c32768);

    __m256i r_0 = _mm256_srai_epi32(_mm256_add_epi32(y_0, _mm256_mullo_epi32(R_V, v_0)), 16);
    __m256i r_1 = _mm256_srai_epi32(_mm256_add_epi32(y_1, _mm256_mullo_epi32(R_V, v_1)), 16);
    __m256i g_0 = _mm256_srai_epi32(_mm256_add_epi32(y_0, _mm256_add_epi32(_mm256_mullo_epi32(G_U, u_0), _mm256_mullo_epi32(G_V, v_0))), 16);
    __m256i g_1 = _mm256_srai_epi32(_mm256_add_epi32(y_1, _mm256_add_epi32(_mm256_mullo_epi32(G_U, u_1), _mm256_mullo_epi32(G_V, v_1))), 16);
    __m256i b_0 = _mm256_srai_epi32(_mm256_add_epi32(y_0, _mm256_mullo_epi32(B_U, u_0)), 16);
    __m256i b_1 = _mm256_srai_epi32(_mm256_add_epi32(y_1, _mm256_mullo_epi32(B_U, u_1)), 16);

    //pack to int16
    __m256i r = _mm256_permute4x64_epi64(_mm256_packs_epi32(r_0, r_1), 0xD8);
    __m256i g = _mm256_permute4x64_epi64(_mm256_packs_epi32(g_0, g_1), 0xD8);
    __m256i b = _mm256_permute4x64_epi64(_mm256_packs_epi32(b_0, b_1), 0xD8);

    //clip
    const __m256i ZeroV  = _mm256_setzero_si256();
    __m256i cr = _mm256_max_epi16(_mm256_min_epi16(r, MaxV), ZeroV);
    __m256i cg = _mm256_max_epi16(_mm256_min_epi16(g, MaxV), ZeroV);
    __m256i cb = _mm256_max_epi16(_mm256_min_epi16(b, MaxV), ZeroV);

    //save to output component
    _mm256_store_si256 ((__m256i*)(&(R[i])), cr);
    _mm256_store_si256 ((__m256i*)(&(G[i])), cg);
    _mm256_store_si256 ((__m256i*)(&(B[i])), cb);
  }

  for(uint32 i=Area16; i<Area; i++)
  {
    int32 y = (((int32)Y[i])<<16) + 32768;
    int32 u  = (int32)(U[i]) - Mid;
    int32 v  = (int32)(V[i]) - Mid;
    int32 r = (                                      + ((int32)(65536 *  1.40200 + 0.5)) * v + y)>>16;
    int32 g = (((int32)(65536 * -0.34414 + 0.5)) * u + ((int32)(65536 * -0.71414 + 0.5)) * v + y)>>16;
    int32 b = (((int32)(65536 *  1.77200 + 0.5)) * u                                         + y)>>16;    
    int32 cr = xClipU(r, Max);
    int32 cg = xClipU(g, Max);
    int32 cb = xClipU(b, Max);
    R[i] = (int16)cr;
    G[i] = (int16)cg;
    B[i] = (int16)cb;
  }

}
void xColorSpaceAVX::ConvertRGB2YUV_ANY(int16* restrict Y, int16* restrict U, int16* restrict V, const int16* restrict R, const int16* restrict G, const int16* restrict B, uint32 Area, uint32 BitDepth, eClrSpc YUVColorSpace)
{
  const int32 Index = (int32)YUVColorSpace;
  const __m256i Y_R    = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][0]);
  const __m256i Y_G    = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][1]);
  const __m256i Y_B    = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][2]);
  const __m256i U_R    = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][3]);
  const __m256i U_G    = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][4]);
  const __m256i U_B    = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][5]);
  const __m256i V_R    = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][6]);
  const __m256i V_G    = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][7]);
  const __m256i V_B    = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][8]);
  const __m256i c32768 = _mm256_set1_epi32((int32)32768);
  const int32   Mid    = (int16)xBitDepth2MidValue(BitDepth);
  const int32   Max    = (int16)xBitDepth2MaxValue(BitDepth);

  int32 Area16 = (int32)((uint32)Area & (uint32)0xFFFFFFF0);
  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m256i r = _mm256_load_si256((__m256i*)(&(R[i])));
    __m256i g = _mm256_load_si256((__m256i*)(&(G[i])));
    __m256i b = _mm256_load_si256((__m256i*)(&(B[i])));

    //conver to int32
    __m256i r_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(r,0));
    __m256i r_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(r,1));
    __m256i g_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(g,0));
    __m256i g_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(g,1));
    __m256i b_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(b,0));
    __m256i b_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(b,1));

    //calc YUV
    __m256i y_0 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(Y_R, r_0), _mm256_mullo_epi32(Y_G, g_0)), _mm256_add_epi32(_mm256_mullo_epi32(Y_B, b_0), c32768)), 16);
    __m256i y_1 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(Y_R, r_1), _mm256_mullo_epi32(Y_G, g_1)), _mm256_add_epi32(_mm256_mullo_epi32(Y_B, b_1), c32768)), 16);
    __m256i u_0 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(U_R, r_0), _mm256_mullo_epi32(U_G, g_0)), _mm256_add_epi32(_mm256_mullo_epi32(U_B, b_0), c32768)), 16);
    __m256i u_1 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(U_R, r_1), _mm256_mullo_epi32(U_G, g_1)), _mm256_add_epi32(_mm256_mullo_epi32(U_B, b_1), c32768)), 16);
    __m256i v_0 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(V_R, r_0), _mm256_mullo_epi32(V_G, g_0)), _mm256_add_epi32(_mm256_mullo_epi32(V_B, b_0), c32768)), 16);
    __m256i v_1 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(V_R, r_1), _mm256_mullo_epi32(V_G, g_1)), _mm256_add_epi32(_mm256_mullo_epi32(V_B, b_1), c32768)), 16);

    //pack to int16
    __m256i y = _mm256_permute4x64_epi64(_mm256_packs_epi32(y_0, y_1), 0xD8);
    __m256i u = _mm256_permute4x64_epi64(_mm256_packs_epi32(u_0, u_1), 0xD8);
    __m256i v = _mm256_permute4x64_epi64(_mm256_packs_epi32(v_0, v_1), 0xD8);

    //add half
    const __m256i MidV = _mm256_set1_epi16((int16)Mid);
    u = _mm256_add_epi16(u, MidV);
    v = _mm256_add_epi16(v, MidV);

    //clip
    const __m256i ZeroV = _mm256_setzero_si256();
    const __m256i MaxV  = _mm256_set1_epi16((int16)Max);
    __m256i cy = _mm256_max_epi16(_mm256_min_epi16(y, MaxV), ZeroV);
    __m256i cu = _mm256_max_epi16(_mm256_min_epi16(u, MaxV), ZeroV);
    __m256i cv = _mm256_max_epi16(_mm256_min_epi16(v, MaxV), ZeroV);

    //save to output component
    _mm256_store_si256 ((__m256i*)(&(Y[i])), cy);
    _mm256_store_si256 ((__m256i*)(&(U[i])), cu);
    _mm256_store_si256 ((__m256i*)(&(V[i])), cv);
  }

  if((uint32)Area16 != (uint32)Area)
  {
    const int32 Y_R = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][0];
    const int32 Y_G = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][1];
    const int32 Y_B = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][2];
    const int32 U_R = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][3];
    const int32 U_G = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][4];
    const int32 U_B = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][5];
    const int32 V_R = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][6];
    const int32 V_G = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][7];
    const int32 V_B = xColorSpaceCoeff<int32>::c_RGB2YCbCr[Index][8];
    const int32 Half= 1<<(BitDepth-1);

    for(uint32 i=(uint32)Area16; i<(uint32)Area; i++)
    {
      int32 r = R[i];
      int32 g = G[i];
      int32 b = B[i];
      int32 y = ((Y_R*r + Y_G*g + Y_B*b + 32768)>>16);
      int32 u = ((U_R*r + U_G*g + U_B*b + 32768)>>16);
      int32 v = ((V_R*r + V_G*g + V_B*b + 32768)>>16);    
      Y[i] = (int16)y;
      U[i] = (int16)(u + Half);
      V[i] = (int16)(v + Half);
    }
  }

}
void xColorSpaceAVX::ConvertYUV2RGB_ANY(int16* restrict R, int16* restrict G, int16* restrict B, const int16* restrict Y, const int16* restrict U, const int16* restrict V, uint32 Area, uint32 BitDepth, eClrSpc YUVColorSpace)
{
  const int32 Index = (int32)YUVColorSpace;
  const __m256i R_Y = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][0]);
  const __m256i R_U = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][1]);
  const __m256i R_V = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][2]);
  const __m256i G_Y = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][3]);
  const __m256i G_U = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][4]);
  const __m256i G_V = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][5]);
  const __m256i B_Y = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][6]);
  const __m256i B_U = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][7]);
  const __m256i B_V = _mm256_set1_epi32(xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][8]);
  const __m256i c32768 = _mm256_set1_epi32((int32)32768);
  const int32   Mid    = (int16)xBitDepth2MidValue(BitDepth);
  const int32   Max    = (int16)xBitDepth2MaxValue(BitDepth);
  const __m256i MidV   = _mm256_set1_epi16((int16)Mid);
  const __m256i MaxV   = _mm256_set1_epi16((int16)Max);

  int32 Area16 = (int32)((uint32)Area & (uint32)0xFFFFFFF0);
  for(int32 i=0; i<Area16; i+=16)
  {
    //load
    __m256i y = _mm256_load_si256((__m256i*)(&(Y[i])));
    __m256i u = _mm256_load_si256((__m256i*)(&(U[i])));
    __m256i v = _mm256_load_si256((__m256i*)(&(V[i])));

    //sub half
    u = _mm256_sub_epi16(u, MidV);
    v = _mm256_sub_epi16(v, MidV);

    //conver to int32
    __m256i y_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(y,0));
    __m256i y_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(y,1));
    __m256i u_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(u,0));
    __m256i u_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(u,1));
    __m256i v_0 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(v,0));
    __m256i v_1 = _mm256_cvtepi16_epi32(_mm256_extractf128_si256(v,1));

    //calc YUV
    __m256i r_0 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(R_Y, y_0), _mm256_mullo_epi32(R_U, u_0)), _mm256_add_epi32(_mm256_mullo_epi32(R_V, v_0), c32768)), 16);
    __m256i r_1 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(R_Y, y_1), _mm256_mullo_epi32(R_U, u_1)), _mm256_add_epi32(_mm256_mullo_epi32(R_V, v_1), c32768)), 16);
    __m256i g_0 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(G_Y, y_0), _mm256_mullo_epi32(G_U, u_0)), _mm256_add_epi32(_mm256_mullo_epi32(G_V, v_0), c32768)), 16);
    __m256i g_1 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(G_Y, y_1), _mm256_mullo_epi32(G_U, u_1)), _mm256_add_epi32(_mm256_mullo_epi32(G_V, v_1), c32768)), 16);
    __m256i b_0 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(B_Y, y_0), _mm256_mullo_epi32(B_U, u_0)), _mm256_add_epi32(_mm256_mullo_epi32(B_V, v_0), c32768)), 16);
    __m256i b_1 = _mm256_srai_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_mullo_epi32(B_Y, y_1), _mm256_mullo_epi32(B_U, u_1)), _mm256_add_epi32(_mm256_mullo_epi32(B_V, v_1), c32768)), 16);

    //pack to int16
    __m256i r = _mm256_permute4x64_epi64(_mm256_packs_epi32(r_0, r_1), 0xD8);
    __m256i g = _mm256_permute4x64_epi64(_mm256_packs_epi32(g_0, g_1), 0xD8);
    __m256i b = _mm256_permute4x64_epi64(_mm256_packs_epi32(b_0, b_1), 0xD8);

    //clip
    const __m256i ZeroV  = _mm256_setzero_si256();
    __m256i cr = _mm256_max_epi16(_mm256_min_epi16(r, MaxV), ZeroV);
    __m256i cg = _mm256_max_epi16(_mm256_min_epi16(g, MaxV), ZeroV);
    __m256i cb = _mm256_max_epi16(_mm256_min_epi16(b, MaxV), ZeroV);

    //save to output component
    _mm256_store_si256 ((__m256i*)(&(R[i])), cr);
    _mm256_store_si256 ((__m256i*)(&(G[i])), cg);
    _mm256_store_si256 ((__m256i*)(&(B[i])), cb);
  }

  if((uint32)Area16 != (uint32)Area)
  {
    const int32 R_Y = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][0];
    const int32 R_U = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][1];
    const int32 R_V = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][2];
    const int32 G_Y = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][3];
    const int32 G_U = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][4];
    const int32 G_V = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][5];
    const int32 B_Y = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][6];
    const int32 B_U = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][7];
    const int32 B_V = xColorSpaceCoeff<int32>::c_YCbCr2RGB[Index][8];
    const int32 Half= 1<<(BitDepth-1);

    for(uint32 i=(uint32)Area16; i<(uint32)Area; i++)
    {
      int32 y = Y[i];
      int32 u = U[i] - Half;
      int32 v = V[i] - Half;
      int32 r = ((R_Y*y + R_U*u + R_V*v + 32768)>>16);
      int32 g = ((G_Y*y + G_U*u + G_V*v + 32768)>>16);
      int32 b = ((B_Y*y + B_U*u + B_V*v + 32768)>>16);
      int32 cr = xClipU(r, Max);
      int32 cg = xClipU(g, Max);
      int32 cb = xClipU(b, Max);
      R[i] = (int16)cr;
      G[i] = (int16)cg;
      B[i] = (int16)cb;
    }
  }
}
#endif //X_AVX1 && X_AVX2

//===============================================================================================================================================================================================================

} //end of namespace AVLib


