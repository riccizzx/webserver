#ifndef NETWORKING_INCLUDES
#define NETWORKING_INCLUDES

#include <iostream>
#include <string.h>
#include <string>
#include "algorithm"

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_LEN 1024
const int port = 9909;

namespace webserver{

	class Server{

	public:

		void Init(); // start winsock
		void BindandListen(); // bind and listen
		void HandleClient(); // handle client 
		void ClientInfo(char* host, char* service); // get client info
		int Run(); // run server
		void stop();

	private:
		
		// server variables
		
		WSADATA ws;
		SOCKET server_sock;
		sockaddr_in server_addr;

		// client variables
		sockaddr_in client_addr;
		SOCKET client_sock;

	};


	class Client{

	public:

		bool creation();
		int connect_to_server(const char* host, int port);
		void stop();
		int send_data(const char* data);

	private:

		WSADATA wsa;
		SOCKET sock_client;
		struct sockaddr_in addr_client;

	};

}

#endif