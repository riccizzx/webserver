#include "clientconfig.hpp"

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