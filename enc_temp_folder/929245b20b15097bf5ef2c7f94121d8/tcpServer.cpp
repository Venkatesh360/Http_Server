#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
//Windows Socket Api
#include <WinSock2.h>
#include <string>

int main() {

	SOCKET wsocket;
	SOCKET new_wsocket;
	WSADATA wsaData;
	struct sockaddr_in server;
	int server_len;
	int BUFFER_SIZE = 30720;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "Could not initialize ";
	}

	//create a  socket
	wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (wsocket == INVALID_SOCKET) {
		std::cout << "Could not create socket";
	}

	//bind our socket to a spesific socket

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8080);
	server_len = sizeof(server);

	if (bind(wsocket, (SOCKADDR*)&server, server_len) != 0) {
		std::cout << "Could not bind Socket ";
	}

	//listen to address
	if (listen(wsocket, 20) != 0) {
		std::cout << "Could not Listen ";
	}

	std::cout << "Started listning at 127.0.0.1:8080 ";
	int bytes = 0 ;
	while (true) {
		new_wsocket = accept(wsocket, (SOCKADDR*)&server, &server_len);
		if (new_wsocket == INVALID_SOCKET) {
			std::cout << "Could not accept \n";
		}

		//read request
		char buff[30720] = { 0 };
		bytes = recv(new_wsocket, buff, BUFFER_SIZE, 0);
		if (bytes < 0) {
			std::cout << "Could not read client";
		}

			std::string serverMessage = "HTTP/1.1 200 0k\nContent-Type: text/html\nContent-Length: ";
			std::string response = "<html><h1>Hello World</h1></html>";
			serverMessage.append("\n\n");
			serverMessage.append(response);


			int bytesSent = 0;
			int totalBytesSent = 0;
			while (totalBytesSent < serverMessage.size()) {
				bytesSent = send(new_wsocket, serverMessage.c_str(), serverMessage.size(), 0);
				if (bytesSent < 0) {
					std::cout << "Could Not Send Response";
				}

				totalBytesSent += bytesSent;
			}
			std::cout << "Send a response to client";
			
			closesocket(new_wsocket);
	}

	closesocket(wsocket);
	WSACleanup();
		return 0;
}