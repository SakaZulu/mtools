/*
 * InputDialog.h
 *
 *  Created on: 2010-02-28
 *      Author: root
 */

#ifndef INPUTDIALOG_H_
#define INPUTDIALOG_H_

#include <gtkmm.h>
#include <sigc++/sigc++.h>

class InputDialog {
	// GTK GUI members
	Gtk::Dialog* dialog;

	Gtk::Button* btnOk;
	Gtk::Button* btnCancel;

	Gtk::Entry* txtInput;

	void onBtnOkClick();
	void onBtnCancelClick();

	// Class members
	Glib::ustring text;
public:
	// Signals
	sigc::signal<void, const Glib::ustring> signal_commit;
	sigc::signal<bool, const Glib::ustring, Glib::ustring&> signal_validate;

	InputDialog();
	virtual ~InputDialog();

	void show() {
		dialog->run();
	}

	void hide() {
		dialog->hide();
	}

	Glib::ustring getText() {
		return text;
	}

};

#endif /* INPUTDIALOG_H_ */
