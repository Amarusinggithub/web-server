#include "../include/tcp_server.h"

#include <iostream>
#include <sstream>

namespace http {

TcpServer::TcpServer(int port, std::string ip_address)
    : m_port(port),
      m_ip_address(ip_address),
      m_client_message(),
      m_server_message(),
      m_socketAddress(),
      m_socketAddress_len(sizeof(m_socketAddress)) {
    startServer();
}

TcpServer::~TcpServer() {
    closeServer();
}

int TcpServer::startServer() {
    if (WSAStartup(MAKEWORD(2, 0), &m_wsDATA) != 0) {
        printf("WSAStartup failed with error: %d\n", WSAStartup(MAKEWORD(2, 0), &m_wsDATA));
        return 1;
    }

    m_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (m_socket < 0) {
        printf("Failed to create socket");
    }

    m_socketAddress.sin_family = AF_INET;
    m_socketAddress.sin_port = htons(m_port);
    m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

    if (bind(m_socket, (sockaddr*)&m_socketAddress, m_socketAddress_len) != 0) {
        printf("Failed to bind socket");
    }

    startListening();
    startAccepting();
    return 0;
}



int TcpServer::startAccepting() {

    m_new_socket = accept(m_socket, (sockaddr*)&m_socketAddress, &m_socketAddress_len);

    if (m_new_socket == INVALID_SOCKET) {
        wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
        return 1;
    }

    readRequestBytes();
    return 0;
}

void TcpServer::startListening() const {
    if (listen(m_socket, 20) != 0) {
        printf("Failed to listen on socket");
    }
}

void TcpServer::readRequestBytes() {
    const int BUFFER_SIZE = 30720;
    char buffer[BUFFER_SIZE] = {0};

    if (recv(m_new_socket, buffer, BUFFER_SIZE, 0) == SOCKET_ERROR) {
        printf("Failed to read the received bytes");
    }

    sendAResponse();
}

void TcpServer::sendAResponse() {
    long totalByteSent=0;

    while (true) {
        m_server_message="HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello, World!";
        int byteSent = send(m_new_socket, m_server_message.c_str(), m_server_message.size(), 0);
        if (byteSent > 0) {
            printf("this is the byte size sent ");
            break;
        }

        totalByteSent +=byteSent;
    }

    if (totalByteSent == m_server_message.size()) {
        printf("server message was sent", m_server_message );
    } else {
        printf("Failed to send server message");
    }
}

void TcpServer::closeServer() const {
    closesocket(m_socket);
    closesocket(m_new_socket);
    WSACleanup();
    //exit(0);
}

}
