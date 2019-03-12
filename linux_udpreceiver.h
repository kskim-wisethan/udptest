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
        MULTICAST,      // class D (224.0.0.0 ~ 239.255.255.255)
        BROADCAST
    };

public:
    Linux_UDPReceiver(const std::string& ip, unsigned short port, int type = UNICAST, const std::string& name = "");
    Linux_UDPReceiver(const char* ip, unsigned short port, int type = UNICAST, const std::string& name = "");
    ~Linux_UDPReceiver();

    int init_socket();
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
    std::string m_ip;
    std::string m_name;
    unsigned short m_port;
    struct sockaddr_in m_recvaddr;
};

#endif // LINUX_UDPRECEIVER_H
