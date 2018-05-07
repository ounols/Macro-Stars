#include "UltraSurfScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"


UltraSurfScene::UltraSurfScene() {
	m_name = "UltraSurfScene";


	RESMGR->RegisterImage("img/UltraSurfScene/ready.jpg", "ultra_ready");
	RESMGR->RegisterImage("img/UltraSurfScene/connected.jpg", "ultra_connected");
	RESMGR->RegisterImage("img/UltraSurfScene/caution.jpg", "ultra_caution");
}


UltraSurfScene::~UltraSurfScene() {}


bool UltraSurfScene::CheckFirst() {
	return false;
}


bool UltraSurfScene::CheckScene() {

	needClick = false;
	isCleared = false;
	isCaution = false;

	if(RESMGR->CheckRGB(nullptr, 660, 200, 207, 207, 207)) {
		auto points = RESMGR->FindImages(nullptr, "ultra_ready", 0.999, 1, true, cvRect(660, 200, 600, 120));
		if (!points.empty()) {
			needClick = true;
			return true;
		}

		points = RESMGR->FindImages(nullptr, "ultra_connected", 0.999, 1, true, cvRect(660, 200, 600, 120));
		if (!points.empty()) {
			isCleared = true;
			return true;
		}

		return false;
	}


	auto points = RESMGR->FindImages(nullptr, "ultra_caution", 0.97, 1, true, cvRect(394, 229, 237, 106));
	if (!points.empty()) {
		isCaution = true;
		return true;
	}

	return false;

}


void UltraSurfScene::ActionDecision() {

	if(needClick) {
		GAME->SetMouseClick(967, 433);
		Sleep(2000);
		return;
	}

	if(isCleared) {
		GAME->SendAdbCommand("adb shell am start -n com.kakaogames.estarskr/com.happyelements.kirara.KakaoActivity");
		Sleep(5000);
		return;
	}

	if(isCaution) {
		GAME->SetMouseClick(434, 750);
		Sleep(1000);
		GAME->SetMouseClick(1250, 870);
		Sleep(1000);
		if (GAME->GetIsGunstars()) {
			std::string str = "start ";
			str.append(GAME->GetGunStarsPath());
			system(str.c_str());
			GAME->isQuit = true;
		}
		return;
	}

}
