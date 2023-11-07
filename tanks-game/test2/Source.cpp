#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")


#include <iostream>
#include <winsock2.h>

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return 1;
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

        // You can now connect to the server using TCP or any other protocol
    }

    closesocket(udpSocket);
    WSACleanup();
    return 0;
}
