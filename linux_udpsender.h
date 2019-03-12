#ifndef LINUX_UDPSENDER_H
#define LINUX_UDPSENDER_H

#include <netinet/in.h>
#include <string>

#define DEFAULT_BUFLEN      4096
#define DEFAULT_TTL         64


class Linux_UDPSender
{
public:
    enum UDP_TYPE {
        UNICAST = 1,
        MULTICAST,
        BROADCAST
    };

public:
    Linux_UDPSender(const std::string& ip, unsigned short port, int type = UNICAST);
    Linux_UDPSender(const char* ip, unsigned short port, int type = UNICAST);
    ~Linux_UDPSender();

    int init_socket();
    int send_data(const char* buf, int len);
    int close_socket();

private:
    unsigned short m_port;
    int m_type;
    int m_socket;
    std::string m_ip;
    struct sockaddr_in m_recvaddr;
};

#endif // LINUX_UDPSENDER_H
