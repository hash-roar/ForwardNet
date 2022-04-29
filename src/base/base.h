#ifndef __FNET_BASE__BASE_
#define __FNET_BASE__BASE_

#include <functional>

#define DEBUG_LOG 

namespace base {
  typedef std::function<void()> TimerCallback;

  class noncopyable{
    public:
      noncopyable( const noncopyable& ) =delete;
      void operator=(const noncopyable&) =delete;

      protected:
      noncopyable()=default;
      ~noncopyable()=default;

  };
}


#endif
