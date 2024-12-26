#pragma once

#include "socket/server_socket.h"
#include "socket/selector.h"
#include "utility/singleton.h"

using namespace melon::utility;

namespace melon
{
  namespace socket
  {
    class SocketHandler
    {
      SINGLETON(SocketHandler);
    public:
      // SocketHandler() = default;
      // ~SocketHandler() = default;

      void listen(const std::string& ip, int port);
      void attach(int socket_fd);
      void detach(int socket_fd);
      void handle(int timeout);

    private:
      Socket* m_server = nullptr;
      Selector m_selector;
    };
  }
}
