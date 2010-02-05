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

	static std::string pByteToStrHex(byte_t* bytes, int len) {
		std::stringstream ss;
		for(int i = 0; i < len; i++)
			ss << std::hex << bytes[i];
		return ss.str().c_str();
	}
};

#endif /* UTILS_H_ */
