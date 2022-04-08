#ifndef __FNET_NET_TCPCONN_
#define __FNET_NET_TCPCONN_

#include "../base/base.h"
#include "../base/socket.h"
#include "Channel.h"
#include <any>
#include <cstddef>
#include <memory>
namespace net {

class TcpConnection {

  enum ConnState {
    CONN_CONNECTED,
    CONN_CONNECTING,
    CONN_DISCONNECTED,
    CONN_DISCONNECTING
  };

  private:
  private:
  EventLoop* loop;
  ConnState state_;
  bool reading_;

  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;
  const InetAddress localaddr_;
  const InetAddress peeraddr_;

  ConnectionCallback connection_callback_;
  MessageCallback message_callback_;
  WriteCompelteCallback write_compelte_callback_;
  HighWaterCallback high_water_callback_;
  CloseCallback close_callbcak_;

  size_t high_water_mark_;
  Buffer input_buffer_;
  Buffer output_buffer_;
  std::any context_;
};
}

void test()
{
}

#endif
