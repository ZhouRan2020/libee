#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <memory>

#include "core/connection.h"
#include "core/net_address.h"
#include "core/socket.h"
#include "core/thread_pool.h"

#define BUF_SIZE 2048

namespace libee {
class EchoClient {
 public:
  explicit EchoClient(NetAddress server_address) {
    auto client_socket = std::make_unique<Socket>();
    client_socket->Connect(server_address);
    client_connection = std::make_unique<Connection>(std::move(client_socket));
  }

  void Begin() {
    char buf[BUF_SIZE + 1];
    memset(buf, 0, sizeof(buf));
    int fd = client_connection->GetFd();
    while (true) {
      // scan from user keyboard
      std::cout<<"type\n";
      auto actual_read = read(STDIN_FILENO, buf, BUF_SIZE);
      std::cout<<"read done\n";
      send(fd, buf, actual_read, 0);
      std::cout<<"send done\n";
      memset(buf, 0, sizeof(buf));
      // echo back to screen from server's message
      auto actual_recv = recv(fd, buf, BUF_SIZE, 0);
      std::cout<<"message recved\n";
      write(STDOUT_FILENO, buf, actual_recv);
      memset(buf, 0, sizeof(buf));
    }
  }

 private:
  std::unique_ptr<Connection> client_connection;
};
}  // namespace TURTLE_SERVER

int main() {
  libee::NetAddress local_address("127.0.0.1", 1888);
  libee::EchoClient echo_client(local_address);
  echo_client.Begin();

  return 0;
}