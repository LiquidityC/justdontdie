#ifndef MAP_BASE64_H_
#define	MAP_BASE64_H_

#include <iostream>
#include <string>

/**
 * This class isn't used at the moment
 */
class Base64
{
	public:
		static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
		static std::string base64_decode(std::string const& encoded_string);
};

#endif // MAP_BASE64_H_
