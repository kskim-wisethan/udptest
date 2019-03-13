#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <memory>

#include "linux_udpsender.h"
#include "linux_udpreceiver.h"
#include "sim_cmd.h"
#include "netAddrInfo.h"

#define INPUT_BUFLEN      4096
#define CMD_BUFLEN          64

using namespace std;

void createSimTestThreads();

void* UDPReceiverFunc(void* arg)
{
    Linux_UDPReceiver* receiver = (Linux_UDPReceiver*)arg;

    receiver->init_socket();
    receiver->wait_data();
    receiver->close_socket();

    return NULL;
}

int main(int argc, char* argv[])
{
    // args
    if (argc < 3) {
        printf("usage: udptest <ip> <port> <uni|multi|broad|sim>\n");
        printf("e.g.,  udptest 10.0.2.0 51914\n");
        return 1;
    }

    string strName = argv[0];
    string strIP = argv[1];
    int port = atoi(argv[2]);
    int type = -1;
    if (argc == 4) {
        string strType = argv[3];
        if (strType.compare("uni") == 0) {
            type = 1;
        } else if (strType.compare("multi") == 0) {
            type = 2;
        } else if (strType.compare("broad") == 0) {
            type = 3;
        } else if (strType.compare("sim") == 0) {
            type = 4;
        }
    }

    Linux_UDPSender* sender = new Linux_UDPSender(strIP, port, type);
    
    if (type == 4) {
        createSimTestThreads();

    } else {
        Linux_UDPReceiver* receiver = new Linux_UDPReceiver(strIP, port, type);
        pthread_t hUDPReceiverThread = 0;
        int thread_ret = pthread_create(&hUDPReceiverThread, NULL, &UDPReceiverFunc, (void*)receiver);
        if (thread_ret == 0) {
            printf("Receiver thread created..\n");
        } else {
            printf("[FAILED]: cannot create UDPReceiver thread!!\n");
            sender->close_socket();
            return 2;
        }
    }

    // input
    cout << ">> " << endl;
    string strInput = "";
    char buffer[INPUT_BUFLEN] = {0, };
    while (1) {
        char ch = 0;
        int i = 0;
        while (ch != '\n') {
            ch = getchar();
            buffer[i++] = ch;
        }
        buffer[i - 1] = '\0';
        strInput = string(buffer, i - 1);

        if (strInput.compare("exit") == 0) {
            break;
        } else if (strInput.compare("crank") == 0) {
            Sim_Cmd cmd04CA = Sim_Cmd(0x04CA, 8, 6, 2, 0x02);
            sender->send_data(cmd04CA.getCmd(), cmd04CA.getCmdLength());

            /*
            Sim_Cmd cmd0411 = Sim_Cmd(0x0411, 8, 22, 2, 0x01);
            cmd0411.setValue(28, 2, 0x01);
            cmd0411.setValue(30, 2, 0x01);
            sender->send_data(cmd0411.getCmd(), cmd0411.getCmdLength());
            */

        } else {
            sender->send_data(strInput.c_str(), strInput.length());
        }
        usleep(0);
    }
    cout << "<< " << endl;

    sender->close_socket();

    return 0;
}

void createSimTestThreads()
{
    Linux_UDPReceiver* receiver1 = new Linux_UDPReceiver(UDP_MULTI_IP1, UDP_MULTI_PORT, Linux_UDPReceiver::MULTICAST, "IP1");
    pthread_t hThread1 = 0;
    int thread_ret = pthread_create(&hThread1, NULL, &UDPReceiverFunc, (void*)receiver1);
    if (thread_ret == 0) {
        printf("Receiver thread1 created..\n");
    } else {
        printf("[FAILED]: cannot create UDPReceiver thread1\n");
    }

    Linux_UDPReceiver* receiver2 = new Linux_UDPReceiver(UDP_MULTI_IP2, UDP_MULTI_PORT, Linux_UDPReceiver::MULTICAST, "IP2");
    pthread_t hThread2 = 0;
    thread_ret = pthread_create(&hThread2, NULL, &UDPReceiverFunc, (void*)receiver2);
    if (thread_ret == 0) {
        printf("Receiver thread2 created..\n");
    } else {
        printf("[FAILED]: cannot create UDPReceiver thread2\n");
    }

    Linux_UDPReceiver* receiver3 = new Linux_UDPReceiver(UDP_MULTI_IP3, UDP_MULTI_PORT, Linux_UDPReceiver::MULTICAST, "IP3");
    pthread_t hThread3 = 0;
    thread_ret = pthread_create(&hThread3, NULL, &UDPReceiverFunc, (void*)receiver3);
    if (thread_ret == 0) {
        printf("Receiver thread3 created..\n");
    } else {
        printf("[FAILED]: cannot create UDPReceiver thread3\n");
    }

    Linux_UDPReceiver* receiver4 = new Linux_UDPReceiver(UDP_MULTI_IP4, UDP_MULTI_PORT, Linux_UDPReceiver::MULTICAST, "IP4");
    pthread_t hThread4 = 0;
    thread_ret = pthread_create(&hThread4, NULL, &UDPReceiverFunc, (void*)receiver4);
    if (thread_ret == 0) {
        printf("Receiver thread4 created..\n");
    } else {
        printf("[FAILED]: cannot create UDPReceiver thread4\n");
    }

    Linux_UDPReceiver* receiver5 = new Linux_UDPReceiver(UDP_MULTI_IP4, UDP_MULTI_PORT, Linux_UDPReceiver::MULTICAST, "IP5");
    pthread_t hThread5 = 0;
    thread_ret = pthread_create(&hThread5, NULL, &UDPReceiverFunc, (void*)receiver5);
    if (thread_ret == 0) {
        printf("Receiver thread5 created..\n");
    } else {
        printf("[FAILED]: cannot create UDPReceiver thread5\n");
    }

    Linux_UDPReceiver* receiver6 = new Linux_UDPReceiver(UDP_MULTI_IP6, UDP_MULTI_PORT, Linux_UDPReceiver::MULTICAST, "IP6");
    pthread_t hThread6 = 0;
    thread_ret = pthread_create(&hThread6, NULL, &UDPReceiverFunc, (void*)receiver6);
    if (thread_ret == 0) {
        printf("Receiver thread6 created..\n");
    } else {
        printf("[FAILED]: cannot create UDPReceiver thread6\n");
    }

    Linux_UDPReceiver* receiver7 = new Linux_UDPReceiver(UDP_MULTI_IP7, UDP_MULTI_PORT, Linux_UDPReceiver::MULTICAST, "IP7");
    pthread_t hThread7 = 0;
    thread_ret = pthread_create(&hThread7, NULL, &UDPReceiverFunc, (void*)receiver7);
    if (thread_ret == 0) {
        printf("Receiver thread7 created..\n");
    } else {
        printf("[FAILED]: cannot create UDPReceiver thread7\n");
    }

    Linux_UDPReceiver* receiver8 = new Linux_UDPReceiver(UDP_MULTI_IP8, UDP_MULTI_PORT, Linux_UDPReceiver::MULTICAST, "IP8");
    pthread_t hThread8 = 0;
    thread_ret = pthread_create(&hThread8, NULL, &UDPReceiverFunc, (void*)receiver8);
    if (thread_ret == 0) {
        printf("Receiver thread8 created..\n");
    } else {
        printf("[FAILED]: cannot create UDPReceiver thread8\n");
    }

    Linux_UDPReceiver* receiver9 = new Linux_UDPReceiver(UDP_MULTI_IP9, UDP_MULTI_PORT, Linux_UDPReceiver::MULTICAST, "IP9");
    pthread_t hThread9 = 0;
    thread_ret = pthread_create(&hThread9, NULL, &UDPReceiverFunc, (void*)receiver9);
    if (thread_ret == 0) {
        printf("Receiver thread9 created..\n");
    } else {
        printf("[FAILED]: cannot create UDPReceiver thread9\n");
    }

    Linux_UDPReceiver* receiver10 = new Linux_UDPReceiver(UDP_MULTI_IP10, UDP_MULTI_PORT, Linux_UDPReceiver::MULTICAST, "IP10");
    pthread_t hThread10 = 0;
    thread_ret = pthread_create(&hThread10, NULL, &UDPReceiverFunc, (void*)receiver10);
    if (thread_ret == 0) {
        printf("Receiver thread10 created..\n");
    } else {
        printf("[FAILED]: cannot create UDPReceiver thread10\n");
    }

    Linux_UDPReceiver* receiver11 = new Linux_UDPReceiver(UDP_MULTI_IP11, UDP_MULTI_PORT, Linux_UDPReceiver::MULTICAST, "IP11");
    pthread_t hThread11 = 0;
    thread_ret = pthread_create(&hThread11, NULL, &UDPReceiverFunc, (void*)receiver11);
    if (thread_ret == 0) {
        printf("Receiver thread11 created..\n");
    } else {
        printf("[FAILED]: cannot create UDPReceiver thread11\n");
    }

    Linux_UDPReceiver* receiver12 = new Linux_UDPReceiver(UDP_MULTI_IP12, UDP_MULTI_PORT, Linux_UDPReceiver::MULTICAST, "IP12");
    pthread_t hThread12 = 0;
    thread_ret = pthread_create(&hThread3, NULL, &UDPReceiverFunc, (void*)receiver12);
    if (thread_ret == 0) {
        printf("Receiver thread12 created..\n");
    } else {
        printf("[FAILED]: cannot create UDPReceiver thread12\n");
    }

    Linux_UDPReceiver* receiver13 = new Linux_UDPReceiver(UDP_MULTI_IP13, UDP_MULTI_PORT, Linux_UDPReceiver::MULTICAST, "IP13");
    pthread_t hThread13 = 0;
    thread_ret = pthread_create(&hThread13, NULL, &UDPReceiverFunc, (void*)receiver13);
    if (thread_ret == 0) {
        printf("Receiver thread13 created..\n");
    } else {
        printf("[FAILED]: cannot create UDPReceiver thread13\n");
    }

    Linux_UDPReceiver* receiver14 = new Linux_UDPReceiver(UDP_MULTI_IP14, UDP_MULTI_PORT, Linux_UDPReceiver::MULTICAST, "IP14");
    pthread_t hThread14 = 0;
    thread_ret = pthread_create(&hThread14, NULL, &UDPReceiverFunc, (void*)receiver14);
    if (thread_ret == 0) {
        printf("Receiver thread14 created..\n");
    } else {
        printf("[FAILED]: cannot create UDPReceiver thread14\n");
    }
}