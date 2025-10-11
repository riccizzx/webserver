#include "clientconfig.hpp"

void webserver::Client::stop() {

	if (sock_client != INVALID_SOCKET) {
		sock_client = INVALID_SOCKET;
		closesocket(sock_client);
	}

	WSACleanup();
}

bool webserver::Client::creation()
{

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cerr << "Failed. Error Code: " << WSAGetLastError();
		return false;

	}

	if ((sock_client = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		std::cerr << "Could not create socket: " << WSAGetLastError();
		return false;
	}

	return true;
}

int webserver::Client::connect_to_server(const char* host, int port)
{
	char buff[MAX_BUFFER_LEN];
	memset(buff, 0, sizeof(buff));

	addr_client.sin_family = AF_INET;
	addr_client.sin_port = htons(port);
	inet_pton(AF_INET, host, &addr_client.sin_addr);
	
	if (connect(sock_client, (struct sockaddr*)&addr_client, sizeof(addr_client)) == SOCKET_ERROR) {
		std::cerr << "Connection error: " << WSAGetLastError() << std::endl;
		return -1;
	}

	std::cout << "Connected to server!" << std::endl;

	std::string user_input;

	do {
		
		std::cout << "Enter message ('exit' to quit): ";
		std::getline(std::cin, user_input); // Read user input from console

		if (user_input.length() > 0) {
	
			int sendResult = send(sock_client, user_input.c_str(), user_input.length(), 0);
			if (sendResult == SOCKET_ERROR) {
				std::cerr << "ERROR: Can't send back to the server. ERROR: " << WSAGetLastError() << "\n";
				break;
			}

			memset(buff,0, 1024);
			int bytes_recv = recv(sock_client, buff, sizeof(buff), 0);
			if (bytes_recv > 0) {
				buff[bytes_recv];
				std::cout << "Server echo: " << std::string(buff) << "\n";
			}
			else if (bytes_recv == 0) {
				std::cout << "Server disconnected. \n";
				break;
			}
			else {
				std::cerr << "ERROR: Error receiving data! Error: " << WSAGetLastError() << "\n";
				break;
			}
		}
	} while (user_input != "exit" && user_input != "EXIT");// continue untill user type "EXIT"

	return 0;

}
