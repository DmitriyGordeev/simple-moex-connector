#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>

using std::cout;
using std::endl;

/** simple cheat sheet for TCP client */

int main() {

    const int PORT = 19000;

    struct sockaddr_in server_address;
    std::string hello = "Hello from client";
    const int buffer_len = 1024;
    char buffer[buffer_len] = {0};

//    memset(&server_address, 0, sizeof(server_address));

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cout << "error: sock < 0";
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);


    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        cout << "Invalid address" << endl;
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        cout << "Connection failed" << endl;
        return 1;
    }

    send(sock, hello.c_str(), hello.size(), 0);

    // This will block the execution, until server responds or timeout passed
    int value_read = read(sock, buffer, buffer_len);
    cout << "val_read = " << value_read << endl;

    return 0;
}
