#include <sys/select.h>

#include "socket/server_socket.h"

using namespace melon::socket;

int main()
{
  Singleton<LogSystem>::getInstance()->open("./../server.log");
  ServerSocket server("127.0.0.1", 7777);

  fd_set fds;
  FD_ZERO(&fds);

  FD_SET(server.fd(), &fds);
  int max_fd = server.fd();

  while (true)
  {
    fd_set read_fds = fds;

    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    int ret = ::select(max_fd + 1, &read_fds, nullptr, nullptr, &tv);
    if (ret < 0)
    {
      log_error("select error: errno = %d, errmsg = %s.", errno, strerror(errno));
      break;
    }
    else if (ret == 0)
    {
      log_debug("selecet timeout.");
      continue;
    }
    log_debug("select ok: ret = %d.", ret);

    for (int fd = 0; fd < max_fd + 1; ++ fd)
    {
      if (!FD_ISSET(fd, &read_fds))
        continue;
      else if (fd == server.fd())
      {
        int conn_fd = server.accept();
        if (conn_fd < 0)
          continue;
        FD_SET(conn_fd, &fds);
        if (max_fd < conn_fd)
          max_fd = conn_fd;
      }
      else
      {
        Socket client(fd);

        char buf[1024] = {0};
        std::size_t len = client.recv(buf, sizeof(buf));

        if (len == 0)
        {
          log_debug("socket closed by peer: conn_fd = %d.", fd);
          FD_CLR(fd, &fds);
          client.close();
        }
        else if (len > 0)
        {
          log_debug("recv: conn = %d, msg = %s", fd, buf);

          std::string msg = "hi, I am server!";
          client.send(msg.c_str(), msg.size());

          client.clear();
        }
      }
    }
  }

  server.close();

  return 0;
}
