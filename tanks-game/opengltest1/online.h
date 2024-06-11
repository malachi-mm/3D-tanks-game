#include "global_dic.h"



static void ClientThread()
{
	while (isRunning)
	{
		int len;
		char* msgType;
		recv(Connection, (char*)&len, sizeof(int), NULL);
		if (len != 0)
		{
			msgType = new char[len + 1];
			msgType[len] = '\0';
			recv(Connection, msgType, len, NULL);
			if ((std::string)msgType == "exit")
			{
				isRunning = false;
				delete[] msgType;
			}
			else if ((std::string)msgType == "get")
			{
				playerT pt;
				pt.pos[0] = myPos[0];
				pt.pos[1] = myPos[1];
				pt.pos[2] = myPos[2];

				pt.forward[0] = tanks[MyTank].forward.x;
				pt.forward[1] = tanks[MyTank].forward.y;
				pt.forward[2] = tanks[MyTank].forward.z;

				pt.up[0] = tanks[MyTank].up.x;
				pt.up[1] = tanks[MyTank].up.y;
				pt.up[2] = tanks[MyTank].up.z;

				pt.Pos[0] = tanks[MyTank].PoS[0];
				pt.Pos[1] = tanks[MyTank].PoS[1];
				pt.Pos[2] = tanks[MyTank].PoS[2];

				pt.PosF[0] = tanks[MyTank].PoSF[0];
				pt.PosF[1] = tanks[MyTank].PoSF[1];
				pt.PosF[2] = tanks[MyTank].PoSF[2];

				pt.yeriya = tanks[MyTank].yeriya;

				pt.Tpos[0] = tanks[MyTank].Tr.pos.x;
				pt.Tpos[1] = tanks[MyTank].Tr.pos.y;
				pt.Tpos[2] = tanks[MyTank].Tr.pos.z;

				pt.Trot[0] = tanks[MyTank].Tr.rot.x;
				pt.Trot[1] = tanks[MyTank].Tr.rot.y;
				pt.Trot[2] = tanks[MyTank].Tr.rot.z;

				pt.Tscale[0] = tanks[MyTank].Tr.scale.x;
				pt.Tscale[1] = tanks[MyTank].Tr.scale.y;
				pt.Tscale[2] = tanks[MyTank].Tr.scale.z;

				for (int k = 0; k < 4; k++)
				{
					pt.Tmat[k][0] = tanks[MyTank].Tr.mat[k].x;
					pt.Tmat[k][1] = tanks[MyTank].Tr.mat[k].y;
					pt.Tmat[k][2] = tanks[MyTank].Tr.mat[k].z;
					pt.Tmat[k][3] = tanks[MyTank].Tr.mat[k].w;
				}

				send(Connection, (char*)&pt, sizeof(pt), NULL);
				//send(Connection, (char*)&tanks[MyTank].forward, sizeof tanks[MyTank].forward, NULL);
				//send(Connection, (char*)&tanks[MyTank].PoS, sizeof tanks[MyTank].PoS, NULL);
				//send(Connection, (char*)&tanks[MyTank].PoSF, sizeof tanks[MyTank].PoSF, NULL);

			}
			else if ((std::string)msgType == "Vtanks")
			{
				//std::cout << "~~~~~~~~~~~~~~" << std::endl;
				recv(Connection, (char*)&TotalConnections, sizeof(TotalConnections), NULL);


				for (int i = 0; i < TotalConnections; i++)
				{
					playerT pt;
					recv(Connection, (char*)&pt, sizeof(pt), NULL);
					//std::cout << pt.pos[0] << "\t" << pt.pos[1] << "\t" << pt.pos[2] << "\t" << std::endl;
					if (i != MyTank)
					{
						tanks[i].pos = glm::vec3(pt.pos[0], pt.pos[1], pt.pos[2]);

						tanks[i].forward = glm::vec3(pt.forward[0], pt.forward[1], pt.forward[2]);
						tanks[i].up = glm::vec3(pt.up[0], pt.up[1], pt.up[2]);

						tanks[i].PoS = glm::vec3(pt.Pos[0], pt.Pos[1], pt.Pos[2]);
						tanks[i].PoSF = glm::vec3(pt.PosF[0], pt.PosF[1], pt.PosF[2]);

						tanks[i].yeriya = pt.yeriya;


						tanks[i].Tr.pos = glm::vec3(pt.Tpos[0], pt.Tpos[1], pt.Tpos[2]);
						tanks[i].Tr.rot = glm::vec3(pt.Trot[0], pt.Trot[1], pt.Trot[2]);
						tanks[i].Tr.scale = glm::vec3(pt.Tscale[0], pt.Tscale[1], pt.Tscale[2]);
						for (int k = 0; k < 4; k++)
						{
							tanks[i].Tr.mat[k].x = pt.Tmat[k][0];
							tanks[i].Tr.mat[k].y = pt.Tmat[k][1];
							tanks[i].Tr.mat[k].z = pt.Tmat[k][2];
							tanks[i].Tr.mat[k].w = pt.Tmat[k][3];
						}
					}
					//recv(Connection, (char*)&tanks[MyTank].forward, sizeof tanks[MyTank].forward, NULL);
					//recv(Connection, (char*)&tanks[MyTank].PoS, sizeof tanks[MyTank].PoS, NULL);
					//recv(Connection, (char*)&tanks[MyTank].PoSF, sizeof tanks[MyTank].PoSF, NULL);
					//recv(Connection, (char*)&tanks[MyTank].Tr, sizeof tanks[MyTank].Tr, NULL);
				}
			}
		}
	}
}

static std::string getHost() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "Failed to initialize Winsock" << std::endl;
		return "";
	}

	SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in clientAddr;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(12345); // Use the same port for broadcasting
	clientAddr.sin_addr.s_addr = INADDR_ANY;

	bind(udpSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr));

	struct sockaddr_in serverAddr;
	int addrLen = sizeof(serverAddr);

	char buffer[1024];

	while (true) {
		int nBytes = recvfrom(udpSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, &addrLen);
		buffer[nBytes] = '\0';

		std::cout << "Say hi to: " << inet_ntoa(serverAddr.sin_addr) << std::endl;
		closesocket(udpSocket);

		numWindow = FIND_SERVER_WINDOW;

		serverIP = inet_ntoa(serverAddr.sin_addr);
		return inet_ntoa(serverAddr.sin_addr);
		// You can now connect to the server using TCP or any other protocol
	}

	closesocket(udpSocket);
	WSACleanup();
	return 0;
}

static int connectToServer()
{
	numWindow = WAIT;
	std::string hostaddress;
	std::cout << "Searching server...";
	/*std::cin >> hostaddress;
	std::cout << std::endl;
	if (hostaddress == "localhost")
		hostaddress = "127.0.0.1";*/

	hostaddress = getHost();


	//winsock startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBox(NULL, L"Winsock startup failed", L"error", MB_OK | MB_ICONERROR);
		return 1;
	}

	SOCKADDR_IN addr;
	int addrlen = sizeof addr;
	addr.sin_addr.s_addr = inet_addr(hostaddress.c_str());

	int portN = 25579;
	//std::cout << "enter port number: ";
	//std::cin >> portN;

	addr.sin_port = htons(portN);
	addr.sin_family = AF_INET;

	std::string name;
	while (username == "") { Sleep(1); }
	//std::cout << "enter name: ";
	//std::cin >> 
	name = username;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof addr) != 0)
	{
		MessageBox(NULL, L"Failed connecting", L"error", MB_OK | MB_ICONERROR);
		throw "Failed connecting";
	}

	int namelen = name.length();
	send(Connection, (char*)&namelen, sizeof(int), NULL);
	send(Connection, name.c_str(), namelen, NULL);
	recv(Connection, (char*)&MyTank, sizeof(int), NULL);
	//recv(Connection, (char*)&numT, sizeof(int), NULL);

	std::cout << "connected with name: " << name << " num: " << MyTank << "th" << std::endl;
	for (int i = 0; i < Max; i++)
		tanks.push_back(Tank());

	numWindow = GAME_WINDOW;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);
	return 0;
}
