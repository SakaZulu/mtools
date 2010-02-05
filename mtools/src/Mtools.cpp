/*
 * Mtools.cpp
 *
 *  Created on: 2010-01-31
 *      Author: rosek
 */

#include "Mtools.h"

const std::string Mtools::MTOOLS_UI = "mtools.glade";

Mtools::Mtools() {
}

Mtools::~Mtools() {
	if(callbacks)
		delete callbacks;
}

void Mtools::run(int argc, char** argv) {
	Gtk::Main kit(argc, argv);

	//Load the GtkBuilder file and instantiate its widgets:
	refBuilder = Gtk::Builder::create();
#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try {
		refBuilder->add_from_file(MTOOLS_UI);
	} catch(const Glib::FileError& ex) {
		std::cerr << "FileError: " << ex.what() << std::endl;
		return;
	} catch(const Gtk::BuilderError& ex) {
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return;
	}
#else
	std::auto_ptr<Glib::Error> error;

	if (!refBuilder->add_from_file(MTOOLS_UI, error)) {
		std::cerr << error->what() << std::endl;
		return;
	}
#endif /* !GLIBMM_EXCEPTIONS_ENABLED */

	refBuilder->get_widget("winMtools", pWinMtools);
	if(pWinMtools) {
		callbacks = new Callbacks(*pWinMtools, refBuilder);
		kit.run(*pWinMtools);
	}
}
