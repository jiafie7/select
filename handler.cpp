#include "socket/socket_handler.h"

using namespace melon::socket;

int main()
{
  Singleton<LogSystem>::getInstance()->open("./../handler.log");

  SocketHandler handler;

  handler.listen("127.0.0.1", 7777);
  
  handler.handle(2000);

  return 0;
}
