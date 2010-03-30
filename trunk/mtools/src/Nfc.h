/*
 * Nfc.h
 *
 *  Created on: 2010-01-31
 *      Author: rosek
 */

#ifndef NFC_H_
#define NFC_H_

#include <freefare.h>

#include <list>
#include <stdexcept>
#include <iostream>

#include <glibmm/i18n.h>

#include <thread/Runnable.h>
#include <thread/AsyncCallback.h>

/**
 * This class is c++ wrapper for libfreefare library.
 */
class Nfc {
public:
	// Constants
	const static int KEY_A = MFC_KEY_A;
	const static int KEY_B = MFC_KEY_B;

	const static std::string KEY_REGEXPR;

	// Default keys
	const static int defaultKeysNumber = 8;
	const static std::string defaultKeys[defaultKeysNumber];

	class Tag {
		MifareTag tag;
		std::list<std::string> keysStore;

		// Keys conversion functions
		bool keyToMifareClassicKey(std::string str, unsigned char* key);

		unsigned short blockAddress(unsigned short sector, unsigned short block);
	public:
		Tag(MifareTag t) : tag(t) {
			// Setup initial keys store with default keys
			for(int i = 0; i < defaultKeysNumber; i++)
				keysStore.push_back(defaultKeys[i]);
		}

		bool connect();
		bool disconnect();

		bool isClassic();
		bool isClassic1k();
		bool isClassic4k();
		bool isUltralight();

		std::string getUid();
		std::string getType();

		void read(int sector, int block, unsigned char data[16]);
		void write(int sector, int block, unsigned char data[16]);

		void initValue(int sector, int block, int value);
		int readValue(int sector, int block);
		int incValue(int sector, int block, int value);
		int decValue(int sector, int block, int value);

		void authenticate(int sector, int keyType);
		void authenticate(int sector, unsigned char* key, int keyType);

		void addKey(std::string key);
		void removeKey(std::string key);

		const std::list<std::string>& getKeysStore();
		void setKeysStore(std::list<std::string> keys);
	};

	Nfc();
	virtual ~Nfc();

	Nfc::Tag* findTag();
protected:
	nfc_device_t* dev;

	int selectedTag;
	MifareTag* tags;

	void connect();
	void disconnect();

	bool isDeviceSetup();
	bool isTagSelected();
};

#endif /* NFC_H_ */
