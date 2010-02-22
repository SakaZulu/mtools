/*
 * Nfc.cpp
 *
 *  Created on: 2010-01-31
 *      Author: rosek
 */

#include "Nfc.h"
#include "Utils.h"

#include <stdlib.h>
#include <errno.h>

#include <iostream>

const MifareClassicKey Nfc::default_keys[] = {
	    { 0xff,0xff,0xff,0xff,0xff,0xff },
	    { 0xd3,0xf7,0xd3,0xf7,0xd3,0xf7 },
	    { 0xa0,0xa1,0xa2,0xa3,0xa4,0xa5 },
	    { 0xb0,0xb1,0xb2,0xb3,0xb4,0xb5 },
	    { 0x4d,0x3a,0x99,0xc3,0x51,0xdd },
	    { 0x1a,0x98,0x2c,0x7e,0x45,0x9a },
	    { 0xaa,0xbb,0xcc,0xdd,0xee,0xff },
	    { 0x00,0x00,0x00,0x00,0x00,0x00 }
	};

Nfc::Nfc() : dev(NULL) {
	selectedTag = -1;
}

Nfc::~Nfc() {
	disconnect();
}

void Nfc::connect() {
	if(dev) return;
	dev = nfc_connect(NULL);
}

void Nfc::disconnect() {
	if (!dev) return;
	nfc_disconnect(dev);
	dev = NULL;
}

void Nfc::findTags() {
	if(!isDeviceSetup())
		throw std::runtime_error(_("No NFC device found."));

	freefare_free_tags(tags);
	tags = NULL;
	tags = freefare_get_tags (dev);

	if(!tags) {
		disconnect();
		throw std::runtime_error("Error listing MIFARE classic tag.");
	}

	if(!tags[0]) {
		freefare_free_tags(tags);
		tags = NULL;
		disconnect();
		throw std::runtime_error("No MIFARE classic tag on NFC device.");
	}

	MifareTag tag = tags[0];

	if(isClassic()) {
		if(mifare_classic_connect(tag) != 0)
			throw std::runtime_error("Can't connect to MIFARE Classic tag.");
	} else if(isUltralight()) {
		if(mifare_ultralight_connect(tag) != 0)
			throw std::runtime_error("Can't connect to MIFARE Ultraligth tag.");
	}

	selectedTag = 0;
}

std::string Nfc::getTagUid() {
	if(!isDeviceSetup())
		throw std::runtime_error(_("No NFC device found."));
	if(!isTagSelected())
		throw std::runtime_error(_("MIFARE classic tag is not selected."));

	std::string uid = "";
	if(isClassic()) {
		char* pUid = mifare_classic_get_uid (tags[selectedTag]);
		uid = pUid;
		free(pUid);
	} else if(isUltralight()) {
		char* pUid = mifare_ultralight_get_uid (tags[selectedTag]);
		uid = pUid;
		free(pUid);
	}

	return uid;
}

std::string Nfc::getTagType() {
	if(!isDeviceSetup())
		throw std::runtime_error(_("No NFC device found."));
	if(!isTagSelected())
		throw std::runtime_error(_("MIFARE classic tag is not selected."));

	std::string tagType = "";
	if(isClassic1k())
		tagType = "NXP MIFARE Classic 1K";
	else if(isClassic4k())
		tagType = "NXP MIFARE Classic 4K";
	else if(isUltralight())
		tagType = "NXP MIFARE Ultralight";

	return tagType;
}

void Nfc::initValue(int sector, int block, int value) {
	if(!isDeviceSetup())
		throw std::runtime_error(_("No NFC device found."));
	if(!isTagSelected())
		throw std::runtime_error(_("MIFARE classic tag is not selected."));

	MifareTag tag = tags[selectedTag];

	if(!isClassic())
		throw std::runtime_error("Notsupported opperation");

	if(mifare_classic_init_value(tag, blockAddress(sector, block), value, 0) != 0)
		throw std::runtime_error("Can't initialize value of MIFARE classic tag.");
}

int Nfc::readValue(int sector, int block) {
	if(!isDeviceSetup())
		throw std::runtime_error(_("No NFC device found."));
	if(!isTagSelected())
		throw std::runtime_error(_("MIFARE classic tag is not selected."));

	MifareTag tag = tags[selectedTag];

	if(!isClassic())
		throw std::runtime_error("Notsupported opperation");

	int value = 0;
	if(mifare_classic_read_value(tag, blockAddress(sector, block), &value, NULL) != 0)
		throw std::runtime_error("Can't read value of MIFARE classic tag.");

	return value;
}

int Nfc::incValue(int sector, int block, int value) {
	if(!isDeviceSetup())
		throw std::runtime_error(_("No NFC device found."));
	if(!isTagSelected())
		throw std::runtime_error(_("MIFARE classic tag is not selected."));

	MifareTag tag = tags[selectedTag];

	if(!isClassic())
		throw std::runtime_error("Notsupported opperation");

	if(mifare_classic_increment(tag, blockAddress(sector, block), value) != 0)
		throw std::runtime_error("Can't increment value of MIFARE classic tag.");

	if(mifare_classic_transfer (tag, blockAddress(sector, block)) != 0)
		throw std::runtime_error("Can't transfer value to MIFARE classic tag.");

	return readValue(sector, block);
}

int Nfc::decValue(int sector, int block, int value) {
	if(!isDeviceSetup())
		throw std::runtime_error(_("No NFC device found."));
	if(!isTagSelected())
		throw std::runtime_error(_("MIFARE classic tag is not selected."));

	MifareTag tag = tags[selectedTag];

	if(!isClassic())
		throw std::runtime_error("Notsupported opperation");

	if(mifare_classic_decrement(tag, blockAddress(sector, block), value) != 0)
		throw std::runtime_error("Can't increment value of MIFARE classic tag.");

	if(mifare_classic_transfer (tag, blockAddress(sector, block)) != 0)
		throw std::runtime_error("Can't transfer value to MIFARE classic tag.");

	return readValue(sector, block);
}

void Nfc::read(int sector, int block, unsigned char data[16]) {
	if(!isDeviceSetup())
		throw std::runtime_error(_("No NFC device found."));
	if(!isTagSelected())
		throw std::runtime_error(_("MIFARE classic tag is not selected."));

	MifareTag tag = tags[selectedTag];

	MifareUltralightPage pageData;
	if(isUltralight()) {
		if(mifare_ultralight_read(tag, block, &pageData) != 0)
			throw std::runtime_error("Can't read data form MIFARE Ultralight tag.");

		for(unsigned int i = 0; i < sizeof(pageData); i++)
			data[i] = pageData[i];

	} else if(isClassic()) {
		MifareClassicBlock blockData;
		if(mifare_classic_read(tag, blockAddress(sector, block), &blockData) != 0)
			throw std::runtime_error("Can't read data from MIFARE Classic tag.");

		for(unsigned int i = 0; i < sizeof(blockData); i++)
			data[i] = blockData[i];
	} else
		throw std::runtime_error("Notsupported opperation");
}

void Nfc::write(int sector, int block, unsigned char data[16]) {
	if(!isDeviceSetup())
		throw std::runtime_error(_("No NFC device found."));
	if(!isTagSelected())
		throw std::runtime_error(_("MIFARE classic tag is not selected."));

	MifareTag tag = tags[selectedTag];

	if(isUltralight()) {
		if(mifare_ultralight_write(tag, block, data) != 0)
			throw std::runtime_error("Can't read data from MIFARE Ultralight tag.");
	} else if(isClassic()) {
		if(mifare_classic_write(tag, blockAddress(sector, block), data) != 0)
			throw std::runtime_error("Can't read data from MIFARE Classic tag.");
	} else
		throw std::runtime_error("Notsupported opperation");
}

void Nfc::authenticate(int sector, int keyType) {
	if(!isDeviceSetup())
		throw std::runtime_error(_("No NFC device found."));
	if(!isTagSelected())
		throw std::runtime_error(_("MIFARE classic tag is not selected."));

	MifareTag tag = tags[selectedTag];

	if(!isClassic())
		return;

	MifareClassicKeyType type = (keyType == KEY_A) ? MFC_KEY_A : MFC_KEY_B;
    for (unsigned int i = 0; i < (sizeof(default_keys) / sizeof(MifareClassicKey)); i++) {
		if(mifare_classic_authenticate(tag, blockAddress(sector, 0),
						default_keys[i], type) == 0)
			return;
	}

    throw std::runtime_error("Can't authenticate MIFARE classic tag");
}

void Nfc::authenticate(int sector, unsigned char* key, int keyType) {
	if(!isDeviceSetup())
		throw std::runtime_error(_("No NFC device found."));
	if(!isTagSelected())
		throw std::runtime_error(_("MIFARE classic tag is not selected."));

	MifareTag tag = tags[selectedTag];

	if(!isClassic())
		throw std::runtime_error("Notsupported opperation");

	MifareClassicKeyType type = (keyType == KEY_A) ? MFC_KEY_A : MFC_KEY_B;
	if(mifare_classic_authenticate(tag, blockAddress(sector, 0), key, type) != 0)
		throw std::runtime_error("Can't authenticate MIFARE classic tag");

}
