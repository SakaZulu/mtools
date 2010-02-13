/*
 * DataAccessCondition.h
 *
 *  Created on: 2010-02-13
 *      Author: root
 */

#ifndef DATAACCESSCONDITIONS_H_
#define DATAACCESSCONDITIONS_H_

#include <gtkmm.h>

class DataAccessConditions {
	Gtk::ComboBox* cbRead;
	Gtk::ComboBox* cbWrite;
	Gtk::ComboBox* cbInc;
	Gtk::ComboBox* cbDec;

	Gtk::Button* btnSave;

	void onBtnSaveClicked();

public:
	DataAccessConditions();
	virtual ~DataAccessConditions();
};

#endif /* DATAACCESSCONDITIONS_H_ */
