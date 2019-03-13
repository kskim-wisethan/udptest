#include "pch.h"
#include "win32_udpsender.h"

#include <ws2tcpip.h>

using namespace std;


Win32_UDPSender::Win32_UDPSender(const string& ip, unsigned short port, int type)
	: m_socket(INVALID_SOCKET),
	  m_ip(ip),
	  m_port(port),
	  m_type(type)
{
}

Win32_UDPSender::Win32_UDPSender(const char* ip, unsigned short port, int type)
	: m_socket(INVALID_SOCKET),
	m_ip(ip),
	m_port(port),
	m_type(type)
{
}

Win32_UDPSender::~Win32_UDPSender()
{
	if (m_socket != INVALID_SOCKET) {
		close_socket();
	}
}

int Win32_UDPSender::init_socket()
{
	int ret = 0;

	WSADATA wsadata;
	ret = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (ret != NO_ERROR) {
		printf("[ERROR:%d] WSAStartup failed.\n", ret);
		return -1;
	}

	if ((m_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) <= 0) {
		printf("[ERROR:%d] Win32_UDPSender::init_socket() socket failed.\n", WSAGetLastError());
		return -1;
	}

	if (m_type == MULTICAST) {
		int multicast_ttl = DEFAULT_TTL;
		if (setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&multicast_ttl, sizeof(multicast_ttl)) < 0) {
			printf("[ERROR:%d] Win32_UDPSender::init_socket() setsockopt failed.\n", WSAGetLastError());
			close_socket();
			return -1;
		}
	}
	else if (m_type == BROADCAST) {
		int broadcast_permission = 1;
		if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast_permission, sizeof(broadcast_permission)) < 0) {
			printf("[ERROR:%d] Win32_UDPSender::init_socket() setsockopt failed.\n", WSAGetLastError());
			close_socket();
			return -1;
		}
	}

	memset(&m_recvaddr, 0, sizeof(m_recvaddr));
	m_recvaddr.sin_family = AF_INET;
	m_recvaddr.sin_port = htons(m_port);
	in_addr addr;
	if (inet_pton(AF_INET, m_ip.c_str(), &addr) != 1) {
		printf("[ERROR:%d] Win32_UDPSender::init_socket() invalid address.\n", WSAGetLastError());
		close_socket();
		return -1;
	}
	m_recvaddr.sin_addr = addr;

	return ret;
}

int Win32_UDPSender::send_data(const char* buf, int len)
{
	int ret = 0;

	ret = sendto(m_socket, buf, len, 0, (const struct sockaddr *)&m_recvaddr, sizeof(m_recvaddr));
	if (ret < 0) {
		printf("[ERROR:%d] Win32_UDPSender::send_data() sendto failed.\n", WSAGetLastError());
	}

	return ret;
}

int Win32_UDPSender::close_socket()
{
	int ret = 0;
	ret = closesocket(m_socket);
	if (ret != 0) {
		printf("[ERROR:%d] Win32_UDPSender::close_socket() close failed.\n", WSAGetLastError());
	}
	m_socket = INVALID_SOCKET;
	WSACleanup();

	return ret;
}
