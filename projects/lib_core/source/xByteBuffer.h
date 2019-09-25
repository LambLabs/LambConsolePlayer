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
#include "xFile.h"
#include <mutex>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace AVlib {

//=============================================================================================================================================================================

class xByteBufferTank;

//=============================================================================================================================================================================

class xByteBuffer
{
public:
  typedef xByteBufferTank tCorespondingTank;

protected:
  byte*    m_Buffer;

  int32    m_BufferSize;
  int32    m_DataSize;
  int32    m_DataOffset;
  
  uint64   m_EightCC;
  int64    m_POC;
  int64    m_Timestamp;

  bool     m_OwnsBuffer;
  
public:
  //base functions
  xByteBuffer               ();
  xByteBuffer               (int32 BufferSize) { create(BufferSize); }
  xByteBuffer               (xByteBuffer* RefBuffer) { create(RefBuffer); }
  xByteBuffer               (byte* Memmory, int32 BufferSize                ) { m_BufferSize = BufferSize; m_DataSize = 0;        m_DataOffset = 0; m_OwnsBuffer = false; m_Buffer = Memmory; }
  xByteBuffer               (byte* Memmory, int32 BufferSize, int32 DataSize) { m_BufferSize = BufferSize; m_DataSize = DataSize; m_DataOffset = 0; m_OwnsBuffer = false; m_Buffer = Memmory; }
  ~xByteBuffer              () { destroy(); }

  void     create           (int32 BufferSize);
  void     create           (xByteBuffer* RefBuffer) { create(RefBuffer->getBufferSize()); }
  void     duplicate        (xByteBuffer* RefBuffer) { create(RefBuffer->getBufferSize()); copy(RefBuffer); }
  void     destroy          ();
  void     resize           (int32 NewBufferSize);
  void     reset            () { m_DataSize = 0; m_DataOffset = 0; }
  void     clear            () { reset(); std::memset(m_Buffer, 0, m_BufferSize); }
  void     copy             (xByteBuffer* Src);
  void     copy             (byte* Memmory, int32 Size);

  //tool functions
  bool     isSameSize       (int32 Size) { return (m_BufferSize==Size); }
  bool     isSameSize       (xByteBuffer* RefBuffer) { xAssert(RefBuffer!=nullptr); return isSameSize(RefBuffer->m_BufferSize); }

  //comparison functions
  bool     isSameDataSize   (xByteBuffer* RefBuffer) { return (getDataSize() == RefBuffer->getDataSize()); }
  bool     isSameData       (xByteBuffer* RefBuffer) { return ( isSameDataSize(RefBuffer) || std::memcmp(getReadPtr(), RefBuffer->getReadPtr(), getDataSize()) == 0); }

  //peek - read from beggining (pointed by DataOffset) without changing DataOffset (LE = Little Endian)
  bool     peek             (byte* Dst, int32 Size);
  uint8    peekU8           (           ) { xAssert(m_DataSize >= 1); return             (*((uint8 *)(m_Buffer + m_DataOffset)));   }
  uint16   peekU16          (           ) { xAssert(m_DataSize >= 2); return xSwapBytes16(*((uint16*)(m_Buffer + m_DataOffset))); }
  uint16   peekU16_LE       (           ) { xAssert(m_DataSize >= 2); return             (*((uint16*)(m_Buffer + m_DataOffset))); }  
  uint32   peekU32          (           ) { xAssert(m_DataSize >= 4); return xSwapBytes32(*((uint32*)(m_Buffer + m_DataOffset))); }
  uint32   peekU32_LE       (           ) { xAssert(m_DataSize >= 4); return             (*((uint32*)(m_Buffer + m_DataOffset))); }
  uint64   peekU64          (           ) { xAssert(m_DataSize >= 8); return xSwapBytes64(*((uint64*)(m_Buffer + m_DataOffset))); }
  uint64   peekU64_LE       (           ) { xAssert(m_DataSize >= 8); return             (*((uint64*)(m_Buffer + m_DataOffset))); }
    
  //extract - read from beggining (pointed by DataOffset) with DataOffset update (LE = Little Endian)
  bool     extract          (byte* Dst, int32 Size);
  uint8    extractU8        (           ) { xAssert(m_DataSize >= 1); uint8  Data =             (*((uint8 *)(m_Buffer + m_DataOffset))); m_DataSize-- ; m_DataOffset++ ; return Data; }
  uint16   extractU16       (           ) { xAssert(m_DataSize >= 2); uint16 Data = xSwapBytes16(*((uint16*)(m_Buffer + m_DataOffset))); m_DataSize-=2; m_DataOffset+=2; return Data; }
  uint16   extractU16_LE    (           ) { xAssert(m_DataSize >= 2); uint16 Data =             (*((uint16*)(m_Buffer + m_DataOffset))); m_DataSize-=2; m_DataOffset+=2; return Data; }
  uint32   extractU32       (           ) { xAssert(m_DataSize >= 4); uint32 Data = xSwapBytes32(*((uint32*)(m_Buffer + m_DataOffset))); m_DataSize-=4; m_DataOffset+=4; return Data; }
  uint32   extractU32_LE    (           ) { xAssert(m_DataSize >= 4); uint32 Data =             (*((uint32*)(m_Buffer + m_DataOffset))); m_DataSize-=4; m_DataOffset+=4; return Data; }
  uint64   extractU64       (           ) { xAssert(m_DataSize >= 8); uint64 Data = xSwapBytes64(*((uint64*)(m_Buffer + m_DataOffset))); m_DataSize-=8; m_DataOffset+=8; return Data; }
  uint64   extractU64_LE    (           ) { xAssert(m_DataSize >= 8); uint64 Data =             (*((uint64*)(m_Buffer + m_DataOffset))); m_DataSize-=8; m_DataOffset+=8; return Data; }

  //dispose - write at the end (pointed by DataOffset + DataSize) without changing DataSize (LE = Little Endian)
  bool     dispose          (const byte* Src, int32 Size);
  bool     dispose          (xByteBuffer* Src) { return dispose(Src->getReadPtr(), Src->getDataSize()); }
  void     disposeU8        (uint8  Data) { xAssert((m_BufferSize - (m_DataSize + m_DataOffset)) >= 1); *((uint8 *)(m_Buffer + m_DataOffset + m_DataSize)) = Data;               }
  void     disposeU16       (uint16 Data) { xAssert((m_BufferSize - (m_DataSize + m_DataOffset)) >= 2); *((uint16*)(m_Buffer + m_DataOffset + m_DataSize)) = xSwapBytes16(Data); }
  void     disposeU16_LE    (uint16 Data) { xAssert((m_BufferSize - (m_DataSize + m_DataOffset)) >= 2); *((uint16*)(m_Buffer + m_DataOffset + m_DataSize)) =             (Data); }
  void     disposeU32       (uint32 Data) { xAssert((m_BufferSize - (m_DataSize + m_DataOffset)) >= 4); *((uint32*)(m_Buffer + m_DataOffset + m_DataSize)) = xSwapBytes32(Data); }
  void     disposeU32_LE    (uint32 Data) { xAssert((m_BufferSize - (m_DataSize + m_DataOffset)) >= 4); *((uint32*)(m_Buffer + m_DataOffset + m_DataSize)) =             (Data); }
  void     disposeU64       (uint64 Data) { xAssert((m_BufferSize - (m_DataSize + m_DataOffset)) >= 8); *((uint64*)(m_Buffer + m_DataOffset + m_DataSize)) = xSwapBytes64(Data); }
  void     disposeU64_LE    (uint64 Data) { xAssert((m_BufferSize - (m_DataSize + m_DataOffset)) >= 8); *((uint64*)(m_Buffer + m_DataOffset + m_DataSize)) =             (Data); }

  //append - write at the end (pointed by DataOffset + DataSize) with DataSize update (LE = Little Endian)
  bool     append           (const byte* Src, int32 Size);
  bool     append           (xByteBuffer* Src) { return append(Src->getReadPtr(), Src->getDataSize()); }
  void     appendU8         (uint8  Data) { xAssert((m_BufferSize - (m_DataSize + m_DataOffset)) >= 1); *((uint8 *)(m_Buffer + m_DataOffset + m_DataSize)) = Data;               m_DataSize++;  }
  void     appendU16        (uint16 Data) { xAssert((m_BufferSize - (m_DataSize + m_DataOffset)) >= 2); *((uint16*)(m_Buffer + m_DataOffset + m_DataSize)) = xSwapBytes16(Data); m_DataSize+=2; }
  void     appendU16_LE     (uint16 Data) { xAssert((m_BufferSize - (m_DataSize + m_DataOffset)) >= 2); *((uint16*)(m_Buffer + m_DataOffset + m_DataSize)) = Data;               m_DataSize+=2; }
  void     appendU32        (uint32 Data) { xAssert((m_BufferSize - (m_DataSize + m_DataOffset)) >= 4); *((uint32*)(m_Buffer + m_DataOffset + m_DataSize)) = xSwapBytes32(Data); m_DataSize+=4; }
  void     appendU32_LE     (uint32 Data) { xAssert((m_BufferSize - (m_DataSize + m_DataOffset)) >= 4); *((uint32*)(m_Buffer + m_DataOffset + m_DataSize)) = Data;               m_DataSize+=4; }
  void     appendU64        (uint64 Data) { xAssert((m_BufferSize - (m_DataSize + m_DataOffset)) >= 8); *((uint64*)(m_Buffer + m_DataOffset + m_DataSize)) = xSwapBytes64(Data); m_DataSize+=8; }
  void     appendU64_LE     (uint64 Data) { xAssert((m_BufferSize - (m_DataSize + m_DataOffset)) >= 8); *((uint64*)(m_Buffer + m_DataOffset + m_DataSize)) = Data;               m_DataSize+=8; }

  //transfer - read from beggining (pointed by DataOffset) with DataOffset update AND write at the end (pointed by DataOffset + DataSize) with DataSize update
  bool     transfer         (xByteBuffer* Src, int32 Size);
  bool     transfer         (xByteBuffer* Src) { return transfer(Src, Src->getDataSize()); }

  //align - moves data to begin of the buffer
  void     align            ();

  //read, write
  uint32   writeToStream    (std::ostream* Stream);
  uint32   readFromStream   (std::istream* Stream);
  uint32   readFromStream   (std::istream* Stream, int32 Size);
  uint32   writeToFile      (xFile* File);
  uint32   readFromFile     (xFile* File, int32 Size);
  uint32   readFromFile     (xFile* File) { return readFromFile(File, m_BufferSize); }

  //interfaces
  byte*    getReadPtr       (                 ) { return m_Buffer + m_DataOffset;}
  byte*    getWritePtr      (                 ) { return m_Buffer + m_DataOffset + m_DataSize; }
  int32    getDataSize      (                 ) { return m_DataSize; }
  void     setDataSize      (int32 Size       ) { m_DataSize = Size; }
  void     modifyDataSize   (int32 DeltaSize  ) { m_DataSize += DeltaSize; }
  int32    getDataOffset    (                 ) { return m_DataOffset;      }
  void     setDataOffset    (int32 DataOffset ) { m_DataOffset = DataOffset;}
  void     modifyDataOffset (int32 DeltaOffset) { m_DataOffset += DeltaOffset; }
  void     modifyRead       (int32 NumBytes   ) { modifyDataOffset(NumBytes); modifyDataSize(-NumBytes); }
  void     modifyWritten    (int32 NumBytes   ) { modifyDataSize(NumBytes); }
  int32    getRemainingSize (                 ) { return m_BufferSize - (m_DataSize + m_DataOffset); }
           
  int32    getBufferSize    (                 ) { return m_BufferSize;     }
  byte*    getBufferPtr     (                 ) { return m_Buffer;         }
                                                                           
  void     setEightCC       (uint64 EightCC   ) { m_EightCC = EightCC;     }
  uint64   getEightCC       (                 ) { return m_EightCC;        }
  void     setPOC           (int64 POC        ) { m_POC = POC;             }
  int64    getPOC           (                 ) { return m_POC;            }
  void     setTimestamp     (int64 Timestamp  ) { m_Timestamp = Timestamp; }
  int64    getTimestamp     (                 ) { return m_Timestamp;      }
  void     copyDescriptors  (xByteBuffer* Ref ) { m_EightCC = Ref->m_EightCC; m_POC = Ref->m_POC; m_Timestamp = Ref->m_Timestamp; }
};

//=============================================================================================================================================================================

class xByteBufferTank
{
protected:
  std::vector<xByteBuffer*> m_Buffer;
  std::mutex                m_Mutex;
  uintSize                  m_CreatedUnits;
  uintSize                  m_SizeLimit;

  //Unit creation parameters
  int32    m_BufferSize;

public:
  void         create         (int32 BufferSize,        uintSize InitSize);
  void         create         (xByteBuffer* ByteBuffer, uintSize InitSize) { create(ByteBuffer->getBufferSize(), InitSize); }
  void         recreate       (int32 BufferSize,        uintSize InitSize) { destroy(); create(BufferSize, InitSize); }
  void         recreate       (xByteBuffer* ByteBuffer, uintSize InitSize) { destroy(); create(ByteBuffer, InitSize); }
  void         destroy        () { while(!m_Buffer.empty()) { xDestroyUnit(); } }

  void         setSizeLimit   (uintSize SizeLimit) { m_SizeLimit = SizeLimit; while(m_Buffer.size() > SizeLimit) { xDestroyUnit(); } }
                              
  void         put            (xByteBuffer* ByteBuffer);
  xByteBuffer* get            ();
                              
  bool         isCompatible   (xByteBuffer* ByteBuffer) { xAssert(ByteBuffer != nullptr); return ByteBuffer->isSameSize(m_BufferSize); }

  uintSize     getLoad        () { return m_Buffer.size(); }
  uintSize     getCreatedUnits() { return m_CreatedUnits;  }

protected:
  void         xCreateNewUnit ();
  void         xDestroyUnit   ();
};

//=============================================================================================================================================================================

} //end of namespace AVLib