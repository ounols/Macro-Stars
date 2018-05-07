#include "PlayStoreScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"


PlayStoreScene::PlayStoreScene() {
	m_name = "PlayStoreScene";


	RESMGR->RegisterImage("img/PlayStoreScene/icon.jpg", "playstore_icon");
	RESMGR->RegisterImage("img/PlayStoreScene/bt_update.jpg", "playstore_bt_update");
	RESMGR->RegisterImage("img/PlayStoreScene/bt_open.jpg", "playstore_bt_open");

}


PlayStoreScene::~PlayStoreScene() {
	
}


bool PlayStoreScene::CheckFirst() {
	return false;
}


bool PlayStoreScene::CheckScene() {
	std::cout << "[플레이 스토어]\n";
	auto points = RESMGR->FindImages(nullptr, "playstore_icon", 0.97, 1, true, cvRect(0, 186, 268, 221));
	if (!points.empty()) return true;

	m_unkownCount++;

	if(m_unkownCount > 20) {
		m_unkownCount = 0;

		GAME->SendAdbCommand("adb shell am force-stop com.android.vending");
		Sleep(1000);
		GAME->SendAdbCommand("adb shell am start -a android.intent.action.VIEW -d \'market://details?id=com.kakaogames.estarskr\'");
		Sleep(5000);

	}
}


bool PlayStoreScene::ReadData() {

	m_unkownCount = 0;

	auto points = RESMGR->FindImages(nullptr, "playstore_bt_update", 0.97, 1, true, cvRect(1574, 405, 327, 135));
	if(!points.empty()) {
		isNeedUpdate = true;
		return true;
	}

	points = RESMGR->FindImages(nullptr, "playstore_bt_open", 0.97, 1, true, cvRect(1574, 405, 327, 135));
	if (!points.empty()) {
		isClear = true;
		return true;
	}

	return false;

}


void PlayStoreScene::ActionDecision() {
	if(isNeedUpdate) {
		GAME->SetMouseClick(1747, 480);
		return;
	}

	if(isClear) {
		GAME->SendAdbCommand("adb shell am force-stop com.android.vending");
		Sleep(1000);
		if(GAME->GetIsVpn())
			GAME->SendAdbCommand("adb shell am start -n org.hola/org.hola.browser_activity");
		else
			GAME->SendAdbCommand("adb shell am start -n com.kakaogames.estarskr/com.happyelements.kirara.KakaoActivity");

		//GAME->SendAdbCommand("adb shell am start -n com.easyovpn.easyovpn/com.easyovpn.easyovpn.ui.MainActivity");
		Sleep(5000);
		return;
	}
}
