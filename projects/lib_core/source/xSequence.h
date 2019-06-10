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
#include "xPicture.h"
#include "xPlane.h"
#include "xPacked.h"
#include "xRing.h"
#include "xEvent.h"
#include <map>
#include <future>

namespace AVlib {

//=============================================================================================================================================================================

class xSeqBase
{
public:
  enum class eResult : int32 { Correct, EndOfFile, Error };

protected:
  enum class eMode  : int32 { Unknown, Read, Write };
  enum class eEvent : int32 { Update = 0, Seek, Terminate, Ack, NumberOf };

protected:
  int32                m_Width;
  int32                m_Height;
  int32                m_BitDepth;     
  eImgTp               m_PictureType;
  ePckTp               m_PackedType;
  eCrF                 m_ChromaFormat;  
  eCmpO                m_ComponentOrder;
  uint32               m_PackedFrameSize;   

  eMode                m_SequenceMode;
  std::string          m_FileName;
  int32                m_NumOfFrames;

  int32                m_UserFrame;
  int32                m_IOFrame;
  bool                 m_LoopReading;

  xPacked*             m_PackedFrame;

  int32                m_BufferSize;
  xPackedTank*         m_PackedTank;
  xRing<xPacked*>*     m_WaitingFrames;  
  xStatusEvent<eEvent> m_Event;

  std::future<uint32>  m_Future;
  std::thread          m_Thread;
  std::thread::id      m_ThreadId;

public:
  xSeqBase();
  virtual ~xSeqBase() {}

  inline bool    allowsRead () { return xBackendAllowsRead (); }
  inline bool    allowsWrite() { return xBackendAllowsWrite(); }
  inline bool    allowsSeek () { return xBackendAllowsSeek (); }

  void           createRead       (int32 Width, int32 Height, int32 BitDepth, eImgTp PictureType, ePckTp PackedType, eCrF ChromaFormat, eCmpO ComponentOrder, std::string FileName, int32 BufferSize, xPackedTank* PackedTank);
  void           createWrite      (int32 Width, int32 Height, int32 BitDepth, eImgTp PictureType, ePckTp PackedType, eCrF ChromaFormat, eCmpO ComponentOrder, std::string FileName, int32 BufferSize, xPackedTank* PackedTank);
  void           destroy          ();
   
  eResult        readPacked       (xPacked* DstPacked, bool AllowPackedBufferSwap);
  void           writePacked      (xPacked* SrcPacked, bool AllowPackedBufferSwap);
  eResult        readBuffer       (byte*& DstBuffer, uint32 BufferSize, bool AllowBufferSwap);
  void           writeBuffer      (byte*& SrcBuffer, uint32 BufferSize, bool AllowBufferSwap);

  template <typename PelType> eResult readPicture (xPic  <PelType>* DstPicture);
  template <typename PelType> void    writePicture(xPic  <PelType>* SrcPicture);
  template <typename PelType> eResult readPlane   (xPlane<PelType>* DstPlane  );
  template <typename PelType> void    writePlane  (xPlane<PelType>* SrcPlane  );
                 
  eResult        seek             (int32 SeekFrames, xFile::seek_mode Origin);
  eResult        skip             (int32 NumFramesToSkip);
                 
public:
  void           setLoopReading   (bool LoopReading);
  bool           getLoopReading    ()                 { return m_LoopReading;        }

public:
  int32          getWidth          () { return m_Width; }
  int32          getHeight         () { return m_Height; }
  int32          getBitDepth       () { return m_BitDepth; }
  eImgTp         getPicturType     () { return m_PictureType; }
  eCrF           getChromaFormat   () { return m_ChromaFormat; }
  uint32         getPackedFrameSize() { return m_PackedFrameSize; }
              
  int32          getNumOfFrames    () { return m_NumOfFrames; }
  int32          getFrameIdx       () { return m_UserFrame;   }

protected:
  eResult xCreateInternals ();
  eResult xDestroyInternals();

protected:
  virtual bool    xBackendAllowsRead  () const = 0; //requires xBackendCreateRead, xBackendReadFrame, xBackendSkipFrame
  virtual bool    xBackendAllowsWrite () const = 0; //requires xBackendCreateWrite, xBackendWriteFrame
  virtual bool    xBackendAllowsSeek  () const = 0; //requires xBackendSeekFrame
  virtual eResult xBackendCreateRead  (                    ) = 0;
  virtual eResult xBackendCreateWrite (                    ) = 0;
  virtual eResult xBackendDestroy     (                    ) = 0;
  virtual eResult xBackendReadFrame   (xPacked* PackedFrame) = 0;
  virtual eResult xBackendWriteFrame  (xPacked* PackedFrame) = 0;
  virtual eResult xBackendSeekFrame   (                    ) = 0;
  virtual eResult xBackendSkipFrame   (                    ) = 0;

protected:
  static  uint32 xThreadFuncStarter(xSeqBase* Sequence);
          uint32 xThreadFuncReader (                  );
          uint32 xThreadFuncWriter (                  );  
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename PelType> xSeqBase::eResult xSeqBase::readPicture(xPic<PelType>* DstPicture)
{
  xAssert(m_SequenceMode==eMode::Read);
  xAssert(DstPicture->isCompatible(m_Width, m_Height, m_BitDepth, m_PictureType, m_ChromaFormat));

  if(m_BufferSize > 0) { m_PackedFrame = m_PackedTank->get(); }
  eResult Result = readPacked(m_PackedFrame, true);
  m_PackedFrame->unpack(DstPicture);
  DstPicture->setPOC(m_PackedFrame->getPOC());
  if(m_BufferSize > 0) { m_PackedTank->put(m_PackedFrame); m_PackedFrame = nullptr; }

  return Result;
}
template <typename PelType> void xSeqBase::writePicture(xPic<PelType>* SrcPicture)
{
  xAssert(m_SequenceMode==eMode::Write);
  xAssert(SrcPicture->isCompatible(m_Width, m_Height, m_BitDepth, m_PictureType, m_ChromaFormat));

  if(m_BufferSize > 0) { m_PackedFrame = m_PackedTank->get(); }
  m_PackedFrame->pack(SrcPicture);
  m_PackedFrame->setPOC(SrcPicture->getPOC());
  writePacked(m_PackedFrame, true);
  if(m_BufferSize > 0) { m_PackedTank->put(m_PackedFrame); m_PackedFrame = nullptr; }
}
template <typename PelType> xSeqBase::eResult xSeqBase::readPlane(xPlane<PelType>* DstPlane)
{
  xAssert(m_SequenceMode==eMode::Read);
  xAssert(DstPlane->isCompatible(m_Width, m_Height, m_BitDepth));

  if(m_BufferSize > 0) { m_PackedFrame = m_PackedTank->get(); }
  eResult Result = readPacked(m_PackedFrame, true);
  m_PackedFrame->unpack(DstPlane);
  DstPlane->setPOC(m_PackedFrame->getPOC());
  if(m_BufferSize > 0) { m_PackedTank->put(m_PackedFrame); m_PackedFrame = nullptr; }

  return Result;
}
template <typename PelType> void xSeqBase::writePlane(xPlane<PelType>* SrcPlane)
{
  xAssert(m_SequenceMode==eMode::Write);
  xAssert(SrcPlane->isCompatible(m_Width, m_Height, m_BitDepth));

  if(m_BufferSize > 0) { m_PackedFrame = m_PackedTank->get(); }
  m_PackedFrame->pack(SrcPlane);
  m_PackedFrame->setPOC(SrcPlane->getPOC());
  writePacked(m_PackedFrame, true);
  if(m_BufferSize > 0) { m_PackedTank->put(m_PackedFrame); m_PackedFrame = nullptr; }
}

//=============================================================================================================================================================================

class xSeqNULL : public xSeqBase
{
public:
  xSeqNULL() {}

protected:
  bool    xBackendAllowsRead  () const { return true; }
  bool    xBackendAllowsWrite () const { return true; }
  bool    xBackendAllowsSeek  () const { return true; }
  eResult xBackendCreateRead  (                    );
  eResult xBackendCreateWrite (                    );
  eResult xBackendDestroy     (                    );
  eResult xBackendWriteFrame  (xPacked* PackedFrame);
  eResult xBackendReadFrame   (xPacked* PackedFrame);
  eResult xBackendSeekFrame   (                    );
  eResult xBackendSkipFrame   (                    );

public:
  eResult  overwriteNumOfFrames(int32 NumOfFrames);
};

//=============================================================================================================================================================================

class xSeqRAW : public xSeqBase
{
protected:
  xFile m_File;

public:
  xSeqRAW() {}
  uint64   getSeqFileSize() { return m_File.size(); }
   
protected:
  bool    xBackendAllowsRead  () const { return true; }
  bool    xBackendAllowsWrite () const { return true; }
  bool    xBackendAllowsSeek  () const { return true; }
  eResult xBackendCreateRead  (                    );
  eResult xBackendCreateWrite (                    );
  eResult xBackendDestroy     (                    );
  eResult xBackendWriteFrame  (xPacked* PackedFrame);
  eResult xBackendReadFrame   (xPacked* PackedFrame);
  eResult xBackendSeekFrame   (                    );
  eResult xBackendSkipFrame   (                    );

public:
  eResult  overwriteNumOfFrames(int32 NumOfFrames);
};  

//=============================================================================================================================================================================

} //end of namespace AVLib
