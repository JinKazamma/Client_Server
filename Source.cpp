#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
using namespace std;


int main()
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
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
	const char* sendbuffer = "est probitie!";
	char recvbuffer[512];
	result = getaddrinfo("localhost", "666", &hints, &addrResult);
	if (result != 0)
	{
		cout << "getadrinfo failed with error:" << result << endl;
		WSACleanup();
		return 1;
	}
	ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET)
	{
		cout << "socket creation failed with error:"<< endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	result = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		cout << "Unable connect server" << endl;
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	result = send(ConnectSocket, sendbuffer, strlen(sendbuffer), 0);
	if (result == SOCKET_ERROR)
	{
		cout << " send failed" << result << endl;
		closesocket(ConnectSocket);
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	cout << "Bytes sent" << result << "bytes" << endl;
	result = shutdown(ConnectSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		cout << " shutdown error" << result << endl;
		closesocket(ConnectSocket);
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	ZeroMemory(recvbuffer, 512);
	
	do
	{
		result = recv(ConnectSocket, recvbuffer, 512, 0);
		if (result > 0)
		{
			cout << "Received " << result << "Bytes" << endl;
			cout << "Received Data: " << recvbuffer << endl;
		}
		else if (result == 0)

			cout << "connection closed" << endl;

		else
			cout << "recv failed with Error" << endl;
	} while (result >0);
	closesocket(ConnectSocket);
	freeaddrinfo(addrResult);
	WSACleanup();
	return 0;

}