#ifndef INCLUDED_TCPSERVER_H
#define INCLUDED_TCPSERVER_H

#include <string>
#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#elif defined(__linux__)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
#define PORT 6969
namespace http {
class TcpServer {
public:
  TcpServer(const int port, const std::string &ip_address);
  ~TcpServer();

private:
#ifdef _WIN32
  SOCKET m_socket{};
  SOCKET m_new_socket{};
#elif defined(__linux__)
  int m_socket{};
  int m_new_socket{};
#endif
  int m_port;
  std::string m_ip_address;
  struct sockaddr_in m_socketAddress;
  int m_socketAddress_len;
  long m_client_message;
  std::string m_server_message;
#ifdef _WIN32
  WSADATA m_wsDATA{};
#endif

  void startListening() const;
  int startServer();
  void closeServer() const;
  int startAccepting();
  void readRequestBytes();
  void sendAResponse();
};
} // namespace http
#endif