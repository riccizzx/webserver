#include "../net.hpp"
#include "detect.hpp"

void webserver::Server::ClientInfo(char* host, char* service)
{

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
	
}

void webserver::Server::stop(){

	if (server_sock != INVALID_SOCKET){
		closesocket(server_sock);
		closesocket(client_sock);
	}

	WSACleanup();
}

void webserver::Server::Init(){

	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
		std::cerr << "Failed. Error Code: " << WSAGetLastError();

	}
	

	if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		std::cerr << "Could not create socket: " << WSAGetLastError();

	}

}

void webserver::Server::BindandListen(){

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		std::cerr << "Bind failed with error code: " << WSAGetLastError();

	}

	if (listen(server_sock, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "Listening error: " << WSAGetLastError() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	else{
		std::cout << "Socket listening in PORT: 9909" << std::endl;
	}

}

void webserver::Server::HandleClient(){

	char buffer[MAX_BUFFER_LEN]; // buffer for transfer data with client
	int client_len = sizeof(client_addr); 

	client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_len);
	
	if (client_sock == INVALID_SOCKET) {
		std::cerr << "Error! Can't accept connection! " << WSAGetLastError() << std::endl;
		exit(EXIT_FAILURE);
	}else{

		char host[NI_MAXHOST];
		char service[NI_MAXSERV];

		ClientInfo(host, service);

	}

	// scope to handle messages between server and client

	while (true) {
		memset(buffer,0 ,sizeof(buffer));
		int bytes_recv = recv(client_sock, buffer, sizeof(buffer), 0);



		if (bytes_recv > 0) {
			std::string recv_msg(buffer, bytes_recv);
			printf("Client: ", recv_msg);

			if (recv_msg == "exit") {
				printf( "Client requested to close connection.\n");
				break;
			}

			// echo back
			send(client_sock, buffer, bytes_recv, 0);
			
			// to send custom message from server to client uncomment below
			//std::string msg;
			//std::getline(std::cin, msg);
			//send(client_sock, msg.c_str(), msg.size(), 0);
		}

		std::string printable = escapeHex(buffer, bytes_recv);
		printf("Client: ", printable);

		if (bytes_recv == 0)
		{
			printf("Connection closing...\n");
			break;
		}
		else
		{
			printf("recv failed: %d\n", WSAGetLastError());
			break;
		}
	}

	closesocket(client_sock);
	printf("Connection clossed! \n");
}

int webserver::Server::Run(){
	
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