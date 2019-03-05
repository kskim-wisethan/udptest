﻿#ifndef LINUX_UDPSENDER_H
#define LINUX_UDPSENDER_H

#include <netinet/in.h>
#include <string>

#define DEFAULT_BUFLEN      4096


class Linux_UDPSender
{
public:
    Linux_UDPSender();
    ~Linux_UDPSender();

    int init_socket(const std::string& ip, unsigned short port, bool broadcast = true);
    int send_data(const char* buf, int len);
    int close_socket();

private:
    int m_socket;
    struct sockaddr_in m_recvaddr;
};

#endif // LINUX_UDPSENDER_H
