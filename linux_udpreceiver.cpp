#include "linux_udpreceiver.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


Linux_UDPReceiver::Linux_UDPReceiver()
    : m_waiting_flag(false),
      m_readlen(0),
      m_socket(0)
{
}

Linux_UDPReceiver::~Linux_UDPReceiver()
{
    if (m_socket != 0) {
        int ret = close(m_socket);
        if (ret < 0) {
            printf("[ERROR] Linux_UDPReceiver::~Linux_UDPReceiver() close failed.\n");
        }
        m_socket = 0;
    }
}

int Linux_UDPReceiver::init_socket(const std::string& ip, unsigned short port, int type)
{
    int ret = 0;

    if ((m_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) <= 0) {
        printf("[ERROR] Linux_UDPReceiver::init_socket() rt_dev_socket failed.\n");
        return -1;
    }

    if (type == MULTICAST) {
        int multicast_permission = 1;
        if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (void*)&multicast_permission, sizeof(multicast_permission)) < 0) {
            printf("[ERROR] Linux_UDPReceiver::init_socket() setsockopt 1 failed.\n");
            close(m_socket);
            return -1;
        }

    } else if (type == BROADCAST) {
        int broadcast_permission = 1;
        if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (void*)&broadcast_permission, sizeof(broadcast_permission)) < 0) {
            printf("[ERROR] Linux_UDPReceiver::init_socket() setsockopt 1 failed.\n");
            close(m_socket);
            return -1;
        }
    }

    struct sockaddr_in recvaddr;
    memset(&recvaddr, 0, sizeof(recvaddr));
    recvaddr.sin_family = AF_INET;
    recvaddr.sin_port = htons(port);
    recvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(m_socket, (struct sockaddr*)&recvaddr, sizeof(recvaddr)) < 0) {
        printf("[ERROR] Linux_UDPReceiver::init_socket() bind failed.\n");
        close(m_socket);
        return -1;
    }

    if (type == MULTICAST) {
        struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = inet_addr(ip.c_str());
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&mreq, sizeof(mreq)) < 0) {
            printf("[ERROR] Linux_UDPReceiver::init_socket() setsockopt 3 failed.\n");
            close(m_socket);
            return -1;
        }
    }

    return ret;
}

int Linux_UDPReceiver::wait_data()
{
    int ret = 0;
    m_waiting_flag = true;

    char recvbuf[DEFAULT_BUFLEN] = {0, };
    int buflen = DEFAULT_BUFLEN;

    while (m_waiting_flag) {
        ret = recvfrom(m_socket, recvbuf, buflen, 0, NULL, NULL);
        if (ret > 0) {
            m_readlen = ret;
            recvbuf[m_readlen] = '\0';
            printf("received data: %s (%d)\n", recvbuf, m_readlen);

            //m_icd_parser->init(recvbuf, ret);
            //unsigned int opcode = m_icd_parser->getOpcode();

            //emit readReady(opcode);
        }
    }

    return ret;
}

int Linux_UDPReceiver::close_socket()
{
    int ret = 0;
    ret = close(m_socket);
    if (ret < 0) {
        printf("[ERROR] Linux_UDPReceiver::close_socket() close failed.\n");
    }
    m_socket = 0;

    return ret;
}

void Linux_UDPReceiver::run()
{
    int ret = wait_data();
}
