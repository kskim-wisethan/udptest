#ifndef LINUX_UDPRECEIVER_H
#define LINUX_UDPRECEIVER_H

#include <memory>

#define DEFAULT_BUFLEN      4096


class Linux_UDPReceiver
{
public:
    Linux_UDPReceiver();
    ~Linux_UDPReceiver();

    int init_socket(unsigned short port, bool broadcast = true);
    int wait_data();
    int close_socket();
    bool isWaiting() { return m_waiting_flag; }
    void stop_waiting() { m_waiting_flag = false; }

    size_t getSize() { return m_readlen; }

private:
    void run();

private:
    bool m_waiting_flag;
    int m_socket;
    int m_readlen;
};

#endif // LINUX_UDPRECEIVER_H
