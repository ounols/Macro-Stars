#include "IntroScene.h"
#include "ResMgr.h"
#include "SceneMgr.h"
#include "ReconnectScene.h"
#include <windows.h>
#include "GameClientMgr.h"


IntroScene::IntroScene() {
	
	m_name = "IntroScene";

	RESMGR->RegisterImage("img/IntroScene/title_video.jpg", "intro_title_video");
	RESMGR->RegisterImage("img/IntroScene/kakao_close.jpg", "intro_kakao_close");

}


IntroScene::~IntroScene() {}


bool IntroScene::CheckFirst() {

	bool isScene = true;
	isPopUp = false;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 48, 63, 26, 170, 205);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1770, 370, 246, 242, 231);

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 48, 63, 250, 225, 0);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1731, 63, 250, 225, 0);

	if (isScene) isPopUp = true;

	return isScene;

}


bool IntroScene::CheckScene() {

	if(!isPopUp) {
		auto points = RESMGR->FindImages(nullptr, "intro_title_video", 0.99, 1, true, cvRect(1665, 884, 230, 100));
		if (points.empty()) return false;
	} else {
		auto points = RESMGR->FindImages(nullptr, "intro_kakao_close", 0.98, 1, true, cvRect(1800, 30, 80, 74));
		if (points.empty()) return false;
	}

	return true;

}


void IntroScene::ActionDecision() {

	if(!isPopUp) {

		GAME->SetMouseClick(500, 500);

	}else {
		GAME->SetMouseClick(1850, 70);
	}

}
