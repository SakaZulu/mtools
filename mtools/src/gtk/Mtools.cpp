/*
 * Mtools.cpp
 *
 *  Created on: 2010-01-31
 *      Author: rosek
 */

#include "gtk/Mtools.h"
#include "gtk/Application.h"

const std::string Mtools::MTOOLS_UI = "mtools.glade";

Mtools::Mtools() {
}

Mtools::~Mtools() {
}

void Mtools::run(int argc, char** argv) {
	Gtk::Main kit(argc, argv);

	Application* app = Application::getInstance();
	app->setBuilder(Gtk::Builder::create());

#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try {
		app->getBuilder()->add_from_file(MTOOLS_UI);
	} catch(const Glib::FileError& ex) {
		std::cerr << "FileError: " << ex.what() << std::endl;
		return;
	} catch(const Gtk::BuilderError& ex) {
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return;
	}
#else
	std::auto_ptr<Glib::Error> error;
	if (!app->getBuilder()->add_from_file(MTOOLS_UI, error)) {
		std::cerr << error->what() << std::endl;
		return;
	}
#endif /* !GLIBMM_EXCEPTIONS_ENABLED */

	Gtk::Window* pWindow;
	app->getBuilder()->get_widget("winMtools", pWindow);
	if(pWindow) {
		app->setWindow(pWindow);

		Callbacks callbacks;
		kit.run(*app->getWindow());
	}
}
