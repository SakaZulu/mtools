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

	bool isDeviceSetup() {
		if(!dev) {
			connect();
			if(!dev)
				return false;
		}

		return true;
	}

public:
	bool isTagSelected() {
		if(selectedTag < 0)
			return false;
		return true;
	}

	bool isClassic1k() {
		if(!isTagSelected())
			return false;

		if (freefare_get_tag_type(tags[selectedTag]) == CLASSIC_1K)
			return true;
		return false;
	}

	bool isClassic4k() {
		if(!isTagSelected())
			return false;

		if(freefare_get_tag_type(tags[selectedTag]) == CLASSIC_4K)
			return true;
		return false;
	}

	bool isClassic() {
		if(isClassic1k() || isClassic4k())
			return true;
		return false;
	}

	bool isUltralight() {
		if(!isTagSelected())
			return false;

		if(freefare_get_tag_type(tags[selectedTag]) == ULTRALIGHT)
			return true;
		return false;
	}

protected:
	unsigned short blockAddress(unsigned short sector, unsigned short block) {
		return (sector * 4) + block;
	}

};

#endif /* NFC_H_ */
