# select

This C++ Select-based Socket Handling Framework simplifies the use of select system calls and socket management in server applications. The library encapsulates adding, removing, checking, and handling file descriptors into a Selector class. It also introduces SocketHandler for managing socket listening and operations, and an EchoTask class to handle the business logic, achieving a clean separation between framework and application code.

## Features

- **Select Encapsulation**: Simplifies the use of the select system call.
- **Socket Management**: Handles socket listening, attachment, and detachment for efficient I/O operations.
- **Task-Based Design**: Separates application-specific logic from the framework using the EchoTask class.
- **Efficient and Scalable**: Built for high-performance server applications.

## Getting Started

1. Prerequisites

   - C++ 11 or later

2. Clone the Repository

```bash
git clone https://github.com/jiafie7/select.git
cd select
```

## Usage

1. Example Code

- handler.cpp

```c
#include "socket/socket_handler.h"

using namespace melon::socket;

int main()
{
  Singleton<LogSystem>::getInstance()->open("./../handler.log");

  SocketHandler* handler = Singleton<SocketHandler>::getInstance();

  handler->listen("127.0.0.1", 7777);

  handler->handle(2000);

  return 0;
}
```

- client.cpp

```c
#include "socket/client_socket.h"

using namespace melon::socket;

int main()
{
  Singleton<LogSystem>::getInstance()->open("./../client.log");
  ClientSocket client("127.0.0.1", 7777);

  std::string msg = "hi, I am client!";
  client.send(msg.c_str(), msg.size());

  char buf[1024] = {0};
  client.recv(buf, sizeof(buf));
  log_debug("recv: %s", buf);

  client.close();

  return 0;
}
```

2. Build the Project

```bash
mkdir build
cd build
cmake ..
make
./handler
./client
```

3. Output

- handler

```bash
select ok: ret = 1.
socket accept: conn_fd = 5.
select ok: ret = 1.
echo task run.
recv: conn = 5, msg = hi, I am client!
echo task destroy.
select ok: ret = 1.
echo task run.
socket closed by peer: conn = 5
echo task destroy.
```

- client

```bash
recv: hi, I am server!
```

## API Reference

### Selector Class

- `Selector()`: Constructor to initialize the selector.
- `~Selector()`: Destructor to clean up resources.
- `void setFd(int fd)`: Adds a file descriptor to the monitoring set.
- `void delFd(int fd)`: Removes a file descriptor from the monitoring set.
- `int select(int milliseconds)`: Waits for I/O events on the monitored file descriptors.
- `int maxFd()`: Returns the highest file descriptor being monitored.
- `bool isSet(int fd)`: Checks if a file descriptor is ready for I/O.

### SocketHandler Class

- `void listen(const std::string& ip, int port)`: Starts listening for connections on the given IP and port.
- `void attach(int socket_fd)`: Adds a socket to the handler for monitoring.
- `void detach(int socket_fd)`: Removes a socket from the handler.
- `void handle(int timeout)`: Processes events on the sockets with a specified timeout.

### EchoTask Class

- `EchoTask(int socket_fd)`: Creates a task for a specific socket.
- `~EchoTask()`: Cleans up resources.
- `bool run()`: Processes incoming data and sends a response.
- `void destroy()`: Cleans up the task and closes the associated socket.

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Contributions

Contributions, bug reports, and feature requests are welcome. Feel free to fork the repository, open issues, or submit pull requests.
