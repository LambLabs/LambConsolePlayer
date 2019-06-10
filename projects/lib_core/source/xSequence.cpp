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

#include "xSequence.h"
#include "xPacked.h"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

namespace AVlib {

//=============================================================================================================================================================================

xSeqBase::xSeqBase() : m_Event(false)
{
  m_Width           = NOT_VALID;
  m_Height          = NOT_VALID;
  m_BitDepth        = NOT_VALID;     
  m_PictureType     = eImgTp::INVALID;
  m_PackedType      = ePckTp::INVALID;
  m_ChromaFormat    = CrF_INVALID;
  m_ComponentOrder  = eCmpO::INVALID;
  m_PackedFrameSize = NOT_VALID;

  m_SequenceMode    = eMode::Unknown;
  m_NumOfFrames     = NOT_VALID;

  m_UserFrame       = NOT_VALID;
  m_IOFrame         = NOT_VALID;
  m_LoopReading     = false;

  m_PackedFrame     = nullptr;

  m_BufferSize      = NOT_VALID;
  m_PackedTank      = nullptr;
  m_WaitingFrames   = nullptr;  
}
void xSeqBase::createRead(int32 Width, int32 Height, int32 BitDepth, eImgTp PictureType, ePckTp PackedType, eCrF ChromaFormat, eCmpO ComponentOrder, std::string FileName, int32 BufferSize, xPackedTank* PackedTank)
{
  if(!allowsRead()) { xAssert(0); return; }
  m_Width = Width; m_Height = Height; m_BitDepth = BitDepth; m_PictureType = PictureType; m_PackedType = PackedType; m_ChromaFormat = ChromaFormat; m_ComponentOrder = ComponentOrder;
  m_PackedFrameSize = xPacked::calkPackedSize(Width, Height, m_BitDepth, m_PictureType, m_ChromaFormat);
  m_SequenceMode    = eMode::Read;
  m_FileName        = FileName;
  m_BufferSize      = BufferSize; 
  if(BufferSize>0) { xAssert(PackedTank!=nullptr); m_PackedTank = PackedTank; } 
  xBackendCreateRead();
  xCreateInternals  ();
}
void xSeqBase::createWrite(int32 Width, int32 Height, int32 BitDepth, eImgTp PictureType, ePckTp PackedType, eCrF ChromaFormat, eCmpO ComponentOrder, std::string FileName, int32 BufferSize, xPackedTank* PackedTank)
{
  m_Width = Width; m_Height = Height; m_BitDepth = BitDepth; m_PictureType = PictureType; m_PackedType = PackedType; m_ChromaFormat = ChromaFormat; m_ComponentOrder = ComponentOrder;
  m_PackedFrameSize = xPacked::calkPackedSize(Width, Height, m_BitDepth, m_PictureType, m_ChromaFormat);
  m_SequenceMode    = eMode::Write;
  m_FileName        = FileName;
  m_BufferSize      = BufferSize;  
  if(BufferSize>0) { xAssert(PackedTank!=nullptr); m_PackedTank = PackedTank; } 
  xBackendCreateWrite();
  xCreateInternals   ();
}
void xSeqBase::destroy()
{
  xDestroyInternals();
  xBackendDestroy  ();
}
xSeqBase::eResult xSeqBase::readPacked(xPacked* DstPacked, bool AllowPackedBufferSwap)
{
  xAssert(m_SequenceMode==eMode::Read);
  xAssert(DstPacked->isCompatible(m_Width, m_Height, m_BitDepth, m_PictureType, m_PackedType, m_ChromaFormat, m_ComponentOrder));
  if(!m_LoopReading && m_UserFrame >= m_NumOfFrames) { return eResult::EndOfFile; }
  if(m_BufferSize>0)
  {
    xPacked* PackedFrame = m_WaitingFrames->get();
    if(AllowPackedBufferSwap) { DstPacked->swapPackedBuffer(PackedFrame); }
    else                      { DstPacked->copy(PackedFrame);             }
    m_PackedTank->put(PackedFrame);
    m_Event.set(eEvent::Update);    
  }
  else
  {
    xBackendReadFrame(DstPacked);
    if(m_LoopReading && m_IOFrame == m_NumOfFrames) { m_IOFrame = 0; seek(0, xFile::seek_mode::beg); }
  }
  DstPacked->setPOC(m_UserFrame);
  m_UserFrame++;
  return eResult::Correct;
}
void xSeqBase::writePacked(xPacked* SrcPacked, bool AllowPackedBufferSwap)
{
  xAssert(m_SequenceMode==eMode::Write);
  xAssert(SrcPacked->isCompatible(m_Width, m_Height, m_BitDepth, m_PictureType, m_PackedType, m_ChromaFormat, m_ComponentOrder));
  if(m_BufferSize>0)
  {
    xPacked* PackedFrame = m_PackedTank->get();
    if(AllowPackedBufferSwap) { SrcPacked->swapPackedBuffer(PackedFrame); }
    else                      { PackedFrame->copy(SrcPacked); }
    m_WaitingFrames->put(PackedFrame);
    m_Event.set(eEvent::Update);
  }
  else
  {
    xBackendWriteFrame(SrcPacked);
  }
  m_UserFrame++;
  m_NumOfFrames++;
}
xSeqBase::eResult xSeqBase::readBuffer(byte*& DstBuffer, uint32 BufferSize, bool AllowBufferSwap)
{
  xAssert(m_SequenceMode==eMode::Read);
  xAssert(m_PackedFrameSize == BufferSize);

  if(m_BufferSize > 0) { m_PackedFrame = m_PackedTank->get(); }
  eResult Result = readPacked(m_PackedFrame, true);
  if(AllowBufferSwap) { m_PackedFrame->swapPackedBuffer(DstBuffer); }
  else                { std::memcpy(DstBuffer, m_PackedFrame->getPackedBuffer(), BufferSize); }
  if(m_BufferSize > 0) { m_PackedTank->put(m_PackedFrame); m_PackedFrame = nullptr; }

  return Result;
}
void xSeqBase::writeBuffer(byte*& SrcBuffer, uint32 BufferSize, bool AllowBufferSwap)
{
  xAssert(m_SequenceMode==eMode::Write);
  xAssert(m_PackedFrameSize == BufferSize);

  if(m_BufferSize > 0) { m_PackedFrame = m_PackedTank->get(); }
  if(AllowBufferSwap) { m_PackedFrame->swapPackedBuffer(SrcBuffer); }
  else                { std::memcpy(m_PackedFrame->getPackedBuffer(), SrcBuffer, m_PackedFrameSize); }
  writePacked(m_PackedFrame, true);
  if(m_BufferSize > 0) { m_PackedTank->put(m_PackedFrame); m_PackedFrame = nullptr; }
}
xSeqBase::eResult xSeqBase::seek(int32 SeekFrames, xFile::seek_mode Origin)
{
  if(!allowsSeek()) { xAssert(0); return eResult::Error; }
  switch(Origin)
  {
    case xFile::seek_mode::beg: m_UserFrame = SeekFrames;                 break;
    case xFile::seek_mode::cur: m_UserFrame += SeekFrames;                break;
    case xFile::seek_mode::end: m_UserFrame = m_NumOfFrames + SeekFrames; break;
    default: xAssert(0); break;
  }

  if(m_UserFrame < 0) { m_UserFrame=0; }

  if(m_BufferSize>0)
  {
    m_Event.set(eEvent::Seek);
    m_Event.wait_specific(eEvent::Ack);
    m_Event.set(eEvent::Update);
  }
  else
  {
    m_IOFrame = m_UserFrame;
    xBackendSeekFrame();
  }
  return eResult::Correct;
}
xSeqBase::eResult xSeqBase::skip(int32 NumFramesToSkip)
{
  xAssert(m_SequenceMode==eMode::Read);
  for(int32 i=0; i < NumFramesToSkip; i++)
  {
    if(!m_LoopReading && m_UserFrame >= m_NumOfFrames) { return eResult::EndOfFile; }
    if(m_BufferSize>0)
    {
      m_PackedTank->put(m_WaitingFrames->get());
      m_Event.set(eEvent::Update);
    }
    else
    {
      xBackendSkipFrame();
      if(m_LoopReading && m_IOFrame == m_NumOfFrames) { m_IOFrame = 0; seek(0, xFile::seek_mode::beg); }
    }
    m_UserFrame++;    
  }  
  return eResult::Correct;
}
void xSeqBase::setLoopReading(bool LoopReading)
{
  xAssert(m_SequenceMode==eMode::Read);
  if(!allowsSeek()) { xAssert(0); return; }
  m_LoopReading = LoopReading;
  if(m_BufferSize > 0) { m_Event.set(eEvent::Update); }
}
xSeqBase::eResult xSeqBase::xCreateInternals()
{    
  m_UserFrame   = 0;
  m_IOFrame     = 0;  
  m_LoopReading = false;

  if(m_BufferSize==0)
  {
    m_PackedFrame = new xPacked;
    m_PackedFrame->create(m_Width, m_Height, m_BitDepth, m_PictureType, m_PackedType, m_ChromaFormat, m_ComponentOrder);
  }
  else
  {
    m_WaitingFrames = new xRing<xPacked*>;
    m_WaitingFrames->create(m_BufferSize);   

    std::packaged_task<uint32(xSeqBase*)> PackagedTask(xThreadFuncStarter);
    m_Future   = PackagedTask.get_future();
    m_Thread   = std::thread(std::move(PackagedTask), this);
    m_ThreadId = m_Thread.get_id();
  }
  return eResult::Correct;
}
xSeqBase::eResult xSeqBase::xDestroyInternals()
{
  if(m_BufferSize==0)
  {
    m_PackedFrame->destroy(); delete m_PackedFrame; m_PackedFrame = nullptr;
  }
  else
  {
    m_Event.set(eEvent::Terminate);
    std::future_status Status = m_Future.wait_for(500ms);
    if(Status == std::future_status::ready && m_Thread.joinable())
    {
      m_Thread.join();
    }
    else
    {
      std::future_status Status = m_Future.wait_for(5s);
      if(Status == std::future_status::ready && m_Thread.joinable()) { m_Thread.join();    } 
      else                                                           { m_Thread.~thread(); }
    }

    while(!m_WaitingFrames->isEmpty()) { m_PackedTank->put(m_WaitingFrames->get()); }
    m_WaitingFrames->destroy();
    delete m_WaitingFrames;
  }
  return eResult::Correct;
}
uint32 xSeqBase::xThreadFuncStarter(xSeqBase* Sequence)
{
  switch(Sequence->m_SequenceMode)
  {
    case eMode::Read:  return Sequence->xThreadFuncReader(); break;
    case eMode::Write: return Sequence->xThreadFuncWriter(); break;
    default: xAssert(0);  return EXIT_FAILURE; break;
  }
}
uint32 xSeqBase::xThreadFuncReader()
{
  //prefill buffer
  while(!m_WaitingFrames->isFull() && m_IOFrame<m_NumOfFrames)
  {
    xPacked* PackedFrame = m_PackedTank->get();
    xBackendReadFrame(PackedFrame);
    m_WaitingFrames->put(PackedFrame);
    if(m_LoopReading && m_IOFrame == m_NumOfFrames) { m_IOFrame = 0; xBackendSeekFrame(); }
  }
  while(1)
  {
    eEvent SeqEvent = m_Event.wait_any();
    switch(SeqEvent)
    {
      case eEvent::Update:
        while(!m_WaitingFrames->isFull() && m_IOFrame<m_NumOfFrames) //refill buffer
        {
          xPacked* PackedFrame = m_PackedTank->get();
          xBackendReadFrame(PackedFrame);
          m_WaitingFrames->put(PackedFrame);
          if(m_LoopReading && m_IOFrame == m_NumOfFrames) { m_IOFrame = 0; xBackendSeekFrame(); }
        }
        break;
      case eEvent::Seek:
        while(!m_WaitingFrames->isEmpty()) { m_PackedTank->put(m_WaitingFrames->get()); } //clean buffer 
        m_IOFrame = m_UserFrame;
        xBackendSeekFrame();
        m_Event.set(eEvent::Ack);
        break;
      case eEvent::Terminate:
        return EXIT_SUCCESS;
        break;
      case eEvent::Ack:
        m_Event.set(eEvent::Ack); //protection against unwanted caching of eSeqEvent::Ack event
        std::this_thread::yield();
        break;
      default:
        xAssert(0);
        return EXIT_FAILURE;
        break;
    }
  }  
}
uint32 xSeqBase::xThreadFuncWriter()
{
  while(1)
  {
    eEvent SeqEvent = m_Event.wait_any();
    switch(SeqEvent)
    {
      case eEvent::Update:
        while(!m_WaitingFrames->isEmpty()) //store buffered data
        {
          xPacked* PackedFrame = m_WaitingFrames->get(); 
          xBackendWriteFrame(PackedFrame);
          m_PackedTank->put(PackedFrame);
        }
        break;
      case eEvent::Seek:
        while(!m_WaitingFrames->isEmpty()) //store buffered data
        {
          xPacked* PackedFrame = m_WaitingFrames->get(); 
          xBackendWriteFrame(PackedFrame);
          m_PackedTank->put(PackedFrame);
        }
        m_IOFrame = m_UserFrame;
        xBackendSeekFrame();
        m_Event.set(eEvent::Ack);
        break;
      case eEvent::Terminate:
        while(!m_WaitingFrames->isEmpty()) //store buffered data
        {
          xPacked* PackedFrame = m_WaitingFrames->get(); 
          xBackendWriteFrame(PackedFrame);
          m_PackedTank->put(PackedFrame);
        }
        return EXIT_SUCCESS;
        break;
      case eEvent::Ack:
        m_Event.set(eEvent::Ack); //protection against unwanted caching of eSeqEvent::Ack event
        std::this_thread::yield();
        break;
      default:
        xAssert(0);
        return EXIT_FAILURE;
        break;
    }    
  }
}

//=============================================================================================================================================================================

xSeqNULL::eResult xSeqNULL::xBackendCreateRead()
{ 
  m_NumOfFrames = int32_max;
  return eResult::Correct;
}
xSeqNULL::eResult xSeqNULL::xBackendCreateWrite()
{
  m_NumOfFrames = 0;
  return eResult::Correct;
}
xSeqNULL::eResult xSeqNULL::xBackendDestroy()
{
  return eResult::Correct;
}
xSeqNULL::eResult xSeqNULL::xBackendWriteFrame(xPacked* PackedFrame)
{
  m_IOFrame++;
  return eResult::Correct;
}
xSeqNULL::eResult xSeqNULL::xBackendReadFrame(xPacked* PackedFrame)
{
  PackedFrame->clear();
  m_IOFrame++;
  return eResult::Correct;
}
xSeqNULL::eResult xSeqNULL::xBackendSeekFrame()
{
  if(m_LoopReading && m_IOFrame > m_NumOfFrames) { m_IOFrame = m_IOFrame % m_NumOfFrames; }
  return eResult::Correct;
}
xSeqNULL::eResult xSeqNULL::xBackendSkipFrame()
{
  m_IOFrame++;
  return eResult::Correct;
}
xSeqNULL::eResult xSeqNULL::overwriteNumOfFrames(int32 NumOfFrames)
{
  if(m_SequenceMode != eMode::Read) { xAssert(0); }
  if(NumOfFrames > m_NumOfFrames) { xAssert(0); }

  if(NumOfFrames == m_NumOfFrames) { return eResult::Correct; }

  m_UserFrame = 0;

  m_NumOfFrames = xMin(NumOfFrames, m_NumOfFrames);
  
  return eResult::Correct;
}

//=============================================================================================================================================================================

xSeqRAW::eResult xSeqRAW::xBackendCreateRead()
{
  m_File.open(m_FileName, "r");
  if(!m_File.valid()) { return eResult::Error; }
  m_NumOfFrames = (uint32)(m_File.size() / m_PackedFrameSize);
  return eResult::Correct;
}
xSeqRAW::eResult xSeqRAW::xBackendCreateWrite()
{
  m_File.open(m_FileName, "w");
  if(!m_File.valid()) { return eResult::Error; }
  m_NumOfFrames = 0;
  return eResult::Correct;
}
xSeqRAW::eResult xSeqRAW::xBackendDestroy()
{
  m_File.close();
  return eResult::Correct;
}
xSeqRAW::eResult xSeqRAW::xBackendWriteFrame(xPacked* PackedFrame)
{
  PackedFrame->writeToFile(&m_File);
  m_IOFrame++;
  return eResult::Correct;
}
xSeqRAW::eResult xSeqRAW::xBackendReadFrame(xPacked* PackedFrame)
{
  PackedFrame->readFromFile(&m_File);
  m_IOFrame++;
  return eResult::Correct;
}
xSeqRAW::eResult xSeqRAW::xBackendSeekFrame()
{
  if(m_LoopReading && m_IOFrame > m_NumOfFrames) { m_IOFrame = m_IOFrame % m_NumOfFrames; }
  bool Result = m_File.seek(m_IOFrame*m_PackedFrameSize, xFile::seek_mode::beg);
  return Result ? eResult::Correct : eResult::Error;
}
xSeqRAW::eResult xSeqRAW::xBackendSkipFrame()
{
  m_File.seek(m_PackedFrameSize, xFile::seek_mode::cur);
  m_IOFrame++;
  return eResult::Correct;
}
xSeqRAW::eResult xSeqRAW::overwriteNumOfFrames(int32 NumOfFrames)
{
  if(m_SequenceMode != eMode::Read) { xAssert(0); }
  if(NumOfFrames > m_NumOfFrames) { xAssert(0); }

  if(NumOfFrames == m_NumOfFrames) { return eResult::Correct; }

  m_UserFrame = 0;

  if(m_BufferSize>0)
  {
    m_Event.set(eEvent::Seek);
    m_Event.wait_specific(eEvent::Ack);
    m_NumOfFrames = xMin(NumOfFrames, m_NumOfFrames);
    m_Event.set(eEvent::Update);
  }
  else
  {
    xBackendSeekFrame();
    m_NumOfFrames = xMin(NumOfFrames, m_NumOfFrames);
  }

  return eResult::Correct;
}

//=============================================================================================================================================================================

} //end of namespace AVLib




