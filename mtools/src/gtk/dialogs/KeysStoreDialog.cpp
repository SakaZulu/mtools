/*
 * KeysStoreDialog.cpp
 *
 *  Created on: 2010-02-27
 *      Author: root
 */

#include "KeysStoreDialog.h"

#include <boost/regex.hpp>

#include "gtk/Application.h"

#include "gtk/dialogs/ErrorDialog.h"

KeysStoreDialog::KeysStoreDialog() {
	Application* app = Application::getInstance();

	app->getBuilder()->get_widget("dlgKeysStore", dialog);

	app->getBuilder()->get_widget("tvKeysStore", treeView);
	Glib::RefPtr<Gtk::TreeSelection> sel = treeView->get_selection();
	sel->set_mode(Gtk::SELECTION_MULTIPLE);

	app->getBuilder()->get_widget("btnKeysStoreAdd", btnAdd);
	btnAdd->signal_clicked().connect(sigc::mem_fun(*this, &KeysStoreDialog::onBtnAddClick));

	app->getBuilder()->get_widget("btnKeysStoreRemove", btnRemove);
	btnRemove->signal_clicked().connect(sigc::mem_fun(*this, &KeysStoreDialog::onBtnRemoveClick));

	app->getBuilder()->get_widget("btnKeysStoreClose", btnClose);
	btnClose->signal_clicked().connect(sigc::mem_fun(*this, &KeysStoreDialog::onBtnCloseClick));

	input.signal_commit.connect(sigc::mem_fun(*this, &KeysStoreDialog::onInputCommit));
	input.signal_validate.connect(sigc::mem_fun(*this, &KeysStoreDialog::onInputValidation));
}

KeysStoreDialog::~KeysStoreDialog() {
}

void KeysStoreDialog::onBtnAddClick() {
	input.show();
}

void KeysStoreDialog::onBtnRemoveClick() {
	bool changed = false;

	Glib::RefPtr<Gtk::TreeModel> model = treeView->get_model();
	Glib::RefPtr<Gtk::ListStore> listStore = Glib::RefPtr<Gtk::ListStore>::cast_static(model);

	Glib::RefPtr<Gtk::TreeSelection> sel = treeView->get_selection();
	std::vector<Gtk::TreeModel::Path> pathlist = sel->get_selected_rows();

	for(int i = pathlist.size() - 1; i >= 0 ; i--) {
		Gtk::TreeModel::iterator iter = treeView->get_model()->get_iter(pathlist[i]);
		Gtk::TreeModel::Row row = *iter;

		listStore->erase(iter);
		changed = true;
	}

	// Emit signal if any change appear
	if(changed)
		signal_changed(prepareModel());
}

void KeysStoreDialog::onBtnCloseClick() {
	dialog->hide();
}

void KeysStoreDialog::run() {
	dialog->run();
}

void KeysStoreDialog::onInputCommit(const Glib::ustring key) {
	// Add key to list store
	if(key == "")
		return;

	Glib::RefPtr<Gtk::TreeModel> model = treeView->get_model();
	Glib::RefPtr<Gtk::ListStore> listStore = Glib::RefPtr<Gtk::ListStore>::cast_static(model);

	Gtk::TreeModel::Row row = *(listStore->append());
	row.set_value<Glib::ustring>(0, key);

	signal_changed(prepareModel());
}

bool KeysStoreDialog::onInputValidation(const Glib::ustring key, Glib::ustring& message) {
	const static boost::regex e("([\\da-fA-F][\\da-fA-F]:){5}[\\da-fA-F][\\da-fA-F]");

	if(key == "") {
		message = "Input field is empty.";
		return false;
	}

	if(!boost::regex_match(key.raw(), e)) {
		message = "Input text do not match to the key pattern.";
		return false;
	}

	// Check if key is already in listStore
	Glib::RefPtr<Gtk::TreeModel> model = treeView->get_model();
	Glib::RefPtr<Gtk::ListStore> listStore = Glib::RefPtr<Gtk::ListStore>::cast_static(model);

	for(Gtk::TreeModel::iterator iter = model->children().begin(); iter != model->children().end(); iter++) {
		Gtk::TreeModel::Row row = *iter;

		Glib::ustring text;
		row.get_value<Glib::ustring>(0, text);

		if(text == key) {
			message = "Key is already on the list.";
			return false;
		}
	}

	return true;
}

std::vector<std::string> KeysStoreDialog::prepareModel() {
	this->model.clear();

	// Check if key is already in listStore
	Glib::RefPtr<Gtk::TreeModel> model = treeView->get_model();
	Glib::RefPtr<Gtk::ListStore> listStore = Glib::RefPtr<Gtk::ListStore>::cast_static(model);

	for(Gtk::TreeModel::iterator iter = model->children().begin(); iter != model->children().end(); iter++) {
		Gtk::TreeModel::Row row = *iter;

		Glib::ustring text;
		row.get_value<Glib::ustring>(0, text);

		this->model.push_back(text.raw());
	}

	return this->model;
}
