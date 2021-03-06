#include "linux_udpreceiver.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <bitset>

#include "sim_cmd.h"

using namespace std;

Linux_UDPReceiver::Linux_UDPReceiver(const string& ip, unsigned short port, int type, const string& name)
    : m_waiting_flag(false),
      m_readlen(0),
      m_socket(0),
      m_ip(ip),
      m_port(port),
      m_type(type),
      m_name(name)
{
}

Linux_UDPReceiver::Linux_UDPReceiver(const char* ip, unsigned short port, int type, const string& name)
    : m_waiting_flag(false),
      m_readlen(0),
      m_socket(0),
      m_ip(ip),
      m_port(port),
      m_type(type),
      m_name(name)
{
}

Linux_UDPReceiver::~Linux_UDPReceiver()
{
    if (m_socket != 0) {
        close_socket();
    }
}

int Linux_UDPReceiver::init_socket()
{
    int ret = 0;

    if ((m_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) <= 0) {
        printf("[ERROR] Linux_UDPReceiver::init_socket() socket failed.\n");
        return -1;
    }

    if (m_type == MULTICAST) {
        int multicast_permission = 1;
        if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (void*)&multicast_permission, sizeof(multicast_permission)) < 0) {
            printf("[ERROR] Linux_UDPReceiver::init_socket() setsockopt 1 failed.\n");
            close(m_socket);
            return -1;
        }

    } else if (m_type == BROADCAST) {
        int broadcast_permission = 1;
        if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (void*)&broadcast_permission, sizeof(broadcast_permission)) < 0) {
            printf("[ERROR] Linux_UDPReceiver::init_socket() setsockopt 2 failed.\n");
            close(m_socket);
            return -1;
        }
    }

    memset(&m_recvaddr, 0, sizeof(m_recvaddr));
    m_recvaddr.sin_family = AF_INET;
    m_recvaddr.sin_port = htons(m_port);
    m_recvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(m_socket, (struct sockaddr*)&m_recvaddr, sizeof(m_recvaddr)) < 0) {
        printf("[ERROR] Linux_UDPReceiver::init_socket() bind failed.\n");
        close(m_socket);
        return -1;
    }

    if (m_type == MULTICAST) {
        struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = inet_addr(m_ip.c_str()); 
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
        int addrlen = sizeof(m_recvaddr);
        ret = recvfrom(m_socket, recvbuf, buflen, 0, (struct sockaddr*)&m_recvaddr, (socklen_t*)&addrlen);

        if (ret > 0) {
            m_readlen = ret;
            recvbuf[m_readlen] = '\0';
            Sim_Cmd cmdRecv = Sim_Cmd(recvbuf, ret);
            //printf("[%s][0x%X][%s:%d]: %s (%d)\n", m_name.c_str(), cmdRecv.getId(), inet_ntoa(m_recvaddr.sin_addr), ntohs(m_recvaddr.sin_port), recvbuf, m_readlen);
            if (cmdRecv.getId() == 0x0411) {
                printf("[%s:%d][0x%X][%d] \n", m_ip.c_str(), m_port, cmdRecv.getId(), m_readlen);
                for (int i = 0; i < m_readlen; i++) {
                    if ((i) % 8 == 0 && i != 0) {
                        printf("\n");
                    }
                    printf("0x%02X(%d) ", (unsigned char)recvbuf[i], recvbuf[i]);
                }
                printf("\n");

                for (int i = 0; i < m_readlen; i++) {
                    if ((i) % 8 == 0 && i != 0) {
                        printf("\n");
                    }
                    bitset<8> bits((unsigned char)recvbuf[i]);
                    printf("%s ", bits.to_string().c_str());
                }
                printf("\n");
            }
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
