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

#include "xCommon.h"
#include <cstdio>
#include <string>
#include <cerrno>

#if X_SYSTEM_WINDOWS
#define X_WINAPI_IN_FILE 1
#endif

#if X_WINAPI_IN_FILE
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif

#if X_SYSTEM_WINDOWS
  #define ftell64(FileStream)               _ftelli64(FileStream)
  #define fseek64(FileStream,Offset,Origin) _fseeki64(FileStream,Offset,Origin)
#elif X_SYSTEM_LINUX
  #define ftell64(FileStream)               ftello(FileStream)
  #define fseek64(FileStream,Offset,Origin) fseeko(FileStream,Offset,Origin)
#else
  #error Unrecognized platform
#endif


namespace AVlib {

//=============================================================================================================================================================================

#if X_WINAPI_IN_FILE

class xFile
{
public:
  enum class seek_mode : int32 { beg = 0, cur = 1, end = 2 };

protected:
  HANDLE m_FileHandle;
  bool   m_EOF;

public:
  xFile       (                                                  ) { m_FileHandle = NULL; }
  xFile       (const std::string FilePath, const std::string Attr) { m_FileHandle = NULL; open(FilePath, Attr); }
  void   open (const std::string FilePath, const std::string Attr); 
  void   open (const char*       FilePath, const char*       Attr) { open(std::string(FilePath), std::string(Attr)); };
  void   close();

  uint32 read (      void* Memmory, uint32 Length); 
  uint32 write(const void* Memmory, uint32 Length); 
  uint32 write(const std::string& String) { return write(String.c_str(), (uint32)String.size()); }
  bool   skip (uint32 Length) {   return seek(Length, seek_mode::cur); }

  byte   get  (         );
  void   put  (byte Byte);

  bool   valid();
  uint64 size ();
  uint64 tell ();
  bool   end  ();

  bool   seek (int64 Position, seek_mode SeekMode);  
  void   fluch();

protected:
  void   xPrinterror();

public:
  static bool   exist   (const std::string FilePath);
  static uint64 filesize(const std::string FilePath);
};

#else

class xFile
{
public:
  enum class seek_mode : int32 { beg = 0, cur = 1, end = 2 };

protected:
  FILE*  m_FileHandle;

public:
  xFile       (                                                  ) { m_FileHandle = nullptr; }
  xFile       (const std::string FilePath, const std::string Attr) { m_FileHandle = nullptr; open(FilePath, Attr); }
  void   open (const std::string FilePath, const std::string Attr); 
  void   open (const char*       FilePath, const char*       Attr) { open(std::string(FilePath), std::string(Attr)); };
  void   close();

  uint32 read (      void* Memmory, uint32 Length) { return fread (Memmory, 1, Length, m_FileHandle); }
  uint32 write(const void* Memmory, uint32 Length) { return fwrite(Memmory, 1, Length, m_FileHandle); }
  uint32 write(const std::string& String         ) { return write(String.c_str(), (uint32)String.size()); }
  bool   skip (uint32 Length) {   return seek(Length, seek_mode::cur); }

  byte   get  (         ) { return getc(m_FileHandle); }
  void   put  (byte Byte) { putc(Byte, m_FileHandle); }

  bool   valid() { return (m_FileHandle!=nullptr && !ferror(m_FileHandle)); }
  uint64 size ();
  uint64 tell () { return ftell(m_FileHandle); }
  bool   end  ();

  bool   seek (int64 Position, seek_mode SeekMode);  
  void   fluch() { fflush(m_FileHandle); }

protected:
  void   xPrinterror() { printf("%s ", strerror(errno)); }

public:
  static bool   exist   (const std::string FilePath);
  static uint64 filesize(const std::string FilePath);
};

#endif

//=============================================================================================================================================================================

}; //end of namespace AVlib

