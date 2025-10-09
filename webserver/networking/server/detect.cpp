#include "detect.hpp"

static std::string escapeHex(const char* data, size_t len)
{
	std::ostringstream oss; // use ostringstream for formatting
	oss << std::hex << std::setfill('0'); // hex format with leading zeros
    const size_t MAX_PRINT = 512; // avoid console flooding
	size_t n = std::min(len, MAX_PRINT); // limit output length


    for (size_t i = 0; i < n; ++i) { 
		unsigned char c = static_cast<unsigned char>(data[i]); // ensure correct handling of char values
	
		if (std::isprint(c) || c == '\n' || c == '\r' || c == '\t') // printable or common whitespace
			oss << static_cast<char>(c); // print as is
        
		else
			oss << "\\x" << std::setw(2) << (int)c;// hex escape for non-printable
    }
	if (len > n) oss << "...(+" << (len - n) << " bytes)"; // indicate truncation
    return oss.str();
}