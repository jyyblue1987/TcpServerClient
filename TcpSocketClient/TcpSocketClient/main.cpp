#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib,"ws2_32.lib")
using namespace std;

bool bclose = false;
SOCKET sock;






void main()
{
	cout << "\t\t------- TCP Client ----------" << endl;
	cout << endl;

	string ipAddress = "127.0.0.1";
	int port = 54000;
	//initialze winsock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);

	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "can't start winsock! Err#" << wsResult<<endl;
		return;
	}
	//create the sock
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "can't create a socket! Err#" <<WSAGetLastError()<< endl;
		WSACleanup();
		return;
	}
	//Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "can't connect to server , Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}
	 
	//Sever Connected 

	unsigned int myCounter = 0;
	
	
	
	char buf[4096];
	while (!bclose)
	{
		ZeroMemory(buf, 4096);
		bool bsend = true;
		//wait for client to send data
		int bytesReceived = recv(sock, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;
			break;
		}
		if (bytesReceived == 0)
		{
			cerr << "client disconnected " << endl;
			break;
		}

		//cout << "Msg:" << string(buf, 0, bytesReceived) << "\r\n";
		Sleep(10);
	}

	//Gracefully close down everything
	
}