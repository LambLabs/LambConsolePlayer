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
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <queue>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace AVlib {

//=============================================================================================================================================================================
// xQueue - thread safe std::queue (FIFO) for any type of data
//=============================================================================================================================================================================
template <class XXX> class xQueue
{
protected:
  std::queue<XXX> m_Queue;
  uint32          m_QueueSize;

  //threading utils
  std::mutex              m_Mutex;
  std::condition_variable m_EnqueueConditionVariable;
  std::condition_variable m_DequeueConditionVariable;
  
public:
  xQueue(int32 QueueSize = 1) { setSize(QueueSize); }

  int32    getSize  (          ) { return m_QueueSize; }
  void     setSize  (int32 Size) { xAssert(Size>0); std::lock_guard<std::mutex> LockManager(m_Mutex); m_QueueSize = Size; m_EnqueueConditionVariable.notify_all(); }
  bool     isEmpty  (          ) { return m_Queue.empty(); }
  bool     isFull   (          ) { return (m_Queue.size() == m_QueueSize); }
  uintSize getLoad  (          ) { return m_Queue.size(); }

  void EnqueueResize(XXX  Data);
  bool DequeueTry   (XXX& Data);

  void EnqueueWait  (XXX  Data);
  void DequeueWait  (XXX& Data);

  template<class Rep, class Period> bool EnqueueWaitFor(XXX  Data, const std::chrono::duration<Rep, Period>& Duration);
  template<class Rep, class Period> bool DequeueWaitFor(XXX& Data, const std::chrono::duration<Rep, Period>& Duration);

  template<class Clock, class Duration> bool EnqueueWaitUntil(XXX  Data, const std::chrono::time_point<Clock, Duration>& TimePoint);
  template<class Clock, class Duration> bool DequeueWaitUntil(XXX& Data, const std::chrono::time_point<Clock, Duration>& TimePoint);
};

template<class XXX> void xQueue<XXX>::EnqueueResize(XXX Data)
{
  m_Mutex.lock();
  if(isFull()) { m_QueueSize++; }
  m_Queue.push(Data);
  m_Mutex.unlock();
  m_DequeueConditionVariable.notify_one();
}
template<class XXX> bool xQueue<XXX>::DequeueTry(XXX& Data)
{
  m_Mutex.lock(); 
  if(!m_Queue.empty())
  {
    Data = m_Queue.front();
    m_Queue.pop();
    m_Mutex.unlock();
    m_EnqueueConditionVariable.notify_one();
    return true;
  }
  else
  {
    m_Mutex.unlock();
    return false;
  }
}
template<class XXX> void xQueue<XXX>::EnqueueWait(XXX Data)
{
  std::unique_lock<std::mutex> LockManager(m_Mutex);
  while(m_Queue.size()>=m_QueueSize) { m_EnqueueConditionVariable.wait(LockManager, [&]{ return m_Queue.size()<m_QueueSize; } ); }
  m_Queue.push(Data);
  LockManager.unlock();
  m_DequeueConditionVariable.notify_one();
  //release lock - std::unique_lock destructor... 
}
template<class XXX> void xQueue<XXX>::DequeueWait(XXX& Data)
{
  std::unique_lock<std::mutex> LockManager(m_Mutex);
  while(m_Queue.empty()) { m_DequeueConditionVariable.wait(LockManager, [&]{ return !m_Queue.empty(); } ); }
  Data = m_Queue.front();
  m_Queue.pop();
  LockManager.unlock();
  m_EnqueueConditionVariable.notify_one();
}
template<class XXX> template<class Rep, class Period> bool xQueue<XXX>::EnqueueWaitFor(XXX Data, const std::chrono::duration<Rep, Period>& Duration)
{
  if(Duration == std::chrono::duration<Rep, Period>::max()) { EnqueueWait(Data); return true; } //fix for some broken implementations (i.e. MSVC 14)

  std::unique_lock<std::mutex> LockManager(m_Mutex);
  std::cv_status Status = std::cv_status::no_timeout;
  if(m_Queue.size()>=m_QueueSize) { Status = m_EnqueueConditionVariable.wait_for(LockManager, Duration); }
  if(Status==std::cv_status::no_timeout && m_Queue.size()<m_QueueSize)
  {
    m_Queue.push(Data);
    LockManager.unlock();
    m_DequeueConditionVariable.notify_one();
    return true;
  }
  else { return false; }    
  //release lock - std::unique_lock destructor... 
}
template<class XXX> template<class Rep, class Period> bool xQueue<XXX>::DequeueWaitFor(XXX& Data, const std::chrono::duration<Rep, Period>& Duration)
{
  if(Duration == std::chrono::duration<Rep, Period>::max()) { DequeueWait(Data); return true; } //fir for some broken implementations (i.e. MSVC 14)

  std::unique_lock<std::mutex> LockManager(m_Mutex);
  std::cv_status Status = std::cv_status::no_timeout;
  if(m_Queue.empty()) { Status = m_DequeueConditionVariable.wait_for(LockManager, Duration); }
  if(Status==std::cv_status::no_timeout && !m_Queue.empty())
  {
    Data = m_Queue.front();
    m_Queue.pop();
    LockManager.unlock();
    m_EnqueueConditionVariable.notify_one();
    return true;
  }
  else { return false; }
  //release lock - std::unique_lock destructor... 
}
template<class XXX> template<class Clock, class Duration> bool xQueue<XXX>::EnqueueWaitUntil(XXX Data, const std::chrono::time_point<Clock, Duration>& TimePoint)
{
  if(TimePoint == std::chrono::time_point<Clock, Duration>::max()) { EnqueueWait(Data); return true; } //fix for some broken implementations (i.e. MSVC 14)

  std::unique_lock<std::mutex> LockManager(m_Mutex);
  std::cv_status Status = std::cv_status::no_timeout;
  if(m_Queue.size()>=m_QueueSize) { Status = m_EnqueueConditionVariable.wait_until(LockManager, TimePoint); }
  if(Status==std::cv_status::no_timeout && m_Queue.size()<m_QueueSize)
  {
    m_Queue.push(Data);
    LockManager.unlock();
    m_DequeueConditionVariable.notify_one();
    return true;
  }
  else { return false; }    
  //release lock - std::unique_lock destructor... 
}
template<class XXX> template<class Clock, class Duration> bool xQueue<XXX>::DequeueWaitUntil(XXX& Data, const std::chrono::time_point<Clock, Duration>& TimePoint)
{
  if(TimePoint == std::chrono::time_point<Clock, Duration>::max()) { DequeueWait(Data); return true; } //fix for some broken implementations (i.e. MSVC 14)

  std::unique_lock<std::mutex> LockManager(m_Mutex);
  std::cv_status Status = std::cv_status::no_timeout;
  if(m_Queue.empty()) { Status = m_DequeueConditionVariable.wait_until(LockManager, TimePoint); }
  if(Status==std::cv_status::no_timeout && !m_Queue.empty())
  {
    Data = m_Queue.front();
    m_Queue.pop();
    LockManager.unlock();
    m_EnqueueConditionVariable.notify_one();
    return true;
  }
  else { return false; }
  //release lock - std::unique_lock destructor... 
}

//=====================================================================================================================================================================================

template <class XXX> using xFIFO = xQueue<XXX>;

//=====================================================================================================================================================================================

} //end of namespace AVLib

