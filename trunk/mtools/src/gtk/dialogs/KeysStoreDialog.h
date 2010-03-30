/*
 * KeysStoreDialog.h
 *
 *  Created on: 2010-02-27
 *      Author: root
 */

#ifndef KEYSSTOREDIALOG_H_
#define KEYSSTOREDIALOG_H_

#include <gtkmm.h>

#include <vector>
#include <string>

#include "gtk/dialogs/InputDialog.h"

class KeysStoreDialog {
	// GTK GUI members
	Gtk::Dialog* dialog;

	Gtk::TreeView* treeView;

	Gtk::Button* btnAdd;
	Gtk::Button* btnRemove;
	Gtk::Button* btnClose;

	void onBtnAddClick();
	void onBtnRemoveClick();
	void onBtnCloseClick();

	InputDialog input;
	void onInputCommit(const Glib::ustring text);
	bool onInputValidation(const Glib::ustring text, Glib::ustring& message);

	std::vector<std::string> model;
	std::vector<std::string> prepareModel();
public:
	sigc::signal<void, const std::vector<std::string>& > signal_changed;

	KeysStoreDialog();
	virtual ~KeysStoreDialog();

	void run();
};

#endif /* KEYSSTOREDIALOG_H_ */
