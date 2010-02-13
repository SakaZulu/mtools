/*
 * Consts.cpp
 *
 *  Created on: 2010-02-13
 *      Author: root
 */

#include "Consts.h"

Consts* Consts::_instance = 0;

Consts::Consts() {
	acDataBlockRead.push_back("Key A|B");
	acDataBlockRead.push_back("Key B");
	acDataBlockRead.push_back("Never");

	acDataBlockWrite.push_back("Key A|B");
	acDataBlockWrite.push_back("Key B");
	acDataBlockWrite.push_back("Never");

	acDataBlockInc.push_back("Key A|B");
	acDataBlockInc.push_back("Key B");
	acDataBlockInc.push_back("Never");

	acDataBlockDec.push_back("Key A|B");
	acDataBlockDec.push_back("Never");

	acTrailerKeyARead.push_back("Never");
	acTrailerKeyAWrite.push_back("Key A");
	acTrailerKeyAWrite.push_back("Key B");
	acTrailerKeyAWrite.push_back("Never");

	acTrailerAccessBitsRead.push_back("Key A");
	acTrailerAccessBitsRead.push_back("Key A|B");
	acTrailerAccessBitsWrite.push_back("Key A");
	acTrailerAccessBitsWrite.push_back("Key B");
	acTrailerAccessBitsWrite.push_back("Never");

	acTrailerKeyBRead.push_back("Key A");
	acTrailerKeyBRead.push_back("Never");
	acTrailerKeyBWrite.push_back("Key A");
	acTrailerKeyBWrite.push_back("Key B");
	acTrailerKeyBWrite.push_back("Never");

}
