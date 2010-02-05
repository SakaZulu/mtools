/*
 * Nfc.cpp
 *
 *  Created on: 2010-01-31
 *      Author: rosek
 */

#include "Nfc.h"
#include "Utils.h"

#include <stdlib.h>

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

	tags.clear();

	MifareClassicTag* mcTags = mifare_classic_get_tags (dev);
	if(!mcTags) {
		disconnect();
		throw std::runtime_error("Error listing MIFARE classic tag.");
	}

	if(!mcTags[0]) {
		mifare_classic_free_tags(mcTags);
		disconnect();
		throw std::runtime_error("No MIFARE classic tag on NFC device.");
	}

	while(*mcTags) {
		tags.push_back(*mcTags);
		mcTags++;
	}

	if(mifare_classic_connect(tags[0]) != 0)
		throw std::runtime_error("Can't connect to MIFARE classic tag.");

	if(!tags.empty())
		selectedTag = 0;

}

std::string Nfc::getTagUid() {
	isNfcDeviceSetup();
	isMifareTagSelected();

	char* pUid = mifare_classic_get_uid (tags[selectedTag]);
	std::string uid = pUid;
	free(pUid);

	return uid;
}

/*
Theses values comes from http://www.libnfc.org/documentation/hardware/tags/iso14443

Manufacturer	Product			ATQA	SAK	ATS (called ATR for contact smartcards)

NXP		MIFARE Mini		00 04 	09
		MIFARE Classic 1K 	00 04 	08
		MIFARE Classic 4K 	00 02 	18
		MIFARE Ultralight 	00 44 	00
		MIFARE DESFire		03 44 	20 	06 75 77 81 02 80
		MIFARE DESFire EV1 	03 44 	20 	06 75 77 81 02 80
		JCOP31			03 04 	28 	38 77 b1 4a 43 4f 50 33 31
		JCOP31 v2.4.1		00 48 	20 	78 77 b1 02 4a 43 4f 50 76 32 34 31
		JCOP41 v2.2		00 48 	20 	38 33 b1 4a 43 4f 50 34 31 56 32 32
		JCOP41 v2.3.1		00 04 	28 	38 33 b1 4a 43 4f 50 34 31 56 32 33 31
Infineon 	MIFARE Classic 1K 	00 04 	88
Gemplus 	MPCOS 			00 02 	98
Innovision R&T 	Jewel 			0C 00
*/
std::string Nfc::getTagType() {
	isNfcDeviceSetup();
	isMifareTagSelected();

	std::string tagType = "";
	nfc_iso14443a_info_t info = tags[0]->info;

	if ((info.abtAtqa[0] == 0x00) && (info.abtAtqa[1] == 0x04) && (info.btSak == 0x09)) {
		tagType = "NXP MIFARE Mini";
	} else if ((info.abtAtqa[0] == 0x00) && (info.abtAtqa[1] == 0x04) && (info.btSak == 0x08)) {
		tagType = "NXP MIFARE Classic 1K";
	} else if ((info.abtAtqa[0] == 0x00) && (info.abtAtqa[1] == 0x02) && (info.btSak == 0x18)) {
		tagType = "NXP MIFARE Classic 4K";
	} else if ((info.abtAtqa[0] == 0x00) && (info.abtAtqa[1] == 0x02) && (info.btSak == 0x38)) {
		tagType = "Nokia MIFARE Classic 4K - emulated";
	} else if ((info.abtAtqa[0] == 0x00) && (info.abtAtqa[1] == 0x44) && (info.btSak == 0x00)) {
		tagType = "NXP MIFARE Ultralight";
	} else if ((info.abtAtqa[0] == 0x03) && (info.abtAtqa[1] == 0x44) && (info.btSak == 0x20)) {
		tagType = "NXP MIFARE DESFire";
	} else if ((info.abtAtqa[0] == 0x03) && (info.abtAtqa[1] == 0x04) && (info.btSak == 0x28)) {
		tagType = "NXP JCOP31";
	} else if ((info.abtAtqa[0] == 0x00) && (info.abtAtqa[1] == 0x48) && (info.btSak == 0x20)) {
		tagType = "NXP JCOP31 or JCOP41";
	} else if ((info.abtAtqa[0] == 0x00) && (info.abtAtqa[1] == 0x04) && (info.btSak == 0x28)) {
		tagType = "NXP JCOP41";
	} else if ((info.abtAtqa[0] == 0x00) && (info.abtAtqa[1] == 0x04) && (info.btSak == 0x88)) {
		tagType = "Infineon MIFARE Classic 1K";
	} else if ((info.abtAtqa[0] == 0x00) && (info.abtAtqa[1] == 0x02) && (info.btSak == 0x98)) {
		tagType = "Gemplus MPCOS";
	} else if ((info.abtAtqa[0] == 0x0C) && (info.abtAtqa[1] == 0x00)) {
		/* @note I'm not sure that Jewel can be detected using this modultation and I haven't Jewel tags to test. */
		tagType = "Innovision R&T Jewel";
	} else {
		tagType = "Unknown tag type: ";
		tagType = "ATQA (SENS_RES): " + Utils::pByteToStrHex(info.abtAtqa, 2);
		tagType = ", SAK (SEL_RES): " + Utils::pByteToStrHex(&info.btSak, 1);
		if (info.szAtsLen)
			tagType = ", ATS (ATR): " + Utils::pByteToStrHex(info.abtAts, info.szAtsLen);
	}

	return tagType;
}

void Nfc::initValue(int sector, int block, int value) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	if(mifare_classic_init_value(tags[selectedTag], blockAddress(sector, block), value, 0) != 0)
		throw std::runtime_error("Can't initialize value of MIFARE classic tag.");
}

int Nfc::readValue(int sector, int block) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	int value = 0;
	if(mifare_classic_read_value(tags[selectedTag], blockAddress(sector, block), &value, NULL) != 0)
		throw std::runtime_error("Can't read value of MIFARE classic tag.");

	return value;
}

int Nfc::incValue(int sector, int block, int value) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	if(mifare_classic_increment(tags[selectedTag], blockAddress(sector, block), value) != 0)
		throw std::runtime_error("Can't increment value of MIFARE classic tag.");

	if(mifare_classic_transfer (tags[selectedTag], blockAddress(sector, block)) != 0)
		throw std::runtime_error("Can't transfer value to MIFARE classic tag.");

	return readValue(sector, block);
}

int Nfc::decValue(int sector, int block, int value) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	if(mifare_classic_decrement(tags[selectedTag], blockAddress(sector, block), value) != 0)
		throw std::runtime_error("Can't increment value of MIFARE classic tag.");

	if(mifare_classic_transfer (tags[selectedTag], blockAddress(sector, block)) != 0)
		throw std::runtime_error("Can't transfer value to MIFARE classic tag.");

	return readValue(sector, block);
}

void Nfc::read(int sector, int block, unsigned char data[16]) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	MifareClassicBlock blockData;
	if(mifare_classic_read(tags[selectedTag], blockAddress(sector, block), &blockData) != 0)
		throw std::runtime_error("Can't read data from MIFARE classic tag.");

	for(int i = 0; i < 16; i++)
		data[i] = blockData[i];
}

void Nfc::write(int sector, int block, unsigned char data[16]) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	if(mifare_classic_write(tags[selectedTag], blockAddress(sector, block), data) != 0)
		throw std::runtime_error("Can't read data from MIFARE classic tag.");
}

void Nfc::authenticate(int sector, int keyType) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	MifareClassicKeyType type = (keyType == KEY_A) ? MFC_KEY_A : MFC_KEY_B;
    for (unsigned int i = 0; i < (sizeof(default_keys) / sizeof(MifareClassicKey)); i++) {
		if(mifare_classic_authenticate(tags[selectedTag], blockAddress(sector, 0),
						default_keys[i], type) == 0)
			return;
	}

    throw std::runtime_error("Can't authenticate MIFARE classic tag");
}

void Nfc::authenticate(int sector, unsigned char* key, int keyType) {
	isNfcDeviceSetup();
	isMifareTagSelected();

	MifareClassicKeyType type = (keyType == KEY_A) ? MFC_KEY_A : MFC_KEY_B;
	if(mifare_classic_authenticate(tags[selectedTag], blockAddress(sector, 0), key, type) != 0)
		throw std::runtime_error("Can't authenticate MIFARE classic tag");

}
