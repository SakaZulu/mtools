/*
 * GtkUtil.h
 *
 *  Created on: 2010-02-15
 *      Author: root
 */

#ifndef GTKUTIL_H_
#define GTKUTIL_H_

#include <gtkmm.h>

#include "gtk/Application.h"
#include "gtk/ComboBoxText.h"

class GtkUtil {
	GtkUtil() {}
public:
	static ComboBoxText* getComboBoxText(std::string name) {
		Application* app = Application::getInstance();

		Gtk::ComboBox* cb;
		app->getBuilder()->get_widget(name, cb);

		ComboBoxText* cbt = new ComboBoxText(cb);
		return cbt;
	}

	static ComboBoxText* getComboBoxText(std::string name, std::vector<std::string> v) {
		Application* app = Application::getInstance();

		Gtk::ComboBox* cb;
		app->getBuilder()->get_widget(name, cb);

		ComboBoxText* cbt = new ComboBoxText(cb);

		for(unsigned int i = 0; i < v.size(); i++)
			cbt->append_text(v[i]);
		cbt->set_active_text(v[0]);

		return cbt;
	}
};

#endif /* GTKUTIL_H_ */
