#pragma once

#include <sys/select.h>

namespace melon
{
  namespace socket
  {
    class Selector
    {
    public:
      Selector();
      ~Selector();

      void setFd(int fd);
      void delFd(int fd);
      int select(int milliseconds);
      int maxFd();
      bool isSet(int fd);

    private:
      fd_set m_fds;
      fd_set m_read_fds;
      int m_max_fd;
    };
  }
}
