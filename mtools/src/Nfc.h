/*
 * Nfc.h
 *
 *  Created on: 2010-01-31
 *      Author: rosek
 */

#ifndef NFC_H_
#define NFC_H_

#include <nfc/nfc.h>
#include <nfc/nfc-types.h>
#include <nfc/nfc-messages.h>

#include <nfc/mifaretag.h>
#include <nfc/mifareultag.h>

#include <freefare.h>

#include <vector>
#include <stdexcept>

#include <glibmm/i18n.h>

class Nfc {
public:
	const static int KEY_A = MFC_KEY_A;
	const static int KEY_B = MFC_KEY_B;
	const static MifareClassicKey default_keys[];

	Nfc();
	virtual ~Nfc();

	void findTags();
	std::string getTagUid();
	std::string getTagType();

	void read(int sector, int block, unsigned char data[16]);
	void write(int sector, int block, unsigned char data[16]);

	void initValue(int sector, int block, int value);
	int readValue(int sector, int block);
	int incValue(int sector, int block, int value);
	int decValue(int sector, int block, int value);

	void authenticate(int sector, int keyType);
	void authenticate(int sector, unsigned char* key, int keyType);

protected:
	nfc_device_t* dev;

	int selectedTag;
	MifareTag* tags;

	bool mifareConnected;

	void connect();
	void disconnect();

	void isNfcDeviceSetup() {
		if(!dev) {
			connect();
			if(!dev)
				throw std::runtime_error(_("No NFC device found."));
		}
	}

	void isMifareTagSelected() {
		if(selectedTag < 0)
			throw std::runtime_error(_("MIFARE classic tag is not selected."));
	}

	bool isNXPClassic1k(MifareTag tag) {
		if(tag->type == NXP_CLASSIC_1K)
			return true;
		return false;
	}

	bool isNXPClassic4k(MifareTag tag) {
		if(tag->type == NXP_CLASSIC_4K)
			return true;
		return false;
	}

	bool isNokiaClassic4kEmulated(MifareTag tag) {
		if(tag->type == NOKIA_CLASSIC_4K_EMULATED)
			return true;
		return false;
	}

	bool isClassic(MifareTag tag) {
		if(isNXPClassic1k(tag) ||
				isNXPClassic4k(tag) ||
				isNokiaClassic4kEmulated(tag))
			return true;
		return false;
	}

	bool isNXPUltralight(MifareTag tag) {
		if(tag->type == NXP_ULTRALIGHT)
			return true;
		return false;
	}

	unsigned short blockAddress(unsigned short sector, unsigned short block) {
		return (sector * 4) + block;
	}

};

#endif /* NFC_H_ */
