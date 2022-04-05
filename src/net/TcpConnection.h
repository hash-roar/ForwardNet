#ifndef __FNET_NET_TCPCONN_
#define __FNET_NET_TCPCONN_

#include "Channel.h"
namespace net {

class TcpConnection {

  enum ConnState{
    CONN_CONNECTED,
    CONN_CONNECTING,
    CONN_DISCONNECTED,
    CONN_DISCONNECTING
  };
  private:
  private:
    EventLoop *loop;
    
};
}

#endif
