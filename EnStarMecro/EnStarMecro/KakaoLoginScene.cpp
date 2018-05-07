#include "KakaoLoginScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"


KakaoLoginScene::KakaoLoginScene() {
	m_name = "KakaoLoginScene";

	RESMGR->RegisterImage("img/KakaoLoginScene/bt_login.jpg", "kakaologin_bt_login");
	RESMGR->RegisterImage("img/KakaoLoginScene/title_logout.jpg", "kakaologin_title_logout");
}


KakaoLoginScene::~KakaoLoginScene() {}


bool KakaoLoginScene::CheckFirst() {

	bool isScene = true;
	isLogout = false;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 500, 600, 255, 235, 0, 4);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 500, 700, 131, 131, 131, 4);

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 800, 447, 233, 233, 233, 2);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1253, 667, 233, 233, 233, 2);

	if (isScene) isLogout = true;

	return isScene;

}


bool KakaoLoginScene::CheckScene() {

	if(isLogout) {
		auto points = RESMGR->FindImages(nullptr, "kakaologin_title_logout", 0.98, 1, true, cvRect(606, 255, 420, 210));
		if (!points.empty()) return true;

		return false;
	}

	auto points = RESMGR->FindImages(nullptr, "kakaologin_bt_login", 0.98, 1, true, cvRect(378, 531, 632, 146));
	if (!points.empty()) return true;

	return false;
}


bool KakaoLoginScene::ReadData() {

	return true;

}


void KakaoLoginScene::ActionDecision() {

	if(isLogout) {

		GAME->SendAdbCommand("adb shell am force-stop com.kakaogames.estarskr");
		Sleep(2000);
		GAME->SendAdbCommand("adb shell am start -n com.kakaogames.estarskr/com.happyelements.kirara.KakaoActivity");


		if (GAME->GetIsGunstars()) {
			std::string str = "start ";
			str.append(GAME->GetGunStarsPath());
			system(str.c_str());
			GAME->isQuit = true;
		}

		return;
	}

	GAME->SetMouseClick(687, 600);

}
