#include "QuestScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"
#include "SceneMgr.h"


QuestScene::QuestScene() {
	m_name = "QuestScene";


	RESMGR->RegisterImage("img/QuestScene/bt_get.jpg", "quest_bt_get");
	RESMGR->RegisterImage("img/QuestScene/title_quest.jpg", "quest_title_quest");
}


QuestScene::~QuestScene() {}


bool QuestScene::CheckFirst() {

	bool isScene = true;
	isQuit = false;


	isScene = isScene && RESMGR->CheckRGB(nullptr, 1585, 85, 242, 242, 242, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 31, 65, 255, 197, 0, 5);

	return isScene;
}


bool QuestScene::CheckScene() {

	auto points = RESMGR->FindImages(nullptr, "quest_title_quest", 0.98, 1, true, cvRect(1578, 54, 178, 57));
	if (points.empty()) return false;

	return true;

}


bool QuestScene::ReadData() {

	m_point = cvPoint(-1, -1);

	//일일퀘스트인지 확인
	if(RESMGR->CheckRGB(nullptr, 1753, 367, 244, 244, 244, 2)) {
		m_point = cvPoint(1800, 364);
		return true;
	}

	auto points = RESMGR->FindImages(nullptr, "quest_bt_get", 0.98, 1, true, cvRect(1230, 257, 200, 609));
	if (points.empty()) {
		isQuit = true;
		return true;
	}

	m_point = cvPoint(1230 + points[0].x, 257 + points[0].y + 24);
	return true;

}


void QuestScene::ActionDecision() {

	if(isQuit) {
		GAME->SetMouseClick(1803, 985);
		SCENE->LockScene();
		return;
	}

	GAME->SetMouseClick(m_point.x, m_point.y);

}
