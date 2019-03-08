#ifndef WIN32_UDPSENDER_H
#define WIN32_UDPSENDER_H

#include <winsock2.h>
#include <string>

#define DEFAULT_BUFLEN      4096
#define DEFAULT_TTL         64


class Win32_UDPSender
{
public:
	enum UDP_TYPE {
		UNICAST = 1,
		MULTICAST,
		BROADCAST
	};

public:
	Win32_UDPSender();
	~Win32_UDPSender();

	int init_socket(const std::string& ip, unsigned short port, int type = UNICAST);
	int send_data(const char* buf, int len);
	int close_socket();

private:
	int m_socket;
	struct sockaddr_in m_recvaddr;
};

#endif // WIN32_UDPSENDER_H
