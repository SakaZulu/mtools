/*
 * DataAccessCondition.cpp
 *
 *  Created on: 2010-02-13
 *      Author: root
 */

#include "DataAccessConditions.h"

#include "gtk/Consts.h"
#include "gtk/Application.h"

#include "gtk/GtkUtil.h"
#include "gtk/dialogs/ErrorDialog.h"

#include "Utils.h"

DataAccessConditions::DataAccessConditions() : cbRead(0), cbWrite(0), cbInc(0), cbDec(0) {
	Consts* consts = Consts::getInstance();
	Application* app = Application::getInstance();

	cbRead 	= GtkUtil::setComboBoxText("cbTab3Read", consts->acDataBlockRead);
	cbWrite	= GtkUtil::setComboBoxText("cbTab3Write", consts->acDataBlockWrite);
	cbInc 	= GtkUtil::setComboBoxText("cbTab3Inc", consts->acDataBlockInc);
	cbDec 	= GtkUtil::setComboBoxText("cbTab3Dec", consts->acDataBlockDec);

	app->getBuilder()->get_widget("btnTab3Save", btnSave);
	btnSave->signal_clicked().connect(sigc::mem_fun(*this, &DataAccessConditions::onBtnSaveClicked));
}

DataAccessConditions::~DataAccessConditions() {
	if(cbRead)	delete cbRead;
	if(cbWrite) delete cbWrite;
	if(cbInc)	delete cbInc;
	if(cbDec)	delete cbDec;
}

void DataAccessConditions::onBtnSaveClicked() {
	try {
		Gtk::MessageDialog dialog("value: " + Utils::intToStrHex(calcAccessBits()));
		dialog.run();
	} catch(const std::runtime_error& e) {
		ErrorDialog error;
		error.show(e.what());
	}
}

int DataAccessConditions::calcAccessBits() {
	std::string read = cbRead->get_active_text();
	std::string write = cbWrite->get_active_text();
	std::string inc = cbInc->get_active_text();
	std::string dec = cbDec->get_active_text();

	std::string keyAB = "Key A|B";
	std::string never = "Never";
	std::string keyB  = "Key B";

	if(read.compare(keyAB) == 0 && write.compare(keyAB) == 0 &&
			inc.compare(keyAB) == 0 && dec.compare(keyAB) == 0)
		return 0;

	if(read.compare(keyAB) == 0 && write.compare(never) == 0 &&
			inc.compare(never) == 0 && dec.compare(never) == 0)
		return 2;

	if(read.compare(keyAB) == 0 && write.compare(keyB) == 0 &&
			inc.compare(never) == 0 && dec.compare(never) == 0)
		return 4;

	if(read.compare(keyAB) == 0 && write.compare(keyB) == 0 &&
			inc.compare(keyB) == 0 && dec.compare(keyAB) == 0)
		return 6;

	if(read.compare(keyAB) == 0 && write.compare(never) == 0 &&
			inc.compare(never) == 0 && dec.compare(keyAB) == 0)
		return 1;

	if(read.compare(keyB) == 0 && write.compare(keyB) == 0 &&
			inc.compare(never) == 0 && dec.compare(never) == 0)
		return 3;

	if(read.compare(keyB) == 0 && write.compare(never) == 0 &&
			inc.compare(never) == 0 && dec.compare(never) == 0)
		return 5;

	if(read.compare(never) == 0 && write.compare(never) == 0 &&
			inc.compare(never) == 0 && dec.compare(never) == 0)
		return 7;

	throw std::runtime_error("Can't set this set of options.");
}
