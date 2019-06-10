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

#include "xByteBuffer.h"

namespace AVlib {

//=============================================================================================================================================================================
// xByteBuffer
//=============================================================================================================================================================================
xByteBuffer::xByteBuffer()
{
  m_BufferSize = NOT_VALID;
  m_DataSize   = NOT_VALID;
  m_DataOffset = NOT_VALID;
  m_OwnsBuffer = false;
  m_Buffer     = nullptr;  
  m_POC        = NOT_VALID;
  m_Timestamp  = NOT_VALID;
}
void xByteBuffer::create(int32 BufferSize)
{ 
  m_BufferSize = BufferSize;
  m_DataSize   = 0;
  m_DataOffset = 0;
  m_OwnsBuffer = true;
  m_Buffer     = (byte*)xAlignedMalloc(m_BufferSize, X_AlignmentDef);
}
void xByteBuffer::destroy()
{
  m_BufferSize = NOT_VALID;
  m_DataSize   = NOT_VALID;
  m_DataOffset = NOT_VALID;
  if(m_Buffer != nullptr && m_OwnsBuffer) { xAlignedFree(m_Buffer); }
  m_Buffer     = nullptr;
}
void xByteBuffer::resize(int32 NewBufferSize)
{ 
  xAssert(m_Buffer != nullptr && m_OwnsBuffer);
  byte* TmpBuffer = (byte*)xAlignedMalloc(NewBufferSize, X_AlignmentDef);
  std::memcpy(TmpBuffer, m_Buffer, m_BufferSize);
  xAlignedFree(m_Buffer);
  m_Buffer = TmpBuffer;
  m_BufferSize = NewBufferSize;
}
void xByteBuffer::copy(xByteBuffer* Src)
{
  xAssert(Src->m_DataSize <= m_BufferSize);
  m_DataSize   = Src->m_DataSize;
  m_DataOffset = 0;
  std::memcpy(m_Buffer, Src->getReadPtr(), Src->getDataSize());
}
void xByteBuffer::copy(byte* Memmory, int32 Size)
{
  xAssert(Size <= m_BufferSize);
  m_DataSize   = Size;
  m_DataOffset = 0;
  std::memcpy(m_Buffer, Memmory, Size);
}
bool xByteBuffer::peek(byte* Dst, int32 Size)
{
  if(m_DataSize < Size) { return false; }
  std::memcpy(Dst, m_Buffer + m_DataOffset, Size);
  return true;
}
bool xByteBuffer::extract(byte* Dst, int32 Size)
{
  if(m_DataSize < Size) { return false; }
  std::memcpy(Dst, m_Buffer + m_DataOffset, Size);
  m_DataOffset += Size;
  m_DataSize   -= Size;
  return true;
}
bool xByteBuffer::dispose(const byte* Src, int32 Size)
{
  if(getRemainingSize() < Size) { return false; }
  std::memcpy(m_Buffer + m_DataOffset + m_DataSize, Src, Size);
  return true;
}
bool xByteBuffer::append(const byte* Src, int32 Size)
{
  if(getRemainingSize() < Size) { return false; }
  std::memcpy(m_Buffer + m_DataOffset + m_DataSize, Src, Size);
  m_DataSize += Size;
  return true;
}
bool xByteBuffer::transfer(xByteBuffer* Src, int32 Size)
{
  if((this->getRemainingSize() < Size) || Src->getDataSize() < Size) { return false; }
  std::memcpy(this->getWritePtr(), Src->getReadPtr(), Size);
  this->modifyWritten(Size);
  Src ->modifyRead(Size);
  return true;
}
void xByteBuffer::align()
{
  std::memmove(m_Buffer, getReadPtr(), m_DataSize);
  m_DataOffset = 0;
}
uint32 xByteBuffer::writeToStream(std::ostream* Stream)
{
  xAssert(Stream && Stream->good());
  Stream->write((const char*)m_Buffer+m_DataOffset, m_DataSize);
  return m_DataSize;
}
uint32 xByteBuffer::readFromStream(std::istream* Stream)
{
  xAssert(Stream && Stream->good());
  reset();

  std::streampos CurrPos = Stream->tellg();
  Stream->seekg(0, std::istream::end);
  std::streampos EndPos = (uint32)Stream->tellg();
  Stream->seekg(CurrPos, std::istream::beg);
  uint64 RemainingLen = EndPos - CurrPos;
  uint64 NumBytesToRead = xMin(RemainingLen, (uint64)m_BufferSize);
  Stream->read((char*)m_Buffer, NumBytesToRead);
  modifyWritten((uint32)NumBytesToRead);
  return (uint32)NumBytesToRead;
}
uint32 xByteBuffer::readFromStream(std::istream* Stream, int32 Size)
{
  xAssert(Stream && Stream->good());
  reset();

  std::streampos CurrPos = Stream->tellg();
  Stream->seekg(0, std::istream::end);
  std::streampos EndPos = (uint32)Stream->tellg();
  Stream->seekg(CurrPos, std::istream::beg);
  uint64 RemainingLen = EndPos - CurrPos;
  uint64 NumBytesToRead = xMin(RemainingLen, (uint64)Size);
  Stream->read((char*)m_Buffer, NumBytesToRead);
  modifyWritten((uint32)NumBytesToRead);
  return (uint32)NumBytesToRead;
}
uint32 xByteBuffer::writeToFile(xFile* File)
{
  xAssert(File && File->valid());
  File->write(m_Buffer+m_DataOffset, m_DataSize);
  return m_DataSize;
}
uint32 xByteBuffer::readFromFile(xFile* File, int32 Size)
{
  xAssert(File && File->valid());
  reset();
  uint64 CurrPos  = File->tell();
  uint64 FileSize = File->size();
  uint64 RemainingLen = FileSize - CurrPos;
  uint64 NumBytesToRead = xMin(RemainingLen, (uint64)Size);
  File->read(m_Buffer, (uint32)NumBytesToRead);
  modifyWritten((uint32)NumBytesToRead);
  return (uint32)NumBytesToRead;
}

//=============================================================================================================================================================================
// xByteBufferTank
//=============================================================================================================================================================================
void xByteBufferTank::create(int32 BufferSize, uintSize InitSize)
{
  m_Mutex.lock();
  m_BufferSize   = BufferSize;   
  m_CreatedUnits = 0;
  m_SizeLimit    = uintSize_max;

  for(uintSize i=0; i<InitSize; i++) { xCreateNewUnit(); }
  m_Mutex.unlock();
}
void xByteBufferTank::put(xByteBuffer* ByteBuffer)
{
  m_Mutex.lock();
  xAssert(ByteBuffer != nullptr);
  xAssert(isCompatible(ByteBuffer));
  m_Buffer.push_back(ByteBuffer);
  if(m_Buffer.size() > m_SizeLimit) { xDestroyUnit(); }
  m_Mutex.unlock();
}
xByteBuffer* xByteBufferTank::get()
{
  m_Mutex.lock();
  if(m_Buffer.empty()) { xCreateNewUnit(); }
  xByteBuffer* ByteBuffer = m_Buffer.back();
  m_Buffer.pop_back();
  m_Mutex.unlock();
  ByteBuffer->reset();
  return ByteBuffer;
}
void xByteBufferTank::xCreateNewUnit()
{
  xByteBuffer* Tmp = new xByteBuffer;
  Tmp->create(m_BufferSize); 
  m_Buffer.push_back(Tmp);
  m_CreatedUnits++;
}
void xByteBufferTank::xDestroyUnit()
{
  if(!m_Buffer.empty())
  {
    xByteBuffer* Tmp = m_Buffer.back();
    m_Buffer.pop_back();
    delete Tmp;
  }
}

//=============================================================================================================================================================================

} //end of namespace AVLib