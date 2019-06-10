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
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

#ifdef max
#undef max
#endif

namespace AVlib {

//===============================================================================================================================================================================================================

class xSemaphore
{
protected:
  uint32                  m_Count;
  uint32                  m_MaxCount;
  std::mutex              m_Mutex;
  std::condition_variable m_ConditionVariable;

  std::condition_variable m_NotifyConditionVariable;
  std::condition_variable m_WaitConditionVariable;


public:
  xSemaphore(uint32 InitialCount, uint32 MaximumCount) : m_Count(InitialCount), m_MaxCount(MaximumCount) {}
  xSemaphore(const xSemaphore&) = delete;
  xSemaphore& operator=(const xSemaphore&) = delete;

  inline void Notify();
  inline void Wait  ();
  inline void WaitAndClear(uint32& Count);
  inline bool TryWait();

  template<class Rep, class Period> bool WaitFor        (               const std::chrono::duration<Rep, Period>& Duration);
  template<class Rep, class Period> bool WaitForAndClear(uint32& Count, const std::chrono::duration<Rep, Period>& Duration);

  template<class Clock, class Duration> bool WaitUntil        (               const std::chrono::time_point<Clock, Duration>& TimePoint);
  template<class Clock, class Duration> bool WaitUntilAndClear(uint32& Count, const std::chrono::time_point<Clock, Duration>& TimePoint);
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

inline void xSemaphore::Notify()
{
  std::unique_lock<std::mutex> LockManager(m_Mutex);
  while(m_Count == m_MaxCount){ m_NotifyConditionVariable.wait(LockManager, [&]{ return m_Count < m_MaxCount;}); }
  m_Count++;
  m_WaitConditionVariable.notify_all();
  //release lock - std::unique_lock destructor... 
}
inline void xSemaphore::Wait()
{
  std::unique_lock<std::mutex> LockManager(m_Mutex);
  while(m_Count == 0){ m_WaitConditionVariable.wait(LockManager, [&]{ return m_Count > 0;} ); }
  m_Count--;
  m_NotifyConditionVariable.notify_all();
  //release lock - std::unique_lock destructor... 
}
inline void xSemaphore::WaitAndClear(uint32& Count)
{
  std::unique_lock<std::mutex> LockManager(m_Mutex);
  while(m_Count == 0){ m_WaitConditionVariable.wait(LockManager, [&]{ return m_Count > 0;} ); }
  Count = m_Count;
  m_Count = 0;
  m_NotifyConditionVariable.notify_one();
  //release lock - std::unique_lock destructor... 
}
inline bool xSemaphore::TryWait()
{
  if(m_Mutex.try_lock())
  {
    if(m_Count > 0) { m_Count--; return true; }
    m_Mutex.unlock();
  }
  return false;
}
template<class Rep, class Period> bool xSemaphore::WaitFor(const std::chrono::duration<Rep, Period>& Duration)
{
  if(Duration == std::chrono::duration<Rep, Period>::max()) { Wait(); return true; } //fix for some broken implementations (i.e. MSVC 14)    
  std::unique_lock<std::mutex> LockManager(m_Mutex);
  bool Finished = m_WaitConditionVariable.wait_for(LockManager, Duration, [&]{ return m_Count > 0; }); 
  if (Finished) { --m_Count; }
  m_NotifyConditionVariable.notify_one();
  return Finished;
}
template<class Rep, class Period> bool xSemaphore::WaitForAndClear(uint32& Count, const std::chrono::duration<Rep, Period>& Duration)
{
  if(Duration == std::chrono::duration<Rep, Period>::max()) { WaitAndClear(Count); return true; } //fix for some broken implementations (i.e. MSVC 14)    
  std::unique_lock<std::mutex> LockManager(m_Mutex);
  bool Finished = m_WaitConditionVariable.wait_for(LockManager, Duration, [&]{ return m_Count > 0; }); 
  Count = 0;
  if (Finished) { Count = m_Count; m_Count = 0; }
  m_NotifyConditionVariable.notify_one();
  return Finished;
}
template<class Clock, class Duration> bool xSemaphore::WaitUntil(const std::chrono::time_point<Clock, Duration>& TimePoint)
{
  if(TimePoint == std::chrono::time_point<Clock, Duration>::max()) { Wait(); return true; } //fix for some broken implementations (i.e. MSVC 14)    
  std::unique_lock<std::mutex> LockManager(m_Mutex);
  bool Finished = m_WaitConditionVariable.wait_until(LockManager, TimePoint, [&]{ return m_Count > 0; });
  if (Finished) { --m_Count; }
  m_NotifyConditionVariable.notify_one();
  return Finished;
}
template<class Clock, class Duration> bool xSemaphore::WaitUntilAndClear(uint32& Count, const std::chrono::time_point<Clock, Duration>& TimePoint)
{
  if(TimePoint == std::chrono::time_point<Clock, Duration>::max()) { WaitAndClear(Count); return true; } //fix for some broken implementations (i.e. MSVC 14)    
  std::unique_lock<std::mutex> LockManager(m_Mutex);
  bool Finished = m_WaitConditionVariable.wait_until(LockManager, TimePoint, [&]{ return m_Count > 0; });
  Count = 0;
  if (Finished) { Count = m_Count; m_Count = 0; }
  m_NotifyConditionVariable.notify_one();
  return Finished;
}

//===============================================================================================================================================================================================================

} //end of namespace AVLib