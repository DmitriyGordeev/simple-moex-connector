#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <cstring>
#include <cstdlib>

#include "UDPListener.h"

// This is a simple example of MOEX connector for listening stock market trading data

void write_lines(const std::string& filename, const std::vector<std::string>& lines) {
    std::ofstream f_out(filename);
    for(const auto& l : lines)
        f_out << l << std::endl;
    f_out.close();
}

int main() {

    // MOEX's source ip to receive MarketData
    const std::string sourceIp = "91.203.253.242";

    UDPListener udpListener1(sourceIp, "239.195.12.68", 48068);
    udpListener1.setName("FO-BOOK-1(Snapshot A)");
    if (!udpListener1.init()) {
        std::cout << "init() failed\n";
        return 1;
    }

    UDPListener udpListener2(sourceIp, "239.195.12.72", 48072);
    udpListener2.setName("FO-BOOK-20(Snapshot A)");
    if (!udpListener2.init()) {
        std::cout << "init() failed\n";
        return 1;
    }

    // start listening market data in a loop,
    // each UPDListener instance listens for it's own specified channel
    // e.g. FO-BOOK-20 mean that we subscribed for Futures and Options
    // order book updates up to 20 bid/ask levels
    std::thread t1(&UDPListener::listen, &udpListener1);
    std::thread t2(&UDPListener::listen, &udpListener2);

    t1.join();
    t2.join();

    return 0;
}