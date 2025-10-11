# CMakeList.txt : CMake project for webserver, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (webserver "webserver.cpp" "webserver.h" "networking/server/server.cpp" "networking/client/client.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET webserver PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
