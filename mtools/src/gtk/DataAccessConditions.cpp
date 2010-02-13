/*
 * DataAccessCondition.cpp
 *
 *  Created on: 2010-02-13
 *      Author: root
 */

#include "DataAccessConditions.h"

#include "gtk/GtkUtil.h"
#include "gtk/Consts.h"
#include "gtk/Application.h"

DataAccessConditions::DataAccessConditions() {
	Consts* consts = Consts::getInstance();
	Application* app = Application::getInstance();

	app->getBuilder()->get_widget("cbTab3Read", cbRead);
	GtkUtil::setListStore(cbRead, consts->acDataBlockRead);

	app->getBuilder()->get_widget("cbTab3Write", cbWrite);
	GtkUtil::setListStore(cbWrite, consts->acDataBlockWrite);

	app->getBuilder()->get_widget("cbTab3Inc", cbInc);
	GtkUtil::setListStore(cbInc, consts->acDataBlockInc);

	app->getBuilder()->get_widget("cbTab3Dec", cbDec);
	GtkUtil::setListStore(cbDec, consts->acDataBlockDec);

	app->getBuilder()->get_widget("btnTab3Save", btnSave);
	btnSave->signal_clicked().connect(sigc::mem_fun(*this, &DataAccessConditions::onBtnSaveClicked));
}

DataAccessConditions::~DataAccessConditions() {
}

void DataAccessConditions::onBtnSaveClicked() {
	Gtk::MessageDialog msg("Save");
	msg.run();
}
