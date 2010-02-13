/*
 * Mtools.h
 *
 *  Created on: 2010-01-31
 *      Author: rosek
 */

#ifndef MTOOLS_H_
#define MTOOLS_H_

#include <gtkmm.h>
#include <iostream>

#include "Callbacks.h"

class Mtools {
public:
	Mtools();
	virtual ~Mtools();

	void run(int argc, char** argv);
protected:
	static const std::string MTOOLS_UI;
};

#endif /* MTOOLS_H_ */
