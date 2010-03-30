/*
 * InputDialog.cpp
 *
 *  Created on: 2010-02-28
 *      Author: root
 */

#include "InputDialog.h"

#include "gtk/Application.h"

#include "gtk/dialogs/ErrorDialog.h"

InputDialog::InputDialog() {
	Application* app = Application::getInstance();

	app->getBuilder()->get_widget("dlgInputText", dialog);

	app->getBuilder()->get_widget("btnInputTextOk", btnOk);
	btnOk->signal_clicked().connect(sigc::mem_fun(*this, &InputDialog::onBtnOkClick));

	app->getBuilder()->get_widget("btnInputTextCancel", btnCancel);
	btnCancel->signal_clicked().connect(sigc::mem_fun(*this, &InputDialog::onBtnCancelClick));

	app->getBuilder()->get_widget("txtInputText", txtInput);

}

InputDialog::~InputDialog() {
}

void InputDialog::onBtnOkClick() {
	Glib::ustring message;
	text = txtInput->get_text();

	if(!signal_validate.emit(text, message)) {
		text = "";
		ErrorDialog error;
		error.show(message);
		return;
	}

	signal_commit(text);
	dialog->hide();
}

void InputDialog::onBtnCancelClick() {
	text = "";
	dialog->hide();
}
