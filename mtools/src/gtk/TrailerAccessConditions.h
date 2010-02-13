/*
 * TrailerAccessConditions.h
 *
 *  Created on: 2010-02-13
 *      Author: root
 */

#ifndef TRAILERACCESSCONDITIONS_H_
#define TRAILERACCESSCONDITIONS_H_

#include <gtkmm.h>

class TrailerAccessConditions {
	Gtk::ComboBox* cbKeyARead;
	Gtk::ComboBox* cbKeyAWrite;
	Gtk::ComboBox* cbAcRead;
	Gtk::ComboBox* cbAcWrite;
	Gtk::ComboBox* cbKeyBRead;
	Gtk::ComboBox* cbKeyBWrite;

	Gtk::Button* btnSave;

	void onBtnSaveClicked();
public:
	TrailerAccessConditions();
	virtual ~TrailerAccessConditions();
};

#endif /* TRAILERACCESSCONDITIONS_H_ */
