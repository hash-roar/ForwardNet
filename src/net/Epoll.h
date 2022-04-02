#ifndef __FNET_NET_EPOLL_
#define __FNET_NET_EPOLL_

#include "../base/Timestamp.h"
#include "Channel.h"
#include "EventLoop.h"
#include <sys/epoll.h>
#include <map>
#include <vector>

namespace net {

class Epoll {
  public:
  Epoll(EventLoop* loop);
  ~Epoll();
  base::Timestamp poll(int timeutMs, ChannelList* activeChannels);
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);

  private:
  static const int kInitEventListSize = 16;
  static const char* operationToString(int op);

  void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;

  void update(int operation, Channel* channel);

  void assertInLoopThread() const
  {
    owner_loop_->assertInLoopThread();
  }
  private:
  typedef std::vector<struct epoll_event> EventList;
  typedef std::map<int,Channel*> ChannelMap;
  int epollfd_;
  EventList events_;
  EventLoop* owner_loop_;
  ChannelMap channels_;
};
}

#endif //__FNET_NET_EPOLL_
