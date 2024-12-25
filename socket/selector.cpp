#include "socket/selector.h"

using namespace melon::socket;

Selector::Selector()
  : m_max_fd(0)
{
  FD_ZERO(&m_fds);
  FD_ZERO(&m_read_fds);
}
      
Selector::~Selector()
{
  m_max_fd = 0;
  FD_ZERO(&m_fds);
  FD_ZERO(&m_read_fds);
}

void Selector::setFd(int fd)
{
  FD_SET(fd, &m_fds);
  if (m_max_fd < fd)
    m_max_fd = fd;
}
      
void Selector::delFd(int fd)
{
  FD_CLR(fd, &m_fds);
}
  
int Selector::select(int milliseconds)
{
  struct timeval tv;
  tv.tv_sec = milliseconds / 1000;
  tv.tv_usec = milliseconds % 1000 * 1000;

  m_read_fds = m_fds;
  return ::select(m_max_fd + 1, &m_read_fds, nullptr, nullptr, &tv);
}
      
int Selector::maxFd()
{
  return m_max_fd;
}
      
bool Selector::isSet(int fd)
{
  return FD_ISSET(fd, &m_read_fds);
}
