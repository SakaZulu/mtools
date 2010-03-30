/*
 * KeysChooserDialog.h
 *
 *  Created on: 2010-02-23
 *      Author: root
 */

#ifndef KEYSCHOOSERDIALOG_H_
#define KEYSCHOOSERDIALOG_H_

#include <gtkmm.h>

/**
 * Open file with keys and add parsed
 * keys to default keys list.
 */
class KeysChooserDialog {
	Gtk::Button* pBtnOpen;
	Gtk::Button* pBtnCancel;

	Gtk::FileChooserDialog* pDlgFileChooser;

	void onBtnOpenClick();
	void onBtnCancelClick();

public:
	KeysChooserDialog();
	virtual ~KeysChooserDialog();

	void run() {
		pDlgFileChooser->run();
	}
};

#endif /* KEYSCHOOSERDIALOG_H_ */
