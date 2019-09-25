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
//#include "fmt/format.h"
//#include "fmt/printf.h"
//#include "fmt/ostream.h"
#undef FMT_USE_WINDOWS_H
#undef FMT_HEADER_ONLY

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace AVlib {

//=============================================================================================================================================================================

class xString
{
public:
  static inline bool xIsAlpha   (int32 c) { return ((c>='a' && c<='z') || (c>='A' && c<='Z')); }  
  static inline bool xIsNumeric (int32 c) { return ( c>='0' && c<='9'); }
  static inline bool xIsAlphaNum(int32 c) { return (xIsAlpha(c) || xIsNumeric(c)); }
  static inline bool xIsBlank   (int32 c) { return (c==' '  || c=='\a' || c=='\b' || c=='\t' || c=='\f'); }
  static inline bool xIsEndl    (int32 c) { return (c=='\n' || c=='\r' || c=='\v'); }
  static inline bool xIsSpace   (int32 c) { return (c==' '  || c=='\t' || c=='\n' || c=='\v' || c=='\f' || c=='\r'); }

public:
  static std::string replaceFirst(const std::string& Source, const std::string& Token, const std::string& ReplaceTo);
  static std::string replaceLast (const std::string& Source, const std::string& Token, const std::string& ReplaceTo);
  static std::string replaceAll  (const std::string& Source, const std::string& Token, const std::string& ReplaceTo);

  static std::string formatString(const std::string  Format, ...);
  static std::string formatString(const std::string& Format, va_list VariableArgs);
  static int32       scanString  (const std::string& Scan, const std::string Format, ...);

  template <class XXX> static XXX xStringToXXX(const std::string& Str, XXX Default) { if(Str.length() > 0) { std::istringstream InStringStream(Str, std::istringstream::in); InStringStream >> Default; } return Default; }
  template <class XXX> static std::vector<XXX>xVecOfStringToVecOfXXX(const std::vector<std::string>& VecStr, XXX Default) { std::vector<XXX> VecXXX; std::transform(VecStr.cbegin(), VecStr.cend(), std::back_inserter(VecXXX), [&](const std::string& Str) -> XXX { return xString::xStringToXXX<XXX>(Str, Default); }); return VecXXX; }

  static std::string TimeToString(std::time_t Time                           ) { return TimeToString(Time, "%F %T"); }
  static std::string TimeToString(std::time_t Time, const std::string& Format);

  static int32V2 scanResolution(const std::string& ResolutionString); //parse resolution (format WWWxHHH or WWWXHHH)


public:
  static inline std::string_view Rstrip(std::string_view S) { while(S.length() > 0 && xIsSpace(S.back ())) { S.remove_suffix(1); } return S; }
  static inline std::string_view Lstrip(std::string_view S) { while(S.length() > 0 && xIsSpace(S.front())) { S.remove_prefix(1); } return S; }


  static inline void trimL(std::string& s) { s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {  return !xIsSpace(ch); })); }
  static inline void trimR(std::string& s) { s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !xIsSpace(ch); }).base(), s.end()); }



};

//=============================================================================================================================================================================

} //end of namespace AVLib

