#include "ReconnectScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"
#include "ProducerAI.h"
#include "SceneMgr.h"
#include "IntroScene.h"

ReconnectScene::ReconnectScene() {
	
	m_name = "ReconnectScene";

	RESMGR->RegisterImage("img/ReconnectScene/ok_red.jpg", "reconnect_ok_red");
	RESMGR->RegisterImage("img/ReconnectScene/ok_blue.jpg", "reconnect_ok_blue");
	RESMGR->RegisterImage("img/ReconnectScene/already.jpg", "reconnect_already");
	RESMGR->RegisterImage("img/ReconnectScene/server_setting.jpg", "reconnect_server_setting");
	RESMGR->RegisterImage("img/ReconnectScene/the_concert.jpg", "reconnect_concert");
	RESMGR->RegisterImage("img/ReconnectScene/bt_blue_side_01.jpg", "reconnect_bt_blue_side_01");
	RESMGR->RegisterImage("img/ReconnectScene/bt_red_side_01.jpg", "reconnect_bt_red_side_01");
	RESMGR->RegisterImage("img/ReconnectScene/bt_white_side_01.jpg", "reconnect_bt_white_side_01");

	m_isIgnorePrevScene = true;

}


ReconnectScene::~ReconnectScene() {
	
}


bool ReconnectScene::CheckFirst() {

	bool isScene = true;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 700, 300, 0, 68, 135, 7);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1245, 300, 0, 52, 102, 10);

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 700, 208, 0, 68, 135, 7);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1245, 208, 0, 52, 102, 10);

	return isScene;

}


bool ReconnectScene::CheckScene() {

	m_state = RECONNECT;

	auto points = RESMGR->FindImages(nullptr, "reconnect_already", 0.99, 1, true, cvRect(546, 444, 100, 90));
	if (!points.empty()) return true;

	points.clear();

	points = RESMGR->FindImages(nullptr, "reconnect_concert", 0.99, 1, true, cvRect(700, 424, 200, 70));
	if (!points.empty()) {
		m_state = QUIT_CONCERT;
		return true;
	}

	//서버 설정
	points.clear();

	points = RESMGR->FindImages(nullptr, "reconnect_server_setting", 0.99, 1, true, cvRect(752, 430, 218, 131));
	if (!points.empty()) {
		m_state = SERVER_ERROR;
		m_unkownPos = points[0];
		return true;
	}

	//unkonwn
	points.clear();

	points = RESMGR->FindImages(nullptr, "reconnect_bt_blue_side_01", 0.95, 1, true, cvRect(550, 634, 490, 170));
	if (!points.empty()) {
		m_state = UNKOWN;
		m_unkownPos = points[0];
		return true;
	}

	points.clear();

	points = RESMGR->FindImages(nullptr, "reconnect_bt_white_side_01", 0.95, 1, true, cvRect(550, 634, 490, 290));
	if (!points.empty()) {
		m_state = UNKOWN;
		m_unkownPos = points[0];
		return true;
	}

	points.clear();

	points = RESMGR->FindImages(nullptr, "reconnect_bt_red_side_01", 0.95, 1, true, cvRect(550, 634, 490, 170));
	if (!points.empty()) {
		m_state = UNKOWN_EXIT;
		m_unkownPos = points[0];
		return true;
	}

	return false;

}


void ReconnectScene::ActionDecision() {

	if(isScene<IntroScene>(SCENE->GetPrevScene())){
#ifdef __linux__
		system("shutdown --reboot");
#endif
	}

	switch (m_state) {
		case RECONNECT: 
			ActionReconnect();
		break;
		case QUIT_CONCERT:
			ActionQuitConcert();
		break;
		case SERVER_ERROR:
			ActionServerError();
			break;
		case UNKOWN:
		case UNKOWN_EXIT:
			ActionUnkown();
		break;
		default: break;
	}

}


void ReconnectScene::ActionReconnect() const {

	auto points = RESMGR->FindImages(nullptr, "reconnect_ok_red", 0.99, 1, true, cvRect(855, 645, 192, 125));
	if (!points.empty()) {
		CvPoint pos = points[0];

		pos.x += 855 + 66;
		pos.y += 645 + 43;

		GAME->SetMouseClick(pos.x, pos.y);
		Sleep(60000);

		PRODUCER->SetConcertVocal(EnsembleConcertUnit());
		PRODUCER->SetConcertDance(EnsembleConcertUnit());
		PRODUCER->SetConcertPerformance(EnsembleConcertUnit());
	}

}


void ReconnectScene::ActionQuitConcert() const {

	auto points = RESMGR->FindImages(nullptr, "reconnect_ok_blue", 0.99, 1, true, cvRect(980, 650, 372, 125));
	if (!points.empty()) {
		CvPoint pos = points[0];

		pos.x += 980 + 66;
		pos.y += 650 + 43;

		GAME->SetMouseClick(pos.x, pos.y);
	}

}


void ReconnectScene::ActionUnkown() const {

	std::cout << "touch blue button\n";
	GAME->SetMouseClick(550 + m_unkownPos.x + 150, 634 + m_unkownPos.y + 70);

}


void ReconnectScene::ActionServerError() const {

	std::cout << "ActionServerError\n";
	GAME->SetMouseClick(960, 700);
	Sleep(2000);
	if(GAME->GetIsGunstars()) {
		std::string str = "start ";
		str.append(GAME->GetGunStarsPath());
		system(str.c_str());
		GAME->isQuit = true;
	}

}
