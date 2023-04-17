#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
using namespace std;


int main()
{
	WSADATA wsaData;
	SOCKET ClientSocket = INVALID_SOCKET;
	SOCKET ListenSocked = INVALID_SOCKET;
	int result;
	ADDRINFOA hints;
	PADDRINFOA addrResult = NULL;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		cout << "wsastartup Faild, result =" << result << endl;
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	const char* sendbuffer = "vistrel!";
	char recvbuffer[512];
	result = getaddrinfo(NULL, "666", &hints, &addrResult);
	if (result != 0)
	{
		cout << "getadrinfo failed with error:" << result << endl;
		WSACleanup();
		return 1;
	}
	ListenSocked = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (ListenSocked == INVALID_SOCKET)
	{
		cout << "socket creation failed with error:" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	result = bind(ListenSocked, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		cout << "Binding socked server" << endl;
		closesocket(ListenSocked);
		ListenSocked = INVALID_SOCKET;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	result = listen(ListenSocked, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		cout << "listing socked fail" << endl;
		closesocket(ListenSocked);
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	ClientSocket = accept(ListenSocked, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "Accepting socked fail" << endl;
		closesocket(ListenSocked);
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	closesocket(ListenSocked);


	do
	{
		ZeroMemory(recvbuffer, 512);
		result = recv(ClientSocket, recvbuffer, 512, 0);
		if (result > 0)
		{
			cout << "Received " << result << "Bytes" << endl;
			cout << "Received Data: " << recvbuffer << endl;
			result = send(ClientSocket, sendbuffer, (int)strlen(sendbuffer), 0);
				if (result == SOCKET_ERROR)
				{
					cout << "failed to send data back " << endl;
					closesocket(ClientSocket);
					freeaddrinfo(addrResult);
					WSACleanup();
					return 1;
				}
		}
		else if (result == 0)

			cout << "connection closing" << endl;

		else
		{
			cout << "recv failed with Error" << endl;
			closesocket(ClientSocket);
			freeaddrinfo(addrResult);
			WSACleanup();
			return 1;
		}

	} while (result > 0);
	result = shutdown(ClientSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		cout << "shutdown client socket failed" << endl;
		closesocket(ClientSocket);
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	closesocket(ClientSocket);
	freeaddrinfo(addrResult);
	WSACleanup();
	return 0;

}