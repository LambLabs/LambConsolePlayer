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
#include <cassert>

//================================================================================================
// DEBUG/RELEASE section
//================================================================================================
#ifdef NDEBUG
#define X_RELEASE 
#else
#define X_DEBUG
#endif

#define xAssert(expression) assert(expression)

#define X_VERBOSE 1
#if X_VERBOSE
#define xVerbose(...) fprintf(stderr, __VA_ARGS__);
#else
#define xVerbose(...)
#endif

//================================================================================================
// Architecture section
//================================================================================================
#if ( defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) ))
#define X_ARCH_X86_64 1
#endif
#ifndef X_ARCH_X86_64
#define X_ARCH_X86_32 1
#endif


//================================================================================================
// System section
//================================================================================================
#if (defined(WIN32) || defined(_WIN32) || defined(_WIN64))
#define X_SYSTEM_WINDOWS 1
#elif defined(__linux__)
#define X_SYSTEM_LINUX 1
#else
#defiene X_SYSTEM_UNKNOWN 0
#endif
