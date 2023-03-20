#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <arpa/inet.h>

using std::cout;
using std::endl;


/** simple cheat-sheet for UDP client */


int main() {

    int port = 20000;
    const int buffer_len = 1024;

    char buffer[buffer_len];
    struct sockaddr_in server_address;

    // Creating socket file descriptor
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_address, 0, sizeof(server_address));

    // Filling server information
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
//    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    std::string message = "Hello from client";

    sendto(sock_fd, message.c_str(), message.length(),
           MSG_CONFIRM, (const struct sockaddr *) &server_address,
           sizeof(server_address));
    printf("Hello message sent.\n");


    // SINGLE MESSAGE
//    socklen_t len;
//    socklen_t n = recvfrom(sock_fd, (char *)buffer, buffer_len,
//                 MSG_WAITALL, (struct sockaddr *) &server_address,
//                 &len);
//
//
//    buffer[n] = '\0';
//    cout << "response:" << buffer << endl;
//    cout << "len = " << len << endl;


    // trying to receive multiple messages sent from the udp-server:
    while (true) {
        socklen_t len;
        socklen_t n = recvfrom(sock_fd, (char *)buffer, buffer_len,
                               MSG_WAITALL, (struct sockaddr *) &server_address,
                               &len);

        cout << "n = " << n << endl;

        if (n == 0) {
            break;
        }

        buffer[n] = '\0';
        cout << "response:" << buffer << endl;
        cout << "len = " << len << endl;
    }


    shutdown(sock_fd, SHUT_RDWR);
    cout << "shutdown by the client" << endl;

    return 0;
}

