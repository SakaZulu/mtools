/*
 * TrailerAccessConditions.cpp
 *
 *  Created on: 2010-02-13
 *      Author: root
 */

#include "TrailerAccessConditions.h"

#include "gtk/GtkUtil.h"
#include "gtk/Consts.h"
#include "gtk/Application.h"

TrailerAccessConditions::TrailerAccessConditions() {
	Consts* consts = Consts::getInstance();
	Application* app = Application::getInstance();

	app->getBuilder()->get_widget("cbTab4KeyARead", cbKeyARead);
	GtkUtil::setListStore(cbKeyARead, consts->acTrailerKeyARead);

	app->getBuilder()->get_widget("cbTab4KeyAWrite", cbKeyAWrite);
	GtkUtil::setListStore(cbKeyAWrite, consts->acTrailerKeyAWrite);

	app->getBuilder()->get_widget("cbTab4AcRead", cbAcRead);
	GtkUtil::setListStore(cbAcRead, consts->acTrailerAccessBitsRead);

	app->getBuilder()->get_widget("cbTab4AcWrite", cbAcWrite);
	GtkUtil::setListStore(cbAcWrite, consts->acTrailerAccessBitsWrite);

	app->getBuilder()->get_widget("cbTab4KeyBRead", cbKeyBRead);
	GtkUtil::setListStore(cbKeyBRead, consts->acTrailerKeyBRead);

	app->getBuilder()->get_widget("cbTab4KeyBWrite", cbKeyBWrite);
	GtkUtil::setListStore(cbKeyBWrite, consts->acTrailerKeyBWrite);

	app->getBuilder()->get_widget("btnTab4Save", btnSave);
	btnSave->signal_clicked().connect(sigc::mem_fun(*this, &TrailerAccessConditions::onBtnSaveClicked));

}

TrailerAccessConditions::~TrailerAccessConditions() {
}

void TrailerAccessConditions::onBtnSaveClicked() {
	Gtk::MessageDialog dialog("Save Trailer");
	dialog.run();
}
