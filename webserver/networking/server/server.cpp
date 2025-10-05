#include "../net.hpp"

void webserver::Server::stop()
{
	closesocket(client_sock);
	WSACleanup();
}

void webserver::Server::Init()
{

	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
		std::cerr << "Failed. Error Code: " << WSAGetLastError();

	}
	

	if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		std::cerr << "Could not create socket: " << WSAGetLastError();

	}

}

void webserver::Server::BindandListen()
{

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8080);

	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		std::cerr << "Bind failed with error code: " << WSAGetLastError();

	}

	if (listen(server_sock, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "Listening error: " << WSAGetLastError() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	else
	
	{
		std::cout << "Socket listening in PORT: 9909" << std::endl;
	}

}

void webserver::Server::HandleClient()
{

	client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_len);
	
	if (client_sock == INVALID_SOCKET) {
		std::cerr << "Error! Can't accept connection! " << WSAGetLastError() << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "Client connected!" << std::endl;

	char host[NI_MAXHOST];      // host is the client's remote name
	char service[NI_MAXSERV];   // service is the port client is connected on

	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client_addr, sizeof(client_addr),
		host, NI_MAXHOST,
		service, NI_MAXSERV, 0) == 0) {

		std::cout << host << " connected on port " << service << std::endl;
	
	}
	else {
		inet_ntop(AF_INET, &client_addr.sin_addr, host, NI_MAXHOST);

		std::cout << host << " connected on port " <<
			ntohs(client_addr.sin_port) << std::endl;
	}

	// scope to handle messages between server and client

	char buffer[MAX_BUFFER_LEN];

	while (true) {
		memset(&buffer, 0, sizeof(buffer));

		int bytes_recv = recv(client_sock, buffer, sizeof(buffer), 0);
		if (bytes_recv == SOCKET_ERROR) {
			std::cerr << "Error in recv(). Quitting" << std::endl;
			break;
		}
		if (bytes_recv == 0) {
			std::cout << "Client disconnected " << std::endl;
			break;
		}

		std::cout << "Received: " << buffer << std::endl;
		send(client_sock, buffer, bytes_recv + 1, 0);

	}

}

int webserver::Server::Run()
{
	try {
		webserver::Server sv;
		sv.Init();
		sv.BindandListen();
		sv.HandleClient();
	}

	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}


	return 0;
}