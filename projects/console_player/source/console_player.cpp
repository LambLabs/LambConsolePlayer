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
#include "xCfg.h"
//#include "xSequence.h"
#include "xConsolePlayer.h"
#include <chrono>
#ifdef WIN32
#include <Windows.h>
#endif

using namespace std;
using namespace AVlib;

//TODO
//Add Class for ConsolePlayer and methods to used it

static const char HelpString[] = 
R"AVLIBRAWSTRING(
============================================================================================================= 
Usage:
Cmd | Description
-i    Input filename
-w    Width in pixels
-h    Heigth in pixels
-v    Verbose level

Example:
console_player.exe -i "TestTest_1920x1080.yuv" -w 1920 -h 1080

==============================================================================================================
)AVLIBRAWSTRING";
int main(int argc, char *argv[], char *envp[])
{
  printf("PUT ConsolePlayer software\n");
  printf("Copyright (c) Lider VIII 2018 Poznan University of Technology\n");
  printf("All rights reserved\n");
  printf("\n");

  //==============================================================================
  // CPU check
  xCpuInfo CpuInfo;
  CpuInfo.checkCPIUD();
  if(!CpuInfo.checkCompatibility()) { printf("Your CPU is compatible with all reqired instruction sets\n"); }
  else                              { printf("Your CPU is incompatible with some of reqired instruction sets\n"); }
  printf("\n");

  //==============================================================================
  // parsing configuration
  xCfgParser::printCommandlineArgs(argc, argv);
  xCfgParser CfgParser;
  CfgParser.addCommandlineParam(xCfgCmdParam("-i"  , "", "InputFile"         ));
  CfgParser.addCommandlineParam(xCfgCmdParam("-w"  , "", "Width"             ));
  CfgParser.addCommandlineParam(xCfgCmdParam("-h"  , "", "Height"            ));
  CfgParser.addCommandlineParam(xCfgCmdParam("-cf ", "", "ChromaFormat"      ));
  CfgParser.addCommandlineParam(xCfgCmdParam("-bps", "", "BitDepth"          ));
  CfgParser.addCommandlineParam(xCfgCmdParam("-v"  , "", "VerboseLevel"      ));

  CfgParser.loadFromCommandline(argc, argv, "Cfg");
  xCfgSection& RootSection = CfgParser.getRootSection();

  string   InputFile = RootSection.getParamArg("InputFile", string(""));
  
  //print config
  printf("Configuration:\n");
  printf("InputFile          = %s\n", InputFile .c_str());
  printf("\n");

  //check config
  if(InputFile == string("")) { printf(HelpString); return EXIT_FAILURE; }
  if(!xFile::exist(InputFile)) { printf("File: %s does not exist!!!\n", InputFile.c_str()); return EXIT_FAILURE; }

  xConsolePlayer* ConsolePlayer = new xConsolePlayer; ConsolePlayer->create(&RootSection);
  xConsolePlayer::staticSelf = ConsolePlayer;
  ConsolePlayer->init();
  ConsolePlayer->run();

  return EXIT_SUCCESS;
}




