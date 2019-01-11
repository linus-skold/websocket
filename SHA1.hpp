/*
sha1.hpp - header of

============
SHA-1 in C++
============

100% Public Domain.

Original C Code
-- Steve Reid <steve@edmweb.com>
Small changes to fit into bglibs
-- Bruce Guenter <bruce@untroubled.org>
Translation to simpler C++ Code
-- Volker Diels-Grabsch <v@njh.eu>
Safety fixes
-- Eugene Hopkinson <slowriot at voxelstorm dot com>
*/

#ifndef SHA1_HPP
#define SHA1_HPP


#include <cstdint>
#include <iostream>
#include <string>


class SHA1
{
public:
	SHA1();
	void update(const std::string &s);
	void update(std::istream &is);
	std::string final();
	static std::string from_file(const std::string &filename);

	uint8_t* get_bytelist() { return byte_list; }
	uint32_t* get_digest() { return digest_old; }

private:
	
	uint32_t digest[5];

	union {
		uint8_t byte_list[20];
		uint32_t digest_old[5];
	};
	std::string buffer;
	uint64_t transforms;
};


#endif /* SHA1_HPP */