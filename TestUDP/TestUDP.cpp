#include "pch.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <memory>

#include "win32_udpsender.h"
#include "win32_udpreceiver.h"
#include "sim_cmd.h"
#include "netAddrInfo.h"

#define INPUT_BUFLEN	4096
#define CMD_BUFLEN		64

using namespace std;

void createSimTestThreads();

DWORD WINAPI UDPReceiverFunc(LPVOID lpParam)
{
	Win32_UDPReceiver* receiver = (Win32_UDPReceiver*)lpParam;

	receiver->init_socket();
	receiver->wait_data();
	receiver->close_socket();

	return NULL;
}

int main(int argc, char* argv[])
{
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

	Win32_UDPSender* sender = new Win32_UDPSender(strIP, port, type);
	int ret = sender->init_socket();
	if (ret != 0) {
		return ret;
	}

	if (type == 4) {
		createSimTestThreads();

	} else {
		Win32_UDPReceiver* receiver = new Win32_UDPReceiver(strIP, port, type);
		DWORD dwThreadId;
		HANDLE hUDPReceiverThread = NULL;
		hUDPReceiverThread = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver, 0, &dwThreadId);
		if (hUDPReceiverThread != NULL) {
			printf("Receiver thread created..\n");
		}
		else {
			printf("[ERROR]: cannot create UDPReceiver thread!!\n");
			sender->close_socket();
			return 2;
		}
	}

	cout << ">> " << endl;
	string strInput = "";
	char buffer[INPUT_BUFLEN] = { 0, };
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
		} else {
			sender->send_data(strInput.c_str(), strInput.length());
		}

		Sleep(0);
	}
	cout << "<< " << endl;

	sender->close_socket();

	return 0;
}

void createSimTestThreads()
{
	Win32_UDPReceiver* receiver1 = new Win32_UDPReceiver(UDP_MULTI_IP1, UDP_MULTI_PORT, Win32_UDPReceiver::MULTICAST);
	DWORD dwThreadId1;
	HANDLE hThread1 = NULL;
	hThread1 = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver1, 0, &dwThreadId1);
	if (hThread1 != NULL) {
		printf("Receiver thread1 created..\n");
	}
	else {
		printf("[FAILED]: cannot create UDPReceiver thread1\n");
	}

	Win32_UDPReceiver* receiver2 = new Win32_UDPReceiver(UDP_MULTI_IP2, UDP_MULTI_PORT, Win32_UDPReceiver::MULTICAST);
	DWORD dwThreadId2;
	HANDLE hThread2 = NULL;
	hThread2 = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver2, 0, &dwThreadId2);
	if (hThread2 != NULL) {
		printf("Receiver thread2 created..\n");
	}
	else {
		printf("[FAILED]: cannot create UDPReceiver thread2\n");
	}

	Win32_UDPReceiver* receiver3 = new Win32_UDPReceiver(UDP_MULTI_IP3, UDP_MULTI_PORT, Win32_UDPReceiver::MULTICAST);
	DWORD dwThreadId3;
	HANDLE hThread3 = NULL;
	hThread3 = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver3, 0, &dwThreadId3);
	if (hThread3 != NULL) {
		printf("Receiver thread3 created..\n");
	}
	else {
		printf("[FAILED]: cannot create UDPReceiver thread3\n");
	}

	Win32_UDPReceiver* receiver4 = new Win32_UDPReceiver(UDP_MULTI_IP4, UDP_MULTI_PORT, Win32_UDPReceiver::MULTICAST);
	DWORD dwThreadId4;
	HANDLE hThread4 = NULL;
	hThread4 = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver4, 0, &dwThreadId4);
	if (hThread4 != NULL) {
		printf("Receiver thread4 created..\n");
	}
	else {
		printf("[FAILED]: cannot create UDPReceiver thread4\n");
	}

	Win32_UDPReceiver* receiver5 = new Win32_UDPReceiver(UDP_MULTI_IP5, UDP_MULTI_PORT, Win32_UDPReceiver::MULTICAST);
	DWORD dwThreadId5;
	HANDLE hThread5 = NULL;
	hThread5 = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver5, 0, &dwThreadId5);
	if (hThread5 != NULL) {
		printf("Receiver thread5 created..\n");
	}
	else {
		printf("[FAILED]: cannot create UDPReceiver thread5\n");
	}

	Win32_UDPReceiver* receiver6 = new Win32_UDPReceiver(UDP_MULTI_IP6, UDP_MULTI_PORT, Win32_UDPReceiver::MULTICAST);
	DWORD dwThreadId6;
	HANDLE hThread6 = NULL;
	hThread6 = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver6, 0, &dwThreadId6);
	if (hThread6 != NULL) {
		printf("Receiver thread6 created..\n");
	}
	else {
		printf("[FAILED]: cannot create UDPReceiver thread6\n");
	}

	Win32_UDPReceiver* receiver7 = new Win32_UDPReceiver(UDP_MULTI_IP7, UDP_MULTI_PORT, Win32_UDPReceiver::MULTICAST);
	DWORD dwThreadId7;
	HANDLE hThread7 = NULL;
	hThread7 = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver7, 0, &dwThreadId7);
	if (hThread7 != NULL) {
		printf("Receiver thread7 created..\n");
	}
	else {
		printf("[FAILED]: cannot create UDPReceiver thread7\n");
	}

	Win32_UDPReceiver* receiver8 = new Win32_UDPReceiver(UDP_MULTI_IP8, UDP_MULTI_PORT, Win32_UDPReceiver::MULTICAST);
	DWORD dwThreadId8;
	HANDLE hThread8 = NULL;
	hThread8 = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver8, 0, &dwThreadId8);
	if (hThread8 != NULL) {
		printf("Receiver thread8 created..\n");
	}
	else {
		printf("[FAILED]: cannot create UDPReceiver thread8\n");
	}

	Win32_UDPReceiver* receiver9 = new Win32_UDPReceiver(UDP_MULTI_IP9, UDP_MULTI_PORT, Win32_UDPReceiver::MULTICAST);
	DWORD dwThreadId9;
	HANDLE hThread9 = NULL;
	hThread9 = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver9, 0, &dwThreadId9);
	if (hThread9 != NULL) {
		printf("Receiver thread9 created..\n");
	}
	else {
		printf("[FAILED]: cannot create UDPReceiver thread9\n");
	}

	Win32_UDPReceiver* receiver10 = new Win32_UDPReceiver(UDP_MULTI_IP10, UDP_MULTI_PORT, Win32_UDPReceiver::MULTICAST);
	DWORD dwThreadId10;
	HANDLE hThread10 = NULL;
	hThread10 = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver10, 0, &dwThreadId10);
	if (hThread10 != NULL) {
		printf("Receiver thread10 created..\n");
	}
	else {
		printf("[FAILED]: cannot create UDPReceiver thread10\n");
	}

	Win32_UDPReceiver* receiver11 = new Win32_UDPReceiver(UDP_MULTI_IP11, UDP_MULTI_PORT, Win32_UDPReceiver::MULTICAST);
	DWORD dwThreadId11;
	HANDLE hThread11 = NULL;
	hThread11 = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver11, 0, &dwThreadId11);
	if (hThread11 != NULL) {
		printf("Receiver thread11 created..\n");
	}
	else {
		printf("[FAILED]: cannot create UDPReceiver thread11\n");
	}

	Win32_UDPReceiver* receiver12 = new Win32_UDPReceiver(UDP_MULTI_IP12, UDP_MULTI_PORT, Win32_UDPReceiver::MULTICAST);
	DWORD dwThreadId12;
	HANDLE hThread12 = NULL;
	hThread12 = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver12, 0, &dwThreadId12);
	if (hThread12 != NULL) {
		printf("Receiver thread12 created..\n");
	}
	else {
		printf("[FAILED]: cannot create UDPReceiver thread12\n");
	}

	Win32_UDPReceiver* receiver13 = new Win32_UDPReceiver(UDP_MULTI_IP13, UDP_MULTI_PORT, Win32_UDPReceiver::MULTICAST);
	DWORD dwThreadId13;
	HANDLE hThread13 = NULL;
	hThread13 = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver13, 0, &dwThreadId13);
	if (hThread13 != NULL) {
		printf("Receiver thread13 created..\n");
	}
	else {
		printf("[FAILED]: cannot create UDPReceiver thread13\n");
	}

	Win32_UDPReceiver* receiver14 = new Win32_UDPReceiver(UDP_MULTI_IP14, UDP_MULTI_PORT, Win32_UDPReceiver::MULTICAST);
	DWORD dwThreadId14;
	HANDLE hThread14 = NULL;
	hThread14 = CreateThread(NULL, 0, UDPReceiverFunc, (void*)receiver14, 0, &dwThreadId14);
	if (hThread14 != NULL) {
		printf("Receiver thread14 created..\n");
	}
	else {
		printf("[FAILED]: cannot create UDPReceiver thread14\n");
	}
}
