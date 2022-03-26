#include "cast.h"
#include "base.h"
#include <arpa/inet.h>
#include <unistd.h>

using namespace base;

void fromIpPort(const char* ip, uint16_t port,
                         struct sockaddr_in* addr)
{
  addr->sin_family = AF_INET;
  addr->sin_port = hostToNetwork16(port);
  if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
  {
    DEBUG_LOG
  }
}

