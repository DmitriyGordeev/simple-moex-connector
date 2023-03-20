#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <arpa/inet.h>

using std::cout;
using std::endl;

/** simple cheat-sheet for TCP server */

int main() {

    int port = 19000;

    // 1. define socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // 2. bind
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    int bind_value = bind(sock, (struct sockaddr* )& address, sizeof(address));

    // bind_value < 0 -> unsuccessfull
    if (bind_value < 0) {
        perror("Could not bind socket");
        return 1;
    }


    // 3. listen
    int listen_value = listen(sock, 1);
    if (listen_value < 0) {
        perror("Could not listen");
        return 1;
    }

    // 4. accept
    struct sockaddr_in remote_address;
    socklen_t remote_address_len = sizeof(address);

    cout << "Start waiting for connections" << endl;

    // accept will wait until there's something to accept
    int client_socket = accept(sock, (struct sockaddr*)& remote_address, &remote_address_len);
    if (client_socket < 0) {
        cout << "client_socket < 0" << std::endl;
        return 1;
    }

    std::string client_ip = inet_ntoa(remote_address.sin_addr);
    int remote_port = ntohs(remote_address.sin_port);

    cout << "accepted new client " << client_ip << ":" << remote_port << endl;

    const int buffer_len = 1024;
    char buffer[buffer_len];

    // receive:
//    while (true) {
        memset(buffer, 0, buffer_len);  // clear out buffer before receive and put there some data

        int byte_received = recv(client_socket, buffer, buffer_len - 1, 0);

        if (byte_received < 0) {
            perror("byte_received < 0");
            return 1;
        }

        if (byte_received == 0) {
            cout << "client at " << client_ip << ":" << remote_port << " has disconnected" << endl;
            break;
        }

        // Printing
        cout << "buffer:" << buffer << endl;

        std::string response = "Hello client at " + client_ip + ":" + std::to_string(port);

        int bytes_sent = send(client_socket, response.c_str(), response.length(), 0);
        if (bytes_sent < 0) {
            perror("Could not send");
            return 1;
        }
//    }

    cout << "Shutting down socket" << std::endl;
    shutdown(client_socket, SHUT_RDWR);

    return 0;
}
