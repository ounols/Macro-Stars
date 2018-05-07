#include "KakaoPopUpScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"


KakaoPopUpScene::KakaoPopUpScene() {
	m_name = "KakaoPopUpScene";


	RESMGR->RegisterImage("img/KakaoPopUpScene/pop_kakao_close.jpg", "kakao_pop_close");

}


KakaoPopUpScene::~KakaoPopUpScene() {}


bool KakaoPopUpScene::CheckFirst() {

	isNoticePopUp = false;

	bool isScene = true;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 384, 441, 40, 40, 40, 3);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1521, 441, 40, 40, 40, 3);

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 298, 960, 39, 39, 39);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1620, 960, 39, 39, 39);

	if (isScene) isNoticePopUp = true;

	return isScene;

}


bool KakaoPopUpScene::CheckScene() {

	if(isNoticePopUp) {
		auto points = RESMGR->FindImages(nullptr, "kakao_pop_close", 0.95, 1, true, cvRect(1400, 785, 478, 288));
		if (points.empty()) return false;

		return true;
	}

	return true;
}


bool KakaoPopUpScene::ReadData() {

	return true;
}


void KakaoPopUpScene::ActionDecision() {

	if(isNoticePopUp) {
		GAME->SetMouseClick(1587, 947);
		Sleep(1000);
		return;
	}

	GAME->SetMouseClick(657, 611);
	Sleep(1000);

}
