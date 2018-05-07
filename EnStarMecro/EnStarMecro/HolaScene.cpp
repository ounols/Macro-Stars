#include "HolaScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"


HolaScene::HolaScene() {
	m_name = "HolaScene";

	RESMGR->RegisterImage("img/HolaScene/hola.jpg", "hola_hola");
	RESMGR->RegisterImage("img/HolaScene/hola_enstars_icon.jpg", "hola_enstars_icon");
	RESMGR->RegisterImage("img/HolaScene/open.jpg", "hola_open");
}


HolaScene::~HolaScene() {}


bool HolaScene::CheckFirst() {
	return false;
}


bool HolaScene::CheckScene() {

	m_point = cvPoint(-1, -1);
	isMain = false;

	auto points = RESMGR->FindImages(nullptr, "hola_hola", 0.98, 1, true, cvRect(726, 232, 493, 180));
	if (!points.empty()) {
		isMain = true;
		return true;
	}

	points = RESMGR->FindImages(nullptr, "hola_open", 0.99, 1, true, cvRect(1162, 483, 302, 99));
	if (!points.empty()) {
		m_point = cvPoint(1317, 535);
		return true;
	}

	return false;

}


bool HolaScene::ReadData() {

	if (!isMain) return false;

	auto points = RESMGR->FindImages(nullptr, "hola_enstars_icon", 0.98, 1, true, cvRect(126, 405, 1681, 609));
	if (!points.empty()) {
		m_point = cvPoint(126 + points[0].x + 70, 405 + points[0].y + 85);
		return true;
	}

	return true;
}


void HolaScene::ActionDecision() {

	if (m_point.x < 0) return;

	GAME->SetMouseClick(m_point.x, m_point.y);

	if(!isMain) {
		Sleep(5000);
	}

}
