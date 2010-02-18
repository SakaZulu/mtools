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
	isNfcDeviceSetup();

	mifare_free_tags(tags);
	tags = NULL;
	tags = mifare_get_tags (dev);

	if(!tags) {
		disconnect();
		throw std::runtime_error("Error listing MIFARE classic tag.");
	}

	if(!tags[0]) {
		mifare_free_tags(tags);
		tags = NULL;
		disconnect();
		throw std::runtime_error("No MIFARE classic tag on NFC device.");
	}

	MifareTag tag = tags[0];

	if(isClassic(tag)) {
		if(mifare_classic_connect(tag->tag.mct) != 0)
			throw std::runtime_error("Can't connect to MIFARE Classic tag.");
	} else if(isNXPUltralight(tag)) {
		if(mifare_ultralight_connect(tag->tag.mut) != 0)
			throw std::runtime_error("Can't connect to MIFARE Ultraligth tag.");
	}

	selectedTag = 0;
}

std::string Nfc::getTagUid() {
	isNfcDeviceSetup();
	isMifareTagSelected();

	MifareTag tag = tags[selectedTag];

	std::string uid = "";
	if(isClassic(tag)) {
		char* pUid = mifare_classic_get_uid (tags[selectedTag]->tag.mct);
		uid = pUid;
		free(pUid);
	} else if(isNXPUltralight(tag)) {
		char* pUid = mifare_ultralight_get_uid (tags[selectedTag]->tag.mut);
		uid = pUid;
		free(pUid);
	}

	return uid;
}

std::string Nfc::getTagType() {
	isNfcDeviceSetup();
	isMifareTagSelected();

	MifareTag tag = tags[selectedTag];

	std::string tagType = "";
	if(isNXPClassic1k(tag))
		tagType = "NXP MIFARE Classic 1K";
	else if(isNXPClassic4k(tag))
		tagType = "NXP MIFARE Classic 4K";
	else if(isNXPUltralight(tag))
		tagType = "NXP MIFARE Ultralight";
	else if(isNokiaClassic4kEmulated(tag))
		tagType = "Nokia MIFARE Classic 4K - emulated";

	return tagType;
}

void Nfc::initValue(int sector, int block, int value) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	MifareTag tag = tags[selectedTag];

	if(!isClassic(tag))
		throw std::runtime_error("Notsupported opperation");

	if(mifare_classic_init_value(tag->tag.mct, blockAddress(sector, block), value, 0) != 0)
		throw std::runtime_error("Can't initialize value of MIFARE classic tag.");
}

int Nfc::readValue(int sector, int block) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	MifareTag tag = tags[selectedTag];

	if(!isClassic(tag))
		throw std::runtime_error("Notsupported opperation");

	int value = 0;
	if(mifare_classic_read_value(tag->tag.mct, blockAddress(sector, block), &value, NULL) != 0)
		throw std::runtime_error("Can't read value of MIFARE classic tag.");

	return value;
}

int Nfc::incValue(int sector, int block, int value) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	MifareTag tag = tags[selectedTag];

	if(!isClassic(tag))
		throw std::runtime_error("Notsupported opperation");

	if(mifare_classic_increment(tag->tag.mct, blockAddress(sector, block), value) != 0)
		throw std::runtime_error("Can't increment value of MIFARE classic tag.");

	if(mifare_classic_transfer (tag->tag.mct, blockAddress(sector, block)) != 0)
		throw std::runtime_error("Can't transfer value to MIFARE classic tag.");

	return readValue(sector, block);
}

int Nfc::decValue(int sector, int block, int value) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	MifareTag tag = tags[selectedTag];

	if(!isClassic(tag))
		throw std::runtime_error("Notsupported opperation");

	if(mifare_classic_decrement(tag->tag.mct, blockAddress(sector, block), value) != 0)
		throw std::runtime_error("Can't increment value of MIFARE classic tag.");

	if(mifare_classic_transfer (tag->tag.mct, blockAddress(sector, block)) != 0)
		throw std::runtime_error("Can't transfer value to MIFARE classic tag.");

	return readValue(sector, block);
}

void Nfc::read(int sector, int block, unsigned char data[16]) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	MifareTag tag = tags[selectedTag];

	if(!isClassic(tag))
		throw std::runtime_error("Notsupported opperation");

	MifareClassicBlock blockData;
	if(mifare_classic_read(tag->tag.mct, blockAddress(sector, block), &blockData) != 0)
		throw std::runtime_error("Can't read data from MIFARE classic tag.");

	for(int i = 0; i < 16; i++)
		data[i] = blockData[i];
}

void Nfc::write(int sector, int block, unsigned char data[16]) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	MifareTag tag = tags[selectedTag];

	if(!isClassic(tag))
		throw std::runtime_error("Notsupported opperation");

	if(mifare_classic_write(tag->tag.mct, blockAddress(sector, block), data) != 0)
		throw std::runtime_error("Can't read data from MIFARE classic tag.");
}

void Nfc::authenticate(int sector, int keyType) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	MifareTag tag = tags[selectedTag];

	if(!isClassic(tag))
		throw std::runtime_error("Notsupported opperation");

	MifareClassicKeyType type = (keyType == KEY_A) ? MFC_KEY_A : MFC_KEY_B;
    for (unsigned int i = 0; i < (sizeof(default_keys) / sizeof(MifareClassicKey)); i++) {
		if(mifare_classic_authenticate(tag->tag.mct, blockAddress(sector, 0),
						default_keys[i], type) == 0)
			return;
	}

    throw std::runtime_error("Can't authenticate MIFARE classic tag");
}

void Nfc::authenticate(int sector, unsigned char* key, int keyType) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	MifareTag tag = tags[selectedTag];

	if(!isClassic(tag))
		throw std::runtime_error("Notsupported opperation");

	MifareClassicKeyType type = (keyType == KEY_A) ? MFC_KEY_A : MFC_KEY_B;
	if(mifare_classic_authenticate(tag->tag.mct, blockAddress(sector, 0), key, type) != 0)
		throw std::runtime_error("Can't authenticate MIFARE classic tag");

}
