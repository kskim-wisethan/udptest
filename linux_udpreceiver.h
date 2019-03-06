#ifndef LINUX_UDPRECEIVER_H
#define LINUX_UDPRECEIVER_H

#include <netinet/in.h>
#include <memory>
#include <string>

#define DEFAULT_BUFLEN      4096


class Linux_UDPReceiver
{
public:
    enum UDP_TYPE {
        UNICAST = 1,
        MULTICAST,
        BROADCAST
    };

public:
    Linux_UDPReceiver();
    ~Linux_UDPReceiver();

    int init_socket(const std::string& ip, unsigned short port, int type = UNICAST);
    int wait_data();
    int close_socket();
    bool isWaiting() { return m_waiting_flag; }
    void stop_waiting() { m_waiting_flag = false; }

    int getSize() { return m_readlen; }

private:
    void run();

private:
    bool m_waiting_flag;
    int m_socket;
    int m_readlen;
    int m_type;
    struct sockaddr_in m_recvaddr;
};

#endif // LINUX_UDPRECEIVER_H
