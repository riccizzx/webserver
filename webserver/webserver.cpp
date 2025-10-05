// webserver.cpp : Defines the entry point for the application.
//

#include "webserver.h"

int main()
{
	webserver::Server s;
	try {

		s.Run();
	}
	catch (const  std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}
