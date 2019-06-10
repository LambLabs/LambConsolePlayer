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

#include "xCommon.h"
#include "xPicture.h"
#include "xPlane.h"
#include "xThreadPoolShared.h"

namespace AVlib {

//interpolation directions
#define X_BAYER_DIRECTION_HORIZOTAL 0
#define X_BAYER_DIRECTION_VERTICAL  1

class xBayer
{
public:
  static const int32 c_Margin = 16;

protected:
  int32   m_SizeX;
  int32   m_SizeY;

  xPic<int16>    m_RGB[3]; //interpolated in RGB space
  xPic<int16>    m_YUV[3]; //interpolated in YUV space
  xPic<int16>    m_Hom[3]; //homogeneity buffer (AHD, IAHD)
  xPic<int16>    m_Act;    //activity buffer (AHD, IAHD)
  xPic<int16>    m_Buffer;

protected:
  class xNearestNeighbor
  {
  public:
    static void Interpolate(xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern);
  };

  class xBilinear
  {
  public:
    static void Interpolate(xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern);
  };

  class xVariableNumberOfGradients
  {
  public:
    static void Interpolate(xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern);
  protected:
    static int16V3 xProcessOnGreen(int16** M, int32 x, int32 y, int32 GreenType);
    static std::vector<int16> xGradientsOnRedBlue(int16** M, int32 x, int32 y);
    static std::vector<int16> xGradientsOnGreen  (int16** M, int32 x, int32 y);
    static int16   xInterpOnRedBlue(int16** M, int16** X, int32 x, int32 y) { return 0; }
    static int16V3 xInterpOnGreen  (int16** M, int32 x, int32 y, int32 GradientIdx) ;
  };

  class xPixelGruping
  {
  public:
    static void Interpolate(xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern);
  protected:
    static int16 xGreen                  (xPic<int16>::MemMap& M, int32 x, int32 y);
    static inline int16 xHueTransit      (int16 L0, int16 L1, int16 L2, int16 V0, int16 V2) { return (((L0<L1)&&(L1<L2)) || ((L0>L1)&&(L1>L2))) ? V0+(V2-V0)*(L1-L0)/(L2-L0) : (V0+V2)/2+(L1*2-L0-L2)/4; }
    static inline int16 xRedBlueOnGreenH (xPic<int16>::MemMap& M, xPic<int16>::MemMap& G, int32 x, int32 y) { return xHueTransit(G[y][x-1], G[y][x], G[y][x+1], M[y][x-1], M[y][x+1]); }
    static inline int16 xRedBlueOnGreenV (xPic<int16>::MemMap& M, xPic<int16>::MemMap& G, int32 x, int32 y) { return xHueTransit(G[y-1][x], G[y][x], G[y+1][x], M[y-1][x], M[y+1][x]); }
    static int16 xRedBlueOnRedBlue       (xPic<int16>::MemMap& M, xPic<int16>::MemMap& G, int32 x, int32 y);
  };


  class xAdaptiveHomogeneityDirected
  {
  public:
    static void Interpolate(xPic<int16>* PicM, xPic<int16>* PicRGB, xPic<int16>* TmpRGB, xPic<int16>* TmpYUV, xPic<int16>* Hom, xPic<int16>* Buffer, eBayerPattern SamplePattern);
  protected:
    static void xInterpolateDirectionalG (xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern, int32 BorderLimit, int32 Direction);
    static void xInterpolateRB           (xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern, int32 BorderLimit, int32 Direction);
    static void xInterpolateDirectional  (xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern, int32 BorderLimit, int32 Direction);
    static void xCalculateHomogeneity    (xPic<int16>* HorYUV, xPic<int16>* VerYUV, xPic<int16>* HorHomogeneity, xPic<int16>* VerHomogeneity, int32 BorderLimit);
    static void xAverageHomogeneity      (xPic<int16>* Homogeneity, xPic<int16>* Buffer, int32 BorderLimit);
    static void xSelectSamples           (xPic<int16>* HorSrc, xPic<int16>* VerSrc, xPic<int16>* HorHomogeneity, xPic<int16>* VerHomogeneity, xPic<int16>* Dst, int32 BorderLimit);
  };

  class xImprovedAdaptiveHomogeneityDirected : public xAdaptiveHomogeneityDirected
  {
  public:
    static void Interpolate(xPic<int16>* PicM, xPic<int16>* PicRGB, xPic<int16>* TmpRGB, xPic<int16>* TmpYUV, xPic<int16>* Act, xPic<int16>* Hom, xPic<int16>* Buffer, eBayerPattern SamplePattern);
  protected:
    static void xCalculateActivity       (xPic<int16>* SrcYUV, xPic<int16>* Activity, xPic<int16>* Buffer, int32 BorderLimit);
    static void xCalculateHomogeneityI   (xPic<int16>* HorYUV, xPic<int16>* VerYUV, xPic<int16>* HorHomogeneity, xPic<int16>* VerHomogeneity, xPic<int16>* Activity, int32 BorderLimit);
    static void xAverageHomogeneityI     (xPic<int16>* Homogeneity, xPic<int16>* Buffer, xPic<int16>* Activity, int32 BorderLimit);
    static void xSelectSamplesI          (xPic<int16>* HorSrc, xPic<int16>* VerSrc, xPic<int16>* LinSrc, xPic<int16>* Activity, xPic<int16>* HorHomogeneity, xPic<int16>* VerHomogeneity, xPic<int16>* Dst, int32 BorderLimit);
  };

  class xDirectionalLMMSE
  {
  public:
    static void Interpolate(xPic<int16>* PicM, xPic<int16>* PicRGB, xPic<int16>* TmpRGB, xPic<int16>* TmpYUV, xPic<int16>* Act, xPic<int16>* Hom, xPic<int16>* Buffer, eBayerPattern SamplePattern);
  protected:
  };

  static void xScatter                 (xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern);
  static void xDecimate                (xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern);
  static void xSimulate                (xPic<int16>* PicRGB, xPic<int16>* PicM, eBayerPattern SamplePattern);

public:
  void     create              (int32 SizeX, int32 SizeY, int32 BitDepth); //ColorSpace can be eClrSpc::Unknown for RGB output
  void     destroy             ();

  void     interpolateNearest  (xPic<int16>* Src, xPic<int16>* Dst, eBayerPattern SamplePattern) { xNearestNeighbor::Interpolate(Src, Dst, SamplePattern); }//nearest neighbor interpolation - fastest, poor quality
  void     interpolateBilinear (xPic<int16>* Src, xPic<int16>* Dst, eBayerPattern SamplePattern) { xBilinear::Interpolate(Src, Dst, SamplePattern); }       //bilinear interpolation - fast, moderate quality
  void     interpolateVNG      (xPic<int16>* Src, xPic<int16>* Dst, eBayerPattern SamplePattern) { xVariableNumberOfGradients::Interpolate(Src, Dst, SamplePattern); }   //variable number of gradients
  void     interpolatePG       (xPic<int16>* Src, xPic<int16>* Dst, eBayerPattern SamplePattern) { xPixelGruping::Interpolate(Src, Dst, SamplePattern); }   //pixel grouping
  void     interpolateAHD      (xPic<int16>* Src, xPic<int16>* Dst, eBayerPattern SamplePattern) { xAdaptiveHomogeneityDirected::Interpolate(Src, Dst, m_RGB, m_YUV, m_Hom, &m_Buffer, SamplePattern); } //adaptive homogeneity directed (AHD) - shamefully slow, very good quality
  void     interpolateIAHD     (xPic<int16>* Src, xPic<int16>* Dst, eBayerPattern SamplePattern) { xImprovedAdaptiveHomogeneityDirected::Interpolate(Src, Dst, m_RGB, m_YUV, &m_Act, m_Hom, &m_Buffer, SamplePattern); } //improved AHD - moderate slow, best quality

  void     interpolate         (xPic<int16>* Src, xPic<int16>* Dst, eBayerPattern SamplePattern, eBayerAlgorithm Algorithm);
  void     scatter             (xPic<int16>* Src, xPic<int16>* Dst, eBayerPattern SamplePattern); //scatter pointr from M to RGB, fill empty by 0
  void     decimate            (xPic<int16>* Src, xPic<int16>* Dst, eBayerPattern SamplePattern); //decimate by 2
  void     simulate            (xPic<int16>* Src, xPic<int16>* Dst, eBayerPattern SamplePattern); //simulate bayer sampling
};

} //end of namespace AVLib
