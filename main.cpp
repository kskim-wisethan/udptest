#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <memory>

#include "linux_udpsender.h"
#include "linux_udpreceiver.h"
#include "netAddrInfo.h"

#define INPUT_BUFLEN      4096

using namespace std;

void* UDPReceiverFunc(void* arg)
{
    char** argv = (char**)arg;

    string strIP = argv[1];
    int port = atoi(argv[2]);

    Linux_UDPReceiver* receiver = new Linux_UDPReceiver();
    int ret = receiver->init_socket(strIP, port);
    if (ret != 0) {
        return NULL;
    }

    receiver->wait_data();
    receiver->close_socket();

    return NULL;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("usage: udptest <ip> <port>\n");
        printf("e.g.,  udptest 192.168.10.128 18000\n");
        return 1;
    }

    string strName = argv[0];
    string strIP = argv[1];
    int port = atoi(argv[2]);

    Linux_UDPSender* sender = new Linux_UDPSender();

    int ret = sender->init_socket(strIP, port);
    if (ret != 0) {
        return ret;
    }
    
    pthread_t hUDPReceiverThread = 0;
    int thread_ret = pthread_create(&hUDPReceiverThread, NULL, &UDPReceiverFunc, (void*)argv);
    if (thread_ret == 0) {
        printf("Receiver thread created..\n");
    } else {
        printf("[ERROR]: cannot create UDPReceiver thread!!\n");
        sender->close_socket();
        return 2;
    }

    string strInput = "";
    while (1) {
        cout << "> ";
        cin >> strInput;

        if (strInput.compare("exit") == 0) {
            break;
        }
        sender->send_data(strInput.c_str(), strInput.length());
        usleep(0);
    }

    sender->close_socket();

    return 0;
}