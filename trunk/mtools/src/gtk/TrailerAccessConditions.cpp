/*
 * TrailerAccessConditions.cpp
 *
 *  Created on: 2010-02-13
 *      Author: root
 */

#include "TrailerAccessConditions.h"

#include "gtk/Consts.h"
#include "gtk/Application.h"

#include "gtk/GtkUtil.h"
#include "gtk/dialogs/ErrorDialog.h"


#include "Utils.h"

TrailerAccessConditions::TrailerAccessConditions() {
	Consts* consts = Consts::getInstance();
	Application* app = Application::getInstance();

	cbKeyARead 	= GtkUtil::setComboBoxText("cbTab4KeyARead", consts->acTrailerKeyARead);
	cbKeyAWrite = GtkUtil::setComboBoxText("cbTab4KeyAWrite", consts->acTrailerKeyAWrite);
	cbAcRead 	= GtkUtil::setComboBoxText("cbTab4AcRead", consts->acTrailerAccessBitsRead);
	cbAcWrite 	= GtkUtil::setComboBoxText("cbTab4AcWrite", consts->acTrailerAccessBitsWrite);
	cbKeyBRead 	= GtkUtil::setComboBoxText("cbTab4KeyBRead", consts->acTrailerKeyBRead);
	cbKeyBWrite = GtkUtil::setComboBoxText("cbTab4KeyBWrite", consts->acTrailerKeyBWrite);

	app->getBuilder()->get_widget("btnTab4Save", btnSave);
	btnSave->signal_clicked().connect(sigc::mem_fun(*this, &TrailerAccessConditions::onBtnSaveClicked));
}

TrailerAccessConditions::~TrailerAccessConditions() {
	if(cbKeyARead) 		delete cbKeyARead;
	if(cbKeyAWrite) 	delete cbKeyAWrite;
	if(cbAcRead) 		delete cbAcRead;
	if(cbAcWrite) 		delete cbAcWrite;
	if(cbKeyBRead) 		delete cbKeyBRead;
	if(cbKeyBWrite) 	delete cbKeyBWrite;
}

void TrailerAccessConditions::onBtnSaveClicked() {
	try {
		Gtk::MessageDialog dialog("value: " + Utils::intToStrHex(calcAccessBits()));
		dialog.run();
	} catch(const std::runtime_error& e) {
		ErrorDialog error;
		error.show(e.what());
	}
}

int TrailerAccessConditions::calcAccessBits() {
	std::string keyARead = cbKeyARead->get_active_text();
	std::string keyAWrite = cbKeyAWrite->get_active_text();
	std::string acRead = cbAcRead->get_active_text();
	std::string acWrite = cbAcWrite->get_active_text();
	std::string keyBRead = cbKeyBRead->get_active_text();
	std::string keyBWrite = cbKeyBWrite->get_active_text();

	std::string keyAB = "Key A|B";
	std::string never = "Never";
	std::string keyB  = "Key B";
	std::string keyA  = "Key A";

	if(keyARead.compare(never) == 0 && keyAWrite.compare(keyA) == 0 &&
			acRead.compare(keyA) == 0 && acWrite.compare(never) == 0 &&
			keyBRead.compare(keyA) == 0 && keyBWrite.compare(keyA) == 0)
		return 0;

	if(keyARead.compare(never) == 0 && keyAWrite.compare(never) == 0 &&
			acRead.compare(keyA) == 0 && acWrite.compare(never) == 0 &&
			keyBRead.compare(keyA) == 0 && keyBWrite.compare(never) == 0)
		return 2;

	if(keyARead.compare(never) == 0 && keyAWrite.compare(keyB) == 0 &&
			acRead.compare(keyAB) == 0 && acWrite.compare(never) == 0 &&
			keyBRead.compare(never) == 0 && keyBWrite.compare(keyB) == 0)
		return 4;

	if(keyARead.compare(never) == 0 && keyAWrite.compare(never) == 0 &&
			acRead.compare(keyAB) == 0 && acWrite.compare(never) == 0 &&
			keyBRead.compare(never) == 0 && keyBWrite.compare(never) == 0)
		return 6;

	if(keyARead.compare(never) == 0 && keyAWrite.compare(keyA) == 0 &&
			acRead.compare(keyA) == 0 && acWrite.compare(keyA) == 0 &&
			keyBRead.compare(keyA) == 0 && keyBWrite.compare(keyA) == 0)
		return 1;

	if(keyARead.compare(never) == 0 && keyAWrite.compare(keyB) == 0 &&
			acRead.compare(keyAB) == 0 && acWrite.compare(keyB) == 0 &&
			keyBRead.compare(never) == 0 && keyBWrite.compare(keyB) == 0)
		return 3;

	if(keyARead.compare(never) == 0 && keyAWrite.compare(never) == 0 &&
			acRead.compare(keyAB) == 0 && acWrite.compare(keyB) == 0 &&
			keyBRead.compare(never) == 0 && keyBWrite.compare(never) == 0)
		return 5;

	if(keyARead.compare(never) == 0 && keyAWrite.compare(never) == 0 &&
			acRead.compare(keyAB) == 0 && acWrite.compare(never) == 0 &&
			keyBRead.compare(never) == 0 && keyBWrite.compare(never) == 0)
		return 7;

	throw std::runtime_error("Can't set this set of options.");
}
