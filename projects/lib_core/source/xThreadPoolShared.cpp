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

#include "xThreadPoolShared.h"

using namespace std::chrono_literals;

namespace AVlib {

//=====================================================================================================================================================================================

void xThreadPoolShared::xWorkerTask::StarterFunction(xWorkerTask* WorkerTask, int32 ThreadIdx)
{
  xAssert(WorkerTask->m_Status == eTaskStatus::Waiting);
  WorkerTask->m_Status = eTaskStatus::Processed;
  WorkerTask->WorkingFunction(ThreadIdx);
  WorkerTask->m_Status = eTaskStatus::Completed;
}

//=====================================================================================================================================================================================

void xThreadPoolShared::create(int32 NumThreads, int32 WaitingQueueSize)
{
  xAssert(NumThreads>0);
  xAssert(WaitingQueueSize>0);

  m_NumThreads = NumThreads;
  m_WaitingTasks.setSize(WaitingQueueSize);

  for(int32 i=0; i<m_NumThreads; i++)
  {
    std::packaged_task<uint32(xThreadPoolShared*)> PackagedTask(xThreadStarter);
    m_Future.push_back(PackagedTask.get_future());
    std::thread Thread = std::thread(std::move(PackagedTask), this);
    m_ThreadId.push_back(Thread.get_id());
    m_Thread.push_back(std::move(Thread));      
  } 

  m_Event.set();
}
void xThreadPoolShared::destroy()
{
  bool AnyActive = false;

  xAssert(isWaitingQueueEmpty());

  for(int32 i=0; i<m_NumThreads; i++)
  {
    xWorkerTask* Terminator = new xWorkerTaskTerminate;
    m_WaitingTasks.EnqueueWait(Terminator);
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

  for(std::pair<const uintPtr, xFIFO<xWorkerTask*>>& Pair : m_CompletedTasks)
  {
    xFIFO<xWorkerTask*>& CompletedTaskQueue = Pair.second;
    int32 NumCompleted = (int32)CompletedTaskQueue.getLoad();
    for(int32 i=0; i<NumCompleted; i++)
    {
      xWorkerTask* Task;
      CompletedTaskQueue.DequeueWait(Task);
      delete Task;
    }
  }  
}
bool xThreadPoolShared::registerClient(uintPtr ClientId, int32 CompletedQueueSize)
{
  if(m_CompletedTasks.find(ClientId) != m_CompletedTasks.end()) { return false; }

  m_CompletedTasks.emplace(ClientId, CompletedQueueSize);
  return true;
}
bool xThreadPoolShared::unregisterClient(uintPtr ClientId)
{
  if(m_CompletedTasks.find(ClientId) == m_CompletedTasks.end()) { return false; }

  xFIFO<xWorkerTask*>& CompletedTaskQueue = m_CompletedTasks.at(ClientId);
  int32 NumCompleted = (int32)CompletedTaskQueue.getLoad();
  for(int32 i=0; i<NumCompleted; i++)
  {
    xWorkerTask* Task;
    CompletedTaskQueue.DequeueWait(Task);
    delete Task;
  }

  m_CompletedTasks.erase(ClientId);
  return true;
}
uint32 xThreadPoolShared::xThreadFunc() 
{
  m_Event.wait();
  std::thread::id ThreadId = std::this_thread::get_id();
  int32 ThreadIdx = (int32)(std::find(m_ThreadId.begin(), m_ThreadId.end(), ThreadId) - m_ThreadId.begin());
  while(1)
  {    
    xWorkerTask* Task;
    m_WaitingTasks.DequeueWait(Task);
    if(Task->getStatus() == eTaskStatus::Terminate) 
    {
      delete Task; break;
    }
    xWorkerTask::StarterFunction(Task, ThreadIdx);
    m_CompletedTasks.at(Task->getClientId()).EnqueueWait(Task);
  }
  return EXIT_SUCCESS;
}

//=====================================================================================================================================================================================

void xThreadPoolInterface::init(xThreadPoolShared* ThreadPool, int32 CompletedQueueSize)
{
  m_ThreadPool = ThreadPool;
  m_ThreadPool->registerClient((uintPtr)this, CompletedQueueSize);
  m_NumChunks  = m_ThreadPool->getNumThreads();
}
void xThreadPoolInterface::uininit()
{
  if(m_ThreadPool == nullptr) { return; }
  m_ThreadPool->unregisterClient((uintPtr)this);
  m_ThreadPool = nullptr;
}
void xThreadPoolInterface::addWaitingTask(xThreadPoolShared::xWorkerTask* Task)
{ 
  Task->setClientId((uintPtr)this);
  Task->setPriority(m_Priority);
  m_ThreadPool->addWaitingTask(Task);
}
void xThreadPoolInterface::addWaitingTask(std::function<void(int32)> Function)
{ 
  xThreadPoolShared::xWorkerTaskFunction* Task = new xThreadPoolShared::xWorkerTaskFunction((uintPtr)this, m_Priority, Function);
  m_ThreadPool->addWaitingTask(Task);
}
void xThreadPoolInterface::waitUntilTasksFinished(int32 NumTasksToWaitFor)
{
  for(int32 TaskId=0; TaskId < NumTasksToWaitFor; TaskId++)
  {
    xThreadPoolShared::xWorkerTask* Task = m_ThreadPool->receiveCompletedTask((uintPtr)this);
    delete Task;
  }
}
void xThreadPoolInterface::executeTask(std::function<void(int32)> Function)
{
  addWaitingTask(Function);
  waitUntilTasksFinished(1);
}
std::vector<std::pair<int32,int32>> xThreadPoolInterface::distributeWorkIntoChunks(int32 NumUnits, int32 NumChunks)
{
  NumChunks = xMax(NumChunks, 1);

  int32 UnitsPerThread = NumUnits / NumChunks;
  int32 UnitsRemaining = NumUnits % NumChunks;

  std::vector<std::pair<int32,int32>> Result(NumChunks);
  int32 NextBeg = 0;
  for(int32 i=0; i < NumChunks; i++)
  {
    int32 NextHeight = UnitsPerThread + ((UnitsRemaining--) > 0);
    int32 NextEnd = NextBeg + NextHeight;
    Result[i] = { NextBeg, NextEnd };
    NextBeg = NextEnd;
  }
  return Result;
}

//=====================================================================================================================================================================================

} //end of namespace AVLib