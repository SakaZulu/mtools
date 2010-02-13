/*
 * Application.h
 *
 *  Created on: 2010-02-13
 *      Author: root
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <gtkmm.h>

class Application {
	static Application* _instance;

	Gtk::Window* pWindow;
	Glib::RefPtr<Gtk::Builder> refBuilder;

	Application() {}
public:
	virtual ~Application() {
		if(_instance != 0)
			delete _instance;
	}

	static Application* getInstance() {
		if(_instance == 0)
			_instance = new Application();

		return _instance;
	}

	Gtk::Window* getWindow() {
		return pWindow;
	}

	void setWindow(Gtk::Window* pWindow) {
		this->pWindow = pWindow;
	}

	Glib::RefPtr<Gtk::Builder> getBuilder() {
		return refBuilder;
	}

	void setBuilder(Glib::RefPtr<Gtk::Builder> refBuilder) {
		this->refBuilder = refBuilder;
	}
};

#endif /* APPLICATION_H_ */
