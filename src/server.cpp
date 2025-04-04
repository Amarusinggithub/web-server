#include <tcp_server.h>

int main(int argc, char* argv[]){
    using namespace http;
    TcpServer server=TcpServer(8080,"0.0.0.0");
    return 0;
}