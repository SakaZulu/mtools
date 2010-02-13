/*
 * Consts.h
 *
 *  Created on: 2010-02-13
 *      Author: root
 */

#ifndef CONSTS_H_
#define CONSTS_H_

#include <string>
#include <vector>

class Consts {
private:
	static Consts* _instance;

	Consts();
public:
	virtual ~Consts() {
		if(_instance != 0)
			delete _instance;
	}

	static Consts* getInstance() {
		if(_instance == 0)
			_instance = new Consts();

		return _instance;
	}

	// Access conditions for data blocks constants
	std::vector<std::string> acDataBlockRead;
	std::vector<std::string> acDataBlockWrite;
	std::vector<std::string> acDataBlockInc;
	std::vector<std::string> acDataBlockDec;

	// Access conditions for the sector trailer constants
	std::vector<std::string> acTrailerKeyARead;
	std::vector<std::string> acTrailerKeyAWrite;

	std::vector<std::string> acTrailerAccessBitsRead;
	std::vector<std::string> acTrailerAccessBitsWrite;

	std::vector<std::string> acTrailerKeyBRead;
	std::vector<std::string> acTrailerKeyBWrite;
};

#endif /* CONSTS_H_ */
