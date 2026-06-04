#include "tcp_server.hpp"
int main(int argc, char *argv[]) {
  using namespace http;
  TcpServer server = TcpServer(PORT, "0.0.0.0");
  return 0;
}