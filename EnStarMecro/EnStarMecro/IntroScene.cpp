#include "IntroScene.h"
#include "ResMgr.h"
#include "SceneMgr.h"
#include "ReconnectScene.h"
#ifdef WIN32
#include <windows.h>
#elif __linux__
#include "windows_for_linux.h"
#endif
#include "GameClientMgr.h"
#include "ProducerAI.h"

IntroScene::IntroScene() {
	
	m_name = "IntroScene";

	RESMGR->RegisterImage("img/IntroScene/title_live_2d.jpg", "intro_title_live_2d");
	RESMGR->RegisterImage("img/IntroScene/kakao_close.jpg", "intro_kakao_close");
	RESMGR->RegisterImage("img/IntroScene/logo.jpg", "intro_logo");

}


IntroScene::~IntroScene() {}


bool IntroScene::CheckFirst() {

	bool isScene = true;
	isPopUp = false;
	isLogo = false;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 48, 63, 26, 170, 205, 3);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1770, 370, 246, 242, 231, 3);

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 700, 475, 223, 104, 100, 2);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1546, 735, 255, 255, 255);

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 48, 63, 250, 225, 0);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1731, 63, 250, 225, 0);

	if (isScene) isPopUp = true;

	return isScene;

}


bool IntroScene::CheckScene() {

	if(!isPopUp) {
		auto points = RESMGR->FindImages(nullptr, "intro_title_live_2d", 0.99, 1, true, cvRect(1557, 966, 342, 107));
		if (!points.empty()) return true;

		points = RESMGR->FindImages(nullptr, "intro_logo", 0.98, 1, true, cvRect(390, 278, 471, 471));
		if (points.empty()) return false;

		isLogo = true;

	} else {
		auto points = RESMGR->FindImages(nullptr, "intro_kakao_close", 0.98, 1, true, cvRect(1800, 30, 80, 74));
		if (!points.empty()) return true;
		

		return false;
	}

	if(PRODUCER->m_introSceneCount > -1) {
		PRODUCER->m_introSceneCount++;
		if(PRODUCER->m_introSceneCount > 20) {
			isQuit = true;
		}
	}

	return true;

}


void IntroScene::ActionDecision() {

	if(isQuit) {
		#ifdef __linux__
		system("killall -s SIGKILL anbox");
		#endif
	}

	if (isLogo) return;

	if(!isPopUp) {
		GAME->SetMouseClick(500, 500);
	}else {
		GAME->SetMouseClick(1850, 70);
		Sleep(1000);
		PRODUCER->m_introSceneCount = 0;
		//GAME->SendAdbCommand("adb shell am force-stop org.hola");
		Sleep(1000);
	}

}
