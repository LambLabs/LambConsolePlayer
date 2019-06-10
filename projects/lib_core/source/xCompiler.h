#pragma once
#include "xLicense.h"

#define X_COMPILER_UNKNOWN 0
#define X_COMPILER_ICC     1
#define X_COMPILER_CLANG   2
#define X_COMPILER_GCC     3
#define X_COMPILER_MSVC    4

#ifdef __INTEL_COMPILER
#define X_COMPILER      X_COMPILER_ICC
#define X_COMPILER_VER  __INTEL_COMPILER
#define X_COMPILER_NAME "Intel Compiler"
#endif

#ifndef X_COMPILER
#ifdef __clang__
#define X_COMPILER      X_COMPILER_CLANG
#define X_COMPILER_VER  __clang_major__
#define X_COMPILER_NAME "Clang"
#endif
#endif

#ifndef X_COMPILER
#ifdef __GNUC__
#define X_COMPILER      X_COMPILER_GCC
#define X_COMPILER_VER  __GNUC__
#define X_COMPILER_NAME "GNU Compiler Collection"
#endif
#endif

#ifndef X_COMPILER
#ifdef _MSC_VER
#define X_COMPILER      X_COMPILER_MSVC
#define X_COMPILER_VER  _MSC_VER
#define X_COMPILER_NAME "MS Visual Studio"
#endif
#endif

#ifndef X_COMPILER
#define X_COMPILER      X_COMPILER_UNKNOWN
#define X_COMPILER_VER  
#define X_COMPILER_NAME "unknown"
#endif

