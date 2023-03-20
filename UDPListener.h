#ifndef SOCKET_EXAMPLES_UDPLISTENER_H
#define SOCKET_EXAMPLES_UDPLISTENER_H

#include <iostream>
#include <string>
#include <utility>
#include <thread>


class UDPListener {
public:
    UDPListener(std::string  sourceIp,
                std::string  multicastGroupIp,
                int port):
        m_SourceIp(std::move(sourceIp)),
        m_MulticastGroupIp(std::move(multicastGroupIp)),
        m_Port(port),
        m_BufferSize(1500) {}

    void setName(const std::string& name) {
        m_name = name;
    }

    bool init() {
        fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (fd < 0) {
            std::cout << "Couldn't create socket\n";
            return false;
        }

        // allow multiple sockets to use the same PORT number
        u_int yes = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &yes, sizeof(yes)) < 0) {
            std::cout << "Reusing addr failed\n";
            return false;
        }

        // set up destination address
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY); // differs from sender
        addr.sin_port = htons(m_Port);

        // bind to receive address
        if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            std::cout << "Couldn't bind addr\n";
            return false;
        }

        // join multicast group using m_MulticastGroupIp:
        bool is_source_specific = true;
        if (is_source_specific) {
            struct ip_mreq_source mreq;
            mreq.imr_multiaddr.s_addr = inet_addr(m_MulticastGroupIp.c_str()); // IP multicast group address
            mreq.imr_interface.s_addr = htonl(INADDR_ANY);
            mreq.imr_sourceaddr.s_addr = inet_addr(m_SourceIp.c_str()); // IP address of multicast source

            if (setsockopt(fd, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (char *) &mreq, sizeof(mreq)) < 0) {
                std::cout << "setsockopt() error: IP_ADD_SOURCE_MEMBERSHIP failed\n";
                return false;
            }
        }
        else  {
            struct ip_mreq mreq;
            mreq.imr_multiaddr.s_addr = inet_addr(m_MulticastGroupIp.c_str()); // IP multicast group address
            mreq.imr_interface.s_addr = htonl(INADDR_ANY);
            if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &mreq, sizeof(mreq)) < 0) {
                std::cout << "setsockopt() error: IP_ADD_MEMBERSHIP failed\n";
                return false;
            }
        }

        return true;
    }

    void listen() {
        while(true) {
            char msgbuf[m_BufferSize];
            socklen_t addrlen = sizeof(addr);
            socklen_t nbytes = recvfrom(
                    fd,
                    msgbuf,
                    m_BufferSize,
                    0,
                    (struct sockaddr *) &addr,
                    &addrlen);

            if (nbytes < 0) {
                std::cout << "recvfrom(): nbytes < 0\n";
                return;
            }

            // bussiness logic:
            std::cout << m_name << ":" << nbytes << "\n";
        }
    }


protected:
    std::string m_SourceIp;
    std::string m_MulticastGroupIp;
    int m_Port;
    const int m_BufferSize;

    struct sockaddr_in addr;
    int fd;

    std::string m_name;
};

#endif //SOCKET_EXAMPLES_UDPLISTENER_H
