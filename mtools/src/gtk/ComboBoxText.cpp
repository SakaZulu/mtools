/*
 * ComboBoxText.cpp
 *
 *  Created on: 2010-02-13
 *      Author: root
 */

#include "ComboBoxText.h"

ComboBoxText::ComboBoxText(Gtk::ComboBox* cb) : m_combobox(cb) {
	m_model = Gtk::ListStore::create(m_text_columns);

	m_combobox->set_model(m_model);
	m_combobox->pack_start(m_text_columns.m_column);
}

ComboBoxText::~ComboBoxText() {
}

void ComboBoxText::append_text(const Glib::ustring& text) {
	//We can not use gtk_combo_box_append_text() here, because that can only be used if gtk_combo_box_new_text() has been used.
	//Ideally, we would just store the ListStore as a member variable, but we forgot to do that and not it would break the ABI.
	if(m_model) {
		Gtk::TreeModel::iterator iter = m_model->append();
		Gtk::TreeModel::Row row = *iter;
		row[m_text_columns.m_column] = text;
	}
}

Glib::ustring ComboBoxText::get_active_text() const {
	//We can not use gtk_combobox_get_active_text() here, because that can only be used if gtk_combo_box_new_text() has been used.
	Glib::ustring result;

	//Get the active row:
	Gtk::TreeModel::iterator active_row = m_combobox->get_active();

	if(active_row) {
		Gtk::TreeModel::Row row = *active_row;
		result = row[m_text_columns.m_column];
	}

	return result;
}

void ComboBoxText::set_active_text(const Glib::ustring& text) {
	//Look for the row with this text, and activate it:
	Glib::RefPtr<Gtk::TreeModel> model = m_combobox->get_model();
	if(model) {
		for(Gtk::TreeModel::iterator iter = model->children().begin(); iter != model->children().end(); ++iter) {
			const Glib::ustring& this_text = (*iter)[m_text_columns.m_column];

			if(this_text == text) {
				m_combobox->set_active(iter);
				return; //success
			}
		}
	}

	//Not found, so mark it as blank:
	m_combobox->unset_active();
}
