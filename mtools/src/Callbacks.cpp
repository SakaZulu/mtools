/*
 * Callbacks.cpp
 *
 *  Created on: 2010-01-31
 *      Author: rosek
 */

#include "Nfc.h"

#include "Utils.h"
#include "Callbacks.h"
#include "dialogs/ErrorDialog.h"

Callbacks::Callbacks(Gtk::Window& winMtools, Glib::RefPtr<Gtk::Builder>& refBuilder)
		: winMtools(winMtools), refBuilder(refBuilder) {

	// Button
	refBuilder->get_widget("btnSearch", pBtnSearch);
	pBtnSearch->signal_clicked().connect(sigc::mem_fun(*this, &Callbacks::onBtnSearchClicked));
	refBuilder->get_widget("btnTab1Read", pBtnTab1Read);
	pBtnTab1Read->signal_clicked().connect(sigc::mem_fun(*this, &Callbacks::onBtnTab1ReadClick));
	refBuilder->get_widget("btnTab1Write", pBtnTab1Write);
	pBtnTab1Write->signal_clicked().connect(sigc::mem_fun(*this, &Callbacks::onBtnTab1WriteClick));
	refBuilder->get_widget("btnTab2Init", pBtnTab2Init);
	pBtnTab2Init->signal_clicked().connect(sigc::mem_fun(*this, &Callbacks::onBtnTab2InitClick));
	refBuilder->get_widget("btnTab2Read", pBtnTab2Read);
	pBtnTab2Read->signal_clicked().connect(sigc::mem_fun(*this, &Callbacks::onBtnTab2ReadClick));
	refBuilder->get_widget("btnTab2Inc", pBtnTab2Inc);
	pBtnTab2Inc->signal_clicked().connect(sigc::mem_fun(*this, &Callbacks::onBtnTab2IncClick));
	refBuilder->get_widget("btnTab2Dec", pBtnTab2Dec);
	pBtnTab2Dec->signal_clicked().connect(sigc::mem_fun(*this, &Callbacks::onBtnTab2DecClick));

	// RadioButton
	refBuilder->get_widget("rbKeyA", pRbKeyA);
	refBuilder->get_widget("rbKeyB", pRbKeyB);

	// CheckButton
	refBuilder->get_widget("cbDefKeys", pCbDefKeys);
	pCbDefKeys->signal_clicked().connect(sigc::mem_fun(*this, &Callbacks::onCbDefKeysClicked));

	// SpinButton
	refBuilder->get_widget("spnValue", pSpnValue);
	refBuilder->get_widget("spnBlock", pSpnBlock);
	refBuilder->get_widget("spnSector", pSpnSector);

	// Entry
	refBuilder->get_widget("txtKey", pTxtKey);
	refBuilder->get_widget("txtUid", pTxtUid);
	refBuilder->get_widget("txtCardType", pTxtCardType);
	refBuilder->get_widget("txtReadWrite", pTxtReadWrite);

	// Menu item
	Gtk::ImageMenuItem* pMiQuit;
	refBuilder->get_widget("miQuit", pMiQuit);
	pMiQuit->signal_activate().connect(sigc::mem_fun(*this, &Callbacks::onMiQuitClicked));

	Gtk::ImageMenuItem* pMiSearch;
	refBuilder->get_widget("miSearch", pMiSearch);
	pMiSearch->signal_activate().connect(sigc::mem_fun(*this, &Callbacks::onBtnSearchClicked));

	Gtk::ImageMenuItem* pMiAbout;
	refBuilder->get_widget("miAbout", pMiAbout);
	pMiAbout->signal_activate().connect(sigc::mem_fun(*this, &Callbacks::about));

	// About dialog
	refBuilder->get_widget("dlgAbout", pDlgAbout);
}

Callbacks::~Callbacks() {
}

void Callbacks::onBtnSearchClicked() {
	Gtk::Adjustment *pAdjBlock = pSpnBlock->get_adjustment();
	Gtk::Adjustment *pAdjSector = pSpnSector->get_adjustment();

	try {
		std::string uid = "";
		std::string cardType = "";

		nfc.findTags();
		uid = nfc.getTagUid();
		cardType = nfc.getTagType();

		pTxtUid->set_text(uid);
		pTxtCardType->set_text(cardType);

		if(cardType.find("MIFARE Classic 1K") != std::string::npos) {
			setOperationAdjustment(15, 3);
		} else if(cardType.find("MIFARE Classic 4K") != std::string::npos) {
			setOperationAdjustment(39, 3);
			pAdjSector->signal_value_changed().connect(sigc::mem_fun(*this, &Callbacks::onAdjSectorValueChanged));
		} else {
			pAdjBlock->set_value(0);
			pAdjSector->set_value(0);
			setOperationAdjustment(0, 0);
		}

	} catch(const std::runtime_error& error) {
		clear();
		setOperationAdjustment(0, 0);

		ErrorDialog dialog;
		dialog.show(error.what());
	}
}

void Callbacks::onBtnTab1ReadClick() {
	try {
		unsigned int block = pSpnBlock->get_value_as_int();
		unsigned int sector = pSpnSector->get_value_as_int();

		authenticate(sector);

		unsigned char data[17] = {0};
		nfc.read(sector, block, data);
		pTxtReadWrite->set_text((char*)data);
	} catch(const std::runtime_error& error) {
		ErrorDialog dialog;
		dialog.show(error.what());
	}
}

void Callbacks::onBtnTab1WriteClick() {
	try {
		unsigned int block = pSpnBlock->get_value_as_int();
		unsigned int sector = pSpnSector->get_value_as_int();

		std::string data = pTxtReadWrite->get_text();
		if(data.size() > 16)
			throw std::runtime_error("Maximum length of data is 16.");

		authenticate(sector);

		nfc.write(sector, block, (unsigned char*)data.c_str());
	} catch(const std::runtime_error& error) {
		ErrorDialog dialog;
		dialog.show(error.what());
	}
}

void Callbacks::onBtnTab2InitClick() {
	try {
		int value = pSpnValue->get_value_as_int();

		unsigned int block = pSpnBlock->get_value_as_int();
		unsigned int sector = pSpnSector->get_value_as_int();

		authenticate(sector);

		nfc.initValue(sector, block, value);
	} catch(const std::runtime_error& error) {
		ErrorDialog dialog;
		dialog.show(error.what());
	}
}

void Callbacks::onBtnTab2ReadClick() {
	try {
		unsigned int block = pSpnBlock->get_value_as_int();
		unsigned int sector = pSpnSector->get_value_as_int();

		authenticate(sector);

		int value = nfc.readValue(sector, block);
		pSpnValue->set_value(value);
	} catch(const std::runtime_error& error) {
		ErrorDialog dialog;
		dialog.show(error.what());
	}
}

void Callbacks::onBtnTab2IncClick() {
	try {
		int value = pSpnValue->get_value_as_int();

		unsigned int block = pSpnBlock->get_value_as_int();
		unsigned int sector = pSpnSector->get_value_as_int();

		authenticate(sector);

		value = nfc.incValue(sector, block, value);
		pSpnValue->set_value(value);
	} catch(const std::runtime_error& error) {
		ErrorDialog dialog;
		dialog.show(error.what());
	}
}

void Callbacks::onBtnTab2DecClick() {
	try {
		int value = pSpnValue->get_value_as_int();

		unsigned int block = pSpnBlock->get_value_as_int();
		unsigned int sector = pSpnSector->get_value_as_int();

		authenticate(sector);

		value = nfc.decValue(sector, block, value);
		pSpnValue->set_value(value);
	} catch(const std::runtime_error& error) {
		ErrorDialog dialog;
		dialog.show(error.what());
	}
}

void Callbacks::onCbDefKeysClicked() {
	if(pCbDefKeys->get_active())
		pTxtKey->set_editable(false);
	else
		pTxtKey->set_editable(true);
}

void Callbacks::onAdjSectorValueChanged() {
	Gtk::Adjustment *pAdjBlock = pSpnBlock->get_adjustment();
	Gtk::Adjustment *pAdjSector = pSpnSector->get_adjustment();

	double value = pAdjSector->get_value();
	if(value > 31)
		pAdjBlock->set_upper(15);
	if(value <= 31) {
		pAdjBlock->set_upper(3);
		if(pAdjBlock->get_value() > 3)
			pAdjBlock->set_value(3);
	}
}

void Callbacks::onMiQuitClicked () {
	Gtk::Main::quit();
}

void Callbacks::authenticate(int sector) {
	unsigned char key[6] = {0};

	unsigned int keyType = Nfc::KEY_B;
	if(pRbKeyA->get_active())
		keyType = Nfc::KEY_A;

	if(pCbDefKeys->get_active())
		nfc.authenticate(sector, keyType);
	else {
		std::string skey = pTxtKey->get_text();
		if(skey.size() != 12)
			throw std::runtime_error("Wrong key size.");

		for(int hex, i = 0; i < 6; i++) {
			if(!Utils::strHexToNumeric<int>(hex, skey.substr(i*2, 2), std::hex))
				throw std::runtime_error("Pass appropriate key, key format is like FFFFFFFFFFFF.");

			key[i] = hex;
		}

		for(int i = 0; i < 6; i++)
			std::cout << std::hex << (int)key[i] << ":";
		std::cout << std::endl;

		nfc.authenticate(sector, key, keyType);
	}
}

void Callbacks::clear() {
	pTxtCardType->set_text("");
	pTxtUid->set_text("");
}

void Callbacks::about() {
	pDlgAbout->run();
}
