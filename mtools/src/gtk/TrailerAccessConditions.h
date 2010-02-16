/*
 * TrailerAccessConditions.h
 *
 *  Created on: 2010-02-13
 *      Author: root
 */

#ifndef TRAILERACCESSCONDITIONS_H_
#define TRAILERACCESSCONDITIONS_H_

#include <gtkmm.h>

#include "gtk/ComboBoxText.h"

class TrailerAccessConditions {
	ComboBoxText* cbKeyARead;
	ComboBoxText* cbKeyAWrite;
	ComboBoxText* cbAcRead;
	ComboBoxText* cbAcWrite;
	ComboBoxText* cbKeyBRead;
	ComboBoxText* cbKeyBWrite;

	Gtk::Button* btnSave;

	void onBtnSaveClicked();

	int calcAccessBits();
public:
	TrailerAccessConditions();
	virtual ~TrailerAccessConditions();
};

#endif /* TRAILERACCESSCONDITIONS_H_ */
