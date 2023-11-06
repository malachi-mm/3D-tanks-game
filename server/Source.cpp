#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define Max 20

struct playerT
{
	float pos[3];
	float forward[3];
	float up[3];

	float Pos[3];
	float PosF[3];
	int yeriya;

	float Tpos[3];
	float Trot[3];
	float Tscale[3];
	float Tmat[4][4];
};

SOCKET Connections[Max];
playerT players[Max];
int TotalConnections = 0;
int players_s;



void ClientThread(int index)
{
	SOCKET connection = Connections[index];
	// getting name
	int bytesRead;
	recv(connection, (char*)&bytesRead, sizeof(int), NULL);
	std::cout << bytesRead;
	std::string name;
	char* buffer = new char[bytesRead + 1];
	recv(connection, buffer, bytesRead, NULL);
	buffer[bytesRead] = '\0'; // Null-terminate the received data.
	std::cout << " " << buffer << std::endl;

	// send to the player some data
	send(connection, (char*)&index, sizeof(int), NULL);
	//send(connection, (char*)&players_s, sizeof(int), NULL);

	bool isRunning = true;

	while (isRunning) {
		// get the data from player
		std::string msgType = "get";
		int msgLen = msgType.length();
		send(connection, (char*)&msgLen, sizeof(int), NULL);
		send(connection, (char*)msgType.c_str(), msgLen, NULL);

		recv(connection, (char*)&players[index], sizeof(playerT), NULL);
		
		// give all tanks data
		msgType = "Vtanks";
		msgLen = msgType.length();
		send(connection, (char*)&msgLen, sizeof(int), NULL);
		send(connection, (char*)msgType.c_str(), msgLen, NULL);

		send(connection, (char*)&TotalConnections, sizeof(int), NULL);

		for (int i = 0; i < TotalConnections; i++)
		{
			send(connection, (char*)&players[i], sizeof(playerT), NULL);
		}
		
	}

}

int main()
{
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 2);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		std::cerr << "Winsock startup failed" << std::endl;
		return 1;
	}

	SOCKADDR_IN addr;
	int addrLen = sizeof(addr);
	addr.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces
	addr.sin_port = htons(25569); // Port to listen on
	addr.sin_family = AF_INET;

	SOCKET listener = socket(AF_INET, SOCK_STREAM, 0);
	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, SOMAXCONN);

	//std::cout << "Enter number of players: ";
	//std::cin >> players_s;

	std::cout << "Server is listening for incoming connections..." << std::endl;

	while (true)
	{
		SOCKET newConnection = accept(listener, (SOCKADDR*)&addr, &addrLen);
		Connections[TotalConnections] = newConnection;
		TotalConnections++;

		// Handle the new connection in a separate thread
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, (LPVOID)(TotalConnections - 1), NULL, NULL);
	}

	return 0;
}
