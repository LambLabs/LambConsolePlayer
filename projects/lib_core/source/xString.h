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
#include "xCommon.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ctime>

#define FMT_HEADER_ONLY
#define FMT_USE_WINDOWS_H 0
#include "fmt/format.h"
#include "fmt/printf.h"
#include "fmt/ostream.h"
#undef FMT_USE_WINDOWS_H
#undef FMT_HEADER_ONLY

namespace AVlib {

//=============================================================================================================================================================================

class xString
{
public:
  static std::string replaceFirst(const std::string& Source, const std::string& Token, const std::string& ReplaceTo);
  static std::string replaceLast (const std::string& Source, const std::string& Token, const std::string& ReplaceTo);
  static std::string replaceAll  (const std::string& Source, const std::string& Token, const std::string& ReplaceTo);

  static std::string formatString(const std::string  Format, ...);
  static std::string formatString(const std::string& Format, va_list VariableArgs);
  static int32       scanString  (const std::string& Scan, const std::string Format, ...);

  template <class XXX> static XXX xStringToXXX(const std::string& Str, XXX Default) { if(Str.length() > 0) { std::istringstream InStringStream(Str, std::istringstream::in); InStringStream >> Default; } return Default; }
  template <class XXX> static std::vector<XXX>xVecOfStringToVecOfXXX(std::vector<std::string>& VecStr, XXX Default) { std::vector<XXX> VecXXX; std::transform(VecStr.begin(), VecStr.end(), std::back_inserter(VecXXX), [&](std::string& Str) -> XXX { return xString::xStringToXXX<XXX>(Str, Default); }); return VecXXX; }

  static std::string TimeToString(std::time_t Time                           ) { return TimeToString(Time, "%F %T"); }
  static std::string TimeToString(std::time_t Time, const std::string& Format);
};

//=============================================================================================================================================================================

} //end of namespace AVLib

