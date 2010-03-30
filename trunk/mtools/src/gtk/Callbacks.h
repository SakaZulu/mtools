/*
 * Callbacks.h
 *
 *  Created on: 2010-01-31
 *      Author: rosek
 */

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <gtkmm.h>
#include "../Nfc.h"
#include "gtk/DataAccessConditions.h"
#include "gtk/TrailerAccessConditions.h"

#include "gtk/dialogs/KeysStoreDialog.h"
#include <gtk/dialogs/ErrorDialog.h>
#include "gtk/ComboBoxText.h"


#include <thread/AsyncCallback.h>

class Callbacks {
public:
	Callbacks();
	virtual ~Callbacks();

protected:
	Nfc* nfc;
	Nfc::Tag* tag;

	Gtk::Button* pBtnSearch;

	Gtk::Button* pBtnTab1Read;
	Gtk::Button* pBtnTab1Write;

	Gtk::Button* pBtnTab2Init;
	Gtk::Button* pBtnTab2Read;
	Gtk::Button* pBtnTab2Inc;
	Gtk::Button* pBtnTab2Dec;

	Gtk::RadioButton* pRbKeyA;
	Gtk::RadioButton* pRbKeyB;

	Gtk::SpinButton* pSpnValue;
	Gtk::SpinButton* pSpnBlock;
	Gtk::SpinButton* pSpnSector;

	Gtk::CheckButton* pCbDefKeys;

	Gtk::Entry* pTxtKey;
	Gtk::Entry* pTxtUid;
	Gtk::Entry* pTxtCardType;
	Gtk::Entry* pTxtReadWrite;
	Gtk::Entry* pTxtHexReadWrite;

	Gtk::AboutDialog* pDlgAbout;

	DataAccessConditions dataAC;
	TrailerAccessConditions trailerAC;

	KeysStoreDialog dlgKeysStore;
	void onKeysStoreChange(std::vector<std::string> v);

	void onBtnSearchClicked();
	void onBtnTab1ReadClick();
	void onBtnTab1WriteClick();
	void onBtnTab2InitClick();
	void onBtnTab2ReadClick();
	void onBtnTab2IncClick();
	void onBtnTab2DecClick();

	void onCbDefKeysClicked();

	void onAdjSectorValueChanged();

	void onMiQuitClicked();
	void onMiOpenKeys();

	void onDlgAboutResponse(int responseId);

	void about();

	void setOperationAdjustment(int maxSector, int maxBlock) {
		Gtk::Adjustment *pAdjBlock = pSpnBlock->get_adjustment();
		Gtk::Adjustment *pAdjSector = pSpnSector->get_adjustment();

		if(!(pAdjSector->get_value() <= maxSector &&
				pAdjBlock->get_value() <= maxBlock)) {
			pAdjBlock->set_value(0);
			pAdjSector->set_value(0);
		}

		pAdjBlock->set_upper(maxBlock);
		pAdjBlock->set_lower(0);
		pAdjSector->set_upper(maxSector);
		pAdjSector->set_lower(0);
	}

	void authenticate(int sector);

	void clear();
};

#endif /* CALLBACKS_H_ */
