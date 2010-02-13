/*
 * Util.h
 *
 *  Created on: 2010-02-13
 *      Author: root
 */

#ifndef GTKUTIL_H_
#define GTKUTIL_H_

#include <gtkmm.h>

#include <string>
#include <vector>

class GtkUtil {
	GtkUtil();
public:
	virtual ~GtkUtil();

	static void setListStore(Gtk::ComboBox* cb, std::vector<std::string> v) {
		Gtk::TreeModelColumn<Glib::ustring> name;
		Gtk::TreeModel::ColumnRecord record;
		record.add(name);

		Glib::RefPtr<Gtk::ListStore> model = Gtk::ListStore::create(record);

		Gtk::TreeModel::Row row;
		for(unsigned int i = 0; i < v.size(); i++) {
			row = *(model->append());
			row[name] = v[i];
		}

		cb->set_model(model);
		cb->pack_start(name);
		cb->set_active(0);
	}
};

#endif /* UTIL_H_ */
