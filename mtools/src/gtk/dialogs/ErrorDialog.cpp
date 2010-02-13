/*
 * ErrorDialog.cpp
 *
 *  Created on: 2010-01-30
 *      Author: rosek
 */

#include "ErrorDialog.h"

ErrorDialog::ErrorDialog() : dialog("", false, Gtk::MESSAGE_ERROR) {
}

ErrorDialog::~ErrorDialog() {
}

void ErrorDialog::show(Gtk::Window& parent, const Glib::ustring& message) {
	dialog.set_parent(parent);
	show(message);
}

void ErrorDialog::show(const Glib::ustring& message) {
	dialog.set_message(message, false);
	dialog.run();
}
