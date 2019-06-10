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

#include "xFile.h"

namespace AVlib {

//=============================================================================================================================================================================

#if X_WINAPI_IN_FILE
void xFile::open(const std::string FilePath, const std::string Attr)
{
  if(FilePath=="" || Attr=="") { return; };
  uint32 AttrFlags   = 0;
  uint32 CreateFlags = 0;
  uint32 FileSkip    = 0;
  if(Attr.find('r') != std::string::npos) { AttrFlags |= GENERIC_READ    ; CreateFlags = OPEN_EXISTING; };
  if(Attr.find('w') != std::string::npos) { AttrFlags |= GENERIC_WRITE   ; CreateFlags = CREATE_ALWAYS; };
  if(Attr.find('a') != std::string::npos) { AttrFlags |= FILE_APPEND_DATA; CreateFlags = OPEN_ALWAYS  ; FileSkip = 1; };
  m_FileHandle = CreateFile(FilePath.c_str(), AttrFlags, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CreateFlags, FILE_ATTRIBUTE_NORMAL,  NULL);
  if(m_FileHandle!=INVALID_HANDLE_VALUE && FileSkip) { SetFilePointer(m_FileHandle, 0, NULL,FILE_END ); }  
  if(m_FileHandle==INVALID_HANDLE_VALUE) { xPrinterror(); printf("(%s)\n", FilePath.c_str()); }
}
void xFile::close()
{
  if(m_FileHandle!=NULL)
  {
    if(m_FileHandle!=INVALID_HANDLE_VALUE) { CloseHandle(m_FileHandle); }
    m_FileHandle = NULL;
  }
}
uint32 xFile::read(void* Memmory, uint32 Length)
{
  uint32 NumberOfBytesRead;
  ReadFile(m_FileHandle, (LPVOID)Memmory, Length, (LPDWORD)&NumberOfBytesRead, NULL);
  return(NumberOfBytesRead);
}
uint32 xFile::write(const void* Memmory, uint32 Length)
{
  uint32 NumberOfBytesWritten;
  WriteFile(m_FileHandle, (LPVOID)Memmory, Length, (LPDWORD)&NumberOfBytesWritten, NULL);
  return(NumberOfBytesWritten);
}
byte xFile::get()
{
  byte Data;
  read(&Data, 1);
  return Data;
}
void xFile::put(byte Byte)
{
  write(&Byte, 1);
}
bool xFile::valid()
{
  return (m_FileHandle!=INVALID_HANDLE_VALUE && m_FileHandle != NULL);
}
uint64 xFile::size()
{
  LARGE_INTEGER Size;
  GetFileSizeEx(m_FileHandle, &Size);
  return Size.QuadPart;
}
uint64 xFile::tell()
{
  LARGE_INTEGER Zero, Position;
  memset(&Zero, 0, sizeof(LARGE_INTEGER));
  SetFilePointerEx(m_FileHandle, Zero, &Position, FILE_CURRENT);    
  return Position.QuadPart;
}
bool xFile::end()
{
  return (tell()==size());
}
bool xFile::seek(int64 Position, seek_mode SeekMode)
{
  LARGE_INTEGER Pos;
  Pos.QuadPart = Position;
  uint32 Result = SetFilePointerEx(m_FileHandle, Pos, NULL, (DWORD)SeekMode);
  if(Result == 0) { xPrinterror(); }
  return (Result != 0);
}
void xFile::fluch()
{
  FlushFileBuffers(m_FileHandle);
}
void xFile::xPrinterror()
{
  char* MsgBuf;
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*) &MsgBuf, 0, NULL);
  printf(("%s"),MsgBuf);
  LocalFree(MsgBuf);
}
bool xFile::exist(const std::string FilePath)
{
  DWORD Attributes = GetFileAttributes(FilePath.c_str());
  return (Attributes != INVALID_FILE_ATTRIBUTES && !(Attributes & FILE_ATTRIBUTE_DIRECTORY));
}
uint64 xFile::filesize(const std::string FilePath)
{
  WIN32_FILE_ATTRIBUTE_DATA FileAttributes;
  bool Success = GetFileAttributesExA(FilePath.c_str(), GetFileExInfoStandard, &FileAttributes);
  if(!Success) { return 0; }
  uint64 FileSize = (((uint64)FileAttributes.nFileSizeHigh) << 32) + ((uint64)FileAttributes.nFileSizeLow);
  return FileSize;
}
#else
void xFile::open(const std::string FilePath, const std::string Attr)
{
  if(FilePath=="" || Attr=="") { return; };
  std::string Mode = Attr;
  if(Mode.find('b') == std::string::npos) { Mode.append(1, 'b'); }
  m_FileHandle = fopen(FilePath.c_str(), Mode.c_str());
  if(m_FileHandle==nullptr) { xPrinterror(); printf("(%s)\n", FilePath.c_str()); }
}
void xFile::close()
{
  if(m_FileHandle!=nullptr)
  {
    fclose(m_FileHandle);
    m_FileHandle = nullptr;
  }
}
uint64 xFile::size()
{
  int64 CurrPosition = ftell64(m_FileHandle);
  fseek64(m_FileHandle, 0, (int)seek_mode::end);
  int64 Size = ftell64(m_FileHandle);
  fseek64(m_FileHandle, CurrPosition, (int)seek_mode::beg);
  return Size;
}
bool xFile::end()
{
  int64 CurrPosition = ftell64(m_FileHandle);
  fseek64(m_FileHandle, 0, (int)seek_mode::end);
  int64 EndPosition = ftell64(m_FileHandle);
  if(CurrPosition == EndPosition)
  {
    return true;
  }
  else
  {
    fseek64(m_FileHandle, CurrPosition, (int)seek_mode::beg); return false;
  }
}
bool xFile::seek(int64 Position, seek_mode SeekMode)
{
  int32 Result = fseek64(m_FileHandle, Position, (int)SeekMode);
  if(Result!=0) { xPrinterror(); }
  return (Result == 0);
}
bool xFile::exist(const std::string FilePath)
{
  FILE* FileHandle = fopen(FilePath.c_str(), "r");
  if(FileHandle != nullptr)
  {
    fclose(FileHandle);
    return true;
  }  
  return false;
}
uint64 xFile::filesize(const std::string FilePath)
{
  FILE* FileHandle = fopen(FilePath.c_str(), "r");
  if(FileHandle != nullptr) { return 0; }
  fseek64(FileHandle, 0, (int)seek_mode::end);
  int64 EndPosition = ftell64(FileHandle);
  fclose(FileHandle);
  return EndPosition;
}
#endif

//=============================================================================================================================================================================

} //end of namespace AVLib
