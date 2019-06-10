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

//================================================================================================
// C preprocesor - converts any macro to string within #pragma message()
//================================================================================================
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)
//#pragma message(_MSC_VER)


//================================================================================================
// C99 restrict pointers support
//================================================================================================
#if defined(_MSC_VER) && _MSC_VER >= 1400 //MSVC
  #define restrict __restrict
#elif defined(__GNUC__) && ((__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)) //GCC
  #ifdef __cplusplus
    #define restrict __restrict
  #endif
  #ifdef __STDC_VERSION__ //STANDARD_C_1990
    #define restrict __restrict
  #endif
  #if (__STDC_VERSION__ >= 199409L) //STANDARD_C_1994
    #define restrict __restrict
  #endif
  #if (__STDC_VERSION__ >= 199901L) //STANDARD_C_1999
  //restrict defined
  #endif
#elif defined(__STDC__) //STANDARD_C_1989
  #define restrict
#endif

//================================================================================================
// MSVC issues 
//================================================================================================
//stupid warning conected with strdup
#ifdef _MSC_VER
#define strdup _strdup
//#define _CRT_STDIO_ISO_WIDE_SPECIFIERS
#endif

