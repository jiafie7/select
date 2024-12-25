#include "socket/server_socket.h"
#include "socket/selector.h"

using namespace melon::socket;

int main()
{
  Singleton<LogSystem>::getInstance()->open("./../select_server.log");

  ServerSocket server("127.0.0.1", 7777);

  Selector selector;

  selector.setFd(server.fd());

  while (true)
  {
    int ret = selector.select(2000);
    if (ret < 0)
    {
      log_error("select error: errno = %d, errmsg = %s.", errno, strerror(errno));
      break;
    }
    else if (ret == 0)
    {
      log_debug("select timeout.");
      continue;
    }
    log_debug("select ok: ret = %d.", ret);

    for (int fd = 0; fd < selector.maxFd() + 1; ++ fd)
    {
      if (!selector.isSet(fd))
        continue;
      else if (fd == server.fd())
      {
        int conn_fd = server.accept();
        if (conn_fd < 0)
          continue;
        selector.setFd(conn_fd);
      }
      else
      {
        Socket client(fd);

        char buf[1024] = {0};
        std::size_t len = client.recv(buf, sizeof(buf));
        if (len == 0)
        {
          log_debug("socket closed by peer: conn = %d.", fd);
          selector.delFd(fd);
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

  return 0;
}

