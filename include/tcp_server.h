#ifndef INCLUDED_TCPSERVER_H
#define INCLUDED_TCPSERVER_H

#include <WinSock2.h>
#include <string>
#include <ws2tcpip.h>


namespace http{
    class TcpServer{
    public:
    TcpServer(int port,std::string ip_address);
    ~TcpServer();


    private:
    SOCKET m_socket{};
    SOCKET m_new_socket{};
    int m_port;
    std::string m_ip_address;
    struct sockaddr_in m_socketAddress;
    int m_socketAddress_len;
    long m_client_message;
    std::string m_server_message;
    WSADATA m_wsDATA{};

    void startListening() const;
    int startServer();
    void closeServer() const;
    int startAccepting();
    void readRequestBytes();
    void sendAResponse();
};
}
#endif 