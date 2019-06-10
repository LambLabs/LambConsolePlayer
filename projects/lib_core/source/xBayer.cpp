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

#include "xBayer.h"
#include <algorithm>
using std::vector;
using std::pair;

namespace AVlib {

void xBayer::create(int32 SizeX, int32 SizeY, int32 BitDepth)
{
  m_SizeX  = SizeX;
  m_SizeY  = SizeY;

  for(int32 i=0; i<3; i++)
  {
    m_RGB[i].create(SizeX, SizeY, c_Margin, BitDepth, eImgTp::RGB    , CrF_UNKNOWN);
    m_YUV[i].create(SizeX, SizeY, c_Margin, BitDepth, eImgTp::YUV    , CrF_444    );
    m_Hom[i].create(SizeX, SizeY, c_Margin, BitDepth, eImgTp::UNKNOWN, CrF_UNKNOWN);
  }
  m_Act   .create  (SizeX, SizeY, c_Margin, BitDepth, eImgTp::UNKNOWN, CrF_UNKNOWN);
  m_Buffer.create  (SizeX, SizeY, c_Margin, BitDepth, eImgTp::UNKNOWN, CrF_UNKNOWN);
}
void xBayer::destroy()
{
  for(int32 i=0; i<3; i++)
  {
    m_RGB[i].destroy();
    m_YUV[i].destroy();
    m_Hom[i].destroy();
  }
  m_Act.destroy();
  m_Buffer.destroy();
}
//=======================================================================================================================================================================
void xBayer::xNearestNeighbor::Interpolate(xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern)
{
  xPic<int16>::MemMap M = PicM  ->getMemoryMapper(CMP_0);
  xPic<int16>::MemMap R = PicRGB->getMemoryMapper(CMP_R);
  xPic<int16>::MemMap G = PicRGB->getMemoryMapper(CMP_G);
  xPic<int16>::MemMap B = PicRGB->getMemoryMapper(CMP_B);
  int32    SizeX = PicRGB->getWidth();
  int32    SizeY = PicRGB->getHeight();

  switch(SamplePattern)
  {
  case eBayerPattern::GBRG:
#if X_SSE4_1
    for(int32 y=0; y<SizeY; y+=2)
    {
      for(int32 x=0; x<SizeX; x+=8)
      {
        __m128i Line0A_V = _mm_load_si128((__m128i*)&(M[y  ][x  ]));
        __m128i Line1A_V = _mm_load_si128((__m128i*)&(M[y+1][x  ]));
        __m128i Line0B_V = _mm_shufflehi_epi16(_mm_shufflelo_epi16(Line0A_V, 0xB1), 0xB1);
        __m128i Line1B_V = _mm_shufflehi_epi16(_mm_shufflelo_epi16(Line1A_V, 0xB1), 0xB1);

        __m128i R_V  = _mm_blend_epi16(Line1A_V, Line1B_V, 0xAA);
        __m128i G0_V = _mm_blend_epi16(Line0A_V, Line0B_V, 0xAA);
        __m128i G1_V = _mm_blend_epi16(Line1A_V, Line1B_V, 0x55);
        __m128i G_V  = _mm_avg_epu16(G0_V, G1_V);        
        __m128i B_V  = _mm_blend_epi16(Line0A_V, Line0B_V, 0x55);

        _mm_store_si128((__m128i*)&(R[y  ][x  ]), R_V );
        _mm_store_si128((__m128i*)&(R[y+1][x  ]), R_V );
        _mm_store_si128((__m128i*)&(G[y  ][x  ]), G_V);
        _mm_store_si128((__m128i*)&(G[y+1][x  ]), G_V);
        _mm_store_si128((__m128i*)&(B[y  ][x  ]), B_V );
        _mm_store_si128((__m128i*)&(B[y+1][x  ]), B_V );
      }
    }    
#else
    for(int32 y=0; y<SizeY; y+=2)
    {
      for(int32 x=0; x<SizeX; x+=2)
      {
        int16 sG1 = M[y  ][x  ];
        int16 sB  = M[y  ][x+1];
        int16 sR  = M[y+1][x  ];
        int16 sG2 = M[y+1][x+1];
        int16 sG  = (sG1+sG2+1)>>1;

        R[y  ][x  ]=sR; R[y  ][x+1]=sR; R[y+1][x  ]=sR; R[y+1][x+1]=sR;
        G[y  ][x  ]=sG; G[y  ][x+1]=sG; G[y+1][x  ]=sG; G[y+1][x+1]=sG;
        B[y  ][x  ]=sB; B[y  ][x+1]=sB; B[y+1][x  ]=sB; B[y+1][x+1]=sB;        
      }
    }    
#endif
    break;
  case eBayerPattern::GRBG:
    for(int32 y=0; y<SizeY; y+=2)
    {
      for(int32 x=0; x<SizeX; x+=2)
      {
        int16 sG1 = M[y  ][x  ];
        int16 sR  = M[y  ][x+1];
        int16 sB  = M[y+1][x  ];
        int16 sG2 = M[y+1][x+1];
        int16 sG  = (sG1+sG2+1)>>1;

        R[y  ][x  ]=sR; R[y  ][x+1]=sR; R[y+1][x  ]=sR; R[y+1][x+1]=sR;
        G[y  ][x  ]=sG; G[y  ][x+1]=sG; G[y+1][x  ]=sG; G[y+1][x+1]=sG;
        B[y  ][x  ]=sB; B[y  ][x+1]=sB; B[y+1][x  ]=sB; B[y+1][x+1]=sB;        
      }
    }    
    break;
  case eBayerPattern::BGGR:
    for(int32 y=0; y<SizeY; y+=2)
    {
      for(int32 x=0; x<SizeX; x+=2)
      {
        int16 sB  = M[y  ][x  ];
        int16 sG1 = M[y  ][x+1];
        int16 sG2 = M[y+1][x  ];
        int16 sR  = M[y+1][x+1];
        int16 sG  = (sG1+sG2+1)>>1;

        R[y  ][x  ]=sR; R[y  ][x+1]=sR; R[y+1][x  ]=sR; R[y+1][x+1]=sR;
        G[y  ][x  ]=sG; G[y  ][x+1]=sG; G[y+1][x  ]=sG; G[y+1][x+1]=sG;
        B[y  ][x  ]=sB; B[y  ][x+1]=sB; B[y+1][x  ]=sB; B[y+1][x+1]=sB;        
      }
    }    
    break;
  case eBayerPattern::RGGB:
    for(int32 y=0; y<SizeY; y+=2)
    {
      for(int32 x=0; x<SizeX; x+=2)
      {
        int16 sR  = M[y  ][x  ];
        int16 sG1 = M[y  ][x+1];
        int16 sG2 = M[y+1][x  ];
        int16 sB  = M[y+1][x+1];
        int16 sG  = (sG1+sG2+1)>>1;

        R[y  ][x  ]=sR; R[y  ][x+1]=sR; R[y+1][x  ]=sR; R[y+1][x+1]=sR;
        G[y  ][x  ]=sG; G[y  ][x+1]=sG; G[y+1][x  ]=sG; G[y+1][x+1]=sG;
        B[y  ][x  ]=sB; B[y  ][x+1]=sB; B[y+1][x  ]=sB; B[y+1][x+1]=sB;       
      }
    }    
    break;

  case eBayerPattern::INVALID: xAssert(0); break;
  }
}
//=======================================================================================================================================================================
void xBayer::xBilinear::Interpolate(xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern)
{
  int32 Margin      = PicM->getMargin();
  int32 BorderLimit = Margin>=2 ? 0 : 2;
  if(Margin) { PicM->extendMargin(eCmpExtMode::Bayer); }

  xPic<int16>::MemMap M = PicM  ->getMemoryMapper(CMP_0);;
  xPic<int16>::MemMap R = PicRGB->getMemoryMapper(CMP_R);
  xPic<int16>::MemMap G = PicRGB->getMemoryMapper(CMP_G);
  xPic<int16>::MemMap B = PicRGB->getMemoryMapper(CMP_B);
  int32    SizeX = PicRGB->getWidth();
  int32    SizeY = PicRGB->getHeight();

  switch(SamplePattern)
  {
  case eBayerPattern::GBRG:
    #if X_OMP_BAYER
      #pragma omp parallel for private(y,x)
    #endif
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        int16 Ga =  M[y][x];
        int16 Gb = (M[y][x] + M[y][x+2] + M[y-1][x+1] + M[y+1][x+1] + 2)>>2;      
        int16 Gc = (M[y+1][x-1] + M[y+1][x+1] + M[y][x] + M[y+2][x] + 2)>>2;      
        int16 Gd =  M[y+1][x+1];

        G[y  ][x  ] = Ga; G[y  ][x+1] = Gb; G[y+1][x  ] = Gc; G[y+1][x+1] = Gd;  
     
        int16 Ra = (M[y+1][x] + M[y-1][x] + 1)>>1;
        int16 Rb = (M[y-1][x] + M[y+1][x] + M[y-1][x+2] + M[y+1][x+2] + 2)>>2;
        int16 Rc =  M[y+1][x];
        int16 Rd = (M[y+1][x] + M[y+1][x+2] + 1)>>1;

        R[y  ][x  ] = Ra; R[y  ][x+1] = Rb; R[y+1][x  ] = Rc; R[y+1][x+1] = Rd; 

        int16 Ba = (M[y][x-1] + M[y][x+1] + 1)>>1;
        int16 Bb =  M[y][x+1];
        int16 Bc = (M[y][x-1] + M[y][x+1] + M[y+2][x-1] + M[y+2][x+1] + 2)>>2;
        int16 Bd = (M[y][x+1] + M[y+2][x+1] + 1)>>1;     

        B[y  ][x  ] = Ba; B[y  ][x+1] = Bb; B[y+1][x  ] = Bc; B[y+1][x+1] = Bd;      
      }
    }
    break;

  case eBayerPattern::GRBG:
    #if X_OMP_BAYER
      #pragma omp parallel for private(y,x)
    #endif
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        int16 Ga =  M[y][x];
        int16 Gb = (M[y][x] + M[y][x+2] + M[y-1][x+1] + M[y+1][x+1] + 2)>>2;      
        int16 Gc = (M[y+1][x-1] + M[y+1][x+1] + M[y][x] + M[y+2][x] + 2)>>2;      
        int16 Gd =  M[y+1][x+1];

        G[y  ][x  ] = Ga; G[y  ][x+1] = Gb; G[y+1][x  ] = Gc; G[y+1][x+1] = Gd; 

        int16 Ra = (M[y][x-1] + M[y][x+1] + 1)>>1;
        int16 Rb =  M[y][x+1];
        int16 Rc = (M[y][x-1] + M[y][x+1] + M[y+2][x-1] + M[y+2][x+1] + 2)>>2;
        int16 Rd = (M[y][x+1] + M[y+2][x+1] + 1)/2;

        R[y  ][x  ] = Ra; R[y  ][x+1] = Rb; R[y+1][x  ] = Rc; R[y+1][x+1] = Rd; 

        int16 Ba = (M[y+1][x] + M[y-1][x] + 1)>>1;
        int16 Bb = (M[y-1][x] + M[y+1][x] + M[y-1][x+2] + M[y+1][x+2] + 2)>>2;
        int16 Bc =  M[y+1][x];
        int16 Bd = (M[y+1][x] + M[y+1][x+2] + 1)>>1;    
        
        B[y  ][x  ] = Ba; B[y  ][x+1] = Bb; B[y+1][x  ] = Bc; B[y+1][x+1] = Bd;      
      }
    }
    break;

  case eBayerPattern::BGGR:
    #if X_OMP_BAYER
      #pragma omp parallel for private(y,x)
    #endif
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        int16 Ga = (M[y-1][x] + M[y][x-1] + M[y][x+1] + M[y-1][x] + 2)>>2;
        int16 Gb =  M[y][x+1];    
        int16 Gc =  M[y+1][x];    
        int16 Gd = (M[y][x+1] + M[y+1][x] + M[y+1][x+2] + M[y+2][x+1] + 2)>>2;

        G[y  ][x  ] = Ga; G[y  ][x+1] = Gb; G[y+1][x  ] = Gc; G[y+1][x+1] = Gd;
     
        int16 Ra = (M[y-1][x-1] + M[y-1][x+1] + M[y+1][x-1] + M[y+1][x+1] + 2)>>2;
        int16 Rb = (M[y-1][x+1] + M[y+1][x+1] + 1)>>1;
        int16 Rc = (M[y][x-1] + M[y][x+1] + 1)>>1;
        int16 Rd =  M[y+1][x+1];

        R[y  ][x  ] = Ra; R[y  ][x+1] = Rb; R[y+1][x  ] = Rc; R[y+1][x+1] = Rd; 

        int16 Ba =  M[y][x];
        int16 Bb = (M[y][x] + M[y][x+2] + 1)>>1;
        int16 Bc = (M[y][x] + M[y+2][x] + 1)>>1;
        int16 Bd = (M[y][x] + M[y][x+2] + M[y+2][x] + M[y+2][x+2])>>2;  
        
        B[y  ][x  ] = Ba; B[y  ][x+1] = Bb; B[y+1][x  ] = Bc; B[y+1][x+1] = Bd;     
      }
    }
    break;

  case eBayerPattern::RGGB:
    #if X_OMP_BAYER
      #pragma omp parallel for private(y,x)
    #endif
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        int16 Ga = (M[y-1][x] + M[y][x-1] + M[y][x+1] + M[y-1][x] + 2)/4;
        int16 Gb =  M[y][x+1];    
        int16 Gc =  M[y+1][x];    
        int16 Gd = (M[y][x+1] + M[y+1][x] + M[y+1][x+2] + M[y+2][x+1] + 2)/4;

        G[y  ][x  ] = Ga; G[y  ][x+1] = Gb; G[y+1][x  ] = Gc; G[y+1][x+1] = Gd;  

        int16 Ra =  M[y][x];
        int16 Rb = (M[y][x] + M[y][x+2] + 1)/2;
        int16 Rc = (M[y][x] + M[y+2][x] + 1)/2;
        int16 Rd = (M[y][x] + M[y][x+2] + M[y+2][x] + M[y+2][x+2])/4;  
     
        R[y  ][x  ] = Ra; R[y  ][x+1] = Rb; R[y+1][x  ] = Rc; R[y+1][x+1] = Rd;

        int16 Ba = (M[y-1][x-1] + M[y-1][x+1] + M[y+1][x-1] + M[y+1][x+1] + 2)/4;
        int16 Bb = (M[y-1][x+1] + M[y+1][x+1] + 1)/2;
        int16 Bc = (M[y][x-1] + M[y][x+1] + 1)/2;
        int16 Bd =  M[y+1][x+1];
        
        B[y  ][x  ] = Ba; B[y  ][x+1] = Bb; B[y+1][x  ] = Bc; B[y+1][x+1] = Bd;
      }
    }
    break;

  case eBayerPattern::INVALID: xAssert(0); break;
  }
}
//=======================================================================================================================================================================
void xBayer::xVariableNumberOfGradients::Interpolate(xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern)
{ 
  //int32 Margin      = PicM->getMargin();
  //int32 BorderLimit = Margin>=2 ? 0 : 2;
  //if(Margin) { PicM->getCmp(0)->extendMarginBayer(); }

  //int16** M     = PicM  ->getCmp(0)->getPel();
  //int16** R     = PicRGB->getCmp(0)->getPel();
  //int16** G     = PicRGB->getCmp(1)->getPel();
  //int16** B     = PicRGB->getCmp(2)->getPel();
  //int32   SizeX = PicRGB->getWidth();
  //int32   SizeY = PicRGB->getHeight();

  //  switch(SamplePattern)
  //{
  //case eBayerPattern::GBRG:
  //  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
  //  {
  //    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
  //    {
  //      //R[y  ][x  ] = xRedBlueOnGreenV (M, G, x  , y  );
  //      //R[y  ][x+1] = xRedBlueOnRedBlue(M, G, x+1, y  );
  //      //R[y+1][x  ] = M[y+1][x];
  //      //R[y+1][x+1] = xRedBlueOnGreenH (M, G, x+1, y+1);
  //      //G[y  ][x  ] = M[y][x];
  //      //G[y  ][x+1] = xGreen(M, x+1, y  );
  //      //G[y+1][x  ] = xGreen(M, x  , y+1);
  //      //G[y+1][x+1] = M[y+1][x+1];
  //      //B[y  ][x  ] = xRedBlueOnGreenH (M, G, x  , y  );
  //      //B[y  ][x+1] = M[y][x+1];
  //      //B[y+1][x  ] = xRedBlueOnRedBlue(M, G, x  , y+1);
  //      //B[y+1][x+1] = xRedBlueOnGreenV (M, G, x+1, y+1);
  //    }
  //  }
  //  break;
  //case eBayerPattern::GRBG:
  //  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
  //  {
  //    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
  //    {
  //      //R[y  ][x  ] = xRedBlueOnGreenH (M, G, x  , y  );
  //      //R[y  ][x+1] = M[y][x+1];
  //      //R[y+1][x  ] = xRedBlueOnRedBlue(M, G, x  , y+1);
  //      //R[y+1][x+1] = xRedBlueOnGreenV (M, G, x+1, y+1);
  //      //B[y  ][x  ] = xRedBlueOnGreenV (M, G, x  , y  );
  //      //B[y  ][x+1] = xRedBlueOnRedBlue(M, G, x+1, y  );
  //      //B[y+1][x  ] = M[y+1][x];
  //      //B[y+1][x+1] = xRedBlueOnGreenH (M, G, x+1, y+1);
  //    }
  //  }
  //  break;
  //case eBayerPattern::BGGR:
  //  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
  //  {
  //    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
  //    {
  //      //R[y  ][x  ] = xRedBlueOnRedBlue(M, G, x  , y  );
  //      //R[y  ][x+1] = xRedBlueOnGreenV (M, G, x+1, y  );
  //      //R[y+1][x  ] = xRedBlueOnGreenH (M, G, x  , y+1);
  //      //R[y+1][x+1] = M[y+1][x+1];
  //      //B[y  ][x  ] = M[y][x];
  //      //B[y  ][x+1] = xRedBlueOnGreenH (M, G, x+1, y  );
  //      //B[y+1][x  ] = xRedBlueOnGreenV (M, G, x  , y+1);
  //      //B[y+1][x+1] = xRedBlueOnRedBlue(M, G, x+1, y+1);
  //    }
  //  }
  //  break;
  //case eBayerPattern::RGGB:
  //  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
  //  {
  //    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
  //    {
  //      //R[y  ][x  ] = M[y][x];
  //      //R[y  ][x+1] = xRedBlueOnGreenH (M, G, x+1, y  );
  //      //R[y+1][x  ] = xRedBlueOnGreenV (M, G, x  , y+1);
  //      //R[y+1][x+1] = xRedBlueOnRedBlue(M, G, x+1, y+1);
  //      //B[y  ][x  ] = xRedBlueOnRedBlue(M, G, x  , y  );
  //      //B[y  ][x+1] = xRedBlueOnGreenV (M, G, x+1, y  );
  //      //B[y+1][x  ] = xRedBlueOnGreenH (M, G, x  , y+1);
  //      //B[y+1][x+1] = M[y+1][x+1];
  //    }
  //  }
  //  break;

  //case eBayerPattern::INVALID: xAssert(0); break;
  //}
}
int16V3 xBayer::xVariableNumberOfGradients::xProcessOnGreen(int16** M, int32 x, int32 y, int32 GreenType)
{
  vector<int16> Gradients = xGradientsOnGreen(M, x, y);
  pair<vector<int16>::iterator, vector<int16>::iterator> MinMaxIterators = std::minmax_element(Gradients.begin(), Gradients.end());
  int16 Threshold = (int16)(round(1.5 * (*MinMaxIterators.first) + 0.5 * (*MinMaxIterators.second - *MinMaxIterators.first)));

  int16V3 SumRGB = (int16)0;
  for(uint32 Idx=0; Idx<Gradients.size(); Idx++)
  {
    if(Gradients[Idx]<Threshold)
    {
      SumRGB += xInterpOnGreen(M, x, y, Idx);
    }
  }

  int16 G = M[y][x];
  int16V3 RGB = { (int16)(G + (SumRGB[0] - SumRGB[1])), (int16)(G), (int16)(G + (SumRGB[2] - SumRGB[1])) };
  return RGB;
}
vector<int16> xBayer::xVariableNumberOfGradients::xGradientsOnGreen(int16** M, int32 x, int32 y)
{
  int16 GradientV = xAbs(M[y-1][x  ] - M[y+1][x  ]) + ((xAbs(M[y-1][x-1] - M[y+1][x-1]) + xAbs(M[y-1][x+1] - M[y+1][x+1]) + 1)>>1);
  int16 GradientH = xAbs(M[y  ][x-1] - M[y  ][x+1]) + ((xAbs(M[y-1][x-1] - M[y-1][x+1]) + xAbs(M[y+1][x-1] - M[y+1][x+1]) + 1)>>1);
  int16 GradientA = xAbs(M[y-1][x+1] - M[y+1][x-1]);
  int16 GradientB = xAbs(M[y-1][x-1] - M[y+1][x+1]);
  std::vector<int16> Gradients = { (int16)(GradientV + xAbs(M[y-2][x  ] - M[y][x]) + ((xAbs(M[y-2][x-1] - M[y][x-1]) + xAbs(M[y-2][x+1] - M[y][x+1]) + 1)>>1)), //N
                                   (int16)(GradientV + xAbs(M[y+2][x  ] - M[y][x]) + ((xAbs(M[y+2][x-1] - M[y][x-1]) + xAbs(M[y+2][x+1] - M[y][x+1]) + 1)>>1)), //S
                                   (int16)(GradientH + xAbs(M[y  ][x-2] - M[y][x]) + ((xAbs(M[y-1][x-2] - M[y-1][x]) + xAbs(M[y+1][x-2] - M[y+1][x]) + 1)>>1)), //W
                                   (int16)(GradientH + xAbs(M[y  ][x+2] - M[y][x]) + ((xAbs(M[y-1][x+2] - M[y-1][x]) + xAbs(M[y+1][x+2] - M[y+1][x]) + 1)>>1)), //E
                                   (int16)(GradientA + xAbs(M[y-2][x+2] - M[y][x]) + ((xAbs(M[y-2][x+1] - M[y][x-1]) + xAbs(M[y-1][x+2] - M[y-1][x]) + 1)>>1)), //NE
                                   (int16)(GradientA + xAbs(M[y+2][x-2] - M[y][x]) + ((xAbs(M[y+2][x-1] - M[y][x+1]) + xAbs(M[y+1][x-2] - M[y+1][x]) + 1)>>1)), //SW
                                   (int16)(GradientB + xAbs(M[y-2][x-2] - M[y][x]) + ((xAbs(M[y-2][x-1] - M[y+1][x]) + xAbs(M[y-1][x-2] - M[y][x+1]) + 1)>>1)), //NW
                                   (int16)(GradientB + xAbs(M[y+2][x+2] - M[y][x]) + ((xAbs(M[y+2][x+1] - M[y-1][x]) + xAbs(M[y+1][x+2] - M[y][x-1]) + 1)>>1))  //SE     
  };
  return Gradients;
}
vector<int16> xBayer::xVariableNumberOfGradients::xGradientsOnRedBlue(int16** M, int32 x, int32 y)
{
  int16 GradientV = xAbs(M[y-1][x  ] - M[y+1][x  ]) + ((xAbs(M[y-1][x-1] - M[y+1][x-1]) + xAbs(M[y-1][x+1] - M[y+1][x+1]) + 1)>>1);
  int16 GradientH = xAbs(M[y  ][x-1] - M[y  ][x+1]) + ((xAbs(M[y-1][x-1] - M[y-1][x+1]) + xAbs(M[y+1][x-1] - M[y+1][x+1]) + 1)>>1);
  int16 GradientA = xAbs(M[y-1][x+1] - M[y+1][x-1]) + ((xAbs(M[y-1][x  ] - M[y  ][x-1]) + xAbs(M[y  ][x+1] - M[y+1][x  ]) + 1)>>1);
  int16 GradientB = xAbs(M[y-1][x-1] - M[y+1][x+1]) + ((xAbs(M[y-1][x  ] - M[y  ][x+1]) + xAbs(M[y  ][x-1] - M[y+1][x  ]) + 1)>>1);
  std::vector<int16> Gradients = { (int16)(GradientV + xAbs(M[y-2][x  ] - M[y][x]) + ((xAbs(M[y-2][x-1] - M[y][x-1]) + xAbs(M[y-2][x+1] - M[y][x+1]) + 1)>>1)), //N
                                    (int16)(GradientV + xAbs(M[y+2][x  ] - M[y][x]) + ((xAbs(M[y+2][x-1] - M[y][x-1]) + xAbs(M[y+2][x+1] - M[y][x+1]) + 1)>>1)), //S
                                    (int16)(GradientH + xAbs(M[y  ][x-2] - M[y][x]) + ((xAbs(M[y-1][x-2] - M[y-1][x]) + xAbs(M[y+1][x-2] - M[y+1][x]) + 1)>>1)), //W
                                    (int16)(GradientH + xAbs(M[y  ][x+2] - M[y][x]) + ((xAbs(M[y-1][x+2] - M[y-1][x]) + xAbs(M[y+1][x+2] - M[y+1][x]) + 1)>>1)), //E
                                    (int16)(GradientA + xAbs(M[y-2][x+2] - M[y][x]) + ((xAbs(M[y-2][x+1] - M[y-1][x]) + xAbs(M[y-1][x+2] - M[y][x+1]) + 1)>>1)), //NE
                                    (int16)(GradientA + xAbs(M[y+2][x-2] - M[y][x]) + ((xAbs(M[y+2][x-1] - M[y+1][x]) + xAbs(M[y+1][x-2] - M[y][x-1]) + 1)>>1)), //SW
                                    (int16)(GradientB + xAbs(M[y-2][x-2] - M[y][x]) + ((xAbs(M[y-2][x-1] - M[y-1][x]) + xAbs(M[y-1][x-2] - M[y][x-1]) + 1)>>1)), //NW
                                    (int16)(GradientB + xAbs(M[y+2][x+2] - M[y][x]) + ((xAbs(M[y+2][x+1] - M[y+1][x]) + xAbs(M[y+1][x+2] - M[y][x+1]) + 1)>>1))  //SE     
  };
  return Gradients;
}
int16V3 xBayer::xVariableNumberOfGradients::xInterpOnGreen(int16** M, int32 x, int32 y, int32 GradientIdx)
{
  switch(GradientIdx)
  {
  case 0:  return int16V3(((M[y-2][x-1] + M[y-2][x+1] + M[y][x-1] + M[y][x+1] + 2)>>2), ((M[y][x] + M[y-2][x] + 1)>>1), M[y-1][x]); //N ??
  case 1:  return int16V3(((M[y+2][x-1] + M[y+2][x+1] + M[y][x-1] + M[y][x+1] + 2)>>2), ((M[y][x] + M[y+2][x] + 1)>>1), M[y+1][x]); //S ?? 
  case 2:  return int16V3(((M[y][x] + M[y][x-2] + 1)>>1), M[y][x-1], ((M[y-1][x-2] + M[y-1][x+2] + M[y-1][x] + M[y+1][x] + 2)>>2)); //W ??
  case 3:  return int16V3(((M[y][x] + M[y][x+2] + 1)>>1), M[y][x+1], ((M[y+1][x-2] + M[y+1][x+2] + M[y-1][x] + M[y+1][x] + 2)>>2)); //E ??
  default: return int16V3(NOT_VALID);
  }
}
//=======================================================================================================================================================================
void xBayer::xPixelGruping::Interpolate(xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern)
{ 
  int32 Margin      = PicM->getMargin();
  int32 BorderLimit = Margin>=2 ? 0 : 2;
  if(Margin) { PicM->extendMargin(eCmpExtMode::Bayer); }

  xPic<int16>::MemMap M = PicM  ->getMemoryMapper(CMP_0);;
  xPic<int16>::MemMap R = PicRGB->getMemoryMapper(CMP_R);
  xPic<int16>::MemMap G = PicRGB->getMemoryMapper(CMP_G);
  xPic<int16>::MemMap B = PicRGB->getMemoryMapper(CMP_B);
  int32    SizeX = PicRGB->getWidth();
  int32    SizeY = PicRGB->getHeight();

  switch(SamplePattern)
  {
  case eBayerPattern::GBRG:
  case eBayerPattern::GRBG:
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        G[y  ][x  ] = M[y][x];
        G[y  ][x+1] = xGreen(M, x+1, y  );
        G[y+1][x  ] = xGreen(M, x  , y+1);
        G[y+1][x+1] = M[y+1][x+1];
      }
    }
    break;
  case eBayerPattern::BGGR:
  case eBayerPattern::RGGB:
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {   
        G[y  ][x  ] = xGreen(M, x  , y  );
        G[y  ][x+1] = M[y][x+1];
        G[y+1][x  ] = M[y+1][x];
        G[y+1][x+1] = xGreen(M, x+1, y+1); 
      }
    }
    break;    
  default: xAssert(0); break;
  }

  switch(SamplePattern)
  {
  case eBayerPattern::GBRG:
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        R[y  ][x  ] = xRedBlueOnGreenV (M, G, x  , y  );
        R[y  ][x+1] = xRedBlueOnRedBlue(M, G, x+1, y  );
        R[y+1][x  ] = M[y+1][x];
        R[y+1][x+1] = xRedBlueOnGreenH (M, G, x+1, y+1);
        B[y  ][x  ] = xRedBlueOnGreenH (M, G, x  , y  );
        B[y  ][x+1] = M[y][x+1];
        B[y+1][x  ] = xRedBlueOnRedBlue(M, G, x  , y+1);
        B[y+1][x+1] = xRedBlueOnGreenV (M, G, x+1, y+1);
      }
    }
    break;
  case eBayerPattern::GRBG:
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        R[y  ][x  ] = xRedBlueOnGreenH (M, G, x  , y  );
        R[y  ][x+1] = M[y][x+1];
        R[y+1][x  ] = xRedBlueOnRedBlue(M, G, x  , y+1);
        R[y+1][x+1] = xRedBlueOnGreenV (M, G, x+1, y+1);
        B[y  ][x  ] = xRedBlueOnGreenV (M, G, x  , y  );
        B[y  ][x+1] = xRedBlueOnRedBlue(M, G, x+1, y  );
        B[y+1][x  ] = M[y+1][x];
        B[y+1][x+1] = xRedBlueOnGreenH (M, G, x+1, y+1);
      }
    }
    break;
  case eBayerPattern::BGGR:
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        R[y  ][x  ] = xRedBlueOnRedBlue(M, G, x  , y  );
        R[y  ][x+1] = xRedBlueOnGreenV (M, G, x+1, y  );
        R[y+1][x  ] = xRedBlueOnGreenH (M, G, x  , y+1);
        R[y+1][x+1] = M[y+1][x+1];
        B[y  ][x  ] = M[y][x];
        B[y  ][x+1] = xRedBlueOnGreenH (M, G, x+1, y  );
        B[y+1][x  ] = xRedBlueOnGreenV (M, G, x  , y+1);
        B[y+1][x+1] = xRedBlueOnRedBlue(M, G, x+1, y+1);
      }
    }
    break;
  case eBayerPattern::RGGB:
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        R[y  ][x  ] = M[y][x];
        R[y  ][x+1] = xRedBlueOnGreenH (M, G, x+1, y  );
        R[y+1][x  ] = xRedBlueOnGreenV (M, G, x  , y+1);
        R[y+1][x+1] = xRedBlueOnRedBlue(M, G, x+1, y+1);
        B[y  ][x  ] = xRedBlueOnRedBlue(M, G, x  , y  );
        B[y  ][x+1] = xRedBlueOnGreenV (M, G, x+1, y  );
        B[y+1][x  ] = xRedBlueOnGreenH (M, G, x  , y+1);
        B[y+1][x+1] = M[y+1][x+1];
      }
    }
    break;
  default: xAssert(0); break;
  }
}
int16 xBayer::xPixelGruping::xGreen(xPic<int16>::MemMap& M, int32 x, int32 y)
{
  int16 GradientH = xAbs(M[y][x-1] - M[y][x+1]);
  int16 GradientV = xAbs(M[y-1][x] - M[y+1][x]);
  vector<int16> Gradients = {(int16)((xAbs(M[y][x] - M[y-2][x])<<1) + GradientV),  //N
                             (int16)((xAbs(M[y][x] - M[y+2][x])<<1) + GradientV),  //S
                             (int16)((xAbs(M[y][x] - M[y][x-2])<<1) + GradientH),  //W
                             (int16)((xAbs(M[y][x] - M[y][x+2])<<1) + GradientH)}; //E
  int32 MinGradientIdx = static_cast<int32>(std::min_element(Gradients.begin(), Gradients.end()) - Gradients.begin());
  switch(MinGradientIdx)
  {
    case 0: return (3*M[y-1][x] + M[y][x+1] + M[y][x] - M[y-2][x] + 2)>>2; //N
    case 1: return (3*M[y+1][x] + M[y][x-1] + M[y][x] - M[y+2][x] + 2)>>2; //S
    case 2: return (3*M[y][x-1] + M[y][x+1] + M[y][x] - M[y][x-2] + 2)>>2; //W
    case 3: return (3*M[y][x+1] + M[y][x-1] + M[y][x] - M[y][x+2] + 2)>>2; //E
  }
  return 0;
}
int16 xBayer::xPixelGruping::xRedBlueOnRedBlue(xPic<int16>::MemMap& M, xPic<int16>::MemMap& G, int32 x, int32 y)
{
  int16 GradientA = xAbs(M[y-1][x+1] - M[y+1][x-1]) + xAbs(M[y-2][x+2] - M[y][x]) + xAbs(M[y][x] - M[y+2][x-1]) + xAbs(G[y-1][x+1] - G[y][x]) + xAbs(G[y][x] - G[y+1][x-1]);
  int16 GradientB = xAbs(M[y-1][x-1] - M[y+1][x+1]) + xAbs(M[y-2][x-2] - M[y][x]) + xAbs(M[y][x] - M[y+2][x+1]) + xAbs(G[y-1][x-1] - G[y][x]) + xAbs(G[y][x] - G[y+1][x+1]);
  if(GradientA<GradientB)
  {
    return xHueTransit(G[y-1][x+1], G[y][x], G[y+1][x-1], M[y-1][x+1], M[y+1][x-1]);
  }
  else
  {
    return xHueTransit(G[y-1][x-1], G[y][x], G[y+1][x+1], M[y-1][x-1], M[y+1][x+1]);
  }
}
//=======================================================================================================================================================================
void xBayer::xAdaptiveHomogeneityDirected::Interpolate(xPic<int16>* PicM, xPic<int16>* PicRGB, xPic<int16>* TmpRGB, xPic<int16>* TmpYUV, xPic<int16>* Hom, xPic<int16>* Buffer, eBayerPattern SamplePattern)
{
  int32 Margin       = PicM->getMargin();
  int32 BorderLimitA = Margin>2 ? -6 : 2;
  int32 BorderLimitB = Margin>2 ? -4 : 2;
  int32 BorderLimitC = Margin>2 ? -2 : 2;
  if(Margin) { PicM->extendMargin(eCmpExtMode::Bayer); }

  xInterpolateDirectional(PicM, &TmpRGB[0], SamplePattern, BorderLimitA, X_BAYER_DIRECTION_HORIZOTAL); //interpolation in x direction 
  xInterpolateDirectional(PicM, &TmpRGB[1], SamplePattern, BorderLimitA, X_BAYER_DIRECTION_VERTICAL ); //interpolation in y direction
 
  for(int32 i=0; i<2; i++) { TmpYUV[i].convertRGB2YUV(&TmpRGB[i], eClrSpc::BT709, true); } //rgb to yuv
  
  xCalculateHomogeneity(&TmpYUV[0], &TmpYUV[1], &Hom[0], &Hom[1], BorderLimitB);   //calculate homogeneity

  for(int32 i=0; i<2; i++) { xAverageHomogeneity(&Hom[i], Buffer, BorderLimitC); }  //homogeneity spatial averaging

  xSelectSamples(&TmpRGB[0], &TmpRGB[1], &Hom[0], &Hom[1], PicRGB, BorderLimitC);//RGB output
}
void xBayer::xAdaptiveHomogeneityDirected::xInterpolateDirectionalG(xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern, int32 BorderLimit, int32 Direction)
{
  xPic<int16>::MemMap M = PicM  ->getMemoryMapper(CMP_0);;
  xPic<int16>::MemMap G = PicRGB->getMemoryMapper(CMP_G);
  int32    SizeX = PicRGB->getWidth();
  int32    SizeY = PicRGB->getHeight();

  switch(Direction)
  {
  case X_BAYER_DIRECTION_HORIZOTAL:
    switch(SamplePattern)
      {
      case eBayerPattern::GBRG:
      case eBayerPattern::GRBG:
        #if X_OMP_BAYER
          #pragma omp parallel for private(y,x)
        #endif
        for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
        {
          for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
          {
            int16 Ga = M[y][x];
            int16 Gb = ( (M[y][x] + M[y][x+1] + M[y][x+2] )*2 - M[y][x-1] - M[y][x+3] + 2)/4;     
            Gb = xAutoClip<int16>(Gb, M[y][x], M[y][x+2]);
            int16 Gc = ( (M[y+1][x-1] + M[y+1][x] + M[y+1][x+1] )*2 - M[y+1][x-2] - M[y+1][x+2] + 2)/4;
            Gc = xAutoClip<int16>(Gc, M[y+1][x-1], M[y+1][x+1]);
            int16 Gd = M[y+1][x+1];

            G[y  ][x  ] = Ga; G[y  ][x+1] = Gb; G[y+1][x  ] = Gc; G[y+1][x+1] = Gd;  
          }
        }
        break;
      case eBayerPattern::BGGR:
      case eBayerPattern::RGGB:
        #if X_OMP_BAYER
          #pragma omp parallel for private(y,x)
        #endif
        for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
        {
          for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
          {
            int16 Ga = ( (M[y][x-1] + M[y][x] + M[y][x+1] )*2 - M[y][x] - M[y][x+2] + 2)/4;
            Ga = xAutoClip<int16>(Ga, M[y][x-1], M[y][x+1]);
            int16 Gb = M[y][x+1];
            int16 Gc = M[y+1][x];
            int16 Gd = ( (M[y+1][x] + M[y+1][x+1] + M[y+1][x+2] )*2 - M[y+1][x-1] - M[y+1][x+3] + 2)/4;
            Gd = xAutoClip<int16>(Gd, M[y+1][x], M[y+1][x+2]);

            G[y  ][x  ] = Ga; G[y  ][x+1] = Gb; G[y+1][x  ] = Gc; G[y+1][x+1] = Gd;  
          }
        }
        break; 
      default: xAssert(0); break;
      }
    break;
  case X_BAYER_DIRECTION_VERTICAL:
    switch(SamplePattern)
      {
      case eBayerPattern::GBRG:
      case eBayerPattern::GRBG:
      #if X_OMP_BAYER
        #pragma omp parallel for private(y,x)
      #endif
        for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
        {
          for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
          {
            int16 Ga = M[y][x];
            int16 Gb = ( (M[y-1][x+1] + M[y][x+1] + M[y+1][x+1] )*2 - M[y-2][x+1] - M[y+2][x+1] + 2)/4;     
            Gb = xAutoClip<int16>(Gb, M[y-1][x+1], M[y+1][x+1]);
            int16 Gc = ( (M[y][x] + M[y+1][x] + M[y+2][x] )*2 - M[y-1][x] - M[y+3][x] + 2)/4;
            Gc = xAutoClip<int16>(Gc, M[y][x], M[y+2][x]);
            int16 Gd = M[y+1][x+1];

            G[y  ][x  ] = Ga; G[y  ][x+1] = Gb; G[y+1][x  ] = Gc; G[y+1][x+1] = Gd;  
          }
        }
        break;
      case eBayerPattern::BGGR:
      case eBayerPattern::RGGB:
      #if X_OMP_BAYER
        #pragma omp parallel for private(y,x)
      #endif
        for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
        {
          for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
          {
            int16 Ga = ( (M[y-1][x] + M[y][x] + M[y+1][x] )*2 - M[y-2][x] - M[y+2][x] + 2)/4;
            Ga = xAutoClip<int16>(Ga, M[y-1][x], M[y+1][x]);
            int16 Gb = M[y][x+1];
            int16 Gc = M[y+1][x];
            int16 Gd = ( (M[y][x+1] + M[y+1][x+1] + M[y+2][x+1] )*2 - M[y-1][x+1] - M[y+3][x+1] + 2)/4;
            Gd = xAutoClip<int16>(Gd, M[y][x+1], M[y+2][x+1]);

            G[y  ][x  ] = Ga; G[y  ][x+1] = Gb; G[y+1][x  ] = Gc; G[y+1][x+1] = Gd;  
          }
        }
        break;
        default: xAssert(0); break;
      }
    break;
  }
}
void xBayer::xAdaptiveHomogeneityDirected::xInterpolateRB(xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern, int32 BorderLimit, int32 Direction)
{
  xPic<int16>::MemMap M = PicM  ->getMemoryMapper(CMP_0);;
  xPic<int16>::MemMap R = PicRGB->getMemoryMapper(CMP_R);
  xPic<int16>::MemMap G = PicRGB->getMemoryMapper(CMP_G);
  xPic<int16>::MemMap B = PicRGB->getMemoryMapper(CMP_B);
  int32    SizeX = PicRGB->getWidth();
  int32    SizeY = PicRGB->getHeight();

  switch(SamplePattern)
  {
  case eBayerPattern::GBRG:
    #if X_OMP_BAYER
      #pragma omp parallel for private(y,x)
    #endif
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        int16 Ra = (M[y+1][x]+M[y-1][x]-G[y+1][x]-G[y-1][x]+1)/2 + G[y][x];
        Ra = xClip(Ra, (int16)0, (int16)int16_max);
        int16 Rb = (M[y-1][x]+M[y+1][x]+M[y-1][x+2]+M[y+1][x+2] - G[y-1][x]-G[y+1][x]-G[y-1][x+2]-G[y+1][x+2]+2)/4 + G[y][x+1];
        Rb = xClip(Rb, (int16)0, (int16)int16_max);  
        int16 Rc = M[y+1][x];
        int16 Rd = (M[y+1][x]+M[y+1][x+2]-G[y+1][x]-G[y+1][x+2]+1)/2 + G[y+1][x+1];
        Rd = xClip(Rd, (int16)0, (int16)int16_max);

        R[y  ][x  ] = Ra; R[y  ][x+1] = Rb; R[y+1][x  ] = Rc; R[y+1][x+1] = Rd; 

        int16 Ba = ( M[y][x-1] + M[y][x+1] - G[y][x-1] - G[y][x+1]+1 )/2 + G[y][x];
        Ba = xClip(Ba, (int16)0, (int16)int16_max);    
        int16 Bb = M[y][x+1];
        int16 Bc = ( M[y][x-1] + M[y][x+1] + M[y+2][x-1] + M[y+2][x+1] - G[y][x-1] - G[y][x+1] - G[y+2][x-1] - G[y+2][x+1]+2 )/4 + G[y+1][x];
        Bc = xClip(Bc, (int16)0, (int16)int16_max);      
        int16 Bd = (M[y][x+1]+M[y+2][x+1]-G[y][x+1]-G[y+2][x+1]+1)/2+G[y+1][x+1];
        Bd = xClip(Bd, (int16)0, (int16)int16_max);
        
        B[y  ][x  ] = Ba; B[y  ][x+1] = Bb; B[y+1][x  ] = Bc; B[y+1][x+1] = Bd;      
      }
    }
    break;
  case eBayerPattern::GRBG:
    #if X_OMP_BAYER
      #pragma omp parallel for private(y,x)
    #endif
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        int16 Ra = ( M[y][x-1] + M[y][x+1] - G[y][x-1] - G[y][x+1]+1 )/2 + G[y][x];
        Ra = xClip(Ra, (int16)0, (int16)int16_max);    
        int16 Rb = M[y][x+1];
        int16 Rc = ( M[y][x-1] + M[y][x+1] + M[y+2][x-1] + M[y+2][x+1] - G[y][x-1] - G[y][x+1] - G[y+2][x-1] - G[y+2][x+1]+2 )/4 + G[y+1][x];
        Rc = xClip(Rc, (int16)0, (int16)int16_max);      
        int16 Rd = (M[y][x+1]+M[y+2][x+1]-G[y][x+1]-G[y+2][x+1]+1)/2+G[y+1][x+1];
        Rd = xClip(Rd, (int16)0, (int16)int16_max);        

        R[y  ][x  ] = Ra; R[y  ][x+1] = Rb; R[y+1][x  ] = Rc; R[y+1][x+1] = Rd; 

        int16 Ba = (M[y+1][x]+M[y-1][x]-G[y+1][x]-G[y-1][x]+1)/2 + G[y][x];
        Ba = xClip(Ba, (int16)0, (int16)int16_max);
        int16 Bb = (M[y-1][x]+M[y+1][x]+M[y-1][x+2]+M[y+1][x+2] - G[y-1][x]-G[y+1][x]-G[y-1][x+2]-G[y+1][x+2]+2)/4 + G[y][x+1];
        Bb = xClip(Bb, (int16)0, (int16)int16_max);     
        int16 Bc = M[y+1][x];
        int16 Bd = (M[y+1][x]+M[y+1][x+2]-G[y+1][x]-G[y+1][x+2]+1)/2 + G[y+1][x+1];
        Bd = xClip(Bd, (int16)0, (int16)int16_max);        
        
        B[y  ][x  ] = Ba; B[y  ][x+1] = Bb; B[y+1][x  ] = Bc; B[y+1][x+1] = Bd;      
      }
    }
    break;
  case eBayerPattern::BGGR:
    #if X_OMP_BAYER
      #pragma omp parallel for private(y,x)
    #endif
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        int16 Ra = (M[y-1][x-1]+M[y+1][x-1]+M[y-1][x+1]+M[y+1][x+1] - G[y-1][x-1]-G[y+1][x-1]-G[y-1][x+1]-G[y+1][x+1]+2)/4 + G[y][x];
        Ra = xClip(Ra, (int16)0, (int16)int16_max);
        int16 Rb = (M[y+1][x]+M[y-1][x]-G[y+1][x]-G[y-1][x]+1)/2 + G[y][x];                
        Rb = xClip(Rb, (int16)0, (int16)int16_max);     
        int16 Rc = (M[y+1][x-1]+M[y+1][x+1]-G[y+1][x-1]-G[y+1][x+1]+1)/2 + G[y+1][x];
        Rc = xClip(Rc, (int16)0, (int16)int16_max);
        int16 Rd = M[y+1][x+1];  

        R[y  ][x  ] = Ra; R[y  ][x+1] = Rb; R[y+1][x  ] = Rc; R[y+1][x+1] = Rd; 

        int16 Ba = M[y][x];        
        int16 Bb = (M[y][x] + M[y][x+2] - G[y][x] - G[y][x+2]+1)/2 + G[y][x+1];
        Bb = xClip(Bb, (int16)0, (int16)int16_max);
        int16 Bc = (M[y][x]+M[y+2][x]-G[y][x]-G[y+2][x]+1)/2+G[y+1][x];
        Bc = xClip(Bc, (int16)0, (int16)int16_max);
        int16 Bd = ( M[y][x] + M[y][x+2] + M[y+2][x] + M[y+2][x+2]
                    -G[y][x] - G[y][x+2] - G[y+2][x] - G[y+2][x+2]+2 )/4 + G[y+1][x+1];
        Bd = xClip(Bd, (int16)0, (int16)int16_max);        
        
        B[y  ][x  ] = Ba; B[y  ][x+1] = Bb; B[y+1][x  ] = Bc; B[y+1][x+1] = Bd;     
      }
    }
    break;
  case eBayerPattern::RGGB:
    #if X_OMP_BAYER
      #pragma omp parallel for private(y,x)
    #endif
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        int16 Ra = M[y][x];        
        int16 Rb = (M[y][x] + M[y][x+2] - G[y][x] - G[y][x+2]+1)/2 + G[y][x+1];
        Rb = xClip(Rb, (int16)0, (int16)int16_max);
        int16 Rc = (M[y][x]+M[y+2][x]-G[y][x]-G[y+2][x]+1)/2+G[y+1][x];
        Rc = xClip(Rc, (int16)0, (int16)int16_max);
        int16 Rd = ( M[y][x] + M[y][x+2] + M[y+2][x] + M[y+2][x+2]
                    -G[y][x] - G[y][x+2] - G[y+2][x] - G[y+2][x+2]+2 )/4 + G[y+1][x+1];
        Rd = xClip(Rd, (int16)0, (int16)int16_max);

        R[y  ][x  ] = Ra; R[y  ][x+1] = Rb; R[y+1][x  ] = Rc; R[y+1][x+1] = Rd; 

        int16 Ba = (M[y-1][x-1]+M[y+1][x-1]+M[y-1][x+1]+M[y+1][x+1]
                   -G[y-1][x-1]-G[y+1][x-1]-G[y-1][x+1]-G[y+1][x+1]+2)/4 + G[y][x];
        Ba = xClip(Ba, (int16)0, (int16)int16_max);
        int16 Bb = (M[y+1][x]+M[y-1][x]-G[y+1][x]-G[y-1][x]+1)/2 + G[y][x];                
        Bb = xClip(Bb, (int16)0, (int16)int16_max);    
        int16 Bc = (M[y+1][x-1]+M[y+1][x+1]-G[y+1][x-1]-G[y+1][x+1]+1)/2 + G[y+1][x];
        Bc = xClip(Bc, (int16)0, (int16)int16_max);
        int16 Bd = M[y+1][x+1];  
        
        B[y  ][x  ] = Ba; B[y  ][x+1] = Bb; B[y+1][x  ] = Bc; B[y+1][x+1] = Bd;     
      }
    }
    break;
  default: xAssert(0); break;
  }
}
void xBayer::xAdaptiveHomogeneityDirected::xInterpolateDirectional(xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern, int32 BorderLimit, int32 Direction)
{
  xPic<int16>::MemMap M = PicM  ->getMemoryMapper(CMP_0);;
  xPic<int16>::MemMap R = PicRGB->getMemoryMapper(CMP_R);
  xPic<int16>::MemMap G = PicRGB->getMemoryMapper(CMP_G);
  xPic<int16>::MemMap B = PicRGB->getMemoryMapper(CMP_B);
  int32    SizeX = PicRGB->getWidth();
  int32    SizeY = PicRGB->getHeight();

  switch(Direction)
  {
  case X_BAYER_DIRECTION_HORIZOTAL:
    switch(SamplePattern)
      {
      case eBayerPattern::GBRG:
      case eBayerPattern::GRBG:
        #if X_OMP_BAYER
          #pragma omp parallel for private(y,x)
        #endif
        for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
        {
          for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
          {
            int16 Ga = M[y][x];
            int16 Gb = ( (M[y][x] + M[y][x+1] + M[y][x+2] )*2 - M[y][x-1] - M[y][x+3] + 2)/4;     
            Gb = xAutoClip<int16>(Gb, M[y][x], M[y][x+2]);
            int16 Gc = ( (M[y+1][x-1] + M[y+1][x] + M[y+1][x+1] )*2 - M[y+1][x-2] - M[y+1][x+2] + 2)/4;
            Gc = xAutoClip<int16>(Gc, M[y+1][x-1], M[y+1][x+1]);
            int16 Gd = M[y+1][x+1];

            G[y  ][x  ] = Ga; G[y  ][x+1] = Gb; G[y+1][x  ] = Gc; G[y+1][x+1] = Gd;  
          }
        }
        break;
      case eBayerPattern::BGGR:
      case eBayerPattern::RGGB:
        #if X_OMP_BAYER
          #pragma omp parallel for private(y,x)
        #endif
        for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
        {
          for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
          {
            int16 Ga = ( (M[y][x-1] + M[y][x] + M[y][x+1] )*2 - M[y][x] - M[y][x+2] + 2)/4;
            Ga = xAutoClip<int16>(Ga, M[y][x-1], M[y][x+1]);
            int16 Gb = M[y][x+1];
            int16 Gc = M[y+1][x];
            int16 Gd = ( (M[y+1][x] + M[y+1][x+1] + M[y+1][x+2] )*2 - M[y+1][x-1] - M[y+1][x+3] + 2)/4;
            Gd = xAutoClip<int16>(Gd, M[y+1][x], M[y+1][x+2]);

            G[y  ][x  ] = Ga; G[y  ][x+1] = Gb; G[y+1][x  ] = Gc; G[y+1][x+1] = Gd;  
          }
        }
        break;  
      default: xAssert(0); break;
      }
    break;
  case X_BAYER_DIRECTION_VERTICAL:
    switch(SamplePattern)
      {
      case eBayerPattern::GBRG:
      case eBayerPattern::GRBG:
      #if X_OMP_BAYER
        #pragma omp parallel for private(y,x)
      #endif
        for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
        {
          for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
          {
            int16 Ga = M[y][x];
            int16 Gb = ( (M[y-1][x+1] + M[y][x+1] + M[y+1][x+1] )*2 - M[y-2][x+1] - M[y+2][x+1] + 2)/4;     
            Gb = xAutoClip<int16>(Gb, M[y-1][x+1], M[y+1][x+1]);
            int16 Gc = ( (M[y][x] + M[y+1][x] + M[y+2][x] )*2 - M[y-1][x] - M[y+3][x] + 2)/4;
            Gc = xAutoClip<int16>(Gc, M[y][x], M[y+2][x]);
            int16 Gd = M[y+1][x+1];

            G[y  ][x  ] = Ga; G[y  ][x+1] = Gb; G[y+1][x  ] = Gc; G[y+1][x+1] = Gd;  
          }
        }
        break;
      case eBayerPattern::BGGR:
      case eBayerPattern::RGGB:
      #if X_OMP_BAYER
        #pragma omp parallel for private(y,x)
      #endif
        for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
        {
          for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
          {
            int16 Ga = ( (M[y-1][x] + M[y][x] + M[y+1][x] )*2 - M[y-2][x] - M[y+2][x] + 2)/4;
            Ga = xAutoClip<int16>(Ga, M[y-1][x], M[y+1][x]);
            int16 Gb = M[y][x+1];
            int16 Gc = M[y+1][x];
            int16 Gd = ( (M[y][x+1] + M[y+1][x+1] + M[y+2][x+1] )*2 - M[y-1][x+1] - M[y+3][x+1] + 2)/4;
            Gd = xAutoClip<int16>(Gd, M[y][x+1], M[y+2][x+1]);

            G[y  ][x  ] = Ga; G[y  ][x+1] = Gb; G[y+1][x  ] = Gc; G[y+1][x+1] = Gd;  
          }
        }
        break;
      default: xAssert(0); break;
      }
    break;
  }

  //red and blue interpolation loop
  switch(SamplePattern)
  {
  case eBayerPattern::GBRG:
    #if X_OMP_BAYER
      #pragma omp parallel for private(y,x)
    #endif
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        int16 Ra = (M[y+1][x]+M[y-1][x]-G[y+1][x]-G[y-1][x]+1)/2 + G[y][x];
        Ra = xClip(Ra, (int16)0, (int16)int16_max);
        int16 Rb = (M[y-1][x]+M[y+1][x]+M[y-1][x+2]+M[y+1][x+2] - G[y-1][x]-G[y+1][x]-G[y-1][x+2]-G[y+1][x+2]+2)/4 + G[y][x+1];
        Rb = xClip(Rb, (int16)0, (int16)int16_max);  
        int16 Rc = M[y+1][x];
        int16 Rd = (M[y+1][x]+M[y+1][x+2]-G[y+1][x]-G[y+1][x+2]+1)/2 + G[y+1][x+1];
        Rd = xClip(Rd, (int16)0, (int16)int16_max);

        R[y  ][x  ] = Ra; R[y  ][x+1] = Rb; R[y+1][x  ] = Rc; R[y+1][x+1] = Rd; 

        int16 Ba = ( M[y][x-1] + M[y][x+1] - G[y][x-1] - G[y][x+1]+1 )/2 + G[y][x];
        Ba = xClip(Ba, (int16)0, (int16)int16_max);    
        int16 Bb = M[y][x+1];
        int16 Bc = ( M[y][x-1] + M[y][x+1] + M[y+2][x-1] + M[y+2][x+1] - G[y][x-1] - G[y][x+1] - G[y+2][x-1] - G[y+2][x+1]+2 )/4 + G[y+1][x];
        Bc = xClip(Bc, (int16)0, (int16)int16_max);      
        int16 Bd = (M[y][x+1]+M[y+2][x+1]-G[y][x+1]-G[y+2][x+1]+1)/2+G[y+1][x+1];
        Bd = xClip(Bd, (int16)0, (int16)int16_max);
        
        B[y  ][x  ] = Ba; B[y  ][x+1] = Bb; B[y+1][x  ] = Bc; B[y+1][x+1] = Bd;      
      }
    }
    break;
  case eBayerPattern::GRBG:
    #if X_OMP_BAYER
      #pragma omp parallel for private(y,x)
    #endif
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        int16 Ra = ( M[y][x-1] + M[y][x+1] - G[y][x-1] - G[y][x+1]+1 )/2 + G[y][x];
        Ra = xClip(Ra, (int16)0, (int16)int16_max);    
        int16 Rb = M[y][x+1];
        int16 Rc = ( M[y][x-1] + M[y][x+1] + M[y+2][x-1] + M[y+2][x+1] - G[y][x-1] - G[y][x+1] - G[y+2][x-1] - G[y+2][x+1]+2 )/4 + G[y+1][x];
        Rc = xClip(Rc, (int16)0, (int16)int16_max);      
        int16 Rd = (M[y][x+1]+M[y+2][x+1]-G[y][x+1]-G[y+2][x+1]+1)/2+G[y+1][x+1];
        Rd = xClip(Rd, (int16)0, (int16)int16_max);        

        R[y  ][x  ] = Ra; R[y  ][x+1] = Rb; R[y+1][x  ] = Rc; R[y+1][x+1] = Rd; 

        int16 Ba = (M[y+1][x]+M[y-1][x]-G[y+1][x]-G[y-1][x]+1)/2 + G[y][x];
        Ba = xClip(Ba, (int16)0, (int16)int16_max);
        int16 Bb = (M[y-1][x]+M[y+1][x]+M[y-1][x+2]+M[y+1][x+2] - G[y-1][x]-G[y+1][x]-G[y-1][x+2]-G[y+1][x+2]+2)/4 + G[y][x+1];
        Bb = xClip(Bb, (int16)0, (int16)int16_max);     
        int16 Bc = M[y+1][x];
        int16 Bd = (M[y+1][x]+M[y+1][x+2]-G[y+1][x]-G[y+1][x+2]+1)/2 + G[y+1][x+1];
        Bd = xClip(Bd, (int16)0, (int16)int16_max);        
        
        B[y  ][x  ] = Ba; B[y  ][x+1] = Bb; B[y+1][x  ] = Bc; B[y+1][x+1] = Bd;      
      }
    }
    break;
  case eBayerPattern::BGGR:
    #if X_OMP_BAYER
      #pragma omp parallel for private(y,x)
    #endif
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        int16 Ra = (M[y-1][x-1]+M[y+1][x-1]+M[y-1][x+1]+M[y+1][x+1] - G[y-1][x-1]-G[y+1][x-1]-G[y-1][x+1]-G[y+1][x+1]+2)/4 + G[y][x];
        Ra = xClip(Ra, (int16)0, (int16)int16_max);
        int16 Rb = (M[y+1][x]+M[y-1][x]-G[y+1][x]-G[y-1][x]+1)/2 + G[y][x];                
        Rb = xClip(Rb, (int16)0, (int16)int16_max);     
        int16 Rc = (M[y+1][x-1]+M[y+1][x+1]-G[y+1][x-1]-G[y+1][x+1]+1)/2 + G[y+1][x];
        Rc = xClip(Rc, (int16)0, (int16)int16_max);
        int16 Rd = M[y+1][x+1];  

        R[y  ][x  ] = Ra; R[y  ][x+1] = Rb; R[y+1][x  ] = Rc; R[y+1][x+1] = Rd; 

        int16 Ba = M[y][x];        
        int16 Bb = (M[y][x] + M[y][x+2] - G[y][x] - G[y][x+2]+1)/2 + G[y][x+1];
        Bb = xClip(Bb, (int16)0, (int16)int16_max);
        int16 Bc = (M[y][x]+M[y+2][x]-G[y][x]-G[y+2][x]+1)/2+G[y+1][x];
        Bc = xClip(Bc, (int16)0, (int16)int16_max);
        int16 Bd = ( M[y][x] + M[y][x+2] + M[y+2][x] + M[y+2][x+2]
                    -G[y][x] - G[y][x+2] - G[y+2][x] - G[y+2][x+2]+2 )/4 + G[y+1][x+1];
        Bd = xClip(Bd, (int16)0, (int16)int16_max);        
        
        B[y  ][x  ] = Ba; B[y  ][x+1] = Bb; B[y+1][x  ] = Bc; B[y+1][x+1] = Bd;     
      }
    }
    break;
  case eBayerPattern::RGGB:
    #if X_OMP_BAYER
      #pragma omp parallel for private(y,x)
    #endif
    for(int32 y=BorderLimit; y<SizeY-BorderLimit; y+=2)
    {
      for(int32 x=BorderLimit; x<SizeX-BorderLimit; x+=2)
      {
        int16 Ra = M[y][x];        
        int16 Rb = (M[y][x] + M[y][x+2] - G[y][x] - G[y][x+2]+1)/2 + G[y][x+1];
        Rb = xClip(Rb, (int16)0, (int16)int16_max);
        int16 Rc = (M[y][x]+M[y+2][x]-G[y][x]-G[y+2][x]+1)/2+G[y+1][x];
        Rc = xClip(Rc, (int16)0, (int16)int16_max);
        int16 Rd = ( M[y][x] + M[y][x+2] + M[y+2][x] + M[y+2][x+2]
                    -G[y][x] - G[y][x+2] - G[y+2][x] - G[y+2][x+2]+2 )/4 + G[y+1][x+1];
        Rd = xClip(Rd, (int16)0, (int16)int16_max);

        R[y  ][x  ] = Ra; R[y  ][x+1] = Rb; R[y+1][x  ] = Rc; R[y+1][x+1] = Rd; 

        int16 Ba = (M[y-1][x-1]+M[y+1][x-1]+M[y-1][x+1]+M[y+1][x+1]
                   -G[y-1][x-1]-G[y+1][x-1]-G[y-1][x+1]-G[y+1][x+1]+2)/4 + G[y][x];
        Ba = xClip(Ba, (int16)0, (int16)int16_max);
        int16 Bb = (M[y+1][x]+M[y-1][x]-G[y+1][x]-G[y-1][x]+1)/2 + G[y][x];                
        Bb = xClip(Bb, (int16)0, (int16)int16_max);    
        int16 Bc = (M[y+1][x-1]+M[y+1][x+1]-G[y+1][x-1]-G[y+1][x+1]+1)/2 + G[y+1][x];
        Bc = xClip(Bc, (int16)0, (int16)int16_max);
        int16 Bd = M[y+1][x+1];  
        
        B[y  ][x  ] = Ba; B[y  ][x+1] = Bb; B[y+1][x  ] = Bc; B[y+1][x+1] = Bd;     
      }
    }
    break;
  default: xAssert(0); break;
  }
}
void xBayer::xAdaptiveHomogeneityDirected::xCalculateHomogeneity(xPic<int16>* HorYUV, xPic<int16>* VerYUV, xPic<int16>* HorHomogeneity, xPic<int16>* VerHomogeneity, int32 BorderLimit)
{
  int32 SizeX = HorYUV->getWidth();
  int32 SizeY = HorYUV->getHeight();  

  xPic<int16>::MemMap H[2] = { HorHomogeneity->getMemoryMapper(CMP_0), VerHomogeneity->getMemoryMapper(CMP_0) };

  xPic<int16>::MemMap Y[2] = { HorYUV->getMemoryMapper(CMP_LM), VerYUV->getMemoryMapper(CMP_LM)};
  xPic<int16>::MemMap U[2] = { HorYUV->getMemoryMapper(CMP_CB), VerYUV->getMemoryMapper(CMP_CB)};
  xPic<int16>::MemMap V[2] = { HorYUV->getMemoryMapper(CMP_CR), VerYUV->getMemoryMapper(CMP_CR)};

#if X_OMP_BAYER
  #pragma omp parallel for
#endif
  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y++)
  {
    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x++)
    {
      int16 EpsilonL = xMin(
                         xMax(
                           xAbs(Y[0][y][x]-Y[0][y][x-1]),
                           xAbs(Y[0][y][x]-Y[0][y][x+1])),
                         xMax(
                           xAbs(Y[1][y][x]-Y[1][y-1][x]),
                           xAbs(Y[1][y][x]-Y[1][y+1][x])));
      int16 EpsilonC = xMin(
                         xMax(                            
                           xPow2(U[0][y][x]-U[0][y][x-1]),
                           xPow2(U[0][y][x]-U[0][y][x+1]),                            
                           xPow2(V[0][y][x]-V[0][y][x-1]),
                           xPow2(V[0][y][x]-V[0][y][x+1])),
                         xMax(
                           xPow2(U[1][y][x]-U[1][y-1][x]),
                           xPow2(U[1][y][x]-U[1][y+1][x]),                             
                           xPow2(V[1][y][x]-V[1][y-1][x]),
                           xPow2(V[1][y][x]-V[1][y+1][x])));
      
      for(int32 j=0; j<2; j++)
      {
        bool LL, CC;
        int16 HH = 0;

        LL = xAbs(Y[j][y][x]-Y[j][y][x-1]) <= EpsilonL;
        CC = (xPow2(U[j][y][x]-U[j][y][x-1]) + xPow2(V[j][y][x]-V[j][y][x-1])) <= EpsilonC;
        HH += (LL && CC);

        LL = xAbs(Y[j][y][x]-Y[j][y][x+1]) <= EpsilonL;
        CC = (xPow2(U[j][y][x]-U[j][y][x+1]) + xPow2(V[j][y][x]-V[j][y][x+1])) <= EpsilonC;
        HH += (LL && CC);

        LL = xAbs(Y[j][y][x]-Y[j][y-1][x]) <= EpsilonL;
        CC = (xPow2(U[j][y][x]-U[j][y-1][x]) + xPow2(V[j][y][x]-V[j][y-1][x])) <= EpsilonC;
        HH += (LL && CC);

        LL = xAbs(Y[j][y][x]-Y[j][y+1][x]) <= EpsilonL;
        CC = (xPow2(U[j][y][x]-U[j][y+1][x]) + xPow2(V[j][y][x]-V[j][y+1][x])) <= EpsilonC;
        HH += (LL && CC); 

        H[j][y][x] = HH;
      }      
    }
  } 
}
void xBayer::xAdaptiveHomogeneityDirected::xAverageHomogeneity(xPic<int16>* Homogeneity, xPic<int16>* Buffer, int32 BorderLimit)
{
  //spatial averaging
  int32 SizeX = Homogeneity->getWidth();
  int32 SizeY = Homogeneity->getHeight();  

  Buffer->copy(Homogeneity);

  xPic<int16>::MemMap SrcH = Buffer->getMemoryMapper(CMP_0);
  xPic<int16>::MemMap DstH = Homogeneity->getMemoryMapper(CMP_0);  

#if X_OMP_BAYER
  #pragma omp parallel for private(y,x)
#endif
  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y++)
  {
    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x++)
    {
      int32 Sum = 0;
      for(int32 j=-1; j<=1; j++)
      {
        for(int32 i=-1; i<=1; i++)
        {
          Sum += SrcH[y+j][x+i];
        }
      }
      DstH[y][x] = (int16)Sum;   
    }
  }
}
void xBayer::xAdaptiveHomogeneityDirected::xSelectSamples(xPic<int16>* HorSrc, xPic<int16>* VerSrc, xPic<int16>* HorHomogeneity, xPic<int16>* VerHomogeneity, xPic<int16>* Dst, int32 BorderLimit)
{
  int32 SizeX = Dst->getWidth();
  int32 SizeY = Dst->getHeight();  

  xPic<int16>::MemMap SrcH[3] = { HorSrc->getMemoryMapper(CMP_0), HorSrc->getMemoryMapper(CMP_1), HorSrc->getMemoryMapper(CMP_2) };
  xPic<int16>::MemMap SrcV[3] = { VerSrc->getMemoryMapper(CMP_0), VerSrc->getMemoryMapper(CMP_1), VerSrc->getMemoryMapper(CMP_2) };
  xPic<int16>::MemMap HomH    = HorHomogeneity->getMemoryMapper(CMP_0);
  xPic<int16>::MemMap HomV    = VerHomogeneity->getMemoryMapper(CMP_0);
  xPic<int16>::MemMap DstX[3] = { Dst->getMemoryMapper(CMP_0), Dst->getMemoryMapper(CMP_1), Dst->getMemoryMapper(CMP_2) };  

#if X_OMP_BAYER
  #pragma omp parallel for private(y,x)
#endif
  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y++)
  {
    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x++)
    {
      if(HomH[y][x] > HomV[y][x])
      {      
        DstX[0][y][x] = SrcH[0][y][x];
        DstX[1][y][x] = SrcH[1][y][x];
        DstX[2][y][x] = SrcH[2][y][x];   
      }
      else
      {       
        DstX[0][y][x] = SrcV[0][y][x];
        DstX[1][y][x] = SrcV[1][y][x];
        DstX[2][y][x] = SrcV[2][y][x];   
      }      
    }
  }
}
//=======================================================================================================================================================================
void xBayer::xImprovedAdaptiveHomogeneityDirected::Interpolate(xPic<int16>* PicM, xPic<int16>* PicRGB, xPic<int16>* TmpRGB, xPic<int16>* TmpYUV, xPic<int16>* Act, xPic<int16>* Hom, xPic<int16>* Buffer, eBayerPattern SamplePattern)
{  
  int32 Margin       = PicM->getMargin();
  int32 BorderLimitA = Margin>2 ? -6 : 2;
  int32 BorderLimitB = Margin>2 ? -4 : 2;
  int32 BorderLimitC = Margin>2 ? -2 : 2;
  if(Margin) { PicM->extendMargin(eCmpExtMode::Bayer); }

  xInterpolateDirectional(PicM, &TmpRGB[0], SamplePattern, BorderLimitA, X_BAYER_DIRECTION_HORIZOTAL); //interpolation in x direction 
  xInterpolateDirectional(PicM, &TmpRGB[1], SamplePattern, BorderLimitA, X_BAYER_DIRECTION_VERTICAL ); //interpolation in y direction
  xBilinear::Interpolate (PicM, &TmpRGB[2], SamplePattern); //bilinear interpolation  
  
  for(int32 i=0; i<3; i++) { TmpYUV[i].convertRGB2YUV(&TmpRGB[i], eClrSpc::BT709, true); } //rgb to yuv
  
  xCalculateActivity(&TmpYUV[2], Act, Buffer, BorderLimitB);  //activity 
   
  xCalculateHomogeneityI(&TmpYUV[0], &TmpYUV[1], &Hom[0], &Hom[1], Act, BorderLimitB);   //calculate homogeneity
  
  for(int32 i=0; i<2; i++) { xAverageHomogeneityI(&Hom[i], Buffer, Act, BorderLimitC); }  //homogeneity spatial averaging
    
  xSelectSamplesI(&TmpRGB[0], &TmpRGB[1], &TmpRGB[2], Act, &Hom[0], &Hom[1], PicRGB, BorderLimitC); //RGB output
}
void xBayer::xImprovedAdaptiveHomogeneityDirected::xCalculateActivity(xPic<int16>* SrcYUV, xPic<int16>* Activity, xPic<int16>* Buffer, int32 BorderLimit)
{
  int32 SizeX = Activity->getWidth();
  int32 SizeY = Activity->getHeight();  

  xPic<int16>::MemMap Y = SrcYUV  ->getMemoryMapper(CMP_0);
  xPic<int16>::MemMap A = Activity->getMemoryMapper(CMP_0);  
  xPic<int16>::MemMap B = Buffer  ->getMemoryMapper(CMP_0);

  //calculating average value into B buffer
#if X_OMP_BAYER
  #pragma omp parallel
  {
  #pragma omp for
#endif
  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y++)
  {
    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x++)
    {
      A[y][x] = Y[y][x-1] + Y[y][x] + Y[y][x+1];
    }
  }

#if X_OMP_BAYER
  #pragma omp for
#endif
  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y++)
  {
    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x++)
    {
      B[y][x] = (A[y-1][x] + A[y][x] + A[y+1][x])/9;
    }
  }

  //calculating average abs
#if X_OMP_BAYER
  #pragma omp for
#endif
  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y++)
  {
    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x++)
    {
      int16 a = B[y][x];
      int32 diff = 0;
      int32 j;
      for(j=-1; j<=1; j++)
      {        
        diff += ( xAbs(Y[y+j][x-1]-a) + xAbs(Y[y+j][x]-a) + xAbs(Y[y+j][x+1]-a) );
      }     
      A[y][x] = (int16)diff;     
    }
  }

#if X_OMP_BAYER
  #pragma omp for
#endif
  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y++)
  {
    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x++)
    {      
      B[y][x] = A[y][x-1] + A[y][x] + A[y][x+1];
    }
  }

#if X_OMP_BAYER
  #pragma omp for
#endif
  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y++)
  {
    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x++)
    {      
      int32 tmp = B[y-1][x] + B[y][x] + B[y+1][x];
      A[y][x] = (int16)xThreshold(tmp,256);
    }
  }
#if X_OMP_BAYER
  }
#endif
}
void xBayer::xImprovedAdaptiveHomogeneityDirected::xCalculateHomogeneityI(xPic<int16>* HorYUV, xPic<int16>* VerYUV, xPic<int16>* HorHomogeneity, xPic<int16>* VerHomogeneity, xPic<int16>* Activity, int32 BorderLimit)
{
  int32 SizeX = HorYUV->getWidth();
  int32 SizeY = HorYUV->getHeight();  

  HorHomogeneity->set(0);
  VerHomogeneity->set(0);

  xPic<int16>::MemMap H[2] = { HorHomogeneity->getMemoryMapper(CMP_0), VerHomogeneity->getMemoryMapper(CMP_0) };

  xPic<int16>::MemMap A = Activity->getMemoryMapper(CMP_0);

  xPic<int16>::MemMap Y[2] = { HorYUV->getMemoryMapper(CMP_LM), VerYUV->getMemoryMapper(CMP_LM)};
  xPic<int16>::MemMap U[2] = { HorYUV->getMemoryMapper(CMP_CB), VerYUV->getMemoryMapper(CMP_CB)};
  xPic<int16>::MemMap V[2] = { HorYUV->getMemoryMapper(CMP_CR), VerYUV->getMemoryMapper(CMP_CR)};

#if X_OMP_BAYER
  #pragma omp parallel for
#endif
  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y++)
  {
    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x++)
    {
      if(A[y][x])     
      {
        int16 EpsilonL = xMin(
                           xMax(
                             xAbs(Y[0][y][x]-Y[0][y][x-1]),
                             xAbs(Y[0][y][x]-Y[0][y][x+1])),
                           xMax(
                             xAbs(Y[1][y][x]-Y[1][y-1][x]),
                             xAbs(Y[1][y][x]-Y[1][y+1][x])));
        int16 EpsilonC = xMin(
                           xMax(                            
                             xPow2(U[0][y][x]-U[0][y][x-1]),
                             xPow2(U[0][y][x]-U[0][y][x+1]),                            
                             xPow2(V[0][y][x]-V[0][y][x-1]),
                             xPow2(V[0][y][x]-V[0][y][x+1])),
                           xMax(
                             xPow2(U[1][y][x]-U[1][y-1][x]),
                             xPow2(U[1][y][x]-U[1][y+1][x]),                             
                             xPow2(V[1][y][x]-V[1][y-1][x]),
                             xPow2(V[1][y][x]-V[1][y+1][x])));
        
        for(int32 j=0; j<2; j++)
        {
          bool LL, CC;
          int16 HH = 0;

          LL = xAbs(Y[j][y][x]-Y[j][y][x-1]) <= EpsilonL;
          CC = (xPow2(U[j][y][x]-U[j][y][x-1]) + xPow2(V[j][y][x]-V[j][y][x-1])) <= EpsilonC;
          HH += (LL && CC);

          LL = xAbs(Y[j][y][x]-Y[j][y][x+1]) <= EpsilonL;
          CC = (xPow2(U[j][y][x]-U[j][y][x+1]) + xPow2(V[j][y][x]-V[j][y][x+1])) <= EpsilonC;
          HH += (LL && CC);

          LL = xAbs(Y[j][y][x]-Y[j][y-1][x]) <= EpsilonL;
          CC = (xPow2(U[j][y][x]-U[j][y-1][x]) + xPow2(V[j][y][x]-V[j][y-1][x])) <= EpsilonC;
          HH += (LL && CC);

          LL = xAbs(Y[j][y][x]-Y[j][y+1][x]) <= EpsilonL;
          CC = (xPow2(U[j][y][x]-U[j][y+1][x]) + xPow2(V[j][y][x]-V[j][y+1][x])) <= EpsilonC;
          HH += (LL && CC); 

          H[j][y][x] = HH;
        }
      }      
    }
  } 
}
void xBayer::xImprovedAdaptiveHomogeneityDirected::xAverageHomogeneityI(xPic<int16>* Homogeneity, xPic<int16>* Buffer, xPic<int16>* Activity, int32 BorderLimit)
{
  //spatial averaging
  int32 SizeX = Homogeneity->getWidth();
  int32 SizeY = Homogeneity->getHeight();  

  Buffer->copy(Homogeneity);

  xPic<int16>::MemMap A    = Activity->getMemoryMapper(CMP_0);
  xPic<int16>::MemMap SrcH = Buffer->getMemoryMapper(CMP_0);
  xPic<int16>::MemMap DstH = Homogeneity->getMemoryMapper(CMP_0);  

#if X_OMP_BAYER
  #pragma omp parallel for private(y,x)
#endif
  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y++)
  {
    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x++)
    {
      int32 Sum = 0;
      if(A[y][x])
      {
        for(int32 j=-1; j<=1; j++)
        {
          for(int32 i=-1; i<=1; i++)
          {
            Sum += SrcH[y+j][x+i];
          }
        }
      }
      DstH[y][x] = (int16)Sum;   
    }
  }
}
void xBayer::xImprovedAdaptiveHomogeneityDirected::xSelectSamplesI(xPic<int16>* HorSrc, xPic<int16>* VerSrc, xPic<int16>* LinSrc, xPic<int16>* Activity, xPic<int16>* HorHomogeneity, xPic<int16>* VerHomogeneity, xPic<int16>* Dst, int32 BorderLimit)
{
  int32 SizeX = Dst->getWidth();
  int32 SizeY = Dst->getHeight();  

  xPic<int16>::MemMap SrcH[3] = { HorSrc->getMemoryMapper(CMP_0), HorSrc->getMemoryMapper(CMP_1), HorSrc->getMemoryMapper(CMP_2) };
  xPic<int16>::MemMap SrcV[3] = { VerSrc->getMemoryMapper(CMP_0), VerSrc->getMemoryMapper(CMP_1), VerSrc->getMemoryMapper(CMP_2) };
  xPic<int16>::MemMap HomH    = HorHomogeneity->getMemoryMapper(CMP_0);
  xPic<int16>::MemMap HomV    = VerHomogeneity->getMemoryMapper(CMP_0);
  xPic<int16>::MemMap A       = Activity->getMemoryMapper(CMP_0);
  xPic<int16>::MemMap DstX[3] = { Dst->getMemoryMapper(CMP_0), Dst->getMemoryMapper(CMP_1), Dst->getMemoryMapper(CMP_2) };  

  Dst->copy(LinSrc);

#if X_OMP_BAYER
  #pragma omp parallel for private(y,x)
#endif
  for(int32 y=BorderLimit; y<SizeY-BorderLimit; y++)
  {
    for(int32 x=BorderLimit; x<SizeX-BorderLimit; x++)
    {
      if(A[y][x])
      {
        if(HomH[y][x] > HomV[y][x])
        {      
          DstX[0][y][x] = SrcH[0][y][x];
          DstX[1][y][x] = SrcH[1][y][x];
          DstX[2][y][x] = SrcH[2][y][x];   
        }
        else
        {       
          DstX[0][y][x] = SrcV[0][y][x];
          DstX[1][y][x] = SrcV[1][y][x];
          DstX[2][y][x] = SrcV[2][y][x];   
        }
      }
    }
  }
}
//=======================================================================================================================================================================
//void xBayer::xInterpolateLMMSE(xPic* ImgM, xPic* ImgRGB, int32 SamplePattern)
//{
//	int16** M = ImgM->m_Cmp[0]->getPel();
//	int16** R = ImgRGB->m_Cmp[0]->getPel();
//	int16** G = ImgRGB->m_Cmp[1]->getPel();
//	int16** B = ImgRGB->m_Cmp[2]->getPel();
//	int32   SizeX = ImgM->m_Cmp[0]->getWidth();
//	int32   SizeY = ImgM->m_Cmp[0]->getHeight();
//	const int32 NumPixels = SizeX*SizeY;
//
//	//Widnow size
//	const int8 MW = 4;
//	//avoid divide by zero
//	const float DivEpsilon = 0.1f / (255 * 255);
//	//interpolation filter
//	static float InterpCoeff[5] = { -0.25f, 0.5f, 0.5f, 0.5f, -0.25f }; 
//	//approximately Gaussian smothing filter
//	static float SmoothCoeff[9] = { 0.03125f, 0.0703125f, 0.1171875f,
//		0.1796875f, 0.203125f, 0.1796875f, 0.1171875f, 0.0703125f, 0.03125f };
//	
//	float mom1, h, H, mh, ph, Rh, v, V, mv, pv, Rv, Temp;
//	int32 m, m0, m1;
//
//	float** FilteredV = new float* [SizeY];
//	float** FilteredH = new float* [SizeY];
//	float** DifferenceV = new float* [SizeY];
//	float** DifferenceH = new float* [SizeY];
//	for (int y = 0; y < SizeY; y++)
//	{
//		FilteredV[y] = new float[SizeX];
//		FilteredH[y] = new float[SizeX];
//		DifferenceV[y] = new float[SizeX];
//		DifferenceH[y] = new float[SizeX];
//	}
//	
//	bool Green, RedY;
//	switch (SamplePattern)
//	{
//	case X_BAYER_PATTERN_GBRG:
//		Green = 0;
//		RedY = 1;
//		break;
//	case X_BAYER_PATTERN_GRBG:
//		Green = 0;
//		RedY = 0;
//		break;
//	case X_BAYER_PATTERN_BGGR:
//		Green = 1;
//		RedY = 1;
//		break;
//	case X_BAYER_PATTERN_RGGB:
//		Green = 1;
//		RedY = 0;
//		break;
//	}
//
//	for (int32 x = 0; x < SizeX; x++)
//	{
//		FilteredV[0][x] =
//			(InterpCoeff[2] * M[0][x] +
//				InterpCoeff[3] * M[1][x] +
//				InterpCoeff[4] * M[2][x]) * 5 / 3;
//	}
//	for (int32 x = 0; x < SizeX; x++)
//	{
//		FilteredV[1][x] =
//			(InterpCoeff[1] * M[0][x] +
//				InterpCoeff[2] * M[1][x] +
//				InterpCoeff[3] * M[2][x] +
//				InterpCoeff[4] * M[3][x]) * 1.25;
//	}
//	for (int32 y = 2; y < (SizeY - 2); y++)
//	{
//		for (int32 x = 0; x < SizeX; x++)
//		{
//			FilteredV[y][x] =
//				InterpCoeff[0] * M[y - 2][x] +
//				InterpCoeff[1] * M[y - 1][x] +
//				InterpCoeff[2] * M[y][x] +
//				InterpCoeff[3] * M[y + 1][x] +
//				InterpCoeff[4] * M[y + 2][x];
//		}
//	}
//	for (int32 x = 0; x < SizeX; x++)
//	{
//		FilteredV[SizeY - 2][x] =
//			(InterpCoeff[0] * M[SizeY - 2 - 2][x] +
//				InterpCoeff[1] * M[SizeY - 2 - 1][x] +
//				InterpCoeff[2] * M[SizeY - 2][x] +
//				InterpCoeff[3] * M[SizeY - 2 + 1][x]) * 1.25;
//	}
//	for (int32 x = 0; x < SizeX; x++)
//	{
//		FilteredV[SizeY - 1][x] =
//			(InterpCoeff[0] * M[SizeY - 1 - 2][x] +
//				InterpCoeff[1] * M[SizeY - 1 - 1][x] +
//				InterpCoeff[2] * M[SizeY - 1][x]) * 5 / 3;
//	}
//	for (int32 y = 0; y < SizeY; y++)
//	{
//		FilteredH[y][0] =
//			(InterpCoeff[2] * M[y][0] +
//				InterpCoeff[3] * M[y][1] +
//				InterpCoeff[4] * M[y][2]) * 5 / 3;
//		FilteredH[y][1] =
//			(InterpCoeff[1] * M[y][0] +
//				InterpCoeff[2] * M[y][1] +
//				InterpCoeff[3] * M[y][2] +
//				InterpCoeff[4] * M[y][3]) * 1.25;
//		for (int32 x = 2; x < (SizeX - 2); x++)
//		{
//			FilteredH[y][x] =
//				InterpCoeff[0] * M[y][x - 2] +
//				InterpCoeff[1] * M[y][x - 1] +
//				InterpCoeff[2] * M[y][x] +
//				InterpCoeff[3] * M[y][x + 1] +
//				InterpCoeff[4] * M[y][x + 2];
//		}
//		int32 x = SizeX - 2;
//		FilteredH[y][x] =
//			(InterpCoeff[0] * M[y][x - 2] +
//				InterpCoeff[1] * M[y][x - 1] +
//				InterpCoeff[2] * M[y][x] +
//				InterpCoeff[3] * M[y][x + 1]) * 1.25;
//		x++;
//		FilteredH[y][x] =
//			(InterpCoeff[0] * M[y][x - 2] +
//				InterpCoeff[1] * M[y][x - 1] +
//				InterpCoeff[2] * M[y][x]) * 5 / 3;
//	}
//	for (int32 y = 0; y < SizeY; y++)
//	{
//		for (int32 x = 0; x < SizeX; x++)
//		{
//			if (((x + y) & 1) == Green)
//			{
//				DifferenceH[y][x] = M[y][x] - FilteredH[y][x];
//				DifferenceV[y][x] = M[y][x] - FilteredV[y][x];
//			}
//			else
//			{
//				DifferenceH[y][x] = FilteredH[y][x] - M[y][x];
//				DifferenceV[y][x] = FilteredV[y][x] - M[y][x];
//			}
//		}
//	}
//	for (int32 y = 0; y < 4; y++)
//	{
//		for (int32 x = 0; x < SizeX; x++)
//		{
//			FilteredV[y][x] =
//					(SmoothCoeff[4] * DifferenceV[y][x] +
//					SmoothCoeff[5] * DifferenceV[y + 1][x] +
//					SmoothCoeff[6] * DifferenceV[y + 2][x] +
//					SmoothCoeff[7] * DifferenceV[y + 3][x] +
//					SmoothCoeff[8] * DifferenceV[y + 4][x]) * 9 / 5;
//		}
//	}
//	for (int32 y = 4; y < (SizeY - 4); y++)
//	{
//		for (int32 x = 0; x < SizeX; x++)
//		{
//			FilteredV[y][x] =
//				SmoothCoeff[0] * DifferenceV[y - 4][x] +
//				SmoothCoeff[1] * DifferenceV[y - 3][x] +
//				SmoothCoeff[2] * DifferenceV[y - 2][x] +
//				SmoothCoeff[3] * DifferenceV[y - 1][x] +
//				SmoothCoeff[4] * DifferenceV[y][x] +
//				SmoothCoeff[5] * DifferenceV[y + 1][x] +
//				SmoothCoeff[6] * DifferenceV[y + 2][x] +
//				SmoothCoeff[7] * DifferenceV[y + 3][x] +
//				SmoothCoeff[8] * DifferenceV[y + 4][x];
//		}
//	}
//	for (int32 y = (SizeY - 4); y < SizeY; y++)
//	{
//		for (int32 x = 0; x < SizeX; x++)
//		{
//			FilteredV[y][x] =
//				(SmoothCoeff[0] * DifferenceV[y - 4][x] +
//				SmoothCoeff[1] * DifferenceV[y - 3][x] +
//				SmoothCoeff[2] * DifferenceV[y - 2][x] +
//				SmoothCoeff[3] * DifferenceV[y - 1][x] +
//				SmoothCoeff[4] * DifferenceV[y][x]) * 9 / 5;
//		}
//	}
//	for (int32 y = 0; y < SizeY; y++)
//	{
//		for (int32 x = 0; x < 4; x++)
//		{
//			FilteredH[y][x] =
//				(SmoothCoeff[4] * DifferenceH[y][x] +
//				SmoothCoeff[5] * DifferenceH[y][x + 1] +
//				SmoothCoeff[6] * DifferenceH[y][x + 2] +
//				SmoothCoeff[7] * DifferenceH[y][x + 3] +
//				SmoothCoeff[8] * DifferenceH[y][x + 4]) *9/5;
//		}
//		for (int32 x = 4; x < (SizeX - 4); x++)
//		{
//			FilteredH[y][x] =
//				SmoothCoeff[0] * DifferenceH[y][x - 4] +
//				SmoothCoeff[1] * DifferenceH[y][x - 3] +
//				SmoothCoeff[2] * DifferenceH[y][x - 2] +
//				SmoothCoeff[3] * DifferenceH[y][x - 1] +
//				SmoothCoeff[4] * DifferenceH[y][x] +
//				SmoothCoeff[5] * DifferenceH[y][x + 1] +
//				SmoothCoeff[6] * DifferenceH[y][x + 2] +
//				SmoothCoeff[7] * DifferenceH[y][x + 3] +
//				SmoothCoeff[8] * DifferenceH[y][x + 4];
//		}
//		for (int32 x = (SizeX - 4); x < SizeX; x++)
//		{
//			FilteredH[y][x] =
//				(SmoothCoeff[0] * DifferenceH[y][x - 4] +
//				SmoothCoeff[1] * DifferenceH[y][x - 3] +
//				SmoothCoeff[2] * DifferenceH[y][x - 2] +
//				SmoothCoeff[3] * DifferenceH[y][x - 1] +
//				SmoothCoeff[4] * DifferenceH[y][x]) *9/5;
//		}
//	}
//
//	for (int32 y = 0, i = 0; y < SizeY; y++)
//		for (int32 x = 0; x < SizeX; x++,i++)
//		{
//			if (((x + y) & 1) != Green)
//			{
//				m0 = (x >= MW) ? -MW : -x;
//				m1 = (x < SizeX - MW) ? MW : (SizeX - x - 1);
//
//				for (m = m0, mom1 = ph = Rh = 0; m <= m1; m++)
//				{
//					Temp = FilteredH[y][x + m];
//					mom1 += Temp;
//					ph += Temp*Temp;
//					Temp -= DifferenceH[y][x + m];
//					Rh += Temp*Temp;
//				}
//
//				mh = mom1 / (2 * MW + 1);
//
//				ph = ph / (2 * MW) - mom1*mom1 / (2 * MW*(2 * MW + 1));
//				Rh = Rh / (2 * MW + 1) + DivEpsilon;
//				h = mh + (ph / (ph + Rh))*(DifferenceH[y][x] - mh);
//				H = ph - (ph / (ph + Rh))*ph + DivEpsilon;
//
//				m0 = (y >= MW) ? -MW : -y;
//				m1 = (y < SizeY - MW) ? MW : (SizeY - y - 1);
//
//				for (m = m0, mom1 = pv = Rv = 0; m <= m1; m++)
//				{
//					Temp = FilteredV[y + m][x];
//					mom1 += Temp;
//					pv += Temp*Temp;
//					Temp -= DifferenceV[y + m][x];
//					Rv += Temp*Temp;
//				}
//
//				mv = mom1 / (2 * MW + 1);
//
//				pv = pv / (2 * MW) - mom1*mom1 / (2 * MW*(2 * MW + 1));
//				Rv = Rv / (2 * MW + 1) + DivEpsilon;
//				v = mv + (pv / (pv + Rv))*(DifferenceV[y][x] - mv);
//				V = pv - (pv / (pv + Rv))*pv + DivEpsilon;
//
//				G[y][x] = M[y][x] + (V*h + H*v) / (H + V);
//			}
//			else
//				G[y][x] = M[y][x];
//		}
//
//	/* Compute the primary difference signals:
//	DifferenceGR = Green - Red   (known at red locations)
//	DifferenceGB = Green - Blue  (known at blue locations)   */
//	for (int32 y = 0; y < SizeY; y++)
//		for (int32 x = 0; x < SizeX; x++)
//			if (((x + y) & 1) != Green)
//				(((y & 1) == RedY) ? R : B)[y][x]
//				= G[y][x] - M[y][x];
//
//	/* Interpolate DifferenceGR(R) at blue locations and DifferenceGB(B) at red locations */
//	for (int32 y = 0; y < SizeY; y++)
//		for (int32 x = 0; x < SizeX; x++)
//			if (((x + y) & 1) != Green)
//			{
//				if ((y & 1) == RedY)
//					B[y][x] = xDiagonalAverage(B, SizeX, SizeY, x, y);
//				else
//					R[y][x] = xDiagonalAverage(R, SizeX, SizeY, x, y);
//			}
//	
//	/* Interpolate DifferenceGR(R) and DifferenceGB(B) at green locations */
//	for (int32 y = 0; y < SizeY; y++)
//		for (int32 x = 0; x < SizeX; x++)
//			if (((x + y) & 1) == Green)
//			{
//				B[y][x] = xAxialAverage(B, SizeX, SizeY, x, y);
//				R[y][x] = xAxialAverage(R, SizeX, SizeY, x, y);
//			}
//	/*R=G-DifferenceGR and B=G-DifferenceGB*/
//	for (int32 y = 0; y < SizeY; y++)
//		for (int32 x = 0; x < SizeX; x++)
//		{
//			R[y][x] = G[y][x] - R[y][x];
//			B[y][x] = G[y][x] - B[y][x];
//		}
//
//	for (int32 y = 0; y < SizeY; y++)
//	{
//		delete[] FilteredV[y];
//		delete[] FilteredH[y];
//		delete[] DifferenceV[y];
//		delete[] DifferenceH[y];
//	}
//	delete[] FilteredV;
//	delete[] FilteredH;
//	delete[] DifferenceV;
//	delete[] DifferenceH;
//}
//int16 xBayer::xDiagonalAverage(int16 **Img, int32 SizeX, int32 SizeY, int32 x, int32 y)
//{
//	if (y == 0)
//	{
//		if (x == 0)
//			return Img[y + 1][x + 1];
//		else if (x < SizeX - 1)
//			return (Img[y + 1][x - 1] + Img[y + 1][x + 1]) / 2;
//		else
//			return Img[y + 1][x - 1];
//	}
//	else if (y < SizeY - 1)
//	{
//		if (x == 0)
//			return (Img[y - 1][x + 1] + Img[y + 1][x + 1]) / 2;
//		else if (x < SizeX - 1)
//			return (Img[y - 1][x - 1] + Img[y - 1][x + 1]
//				+ Img[y + 1][x - 1] + Img[y + 1][x + 1]) / 4;
//		else
//			return (Img[y - 1][x - 1] + Img[y + 1][x - 1]) / 2;
//	}
//	else
//	{
//		if (x == 0)
//			return Img[y - 1][x + 1];
//		else if (x < SizeX - 1)
//			return (Img[y - 1][x - 1] + Img[y - 1][x + 1]) / 2;
//		else
//			return Img[y - 1][x - 1];
//	}
//}
//int16 xBayer::xAxialAverage(int16 ** Img, int32 SizeX, int32 SizeY, int32 x, int32 y)
//{
//	if (y == 0)
//	{
//		if (x == 0)
//			return (Img[y][x + 1] + Img[y + 1][x]) / 2;
//		else if (x < SizeX - 1)
//			return (Img[y][x - 1] + Img[y][x + 1] + 2 * Img[y + 1][x]) / 4;
//		else
//			return (Img[y][x - 1] + Img[y + 1][x]) / 2;
//
//	}
//	else if (y < SizeY - 1)
//	{
//		if (x == 0)
//			return (2 * Img[y][x + 1] + Img[y - 1][x] + Img[y + 1][x]) / 4;
//		else if (x < SizeX - 1)
//			return (Img[y][x - 1] + Img[y][x + 1] + Img[y - 1][x] + Img[y + 1][x]) / 4;
//		else
//			return (2 * Img[y][x - 1] + Img[y - 1][x] + Img[y + 1][x]) / 4;
//	}
//	else
//	{
//		if (x == 0)
//			return (Img[y][x + 1] + Img[y - 1][x]) / 2;
//		else if (x < SizeX - 1)
//			return (Img[y][x - 1] + Img[y][x + 1] + 2 * Img[y - 1][x]) / 4;
//		else
//			return (Img[y][x - 1] + Img[y - 1][x]) / 2;
//	}
//}
//=======================================================================================================================================================================
void xBayer::xScatter(xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern)
{
  xPic<int16>::MemMap M = PicM  ->getMemoryMapper(CMP_0);;
  xPic<int16>::MemMap R = PicRGB->getMemoryMapper(CMP_R);
  xPic<int16>::MemMap G = PicRGB->getMemoryMapper(CMP_G);
  xPic<int16>::MemMap B = PicRGB->getMemoryMapper(CMP_B);
  int32    SizeX = PicRGB->getWidth();
  int32    SizeY = PicRGB->getHeight();

  switch(SamplePattern)
  {
  case eBayerPattern::GBRG:
    for(int32 y=0; y<SizeY; y+=2)
    {
      for(int32 x=0; x<SizeX; x+=2)
      {
        int16 sG1 = M[y  ][x  ];
        int16 sB  = M[y  ][x+1];
        int16 sR  = M[y+1][x  ];
        int16 sG2 = M[y+1][x+1];

        R[y  ][x  ]=  0; R[y  ][x+1]=  0; R[y+1][x  ]= sR; R[y+1][x+1]=  0;
        G[y  ][x  ]=sG1; G[y  ][x+1]=  0; G[y+1][x  ]=  0; G[y+1][x+1]=sG2;
        B[y  ][x  ]=  0; B[y  ][x+1]= sB; B[y+1][x  ]=  0; B[y+1][x+1]=  0;        
      }
    }    
    break;
  case eBayerPattern::GRBG:
    for(int32 y=0; y<SizeY; y+=2)
    {
      for(int32 x=0; x<SizeX; x+=2)
      {
        int16 sG1 = M[y  ][x  ];
        int16 sR  = M[y  ][x+1];
        int16 sB  = M[y+1][x  ];
        int16 sG2 = M[y+1][x+1];

        R[y  ][x  ]=  0; R[y  ][x+1]= sR; R[y+1][x  ]=  0; R[y+1][x+1]=  0;
        G[y  ][x  ]=sG1; G[y  ][x+1]=  0; G[y+1][x  ]=  0; G[y+1][x+1]=sG2;
        B[y  ][x  ]=  0; B[y  ][x+1]=  0; B[y+1][x  ]= sB; B[y+1][x+1]=  0;        
      }
    }    
    break;
  case eBayerPattern::BGGR:
    for(int32 y=0; y<SizeY; y+=2)
    {
      for(int32 x=0; x<SizeX; x+=2)
      {
        int16 sB  = M[y  ][x  ];
        int16 sG1 = M[y  ][x+1];
        int16 sG2 = M[y+1][x  ];
        int16 sR  = M[y+1][x+1];

        R[y  ][x  ]=  0; R[y  ][x+1]=  0; R[y+1][x  ]=  0; R[y+1][x+1]=  0;
        G[y  ][x  ]=  0; G[y  ][x+1]=sG1; G[y+1][x  ]=sG2; G[y+1][x+1]=  0;
        B[y  ][x  ]= sB; B[y  ][x+1]=  0; B[y+1][x  ]=  0; B[y+1][x+1]= sR;         
      }
    }    
    break;
  case eBayerPattern::RGGB:
    for(int32 y=0; y<SizeY; y+=2)
    {
      for(int32 x=0; x<SizeX; x+=2)
      {
        int16 sR  = M[y  ][x  ];
        int16 sG1 = M[y  ][x+1];
        int16 sG2 = M[y+1][x  ];
        int16 sB  = M[y+1][x+1];

        R[y  ][x  ]= sR; R[y  ][x+1]=  0; R[y+1][x  ]=  0; R[y+1][x+1]= sB;
        G[y  ][x  ]=  0; G[y  ][x+1]=sG1; G[y+1][x  ]=sG2; G[y+1][x+1]=  0;
        B[y  ][x  ]=  0; B[y  ][x+1]=  0; B[y+1][x  ]=  0; B[y+1][x+1]=  0;        
      }
    }    
    break;
  default: xAssert(0); break;
  }
}
void xBayer::xDecimate(xPic<int16>* PicM, xPic<int16>* PicRGB, eBayerPattern SamplePattern)
{
  xPic<int16>::MemMap M = PicM  ->getMemoryMapper(CMP_0);;
  xPic<int16>::MemMap R = PicRGB->getMemoryMapper(CMP_R);
  xPic<int16>::MemMap G = PicRGB->getMemoryMapper(CMP_G);
  xPic<int16>::MemMap B = PicRGB->getMemoryMapper(CMP_B);
  int32    SizeX = PicM  ->getWidth();
  int32    SizeY = PicM  ->getHeight();

  switch(SamplePattern)
  {
  case eBayerPattern::GBRG:
    for (int32 y = 0; y<SizeY; y += 2)
    {
      for (int32 x = 0; x<SizeX; x += 2)
      {
        int16 sG1 = M[y][x];
        int16 sB  = M[y][x + 1];
        int16 sR  = M[y + 1][x];
        int16 sG2 = M[y + 1][x + 1];
        int16 sG  = (sG1 + sG2 + 1) >> 1;

        int32 i = x >> 1;
        int32 j = y >> 1;
        R[j][i] = sR;
        G[j][i] = sG;
        B[j][i] = sB;
      }
    }
    break;
  case eBayerPattern::GRBG:
    for (int32 y = 0; y<SizeY; y += 2)
    {
      for (int32 x = 0; x<SizeX; x += 2)
      {
        int16 sG1 = M[y][x];
        int16 sR  = M[y][x + 1];
        int16 sB  = M[y + 1][x];
        int16 sG2 = M[y + 1][x + 1];
        int16 sG  = (sG1 + sG2 + 1) >> 1;

        int32 i = x >> 1;
        int32 j = y >> 1;
        R[j][i] = sR;
        G[j][i] = sG;
        B[j][i] = sB;
      }
    }
    break;
  case eBayerPattern::BGGR:
    for (int32 y = 0; y<SizeY; y += 2)
    {
      for (int32 x = 0; x<SizeX; x += 2)
      {
        int16 sB  = M[y][x];
        int16 sG1 = M[y][x + 1];
        int16 sG2 = M[y + 1][x];
        int16 sR  = M[y + 1][x + 1];
        int16 sG  = (sG1 + sG2 + 1) >> 1;

        int32 i = x >> 1;
        int32 j = y >> 1;
        R[j][i] = sR;
        G[j][i] = sG;
        B[j][i] = sB;
      }
    }
    break;
  case eBayerPattern::RGGB:
    for (int32 y = 0; y<SizeY; y += 2)
    {
      for (int32 x = 0; x<SizeX; x += 2)
      {
        int16 sR  = M[y][x];
        int16 sG1 = M[y][x + 1];
        int16 sG2 = M[y + 1][x];
        int16 sB  = M[y + 1][x + 1];
        int16 sG  = (sG1 + sG2 + 1) >> 1;

        int32 i = x >> 1;
        int32 j = y >> 1;
        R[j][i] = sR;
        G[j][i] = sG;
        B[j][i] = sB;
      }
    }
    break;
  default: xAssert(0); break;
  }
}
void xBayer::xSimulate(xPic<int16>* PicRGB, xPic<int16>* PicM, eBayerPattern SamplePattern)
{
  xPic<int16>::MemMap M = PicM  ->getMemoryMapper(CMP_0);
  xPic<int16>::MemMap R = PicRGB->getMemoryMapper(CMP_R);
  xPic<int16>::MemMap G = PicRGB->getMemoryMapper(CMP_G);
  xPic<int16>::MemMap B = PicRGB->getMemoryMapper(CMP_B);
  int32    SizeX = PicRGB->getWidth();
  int32    SizeY = PicRGB->getHeight();

  switch(SamplePattern)
  {
  case eBayerPattern::GBRG:
    for(int32 y=0; y<SizeY; y+=2)
    {
      for(int32 x=0; x<SizeX; x+=2)
      {
        M[y  ][x  ] = G[y  ][x  ];
        M[y  ][x+1] = B[y  ][x+1];
        M[y+1][x  ] = R[y+1][x  ];
        M[y+1][x+1] = G[y+1][x+1];
      }
    }    
    break;
  case eBayerPattern::GRBG:
    for(int32 y=0; y<SizeY; y+=2)
    {
      for(int32 x=0; x<SizeX; x+=2)
      {
        M[y  ][x  ] = G[y  ][x  ];
        M[y  ][x+1] = R[y  ][x+1];
        M[y+1][x  ] = B[y+1][x  ];
        M[y+1][x+1] = G[y+1][x+1];
      }
    }    
    break;
  case eBayerPattern::BGGR:
    for(int32 y=0; y<SizeY; y+=2)
    {
      for(int32 x=0; x<SizeX; x+=2)
      {
        M[y  ][x  ] = B[y  ][x  ];
        M[y  ][x+1] = G[y  ][x+1];
        M[y+1][x  ] = G[y+1][x  ];
        M[y+1][x+1] = R[y+1][x+1];
      }
    }    
    break;
  case eBayerPattern::RGGB:
    for(int32 y=0; y<SizeY; y+=2)
    {
      for(int32 x=0; x<SizeX; x+=2)
      {
        M[y  ][x  ] = R[y  ][x  ];
        M[y  ][x+1] = G[y  ][x+1];
        M[y+1][x  ] = G[y+1][x  ];
        M[y+1][x+1] = B[y+1][x+1];
      }
    }    
    break;
  default: xAssert(0); break;
  }
}
//=======================================================================================================================================================================
void xBayer::interpolate(xPic<int16>* Src, xPic<int16>* Dst, eBayerPattern SamplePattern, eBayerAlgorithm Algorithm)
{
  switch(Algorithm)
  {
  case eBayerAlgorithm::Scatter:  return xScatter(Src, Dst, SamplePattern);
  case eBayerAlgorithm::Nearest:  return interpolateNearest(Src, Dst, SamplePattern);
  case eBayerAlgorithm::Bilinear: return interpolateBilinear(Src, Dst, SamplePattern);
  case eBayerAlgorithm::PG:       return interpolatePG(Src, Dst, SamplePattern);
  case eBayerAlgorithm::AHD:      return interpolateAHD(Src, Dst, SamplePattern);
  case eBayerAlgorithm::IAHD:     return interpolateIAHD(Src, Dst, SamplePattern); 
  default:  xAssert(0); //x_print_err("x_bayer_interpolate: unknown algorithm !!!");
  }
}
void xBayer::scatter(xPic<int16>* Src, xPic<int16>* Dst, eBayerPattern SamplePattern)
{
  xScatter(Src, Dst, SamplePattern);
}
void xBayer::decimate(xPic<int16>* Src, xPic<int16>* Dst, eBayerPattern SamplePattern)
{
  xDecimate(Src, Dst, SamplePattern);
}
void xBayer::simulate(xPic<int16>* Src, xPic<int16>* Dst, eBayerPattern SamplePattern)
{
  xSimulate(Src, Dst, SamplePattern);
}
} //end of namespace AVLib
