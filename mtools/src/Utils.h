/*
 * Utils.h
 *
 *  Created on: 2010-01-31
 *      Author: root
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <sstream>

#include "nfc/nfc-types.h"

class Utils {
	Utils() {}
public:
	virtual ~Utils() {}

	template<class T>
	static bool strHexToNumeric(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&)) {
		std::istringstream iss(s);
		return !(iss >> f >> t).fail();
	}

	static std::string intToStrHex(int i) {
		std::stringstream ss;
		ss << std::hex << i;
		return ss.str().c_str();
	}

	static std::string pByteToStrHex(byte_t* bytes, unsigned int len) {
		std::stringstream ss;
		for(unsigned int i = 0; i < len; i++)
			ss << std::hex << (unsigned int)bytes[i];
		std::string hex = ss.str();

		while(hex.size() < len*2)
			hex = "0" + hex;

		std::string ret = "";
		for(unsigned int i = 0; i < hex.size(); i += 2)
			ret += hex.substr(i, 2) + " ";

		return ret.substr(0, ret.size() - 1);
	}
};

#endif /* UTILS_H_ */
