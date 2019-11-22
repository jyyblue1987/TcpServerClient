#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>  

#pragma comment (lib,"ws2_32.lib")
using namespace std;


fd_set master;
char buf[1000];

void onConnected(SOCKET newsoc) {
	
	char buf1[INET_ADDRSTRLEN] = "";
	struct  sockaddr_in client;
	int clientsize = sizeof(client);
	string ip = "";
	char host[NI_MAXHOST];
	char service[NI_MAXHOST];//service ()
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);
	if (getpeername(newsoc, (struct sockaddr *)&client, &clientsize) == 0) {
		inet_ntop(AF_INET, &client.sin_addr, buf1, sizeof buf1);
		ip = buf1;
	}
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		
		cout << "client with ip:"<<ip << "  " << "connect"<<"\r\n";
	}
}

void onDisConnected(SOCKET closesoc) {
	
	char buf1[INET_ADDRSTRLEN] = "";
	struct  sockaddr_in client;
	int clientsize = sizeof(client);
	string ip = "";
	char host[NI_MAXHOST];
	char service[NI_MAXHOST];//service ()
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);
	if (getpeername(closesoc, (struct sockaddr *)&client, &clientsize) == 0) {
		inet_ntop(AF_INET, &client.sin_addr, buf1, sizeof buf1);
		ip = buf1;
	}
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		
		cout << "client with ip :" << ip <<" "<<"disconnected"<< "\r\n";
	}
}

void main()
{
	cout << "\t\t------- TCP SERVER ----------" << endl;
	cout << endl;
	//initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	
	

	int wsok = WSAStartup(ver, &wsData);
	if (wsok != 0)
	{
		cerr << "can't Initailize winsock! Quitting" << endl;
		return;
	}
	//create the sock
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "can't create a socket! Qutiing" << endl;
		return;
	}
	//bind the socket to an ip adress and port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;//Clould also use inet_pton...
	bind(listening, (sockaddr*)&hint, sizeof(hint));
	//Tell winsock the socket is for listening 
	listen(listening, SOMAXCONN);
	FD_ZERO(&master);

	FD_SET(listening, &master);
	unsigned int myCounter = 0;
	DWORD myThreadID;
	
	while (true)
	{
		fd_set copy = master;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++)
		{
			SOCKET sock = copy.fd_array[i];
			if (sock == listening)
			{
				//accept a new connection
				SOCKET client = accept(listening, nullptr, nullptr);
				onConnected(client);
				//Add the new connection to the list of connecte clients;
				FD_SET(client, &master);
				
			}
			else
			{
				char chMsg[1004];
				ZeroMemory(chMsg, 1004, 0);
				//Receive message
				int bytesIn = recv(sock, chMsg, 1004, 0);
				if (bytesIn <= 0)
				{
					//drop client
					
					onDisConnected(sock);
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				
			}
		}
		Sleep(10);
	}

	//cleanup winsock
	WSACleanup();

	system("pause");
}