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

namespace AVlib {
class xBarier
{
protected:
  int32       m_Count;
  const int32 m_Initial;
  const bool  m_AutoReset;
  std::mutex  m_Mutex;
  std::condition_variable m_ConditionVariable;

public:
  xBarier(int32 Count, bool AutoReset) : m_Count(Count), m_Initial(Count), m_AutoReset(AutoReset) {}
  xBarier(const xBarier&) = delete;
  xBarier& operator=(const xBarier&) = delete;

  void sync()
  {
    std::unique_lock<std::mutex> LockManager(m_Mutex);
    m_Count--;
    if(m_Count != 0) 
    {
      m_ConditionVariable.wait(LockManager, [&]{ return m_Count == 0;} );      
    } 
    else //last thread
    { 
      if(m_AutoReset) { m_Count = m_Initial; }
      m_ConditionVariable.notify_all();
    }
  }

  void reset()
  {
    std::lock_guard<std::mutex> LockManager(m_Mutex);
    m_Count = m_Initial;
    m_ConditionVariable.notify_all();
  }
};

} //end of namespace AVLib

