#include "pch.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <memory>

#include "win32_udpsender.h"
#include "win32_udpreceiver.h"

#define INPUT_BUFLEN	4096

using namespace std;

DWORD WINAPI UDPReceiverFunc(LPVOID lpParam)
{
	Win32_UDPReceiver* receiver = (Win32_UDPReceiver*)lpParam;

	receiver->wait_data();
	receiver->close_socket();

	return NULL;
}

int main(int argc, char* argv[])
{
	if (argc < 3) {
		printf("usage: udptest <ip> <port> <uni|multi|broad>\n");
		printf("e.g.,  udptest 192.168.10.128 18000\n");
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
		}
		else if (strType.compare("multi") == 0) {
			type = 2;
		}
		else if (strType.compare("broad") == 0) {
			type = 3;
		}
	}

	Win32_UDPSender* sender = new Win32_UDPSender();
	int ret = sender->init_socket(strIP, port, type);
	if (ret != 0) {
		return ret;
	}

	Win32_UDPReceiver* receiver = new Win32_UDPReceiver();
	ret = receiver->init_socket(strIP, port, type);
	if (ret != 0) {
		return ret;
	}

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
		}
		sender->send_data(strInput.c_str(), strInput.length());
		Sleep(0);
	}
	cout << "<< " << endl;

	sender->close_socket();

	return 0;
}

