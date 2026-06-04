#include "tcp_server.hpp"
#include <iostream>
namespace http {

TcpServer::TcpServer(const int port, const std::string &ip_address)
    : m_port(port), m_ip_address(ip_address), m_client_message(),
      m_server_message(), m_socketAddress(),
      m_socketAddress_len(sizeof(m_socketAddress)) {
  startServer();
}

TcpServer::~TcpServer() { closeServer(); }

int TcpServer::startServer() {

#ifdef _WIN32
  if (WSAStartup(MAKEWORD(2, 0), &m_wsDATA) != 0) {
    std::cerr << "WSAStartup failed with error: "
              << WSAStartup(MAKEWORD(2, 0), &m_wsDATA);
    return 1;
  }
#endif

  m_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (m_socket < 0) {
    std::cerr << "Failed to create socket" << std::endl;
    ;
  }

  m_socketAddress.sin_family = AF_INET;
  m_socketAddress.sin_port = htons(m_port);
  inet_pton(AF_INET, m_ip_address.c_str(), &m_socketAddress.sin_addr);

  if (bind(m_socket, reinterpret_cast<sockaddr *>(&m_socketAddress),
           m_socketAddress_len) != 0) {
    std::cerr << "Failed to bind socket" << std::endl;
    ;
  }

  startListening();
  startAccepting();
  return 0;
}

int TcpServer::startAccepting() {

  m_new_socket =
      accept(m_socket, (sockaddr *)&m_socketAddress, &m_socketAddress_len);

#ifdef _WIN32
  if (m_new_socket == INVALID_SOCKET) {
    std::wcerr << L"accept failed with error: " << WSAGetLastError() << "\n"
               << std::endl;
    return 1;
  }
#elif defined(__linux__)
  if (m_new_socket == -1) {
    std::cerr "accept failed with error:" << std::endl;
    return 1;
  }
#endif

  readRequestBytes();
  return 0;
}

void TcpServer::startListening() const {
  if (listen(m_socket, 20) != 0) {
    std::cerr << "Failed to listen on socket" << std::endl;
  }
}

void TcpServer::readRequestBytes() {
  constexpr uint32_t BUFFER_SIZE = 30720;
  char buffer[BUFFER_SIZE] = {0};

#ifdef _WIN32
  if (recv(m_new_socket, buffer, BUFFER_SIZE, 0) == SOCKET_ERROR) {
    std::cerr << "Failed to read the received bytes" << std::endl;
  }
#elif defined(__linux__)
  if (recv(m_new_socket, buffer, BUFFER_SIZE, 0) == -1) {
    std::cerr << "Failed to read the received bytes" << std::endl;
  }
#endif

  sendAResponse();
}

void TcpServer::sendAResponse() {
  long totalByteSent = 0;

  while (true) {
    m_server_message = "HTTP/1.1 200 OK\nContent-Type: "
                       "text/plain\nContent-Length: 12\n\nHello, World!";
    int byteSent = send(m_new_socket, m_server_message.c_str(),
                        static_cast<int>(m_server_message.size()), 0);
    if (byteSent > 0) {
      std::cerr << "this is the byte size sent: " << byteSent << "\n"
                << std::endl;
      break;
    }

    totalByteSent += byteSent;
  }

  if (totalByteSent == static_cast<long>(m_server_message.size())) {
    std::cerr << "server message was sent: " << m_server_message.c_str() << "\n"
              << std::endl;
  } else {
    std::cerr << "Failed to send server message" << std::endl;
  }
}

void TcpServer::closeServer() const {

#ifdef _WIN32
  closesocket(m_socket);
  closesocket(m_new_socket);
  WSACleanup();
#elif defined(__linux__)
  close(m_socket);
  close(m_new_socket);
#endif
  exit(0);
}

} // namespace http
