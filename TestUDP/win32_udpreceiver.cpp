#include "pch.h"
#include "win32_udpreceiver.h"

#include <ws2tcpip.h>


Win32_UDPReceiver::Win32_UDPReceiver()
	: m_waiting_flag(false),
	m_readlen(0),
	m_socket(INVALID_SOCKET),
	m_type(0)
{
}

Win32_UDPReceiver::~Win32_UDPReceiver()
{
	if (m_socket != INVALID_SOCKET) {
		close_socket();
	}
}

int Win32_UDPReceiver::init_socket(const std::string& ip, unsigned short port, int type)
{
	int ret = 0;
	m_type = type;

	WSADATA wsadata;
	ret = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (ret != NO_ERROR) {
		printf("[ERROR] WSAStartup failed with error: %d\n", ret);
		return -1;
	}

	if ((m_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) <= 0) {
		printf("[ERROR:%d] Win32_UDPReceiver::init_socket() socket failed.\n", WSAGetLastError());
		return -1;
	}

	if (type == MULTICAST) {
		int reuse_permission = 1;
		if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse_permission, sizeof(reuse_permission)) < 0) {
			printf("[ERROR:%d] Win32_UDPReceiver::init_socket() setsockopt 1 failed.\n", WSAGetLastError());
			close_socket();
			return -1;
		}
	}

	if (type == BROADCAST) {
		int broadcast_permission = 1;
		if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast_permission, sizeof(broadcast_permission)) < 0) {
			printf("[ERROR:%d] Win32_UDPReceiver::init_socket() setsockopt 2 failed.\n", WSAGetLastError());
			close_socket();
			return -1;
		}
	}

	memset(&m_recvaddr, 0, sizeof(m_recvaddr));
	m_recvaddr.sin_family = AF_INET;
	m_recvaddr.sin_port = htons(port);
	m_recvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(m_socket, (struct sockaddr*)&m_recvaddr, sizeof(m_recvaddr)) < 0) {
		char msgbuf[DEFAULT_BUFLEN] = { 0, };
		if (getErrorMessage(WSAGetLastError(), msgbuf, DEFAULT_BUFLEN) > 0) {
			printf("[ERROR:%d] Win32_UDPReceiver::init_socket() bind failed.\n%s", WSAGetLastError(), msgbuf);
		}
		close_socket();
		return -1;
	}

	if (type == MULTICAST) {
		struct ip_mreq mreq;
		in_addr addr;
		if (inet_pton(AF_INET, ip.c_str(), &addr) != 1) {
			printf("[ERROR:%d] Win32_UDPReceiver::init_socket() invalid address.\n", WSAGetLastError());
			close_socket();
			return -1;
		}
		mreq.imr_multiaddr = addr;
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);
		if (setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) < 0) {
			printf("[ERROR:%d] Win32_UDPReceiver::init_socket() setsockopt 3 failed.\n", WSAGetLastError());
			close_socket();
			return -1;
		}
	}

	return ret;
}

int Win32_UDPReceiver::wait_data()
{
	int ret = 0;
	m_waiting_flag = true;

	char recvbuf[DEFAULT_BUFLEN] = { 0, };
	int buflen = DEFAULT_BUFLEN;

	while (m_waiting_flag) {
		int addrlen = sizeof(m_recvaddr);
		ret = recvfrom(m_socket, recvbuf, buflen, 0, (struct sockaddr*)&m_recvaddr, (socklen_t*)&addrlen);

		if (ret > 0) {
			m_readlen = ret;
			recvbuf[m_readlen] = '\0';
			char addrbuf[DEFAULT_BUFLEN] = { 0, };
			inet_ntop(AF_INET, &m_recvaddr.sin_addr, addrbuf, DEFAULT_BUFLEN);
			printf("[%s:%d]: %s (%d)\n", addrbuf, ntohs(m_recvaddr.sin_port), recvbuf, m_readlen);

			for (int i = 0; i < m_readlen; i++) {
				if ((i) % 8 == 0 && i != 0) {
					printf("\n");
				}
				printf("0x%02X(%d) ", recvbuf[i], recvbuf[i]);
			}
			printf("\n");
		}
	}

	return ret;
}

int Win32_UDPReceiver::close_socket()
{
	int ret = closesocket(m_socket);
	if (ret != 0) {
		printf("[ERROR:%d] Win32_UDPReceiver::close_socket() close failed.\n", WSAGetLastError());
	}
	m_socket = INVALID_SOCKET;
	WSACleanup();

	return ret;
}

void Win32_UDPReceiver::run()
{
	int ret = wait_data();
}

int Win32_UDPReceiver::getErrorMessage(int code, char* msgbuf, int bufsize)
{
	int ret = 0;
	ret = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		msgbuf,
		bufsize,
		NULL);
	return ret;
}
