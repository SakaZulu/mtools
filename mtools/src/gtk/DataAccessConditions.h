/*
 * DataAccessCondition.h
 *
 *  Created on: 2010-02-13
 *      Author: root
 */

#ifndef DATAACCESSCONDITIONS_H_
#define DATAACCESSCONDITIONS_H_

#include <gtkmm.h>

#include "gtk/ComboBoxText.h"

class DataAccessConditions {
private:
	ComboBoxText* cbRead;
	ComboBoxText* cbWrite;
	ComboBoxText* cbInc;
	ComboBoxText* cbDec;

	Gtk::Button* btnSave;

	void onBtnSaveClicked();
	int calcAccessBits();
public:
	DataAccessConditions();
	virtual ~DataAccessConditions();
};

#endif /* DATAACCESSCONDITIONS_H_ */
