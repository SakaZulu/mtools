/*
 * Nfc.cpp
 *
 *  Created on: 2010-01-31
 *      Author: rosek
 */

#include "Nfc.h"
#include "Utils.h"

#include <boost/regex.hpp>

#include <stdlib.h>
#include <errno.h>

#include <iostream>

using namespace std;

const string Nfc::KEY_REGEXPR = "([\\da-fA-F][\\da-fA-F]:){5}[\\da-fA-F][\\da-fA-F]";

const string Nfc::defaultKeys[8] = {
		"ff:ff:ff:ff:ff:ff",
		"d3:f7:d3:f7:d3:f7",
	    "a0:a1:a2:a3:a4:a5",
	    "b0:b1:b2:b3:b4:b5",
	    "4d:3a:99:c3:51:dd",
	    "1a:98:2c:7e:45:9a",
	    "aa:bb:cc:dd:ee:ff",
	    "00:00:00:00:00:00"
};

Nfc::Nfc() : dev(NULL), tags(NULL) {
	selectedTag = -1;
	connect();
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

Nfc::Tag* Nfc::findTag() {
	// Wait for device ready
	if (!isDeviceSetup())
		throw std::runtime_error(_("No NFC device found."));

	// Get tags
	tags = freefare_get_tags(dev);
	if (!tags) {
		freefare_free_tags(tags);

		tags = NULL;
		selectedTag = -1;
		throw std::runtime_error(_("Error listing MIFARE tag."));
	}

	// Check tags
	if (!tags[0]) {
		freefare_free_tags(tags);

		tags = NULL;
		selectedTag = -1;
		throw std::runtime_error(_("No MIFARE tag on NFC device."));
	}

	Tag* tag = new Tag(tags[0]);
	if (!tag->connect()) {
		freefare_free_tags(tags);

		tags = NULL;
		selectedTag = -1;
		throw std::runtime_error(_("Can't connect to MIFARE classic tag."));
	}

	selectedTag = 0;
	std::cout << tag->getUid() << std::endl;

	return tag;
}

bool Nfc::isDeviceSetup() {
	if(!dev) {
		connect();
		if(!dev)
			return false;
	}
	return true;
}

bool Nfc::isTagSelected() {
	if(selectedTag < 0)
		return false;
	return true;
}

bool Nfc::Tag::connect() {
	if(isClassic()) {
		if(mifare_classic_connect(tag) != 0)
			return false;
			//	throw std::runtime_error("Can't connect to MIFARE Classic tag.");
	} else if(isUltralight()) {
		if(mifare_ultralight_connect(tag) != 0)
			return false;
			//throw std::runtime_error("Can't connect to MIFARE Ultralight tag.");
	}
	return true;
}

bool Nfc::Tag::disconnect() {
	if(isClassic()) {
		if(mifare_classic_disconnect(tag) != 0)
			return false;
	} else if(isUltralight()) {
		if(mifare_ultralight_disconnect(tag) != 0)
			return false;
	}
	return true;
}

bool Nfc::Tag::isClassic1k() {
	if (freefare_get_tag_type(tag) == CLASSIC_1K)
		return true;
	return false;
}

bool Nfc::Tag::isClassic4k() {
	if(freefare_get_tag_type(tag) == CLASSIC_4K)
		return true;
	return false;
}

bool Nfc::Tag::isClassic() {
	if(isClassic1k() || isClassic4k())
		return true;
	return false;
}

bool Nfc::Tag::isUltralight() {
	if(freefare_get_tag_type(tag) == ULTRALIGHT)
		return true;
	return false;
}

std::string Nfc::Tag::getUid() {
	std::string uid = "";
	if(isClassic()) {
		char* pUid = mifare_classic_get_uid (tag);
		uid = pUid;
		free(pUid);
	} else if(isUltralight()) {
		char* pUid = mifare_ultralight_get_uid (tag);
		uid = pUid;
		free(pUid);
	}

	return uid;
}

std::string Nfc::Tag::getType() {
	std::string tagType = "";
	if(isClassic1k())
		tagType = "NXP MIFARE Classic 1K";
	else if(isClassic4k())
		tagType = "NXP MIFARE Classic 4K";
	else if(isUltralight())
		tagType = "NXP MIFARE Ultralight";

	return tagType;
}

// Mifare Cards functionalities

void Nfc::Tag::read(int sector, int block, unsigned char data[16]) {
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

void Nfc::Tag::write(int sector, int block, unsigned char data[16]) {
	if(isUltralight()) {
		if(mifare_ultralight_write(tag, block, data) != 0)
			throw std::runtime_error("Can't read data from MIFARE Ultralight tag.");
	} else if(isClassic()) {
		if(mifare_classic_write(tag, blockAddress(sector, block), data) != 0)
			throw std::runtime_error("Can't read data from MIFARE Classic tag.");
	} else
		throw std::runtime_error("Notsupported opperation");
}

void Nfc::Tag::initValue(int sector, int block, int value) {
	if(!isClassic())
		throw std::runtime_error("Notsupported opperation");

	if(mifare_classic_init_value(tag, blockAddress(sector, block), value, 0) != 0)
		throw std::runtime_error("Can't initialize value of MIFARE classic tag.");
}

int Nfc::Tag::readValue(int sector, int block) {
	if(!isClassic())
		throw std::runtime_error("Notsupported opperation");

	int value = 0;
	if(mifare_classic_read_value(tag, blockAddress(sector, block), &value, NULL) != 0)
		throw std::runtime_error("Can't read value of MIFARE classic tag.");

	return value;
}

int Nfc::Tag::incValue(int sector, int block, int value) {
	if(!isClassic())
		throw std::runtime_error("Notsupported opperation");

	if(mifare_classic_increment(tag, blockAddress(sector, block), value) != 0)
		throw std::runtime_error("Can't increment value of MIFARE classic tag.");

	if(mifare_classic_transfer (tag, blockAddress(sector, block)) != 0)
		throw std::runtime_error("Can't transfer value to MIFARE classic tag.");

	return readValue(sector, block);
}

int Nfc::Tag::decValue(int sector, int block, int value) {
	if(!isClassic())
		throw std::runtime_error("Notsupported opperation");

	if(mifare_classic_decrement(tag, blockAddress(sector, block), value) != 0)
		throw std::runtime_error("Can't increment value of MIFARE classic tag.");

	if(mifare_classic_transfer (tag, blockAddress(sector, block)) != 0)
		throw std::runtime_error("Can't transfer value to MIFARE classic tag.");

	return readValue(sector, block);
}

void Nfc::Tag::authenticate(int sector, int keyType) {
	if(!isClassic())
		return;

	MifareClassicKeyType type = (keyType == KEY_A) ? MFC_KEY_A : MFC_KEY_B;

	// Use added keys to authenticate sector
	unsigned char hex[6];
	for(list<string>::iterator iter = keysStore.begin(); iter != keysStore.end(); iter++) {
		keyToMifareClassicKey(*iter, hex);
		if(mifare_classic_authenticate(tag, blockAddress(sector, 0), hex, type) == 0)
			return;
	}

    throw std::runtime_error("Can't authenticate MIFARE classic tag");
}

void Nfc::Tag::authenticate(int sector, unsigned char* key, int keyType) {
	if(!isClassic())
		throw std::runtime_error("Notsupported opperation");

	MifareClassicKeyType type = (keyType == KEY_A) ? MFC_KEY_A : MFC_KEY_B;
	if(mifare_classic_authenticate(tag, blockAddress(sector, 0), key, type) != 0)
		throw std::runtime_error("Can't authenticate MIFARE classic tag");
}

// Keys store functions

/**
 * Add key to the keys store
 * \param key Key to add to the key store
 */
void Nfc::Tag::addKey(string key) {
	keysStore.push_back(key);
}

/**
 * Remove key from the key store
 * \param key Key to remove from the key store
 */
void Nfc::Tag::removeKey(string key) {
	keysStore.remove(key);
}

/**
 * Get key store
 * \return Key store
 */
const list<string>& Nfc::Tag::getKeysStore() {
	return keysStore;
}

/**
 * Set key store
 * \param keys Keys to set
 */
void Nfc::Tag::setKeysStore(list<string> keys) {
	keysStore.clear();
	for(list<string>::iterator iter = keys.begin(); iter != keys.end(); iter++)
		keysStore.push_back(*iter);
}

// Keys conversion functions

/**
 * Convert string key format to MifareClassicKey
 */
bool Nfc::Tag::keyToMifareClassicKey(std::string str, unsigned char* key) {
	const static boost::regex expr(KEY_REGEXPR);
	if(!boost::regex_match(str, expr))
		return false;

	int value, pos = 0;
	for(int i = 0; i < 17; i += 3) {
		if(!Utils::strHexToNumeric<int>(value, str.substr(i, 2), std::hex))
			return false;

		key[pos++] = value;
	}

	return true;
}

// Utils functions

/**
 * Convert sector and block numbers into block address.
 * \param sector
 * \param block
 */
unsigned short Nfc::Tag::blockAddress(unsigned short sector, unsigned short block) {
	return (sector * 4) + block;
}
