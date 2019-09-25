#pragma once
//============================================================\\
//                   Lamb Console Player                      \\
//         Terminal YUV Player for linux and windows          \\
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
#include "xSequence.h"
#include "xCfg.h"

using namespace AVlib;

//=============================================================================================================================================================================

class xConsolePlayer
{
protected:
  xCfgSection* m_Cfg = nullptr;

  xPackedTank* m_PackedTankIn = nullptr;
  xSeqRAW*  m_SeqIn = nullptr;
  xPacked*  m_PackedIn = nullptr;
  xPic<int16>* m_PictureIn = nullptr;
  xPic<int16>* m_PictureIn444 = nullptr;
  xPic<int16>* m_PictureInRGB = nullptr;
  xPic<int16>* m_PictureCropRGB = nullptr;
  xPic<int16>* m_PictureOutRGB = nullptr;
  std::vector<xPic<int16>*> m_PictureResizeRGBVector;

  int32 m_ZoomFactor = 1.0;
  int32 m_ResizeSteps;

  int32V2 m_ConsoleSize;
  int32V2 m_InputSize;
  int32V2 m_CropSize; //Size of the image after cropping (for zoom purpose)
  int32V2 m_CropOrign; //Origin of the image after cropping (for zoom purpose)
  int32V2 m_OutputSize;

  int32 m_FrameId;
  int32 m_NumberOfFrames;

  bool m_Done = false;
  bool m_ShowInfo = true;
  bool m_ShowHelp = false;
  bool m_RefreshScreen = true;
protected:
  void xDisplayPicture(xPic<int16>* pic);
  void xDisplayHelp();
  void xGetFrameFromUser();
  int32V2 xGetConsoleSize();
  int32V2 xCalcOutputSize();
  void xCheckCropSize();
  void xReadFrame();
  void xRescaleFrame();
  void xHideCursor();
  void xShowCursor();
  void xPrepareConsole();
#ifdef WIN32
  void xKeyEventProc(KEY_EVENT_RECORD ker);
  void xResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr);
#else
  void xKeyEventProc(int32 ker);
  void xResizeEventProc(int32 wbsr);
  static void xStaticResizeSignal(int32 a);
#endif
public:
  static xConsolePlayer* staticSelf;
  void create(xCfgSection* Cfg);
  void init();
  void run();
};
