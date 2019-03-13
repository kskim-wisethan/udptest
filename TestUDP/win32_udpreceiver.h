#ifndef WIN32_UDPRECEIVER_H
#define WIN32_UDPRECEIVER_H

#include <winsock2.h>
#include <memory>
#include <string>

#define DEFAULT_BUFLEN      4096


class Win32_UDPReceiver
{
public:
	enum UDP_TYPE {
		UNICAST = 1,
		MULTICAST,      // class D (224.0.0.0 ~ 239.255.255.255)
		BROADCAST
	};

public:
	Win32_UDPReceiver();
	~Win32_UDPReceiver();

	int init_socket(const std::string& ip, unsigned short port, int type = UNICAST);
	int wait_data();
	int close_socket();
	bool isWaiting() { return m_waiting_flag; }
	void stop_waiting() { m_waiting_flag = false; }

	int getSize() { return m_readlen; }

private:
	void run();
	int getErrorMessage(int code, char* msgbuf, int bufsize);

private:
	bool m_waiting_flag;
	int m_socket;
	int m_readlen;
	int m_type;
	struct sockaddr_in m_recvaddr;
};

#endif // WIN32_UDPRECEIVER_H

