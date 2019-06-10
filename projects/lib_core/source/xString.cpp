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

#include "xString.h"
#include <time.h>

namespace AVlib {

//=============================================================================================================================================================================

std::string xString::replaceFirst(const std::string& Source, const std::string& Token, const std::string& ReplaceTo)
{
  std::string Result = Source;
  size_t CurrentPos = Result.find(Token);
  if(CurrentPos != std::string::npos)
  {
    Result.replace(CurrentPos, Token.length(), ReplaceTo);      
  }
  return Result;
}
std::string xString::replaceLast(const std::string& Source, const std::string& Token, const std::string& ReplaceTo)
{
  std::string Result = Source;
  size_t CurrentPos = Result.rfind(Token);
  if(CurrentPos != std::string::npos)
  {
    Result.replace(CurrentPos, Token.length(), ReplaceTo);      
  }
  return Result;
}
std::string xString::replaceAll(const std::string& Source, const std::string& Token, const std::string& ReplaceTo)
{
  std::string Result = Source;
  size_t CurrentPos = 0;
  while((CurrentPos = Result.find(Token, CurrentPos)) != std::string::npos)
  {
    Result.replace(CurrentPos, Token.length(), ReplaceTo);
    CurrentPos += ReplaceTo.length();
  }
  return Result;
}
std::string xString::formatString(std::string Format, ...)
{
  va_list VariableArgs;
  va_start(VariableArgs, Format);
  std::string Result = formatString(Format, VariableArgs);
  va_end(VariableArgs);
  return Result;
}
std::string xString::formatString(const std::string& Format, va_list VariableArgs)
{
  va_list VariableArgsTmp;
  va_copy(VariableArgsTmp, VariableArgs);
  size_t Size = vsnprintf(nullptr, 0, Format.c_str(), VariableArgsTmp) + 1; // Extra space for '\0'
  va_end(VariableArgsTmp);
  char* Buffer = (char*)xMalloc(Size);
  size_t Size2 = vsnprintf(Buffer, Size, Format.c_str(), VariableArgs);
  std::string Result(Buffer, Size2);
  xFree(Buffer);
  return Result;
}
int32 xString::scanString(const std::string& Scan, const std::string Format, ...)
{
  va_list VariableArgs;
  va_start(VariableArgs, Format);
  int32 RetVal = vsscanf(Scan.c_str(), Format.c_str(), VariableArgs);
  va_end(VariableArgs);
  return RetVal;
}
std::string xString::TimeToString(std::time_t Time, const std::string& Format)
{ 
  std::ostringstream O;
#if X_SYSTEM_WINDOWS
  std::tm TimeType;
  localtime_s(&TimeType, &Time);
  O << std::put_time(&TimeType, Format.c_str());
#elif X_SYSTEM_LINUX
  std::tm TimeType;
  localtime_r(&Time, &TimeType);
  O << std::put_time(&TimeType, Format.c_str());
#else
  O << std::put_time(std::localtime(&Time), Format.c_str());
#endif
  return O.str();
}

//=============================================================================================================================================================================

} //end of namespace AVLib
