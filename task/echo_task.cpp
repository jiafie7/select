#include "task/echo_task.h"

using namespace melon::task;

EchoTask::EchoTask(int socket_fd)
  : m_socket_fd(socket_fd)
{
}

EchoTask::~EchoTask()
{
  destroy();
}

bool EchoTask::run()
{
  log_debug("echo task run.");

  Socket socket(m_socket_fd);

  char buf[1024] = {0};

  int len = socket.recv(buf, sizeof(buf));

  if (len < 0)
  {
    if (errno == EAGAIN || errno == EWOULDBLOCK) 
    {
      log_debug("socket recv/send would block: conn = %d", m_socket_fd);
      return true;
    }
    else if (errno == EINTR)
    {
      log_debug("socket recv interrupted: conn = %d", m_socket_fd);
      return true;
    }
    log_debug("socket connection abort: conn = %d", m_socket_fd);
    return false;
  }
  else if (len == 0)
  {
    log_debug("socket closed by peer: conn = %d", m_socket_fd);
    return false;
  }
  log_debug("recv: conn = %d, msg = %s", m_socket_fd, buf);
  
  std::string msg = "hi, I am server!";
  socket.send(msg.c_str(), msg.size());
  
  return true;
}

void EchoTask::destroy()
{
  log_debug("echo task destroy.");
}


