//============================================================================
// Name        : mtools.cpp
// Author      : Krzysztof Rosiński
// Version     :
// Copyright   : GPL
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Mtools.h"
#include <glibmm/i18n.h>
#include "../config.h"

int main (int argc, char **argv) {
	bindtextdomain(GETTEXT_PACKAGE, MTOOLS_LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);

	Mtools mtools;
	mtools.run(argc, argv);
	return 0;
}
