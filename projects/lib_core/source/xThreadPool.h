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
#include "xRing.h"
#include "xEvent.h"
#include <vector>
#include <future>

namespace AVlib {

//=====================================================================================================================================================================================

class xThreadPool
{
public:
  class xWorkerTask
  {
  public:
    enum eTaskStatus
    {
      TaskStatus_INVALID = NOT_VALID,
      TaskStatus_UNKNOWN = 0,
      TaskStatus_Waiting,
      TaskStatus_Processed,
      TaskStatus_Completed,
      TaskStatus_Terminate,
    };

  protected:
    eTaskStatus m_Status; 

  public:
    virtual ~xWorkerTask() {}
    virtual void WorkingFunction(int32 ThreadIdx) = 0;
    static  void StarterFunction(xWorkerTask* WorkerTask, int32 ThreadIdx);
    void         setStatus(eTaskStatus Status) { m_Status = Status; }
    eTaskStatus  getStatus() { return m_Status; }
  };

private:
  class xWorkerTaskTerminate final : public xWorkerTask
  {
    void WorkingFunction(int32 ThreadIdx) {}
  };

protected:
  std::vector<std::future<uint32>> m_Future;
  std::vector<std::thread>         m_Thread;
  std::vector<std::thread::id>     m_ThreadId;

  int32                   m_NumThreads;
  int32                   m_QueueSize;
  xRing<xWorkerTask*>     m_WaitingTasks;
  xRing<xWorkerTask*>     m_CompletedTasks;  
  xEvent                  m_Event;

protected:  
  uint32        xThreadFunc();
  static uint32 xThreadStarter(xThreadPool* ThreadPool) { return ThreadPool->xThreadFunc(); }

public:
  xThreadPool() : m_Event(true, false) {}

  void   create   (int32 NumThreads, int32 QueueSize);
  void   destroy  ();

  void          addWaitingTask       (xWorkerTask* Task) { m_WaitingTasks.put(Task); }
  xWorkerTask*  receiveCompletedTask (                 ) { return m_CompletedTasks.get(); }
  int32         getNumThreads        (                 ) { return m_NumThreads; }
  int32         getQueueSize         (                 ) { return m_QueueSize;  }
  bool          isEmpty              (                 ) { return m_CompletedTasks.isEmpty() && m_WaitingTasks.isEmpty(); }
};

//=====================================================================================================================================================================================

} //end of namespace AVLib
