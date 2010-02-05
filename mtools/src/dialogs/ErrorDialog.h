/*
 * ErrorDialog.h
 *
 *  Created on: 2010-01-30
 *      Author: rosek
 */

#ifndef ERRORDIALOG_H_
#define ERRORDIALOG_H_

#include <gtkmm.h>
#include <iostream>

class ErrorDialog {
protected:
	Gtk::MessageDialog dialog;

public:
	ErrorDialog();
	virtual ~ErrorDialog();

	void show(const Glib::ustring& message);
	void show(Gtk::Window& parent, const Glib::ustring& message);
};

#endif /* ERRORDIALOG_H_ */
