#ifndef __FNET_NET_THREAD_
#define __FNET_NET_THREAD_

#include <ctime>
#include <sys/syscall.h>
#include <unistd.h>
namespace net {

namespace Thread {

  /* --------------------------------------------------------------------------*/
  /**
   * @synopsis   get current thread tid by systyem call to get unique thread identity
   *
   * @returns   thread id
   */
  /* ----------------------------------------------------------------------------*/
  pid_t gettid();

}  // namespace Thread
 
namespace CurrentThread {
  extern thread_local int cached_current_tid;
  extern thread_local char tid_string[32];
  extern thread_local int tid_strng_length;
  extern thread_local const char* thread_name;

  void cacheThreadId();

  inline int tid()
  {
    if (cached_current_tid == 0) {
      cacheThreadId();
    }
    return cached_current_tid;
  }
} //CurrentThread

}

#endif
