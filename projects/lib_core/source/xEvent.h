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

namespace AVlib {

//=============================================================================================================================================================================
// xEvent - thread safe binary event
//=============================================================================================================================================================================
class xEvent
{
protected:
  bool                    m_State;
  bool                    m_ManualReset;
  std::mutex              m_Mutex;
  std::condition_variable m_ConditionVariable;

public:
  xEvent(bool ManualReset, bool InitialState) : m_State(InitialState), m_ManualReset(ManualReset) {}
  xEvent(const xEvent&) = delete;
  xEvent& operator=(const xEvent&) = delete;

  void set()
  {
    std::lock_guard<std::mutex> LockManager(m_Mutex);
    m_State = true;
    m_ConditionVariable.notify_all();
  }  
  void reset()
  {
    std::lock_guard<std::mutex> LockManager(m_Mutex);
    m_State = false;
  }
  void wait()
  {
    std::unique_lock<std::mutex> LockManager(m_Mutex);
    while(m_State == false){ m_ConditionVariable.wait(LockManager, [&]{ return m_State;}); }
    if(!m_ManualReset) { m_State = false; }
  }
  bool try_wait()
  {
    if(m_Mutex.try_lock())
    {
      if(m_State)
      {
        if(!m_ManualReset) { m_State = false; }
        return true;
      }
      m_Mutex.unlock();
    }
    return false;
  }
  template<class Rep, class Period> bool wait_for(const std::chrono::duration<Rep, Period>& Duration)
  {
    std::unique_lock<std::mutex> LockManager(m_Mutex);
    std::cv_status Status = m_ConditionVariable.wait_for(LockManager, Duration, [&]{ return m_State; });
    if(!m_ManualReset && Status==std::cv_status::no_timeout) { m_State = false; }
    return Status==std::cv_status::no_timeout;
  }
  template<class Clock, class Duration> bool wait_until(const std::chrono::time_point<Clock, Duration>& TimePoint)
  {
    std::unique_lock<std::mutex> LockManager{m_Mutex};
    std::cv_status Status = m_ConditionVariable.wait_until(LockManager, TimePoint, [&]{ return m_State; });
    if(!m_ManualReset && Status==std::cv_status::no_timeout) { m_State = false; }
    return Status==std::cv_status::no_timeout;
  }
};

//=============================================================================================================================================================================
// xEvent - thread safe event with additional statuc object
//=============================================================================================================================================================================
template <class XXX> class xStatusEvent
{
protected:
  XXX                     m_Status;
  bool                    m_State;
  bool                    m_ManualReset;
  std::mutex              m_Mutex;
  std::condition_variable m_ConditionVariable;

public:
  xStatusEvent(bool ManualReset) : m_State(false), m_ManualReset(ManualReset) {}
  xStatusEvent(const xEvent&) = delete;
  xStatusEvent& operator=(const xEvent&) = delete;

  void set(XXX Status)
  {
    std::lock_guard<std::mutex> LockManager(m_Mutex);
    m_Status = Status;
    m_State = true;
    m_ConditionVariable.notify_all();
  }  
  void reset()
  {
    std::lock_guard<std::mutex> LockManager(m_Mutex);
    m_State = false;
  }
  XXX wait_any()
  {
    std::unique_lock<std::mutex> LockManager(m_Mutex);
    while(m_State == false){ m_ConditionVariable.wait(LockManager, [&]{ return m_State;}); }
    if(!m_ManualReset) { m_State = false; }
    return m_Status;
  }
  void wait_specific(XXX SpecificStatus)
  {
    std::unique_lock<std::mutex> LockManager(m_Mutex);
    while(m_State == false || m_Status != SpecificStatus){ m_ConditionVariable.wait(LockManager, [&]{ return m_State && m_Status == SpecificStatus;}); }
    if(!m_ManualReset) { m_State = false; }
    return;
  }
  XXX check()
  {
    std::lock_guard<std::mutex> LockManager(m_Mutex);
    if(!m_ManualReset) { m_State = false; }
    return m_Status;
  } 
};

//=============================================================================================================================================================================

} //end of namespace AVLib

