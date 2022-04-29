#include "Thread.h"
#include <cstdio>
#include <stdio.h>

using namespace net::Thread;
using namespace net::CurrentThread;
// using namespace bidentifierase;

pid_t gettid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

void cacheThreadId()
{
  if (cached_current_tid == 0) {
    cached_current_tid = net::Thread::gettid();
    tid_strng_length = snprintf(tid_string, sizeof(tid_string), "%5d", cached_current_tid);
  }
}
