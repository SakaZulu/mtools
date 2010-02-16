/*
 * ComboBoxText.h
 *
 *  Created on: 2010-02-13
 *      Author: root
 */

#ifndef COMBOBOXTEXT_H_
#define COMBOBOXTEXT_H_

#include <gtkmm.h>
#include <string>

class ComboBoxText {
public:
	ComboBoxText(Gtk::ComboBox* cb);
	virtual ~ComboBoxText();

	void append_text(const Glib::ustring& text);

	Glib::ustring get_active_text() const;

	void set_active_text(const Glib::ustring& text);

private:
	//Tree model columns:
	//These columns are used by the model that is created by the default constructor
	class TextModelColumns : public Gtk::TreeModel::ColumnRecord {
	public:
		TextModelColumns() {
			add(m_column);
		}

		Gtk::TreeModelColumn<Glib::ustring> m_column;
	};

	TextModelColumns m_text_columns;
	Glib::RefPtr<Gtk::ListStore> m_model;

	Gtk::ComboBox* m_combobox;
};

#endif /* COMBOBOXTEXT_H_ */
