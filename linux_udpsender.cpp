#include "linux_udpsender.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <arpa/inet.h>


using namespace std;


Linux_UDPSender::Linux_UDPSender(const string& ip, unsigned short port, int type)
    : m_socket(0),
      m_ip(ip),
      m_port(port),
      m_type(type)
{
}

Linux_UDPSender::Linux_UDPSender(const char* ip, unsigned short port, int type)
    : m_socket(0),
      m_ip(ip),
      m_port(port),
      m_type(type)
{
}

Linux_UDPSender::~Linux_UDPSender()
{
    if (m_socket != 0) {
        close_socket();
    }
}

int Linux_UDPSender::init_socket()
{
    int ret = 0;

    if ((m_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) <= 0) {
        printf("[ERROR] Linux_UDPSender::init_socket() socket failed.\n");
        return -1;
    }

    if (m_type == MULTICAST) {
        int multicast_ttl = DEFAULT_TTL;
        if (setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&multicast_ttl, sizeof(multicast_ttl)) < 0) {
            printf("[ERROR] Linux_UDPSender::init_socket() setsockopt failed.\n");
            close(m_socket);
            return -1;
        }
    } else if (m_type == BROADCAST) {
        int broadcast_permission = 1;
        if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (void*)&broadcast_permission, sizeof(broadcast_permission)) < 0) {
            printf("[ERROR] Linux_UDPSender::init_socket() setsockopt failed.\n");
            close(m_socket);
            return -1;
        }
    }

    memset(&m_recvaddr, 0, sizeof(m_recvaddr));
    m_recvaddr.sin_family = AF_INET;
    m_recvaddr.sin_port = htons(m_port);
    if (((m_recvaddr.sin_addr.s_addr = inet_addr(m_ip.c_str())) == 0)) {
        printf("[ERROR] Linux_UDPSender::init_socket() invalid address.\n");
        return -1;
    }

    return ret;
}

int Linux_UDPSender::send_data(const char* buf, int len)
{
    int ret = 0;

    ret = sendto(m_socket, buf, len, 0, (const struct sockaddr *)&m_recvaddr, sizeof(m_recvaddr));
    if (ret < 0) {
        printf("[ERROR] Linux_UDPSender::send_data() sendto failed.\n");
    }

    return ret;
}

int Linux_UDPSender::close_socket()
{
    int ret = 0;
    ret = close(m_socket);
    if (ret < 0) {
        printf("[ERROR] Linux_UDPSender::close_socket() close failed.\n");
    }
    m_socket = 0;

    return ret;
}
