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

#include "xConsolePlayer.h"
#ifndef WIN32
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif

using namespace std;
using namespace AVlib;

xConsolePlayer* xConsolePlayer::staticSelf = nullptr;

//=============================================================================================================================================================================

#define ESC "\x1b"
#define CLEAR_SCREEN ESC"[2J"
#define SAVE_CURSOR_POSITION ESC"\x1b""7"
#define RESTORE_CURSOR_POSITION ESC"\x1b""8"
#define RESET_FORMATS ESC"[0m"
#define FOREGROUND_COLOR "[38;2;%03d;%03d;%03dm"
#define BACKGROUND_COLOR "[48;2;%03d;%03d;%03dm"
#define HIDE_CURSOR ESC"[?25l"
#define SHOW_CURSOR ESC"[?25h"

//=============================================================================================================================================================================
static const char HelpString[] =
R"AVLIBRAWSTRING(
Lamb Console Player 1.0
F1     - Show help
F2     - OSD on/off
J      - Jump to frame
Q      - Exit
+/-    - Zoom in/out
A,S,D,W- Move zoom window
Arrows - Next/previous frame
)AVLIBRAWSTRING";

//=============================================================================================================================================================================
void xConsolePlayer::xPrepareConsole()
{
#ifdef WIN32
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
  // enable ANSI sequences for windows 10:
  HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD consoleMode;
  GetConsoleMode(console, &consoleMode);
  consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(console, consoleMode);
#endif
}
//=============================================================================================================================================================================
void xConsolePlayer::xHideCursor()
{
#ifndef WIN32
  int kfd = 0;
  struct termios cooked, raw;
  char c;
  bool dirty = false;

  //get the console in raw mode
  //*
  tcgetattr(kfd, &cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  //raw.c_lflag &= ~(ICANON | ECHO);
  raw.c_lflag &= ~(ICANON | ECHO);
  // Setting a new line, then end of file
  raw.c_cc[VEOL] = 1;
  raw.c_cc[VEOF] = 2;
  tcsetattr(kfd, TCSANOW, &raw);
  //*/
  int flags = fcntl(0, F_GETFL, 0); /* get current file status flags */
  flags |= O_NONBLOCK;          /* turn off blocking flag */
  fcntl(0, F_SETFL, flags);         /* set up non-blocking read */
#endif
  printf(HIDE_CURSOR);
}
//=============================================================================================================================================================================
void xConsolePlayer::xShowCursor()
{
  printf(SHOW_CURSOR);
#ifndef WIN32
  int kfd = 0;
  struct termios cooked, raw;
  char c;

  //*
  tcgetattr(kfd, &cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  raw.c_lflag |= (ICANON | ECHO);
  // Setting a new line, then end of file
  raw.c_cc[VEOL] = 1;
  raw.c_cc[VEOF] = 2;
  tcsetattr(kfd, TCSANOW, &raw);//*/

  int flags = fcntl(0, F_GETFL, 0); /* get current file status flags */
  flags &= ~O_NONBLOCK;          /* turn off blocking flag */
  fcntl(0, F_SETFL, flags);         /* set up non-blocking read */
#endif
}
//=============================================================================================================================================================================
int32V2 xConsolePlayer::xGetConsoleSize()
{
  // get console information
  int32 Columns, Rows;
#if WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  Columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  Rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  Rows = w.ws_row;
  Columns = w.ws_col;
#endif
  //printf("%d \t %d", Columns, Rows);
  return { Columns, Rows };
}
//=============================================================================================================================================================================
int32V2 xConsolePlayer::xCalcOutputSize()
{
  int32 OutWidth;
  int32 OutHeight;

  //Keep aspect ratio
  OutWidth = m_ConsoleSize[0];
  OutHeight = m_InputSize[1] * OutWidth / m_InputSize[0];
  if (OutHeight > m_ConsoleSize[1] * 2)
  {
    OutHeight = m_ConsoleSize[1] * 2;
    OutWidth = m_InputSize[0] * OutHeight / m_InputSize[1];
  }

  return { OutWidth, OutHeight };
}
//=============================================================================================================================================================================
void xConsolePlayer::xCheckCropSize()
{
  if (m_CropOrign.getX() + m_CropSize.getX() > m_InputSize.getX())
  {
    m_CropOrign.setX(m_InputSize.getX() - m_CropSize.getX());
  }
  if (m_CropOrign.getY() + m_CropSize.getY() > m_InputSize.getY())
  {
    m_CropOrign.setY(m_InputSize.getY() - m_CropSize.getY());
  }
  if (m_CropOrign.getX() < 0)
  {
    m_CropOrign.setX(0);
  }
  if (m_CropOrign.getY() < 0)
  {
    m_CropOrign.setY(0);
  }
}
//=============================================================================================================================================================================
void xConsolePlayer::xDisplayPicture(xPic<int16>* pic)
{
  int32 Width = pic->getWidth();
  int32 Height = pic->getHeight();

  auto DisplayPixel = [pic](int32 x, int32 y)
  {
    int32V2 PosUpper = { x, y };
    int32V2 PosLower = { x, y + 1 };
    int16V3 RGBUpper = pic->getPixelValues3(PosUpper);
    int16V3 RGBLower = pic->getPixelValues3(PosLower);
    /*int16V3 RGBLower;
    if (y + 1 < pic->getHeight())
      RGBLower = pic->getPixelValues3(PosLower);
    else
      RGBLower = { 0,0,0 };//*/

    printf(ESC FOREGROUND_COLOR ESC BACKGROUND_COLOR, RGBUpper[0], RGBUpper[1], RGBUpper[2], RGBLower[0], RGBLower[1], RGBLower[2]);
    //printf("\xdc");
#ifdef WIN32
    printf("\xdf");
#else
    //printf("\xdc");
    //wprintf(L"\xe2\x96\x84");
    //printf("\xe2\x96\x84");
    printf("\xe2\x96\x80");
    //printf("\xdc");
#endif
    //printf("x");
  };

  std::ios_base::sync_with_stdio(false);
  //printf(CLEAR_SCREEN);
  printf(RESTORE_CURSOR_POSITION);
  char Frames[200];
  sprintf(Frames, "%d / %d", m_FrameId, m_NumberOfFrames);
  int32 FrameStrLen = strlen(Frames);
  if (m_ShowInfo)
  {
    for (int32 x = 0; x < Width - FrameStrLen; x++)
    {
      DisplayPixel(x, 0);
    }
    printf(ESC FOREGROUND_COLOR ESC BACKGROUND_COLOR "%s\n", 255, 255, 255, 100, 100, 100, Frames);
  }
  else
  {
    for (int32 x = 0; x < Width; x++)
    {
      DisplayPixel(x, 0);
    }
    printf("\n");
  }  
  for (int32 y = 2; y < (Height / 2 - 1) * 2; y += 2)
  {
    for (int32 x = 0; x < Width; x++)
    {
      DisplayPixel(x, y);
    }
    printf("\n");
  }
  for (int32 x = 0; x < Width - 1; x++)
  {
    DisplayPixel(x, Height - 2);
  }

  //printf("\x1b[0m");
  //printf("\x1b[38;2;%03d;%03d;%03dm\x1b[48;2;%03d;%03d;%03dm", 30, 30, 30, 80, 80, 80);
  //wprintf(L"\xe2\x96\x84");
  //printf("\xdc");
  printf(RESET_FORMATS);
  fflush(stdout);
}
//=============================================================================================================================================================================
void xConsolePlayer::xDisplayHelp()
{
  printf(RESTORE_CURSOR_POSITION"%s", HelpString);
}
//=============================================================================================================================================================================
void xConsolePlayer::xGetFrameFromUser()
{
  int32 log10Frame = log10(m_NumberOfFrames) + 1;
  int32 xPos = (m_ConsoleSize[0] - (11 + log10Frame)) / 2;
  int32 yPos = m_ConsoleSize[1] / 2;
  printf(RESTORE_CURSOR_POSITION);
  //printf(ESC"(0");
  printf(ESC"[%d;%dH", yPos - 1, xPos);
#ifdef WIN32
  printf("\xda\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4");//10
#else
  printf("\xe2\x94\x8c\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80");
#endif
  for (int32 i = 0; i < log10Frame; i++)
#ifdef WIN32
    printf("\xc4"); //log10Frame
#else
    printf("\xe2\x94\x80");
#endif
#ifdef WIN32
  printf("\xbf"); //1
#else
  printf("\xe2\x94\x90");
#endif

  printf(ESC"[%d;%dH", yPos, xPos);
#ifdef WIN32
  printf("\xb3 Frame: ");
#else
  printf("\xe2\x94\x82 Frame: ");
#endif
  for (int32 i = 0; i < log10Frame; i++)printf(" ");
#ifdef WIN32
  printf(" \xb3");
#else
  printf(" \xe2\x94\x82");
#endif

  printf(ESC"[%d;%dH", yPos + 1, xPos);
#ifdef WIN32
  printf("\xc0\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4");
#else
  printf("\xe2\x94\x94\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80");
#endif
  for (int32 i = 0; i < log10Frame; i++) 
#ifdef WIN32
    printf("\xc4");
#else
    printf("\xe2\x94\x80");
#endif
#ifdef WIN32
  printf("\xd9");
#else
  printf("\xe2\x94\x98");
#endif
  //printf(ESC"(B");

  printf(ESC"[%d;%dH", yPos, xPos + 9);
  
  xShowCursor();
  int32 FrameId;
  scanf("%d", &FrameId);
  xHideCursor();
  
  if ((FrameId >= 0) && (FrameId < m_NumberOfFrames))
  {
    m_FrameId = FrameId;
    xReadFrame();
    xRescaleFrame();
  }
}
//=============================================================================================================================================================================
void xConsolePlayer::xReadFrame()
{
  m_SeqIn->seek(m_FrameId, xFile::seek_mode::beg);
  m_SeqIn->readPacked(m_PackedIn, true);
  m_PackedIn->unpack(m_PictureIn);
  m_PictureIn444->rescaleChroma(m_PictureIn, true);
  m_PictureInRGB->convertYUV2RGB(m_PictureIn444, eClrSpc::BT601);
}
//=============================================================================================================================================================================
void xConsolePlayer::xRescaleFrame()
{
  if (m_ZoomFactor > 1)
  {
    m_PictureCropRGB->copyEx(m_PictureInRGB, 0, 0, m_CropOrign.getX(), m_CropOrign.getY(), m_CropOrign.getX()+m_CropSize.getX()-1, m_CropOrign.getY()+m_CropSize.getY()-1);
  }
  else
  {
    std::swap(m_PictureInRGB, m_PictureCropRGB);
  }

  if (m_ResizeSteps > 1)
  {
    m_PictureResizeRGBVector[0]->rescaleD2Avg(m_PictureCropRGB);
    for (int32 s = 1; s < m_ResizeSteps - 1; s++) m_PictureResizeRGBVector[s]->rescaleD2Avg(m_PictureResizeRGBVector[s - 1]);
    m_PictureOutRGB->rescaleBilinear(m_PictureResizeRGBVector[m_ResizeSteps - 2]);
  }
  else
  {
    m_PictureOutRGB->rescaleBilinear(m_PictureCropRGB);
  }

  if(!(m_ZoomFactor > 1))
  {
    std::swap(m_PictureInRGB, m_PictureCropRGB);
  }
}
//=============================================================================================================================================================================
#ifdef WIN32
void xConsolePlayer::xKeyEventProc(KEY_EVENT_RECORD ker)
{
#define VK_Q 0x51 
#define VK_J 0x4A
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
#define VK_W 0x57
#else
void xConsolePlayer::xKeyEventProc(int key)
{
#define VK_LEFT 68
#define VK_RIGHT 67
#define VK_F1 80
#define VK_F2 81
#define VK_Q 113
#define VK_J 106
#define VK_A 97
#define VK_S 115
#define VK_D 100
#define VK_W 119
#define VK_OEM_PLUS '='
#define VK_OEM_MINUS '-'
#endif
  
#ifdef WIN32
  if (ker.bKeyDown == true)
  {
    switch (ker.wVirtualKeyCode)
#else
  {
    switch (key)
#endif
    {
    case VK_RIGHT:
    {
      m_FrameId = m_FrameId + 1;
      if (m_FrameId == m_NumberOfFrames) m_FrameId = 0;
      xReadFrame();
      xRescaleFrame();
      m_RefreshScreen = true;
    }
    break;
    case VK_LEFT:
    {
      m_FrameId = m_FrameId - 1;
      if (m_FrameId < 0) m_FrameId = m_NumberOfFrames - 1;
      xReadFrame();
      xRescaleFrame();
      m_RefreshScreen = true;
    }
    break;
    case VK_F1:
    {
      m_ShowHelp = !m_ShowHelp;
      m_RefreshScreen = true;
    }
    break;
    case VK_F2:
    {
      m_ShowInfo = !m_ShowInfo;
      m_RefreshScreen = true;
    }
    break;
    case VK_Q: //Q
    {
      m_Done = true;
    }
    break;
    case VK_J: //J
    {
      //m_Done = true;
      xGetFrameFromUser();
      m_RefreshScreen = true;
    }
    break;
    case VK_A://A
    {
      m_CropOrign -= {m_CropSize.getX() >> 3, 0};
      xCheckCropSize();
      xRescaleFrame();
      m_RefreshScreen = true;
    }
    break;
    case VK_D://D
    {
      m_CropOrign += {m_CropSize.getX() >> 3, 0};
      xCheckCropSize();
      xRescaleFrame();
      m_RefreshScreen = true;
    }
    break;
    case VK_W://W
    {
      m_CropOrign -= {0, m_CropSize.getY() >> 3};
      xCheckCropSize();
      xRescaleFrame();
      m_RefreshScreen = true;
    }
    break;
    case VK_S://S
    {
      m_CropOrign += {0, m_CropSize.getY() >> 3};
      xCheckCropSize();
      xRescaleFrame();
      m_RefreshScreen = true;
    }
    break;
    case VK_OEM_PLUS: //+187
    {
      m_ZoomFactor += 1;
      m_CropOrign += m_CropSize >> 2;

      if (m_ZoomFactor > 1) m_CropSize = m_InputSize >> (m_ZoomFactor-1);
      else                  m_CropSize = m_InputSize;
      init();
      xRescaleFrame();
      m_RefreshScreen = true;
    }
    break;
    case VK_OEM_MINUS: //-188
    {
      if (m_ZoomFactor > 1) 
      {
        m_ZoomFactor -= 1;
        m_CropOrign -= m_CropSize >> 1;
      }
      if (m_ZoomFactor > 1) m_CropSize = m_InputSize >> (m_ZoomFactor-1);
      else                  m_CropSize = m_InputSize;
      xCheckCropSize();
      init();
      xRescaleFrame();
      m_RefreshScreen = true;
    }
    break;
    }
  }
}
//=============================================================================================================================================================================
#ifdef WIN32
void xConsolePlayer::xResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
#else
void xConsolePlayer::xResizeEventProc(int wbsr)
#endif
{
  init();
  xReadFrame();
  xRescaleFrame();
  //wbsr.dwSize.X, wbsr.dwSize.Y);
  m_RefreshScreen = true;
}
#ifndef WIN32
void xConsolePlayer::xStaticResizeSignal(int32 a)
{
  staticSelf->xResizeEventProc(a);
}
#endif
//=============================================================================================================================================================================
void xConsolePlayer::create(xCfgSection* Cfg)
{
  //readed from commandline/config
  string   InputFile      =         Cfg->getParamArg("InputFile"     , string(""));
  int32    Width          =         Cfg->getParamArg("Width"         , 640);
  int32    Height         =         Cfg->getParamArg("Height"        , 480);
  eCrF     ChromaFormat   = (eCrF)  Cfg->getParamArg("ChromaFormat"  , 420);
  int32    BitDepth       =         Cfg->getParamArg("BitDepth"      , 8);
  int32    PrefetchSize   =         Cfg->getParamArg("PrefetchSize"  , 0);
  int32    VerboseLevel   =         Cfg->getParamArg("VerboseLevel"  , 0);

  m_PackedTankIn = new xPackedTank; m_PackedTankIn->create(Width, Height, BitDepth, eImgTp::YUV, ePckTp::Planar, ChromaFormat, eCmpO::YUV, 0);
  m_SeqIn = new xSeqRAW; m_SeqIn->createRead(Width, Height, BitDepth, eImgTp::YUV, ePckTp::Planar, ChromaFormat, eCmpO::YUV, InputFile, PrefetchSize, m_PackedTankIn);
  m_NumberOfFrames = m_SeqIn->getNumOfFrames();
  m_FrameId = 0;

  m_PackedIn = m_PackedTankIn->get();
  m_PictureIn = new xPic<int16>; m_PictureIn->create(Width, Height, 0, BitDepth, eImgTp::YUV, ChromaFormat);
  m_PictureIn444 = new xPic<int16>; m_PictureIn444->create(Width, Height, 0, BitDepth, eImgTp::YUV, CrF_444);
  m_PictureInRGB = new xPic<int16>; m_PictureInRGB->create(Width, Height, 0, BitDepth, eImgTp::RGB, CrF_444);

  m_InputSize = { Width, Height };
  m_CropSize = m_InputSize;
  m_CropOrign = { 0, 0 };
}
//=============================================================================================================================================================================
void xConsolePlayer::init()
{
  m_ConsoleSize = xGetConsoleSize();
  m_OutputSize = xCalcOutputSize();

  if (m_PictureOutRGB != nullptr) { m_PictureOutRGB->destroy(); delete m_PictureOutRGB; };
  m_PictureOutRGB = new xPic<int16>; m_PictureOutRGB->create(m_OutputSize, 0, 8, eImgTp::RGB, CrF_444);

  while (!m_PictureResizeRGBVector.empty())
  {
    xPic<int16>* PictureResizeRGB = m_PictureResizeRGBVector.back();
    m_PictureResizeRGBVector.pop_back();
    if (PictureResizeRGB != nullptr) { PictureResizeRGB->destroy(); delete PictureResizeRGB; };
  }

  m_ResizeSteps = xLog2((uint32)m_CropSize[0] / m_OutputSize[0]);
  for (int32 i = 0; i < m_ResizeSteps - 1; i++)
  {
    xPic<int16>* PictureResizeRGB = new xPic<int16>; PictureResizeRGB->create(m_CropSize >> (i + 1), 0, m_PictureInRGB->getBitDepth(), eImgTp::RGB, CrF_444);
    m_PictureResizeRGBVector.push_back(PictureResizeRGB);
  }

  if (m_PictureCropRGB != nullptr) { m_PictureCropRGB->destroy(); delete m_PictureCropRGB; };
  m_PictureCropRGB = new xPic<int16>; m_PictureCropRGB->create(m_CropSize, 0, m_PictureInRGB->getBitDepth(), eImgTp::RGB, CrF_444);
}
//=============================================================================================================================================================================
void xConsolePlayer::run()
{
#ifdef WIN32
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
#else
  staticSelf = this;
#endif

  xPrepareConsole();
  printf(CLEAR_SCREEN SAVE_CURSOR_POSITION); //Clear the screen Save cursor position in memory
  xHideCursor();
#ifndef WIN32
  signal(SIGWINCH, xConsolePlayer::xStaticResizeSignal);
#endif
  
  xReadFrame();
  xRescaleFrame();

  while (!m_Done)
  //for(int32 i=0;i<100;i++)
  {
    if (m_RefreshScreen)
    {
      if (m_ShowHelp) xDisplayHelp();
      else            xDisplayPicture(m_PictureOutRGB);
      m_RefreshScreen = false;
    }

#ifdef WIN32
    DWORD cNumRead;
    INPUT_RECORD irInBuf[128];
    ReadConsoleInput(hStdin,irInBuf,128,&cNumRead);
    for (int32 i = 0; i < cNumRead; i++)
    {
      switch (irInBuf[i].EventType)
      {
      case KEY_EVENT: // keyboard input 
        xKeyEventProc(irInBuf[i].Event.KeyEvent);
        break;

      case MOUSE_EVENT: // mouse input 
        //MouseEventProc(irInBuf[i].Event.MouseEvent);
        break;

      case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
        xResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
        break;

      case FOCUS_EVENT:  // disregard focus events 

      case MENU_EVENT:   // disregard menu events 
        break;

      default:
        //ErrorExit("Unknown event type");
        break;
      }
    }
#else
    int kfd = 0;
    char c;
    size_t len = read(kfd, &c, 1);
    if (len < 0)
    {
      //perror("read():");
      //exit(-1);
    }
    //printf("%c", c);
    if (len==1) xKeyEventProc(c);
#endif
  } //end of main loop
  xShowCursor();
  printf("\n");
  //signal(SIDWINCH,do_resize)
}

