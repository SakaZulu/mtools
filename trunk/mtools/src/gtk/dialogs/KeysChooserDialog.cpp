/*
 * KeysChooserDialog.cpp
 *
 *  Created on: 2010-02-23
 *      Author: root
 */

#include "KeysChooserDialog.h"

#include "gtk/Application.h"

KeysChooserDialog::KeysChooserDialog() {
	Application* app = Application::getInstance();

	app->getBuilder()->get_widget("fcDlgOpenKeys", pDlgFileChooser);

	app->getBuilder()->get_widget("btnFcDlgOpen", pBtnOpen);
	pBtnOpen->signal_clicked().connect(sigc::mem_fun(*this, &KeysChooserDialog::onBtnOpenClick));

	app->getBuilder()->get_widget("btnFcDlgCancel", pBtnCancel);
	pBtnCancel->signal_clicked().connect(sigc::mem_fun(*this, &KeysChooserDialog::onBtnCancelClick));
}

KeysChooserDialog::~KeysChooserDialog() {
}

void KeysChooserDialog::onBtnCancelClick() {
	pDlgFileChooser->hide();
}

void KeysChooserDialog::onBtnOpenClick() {
	pDlgFileChooser->hide();
}

