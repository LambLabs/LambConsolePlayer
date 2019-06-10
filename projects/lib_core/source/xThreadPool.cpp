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

#include "xThreadPool.h"

using namespace std::chrono_literals;

namespace AVlib {

//=====================================================================================================================================================================================

void xThreadPool::xWorkerTask::StarterFunction(xWorkerTask* WorkerTask, int32 ThreadIdx)
{
  xAssert(WorkerTask->m_Status == TaskStatus_Waiting);
  WorkerTask->m_Status = TaskStatus_Processed;
  WorkerTask->WorkingFunction(ThreadIdx);
  WorkerTask->m_Status = TaskStatus_Completed;
}

//=====================================================================================================================================================================================

void xThreadPool::create(int32 NumThreads, int32 QueueSize)
{
  xAssert(NumThreads>0);
  xAssert(QueueSize>=NumThreads);

  m_NumThreads = NumThreads;
  m_QueueSize  = QueueSize;
  m_WaitingTasks.create(QueueSize);
  m_CompletedTasks.create(QueueSize);

  for(int32 i=0; i<m_NumThreads; i++)
  {
    std::packaged_task<uint32(xThreadPool*)> PackagedTask(xThreadStarter);
    m_Future.push_back(PackagedTask.get_future());
    std::thread Thread = std::thread(std::move(PackagedTask), this);
    m_ThreadId.push_back(Thread.get_id());
    m_Thread.push_back(std::move(Thread));      
  } 

  m_Event.set();
}
void xThreadPool::destroy()
{
  bool AnyActive = false;

  xAssert(isEmpty());

  for(int32 i=0; i<m_NumThreads; i++)
  {
    xWorkerTask* Terminator = new xWorkerTaskTerminate;
    Terminator->setStatus(xWorkerTask::TaskStatus_Terminate);
    m_WaitingTasks.put(Terminator);
  }
  
  for(int32 i=0; i<m_NumThreads; i++)
  {   
    std::future_status Status = m_Future[i].wait_for(500ms);
    if(Status == std::future_status::ready && m_Thread[i].joinable()) { m_Thread[i].join(); }
    else                                                              { AnyActive = true;   }
  }

  if(AnyActive)
  {
    for(int32 i=0; i<m_NumThreads; i++)
    {
      std::future_status Status = m_Future[i].wait_for(5s);
      if(Status == std::future_status::ready && m_Thread[i].joinable()) { m_Thread[i].join();    }
      else                                                              { m_Thread[i].~thread(); }
    }
  }

  int32 NumCompleted = m_CompletedTasks.getLoad();
  for(int32 i=0; i<NumCompleted; i++)
  {
    xWorkerTask* Task = m_CompletedTasks.get();
    delete Task;
  }

  m_WaitingTasks.destroy();
  m_CompletedTasks.destroy();
}
uint32 xThreadPool::xThreadFunc() 
{
  m_Event.wait();
  std::thread::id ThreadId = std::this_thread::get_id();
  int32 ThreadIdx = (int32)(std::find(m_ThreadId.begin(), m_ThreadId.end(), ThreadId) - m_ThreadId.begin());
  while(1)
  {    
    xWorkerTask* Task = m_WaitingTasks.get();
    if(Task->getStatus() == xWorkerTask::TaskStatus_Terminate) 
    {
      m_CompletedTasks.put(Task);
      break;
    }
    xWorkerTask::StarterFunction(Task, ThreadIdx);
    m_CompletedTasks.put(Task);
  }
  return EXIT_SUCCESS;
}

//=====================================================================================================================================================================================

} //end of namespace AVLib