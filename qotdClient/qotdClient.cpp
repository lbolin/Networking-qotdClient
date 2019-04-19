//LAUREN BOLIN

//qotd Client

//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <string>
using namespace std;
#pragma comment(lib,"ws2_32.lib")

const int MAXBUF = 128;
int UDP_recv(SOCKET, char*, int, char*, char*);
int UDP_send(SOCKET, char*, int, char*, char*);
int wait(SOCKET, int, int);
SOCKET connectsock(const char*, const char*, const char*);

int main()
{
	char server_name[81] = "";
	char message[MAXBUF] = "";
	char buf[MAXBUF] = "";
	char port[MAXBUF];

	cout << "QOTD Server?";
	cin.getline(server_name, 80);
	strcpy_s(port, "qotd");

	/* Must always initialize the Windows Socket TCP/IP libary */
	WORD VersionRequired = 0x0202;
	WSADATA wsaData;
	int err = WSAStartup(VersionRequired, &wsaData);
	if (err)
	{
		cout << "Unable to initialize Windows Socket library." << endl;
		return 0;
	}

	/*Call function to create a UDP socket */
	SOCKET s = connectsock("", "", "udp");
	if (s != INVALID_SOCKET)
	{
		cout << "Command to send? ";
		cin.getline(message, MAXBUF - 1);
		int msg_len = (int)strlen(message);

		while (_stricmp(message, "QUIT") != 0 && msg_len > 0) {
			int len = UDP_send(s, message, msg_len + 1, server_name, port);
			if (len > 0)
			{
				cout << endl << "Quote of the day:" << endl;
				len = wait(s, 10, 0);
				if (len > 0)
				{
					len = UDP_recv(s, buf, MAXBUF, server_name, port);
					while (len > 0)
					{
						for (int i = 0; i < len; i++)cout << buf[i];
						len = wait(s, 1, 0);
						if (len > 0)len = UDP_recv(s, buf, MAXBUF, server_name, port);
					}
				}
				cout << endl;
			}
			cout << "Command to send? ";
			cin.getline(message, MAXBUF - 1);
			msg_len = (int)strlen(message);
		}
		WSACleanup();
	}
	return 0;
}

