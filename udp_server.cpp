#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <arpa/inet.h>

// for sleeping
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using std::cout;
using std::endl;


int main() {

    int port = 20000;

    const int buffer_len = 1024;
    char buffer[buffer_len];

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    // create socket file descriptor
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("sock_fd < 0");
        return 1;
    }

    memset(&server_address, 0, sizeof(server_address));
    memset(&client_address, 0, sizeof(client_address));

    // filling server info
    server_address.sin_family = AF_INET;
//    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(port);

    // bind the socket with the server address
    int bind_result = bind(sock_fd, (const struct sockaddr*) &server_address, sizeof(server_address));
    if (bind_result < 0) {
        perror("bind failed");
        return 1;
    }

    cout << "start listening for UDP" << endl;

    socklen_t len = sizeof(client_address);
    socklen_t n = recvfrom(sock_fd,
                     (char*) buffer, buffer_len,
                     MSG_WAITALL,
                     (struct sockaddr*)&client_address,
                     &len);

    buffer[n] = '\0';
    cout << buffer << endl;

//    std::string server_response = "Answer from the server!";
//    sendto(sock_fd,
//           server_response.c_str(),
//           server_response.length(),
//           MSG_CONFIRM,
//           (const struct sockaddr*) &client_address,
//                   len);

    // Simple data stream:
    for (int i = 0; i < 10; i++) {
        std::string response_update = "Update " + std::to_string(i);
        sendto(sock_fd,
               response_update.c_str(),
               response_update.length(),
               MSG_CONFIRM,
               (const struct sockaddr*) &client_address,
                       len);
        std::this_thread::sleep_for (std::chrono::seconds(2));
    }

    shutdown(sock_fd, SHUT_RDWR);
    cout << "shutdown by the server" << endl;

    return 0;
}
