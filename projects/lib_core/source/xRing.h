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

//=============================================================================================================================================================================
//xRing - thread safe FIFO with ring buffer
//=============================================================================================================================================================================
template <class XXX> class xRing
{
protected:
  XXX*   m_Ring;   
  int32  m_RingSize; //ring size
  int32  m_DataCnt;  //number of units in ring
  int32  m_WriteId;  //writting address 
  int32  m_ReadId;   //reading address
  int32  m_TailId;   //tail address   

  //threading utils
  std::mutex              m_Mutex;
  std::condition_variable m_ConditionVariable;

public:
  void   create        (int32 RingSize);
  void   destroy       ();
  void   resize        (int32 NewRingSize);
  int32  getSize       ();

  void   put           (XXX NodeData);
  XXX    get           ();
  XXX    peekLast      ();
  bool   isEmpty       ();
  bool   isFull        ();  
  int32  getLoad       ();
};  

template <class XXX> void xRing<XXX>::create(int32 RingSize)
{
  xAssert(RingSize>0);

  m_Ring     = (XXX*)calloc(1, RingSize*sizeof(XXX));
  m_RingSize = RingSize;
  m_DataCnt  = 0;
  m_WriteId  = 0;
  m_ReadId   = 0;
  m_TailId   = 0;
}
template <class XXX> void xRing<XXX>::destroy()
{
  free(m_Ring); 
}
template <class XXX> void xRing<XXX>::resize(int32 NewRingSize)
{
  xAssert(NewRingSize>0);
  m_Mutex.lock();
  xAssert(NewRingSize>m_DataCnt);
  XXX* NewRing = (XXX*)calloc(1, NewRingSize*sizeof(XXX));  
  for(int32 i=0; i<m_DataCnt; i++)
  { 
    NewRing[i] = m_Ring[m_ReadId];
    m_ReadId = (m_ReadId+1)%m_RingSize;
  }
  m_Ring     = NewRing;
  m_RingSize = NewRingSize;
  m_WriteId  = m_DataCnt;
  m_ReadId   = 0;
  m_TailId   = m_WriteId - 1; 
  m_Mutex.unlock();
}
template <class XXX> int32 xRing<XXX>::getSize()
{
  int32 Size;
  m_Mutex.lock();
  Size = m_RingSize;
  m_Mutex.unlock();
  return Size;
}
template <class XXX> void xRing<XXX>::put(XXX NodeData)
{
  std::unique_lock<std::mutex> LockManager(m_Mutex);
  while(m_DataCnt>=m_RingSize) { m_ConditionVariable.wait(LockManager, [&]{ return m_DataCnt<m_RingSize;} ); }
  m_Ring[m_WriteId] = NodeData;
  m_TailId          = m_WriteId;
  m_WriteId         = (m_WriteId+1)%m_RingSize;
  m_DataCnt++;
  m_ConditionVariable.notify_all();
  //release lock - std::unique_lock destructor... 
}
template <class XXX> XXX xRing<XXX>::get()
{
  XXX NodeData;
  std::unique_lock<std::mutex> LockManager(m_Mutex);
  while(m_DataCnt<=0) { m_ConditionVariable.wait(LockManager, [&]{ return m_DataCnt>0;} ); }
  NodeData = m_Ring[m_ReadId];
  m_ReadId = (m_ReadId+1)%m_RingSize;
  m_DataCnt--;
  m_ConditionVariable.notify_all();
  //release lock - std::unique_lock destructor... 
  return NodeData;
}
template <class XXX> XXX xRing<XXX>::peekLast()
{
  XXX NodeData;
  m_Mutex.lock();
  xAssert(m_DataCnt > 0);
  NodeData = m_Ring[m_TailId];
  m_Mutex.unlock();
  return NodeData;
}
template <class XXX> bool xRing<XXX>::isEmpty()
{
  bool isEmpty;
  m_Mutex.lock();
  isEmpty = m_DataCnt==0;
  m_Mutex.unlock();
  return isEmpty;
}
template <class XXX> bool xRing<XXX>::isFull()
{
  bool isFull;
  m_Mutex.lock();
  isFull = m_DataCnt==m_RingSize;
  m_Mutex.unlock();
  return isFull;
}
template <class XXX> int32 xRing<XXX>::getLoad()
{
  int32 NumElements;
  m_Mutex.lock();
  NumElements = m_DataCnt;
  m_Mutex.unlock();
  return NumElements;
}

//=============================================================================================================================================================================
//xRing - thread safe priority queue with ring buffer
//=============================================================================================================================================================================
template <class XXX> class xPriorityRing2 : public xRing<XXX>
{
protected:
  int32* m_Priority;

public:
  void   create        (int32 RingSize);
  void   destroy       ();
  void   put           (XXX NodeData, int32 NodePriority);
};

//=========================================================================================================================

template <class XXX> void xPriorityRing2<XXX>::create(int32 RingSize)
{
  xRing<XXX>::create(RingSize);
  m_Priority = (int32*)calloc(RingSize, sizeof(int32*));
}
template <class XXX> void xPriorityRing2<XXX>::destroy()
{
  xRing<XXX>::destroy();
  free(m_Priority);
}
template <class XXX> void xPriorityRing2<XXX>::put(XXX NodeData, int32 NodePriority)
{
  std::unique_lock<std::mutex> LockManager(xRing<XXX>::m_Mutex);
  while(xRing<XXX>::m_DataCnt >= xRing<XXX>::m_RingSize) { xRing<XXX>::m_ConditionVariable.wait(LockManager, [&]{ return xRing<XXX>::m_DataCnt < xRing<XXX>::m_RingSize;} ); }

  int32 Id = xRing<XXX>::m_WriteId;
  while(1)
  {
    int32 PrevId = Id == 0 ? xRing<XXX>::m_RingSize-1 : Id-1;

    if(xRing<XXX>::m_Priority[PrevId] <= NodePriority || Id == xRing<XXX>::m_WriteId)
    {
      xRing<XXX>::m_Ring[Id]                                = NodeData;
      xRing<XXX>::m_Priority[Id]                            = NodePriority;
      if(Id==xRing<XXX>::m_WriteId) xRing<XXX>::m_TailId    = xRing<XXX>::m_WriteId;
      xRing<XXX>::m_WriteId                                 = (xRing<XXX>::m_WriteId+1)%xRing<XXX>::m_RingSize;
      xRing<XXX>::m_DataCnt++;
      break;
    }
    else
    {
      xRing<XXX>::m_Ring[Id]     = xRing<XXX>::m_Ring[PrevId];
      xRing<XXX>::m_Priority[Id] = xRing<XXX>::m_Priority[PrevId];
      Id                         = PrevId;
    }
  }
  xRing<XXX>::m_ConditionVariable.notify_all();
  //release lock - std::unique_lock destructor...
}

} //end of namespace AVLib
