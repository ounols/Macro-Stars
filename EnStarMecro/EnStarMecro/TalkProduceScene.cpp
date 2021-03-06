#include "TalkProduceScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"


TalkProduceScene::TalkProduceScene() {
	m_name = "TalkProduceScene";

	RESMGR->RegisterImage("img/TalkProduceScene/star.jpg", "talk_produce_star");
}


TalkProduceScene::~TalkProduceScene() {}


bool TalkProduceScene::CheckFirst() {
	bool isScene = true;
	isQuestTalk = false;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 460, 800, 43, 162, 226, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1528, 1037, 255, 255, 255, 5);

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 460, 786, 43, 162, 226, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1500, 1032, 255, 255, 255, 5);

	if(isScene) {
		isQuestTalk = true;
	}

	return isScene;
}


bool TalkProduceScene::CheckScene() {

	auto points = RESMGR->FindImages(nullptr, "talk_produce_star", 0.98, 1, true, cvRect(1420, 945, 95, 95));
	if (points.empty()) return false;

	return true;

}



void TalkProduceScene::ActionDecision() {

	if(isQuestTalk) {
		GAME->SetMouseClick(1840, 93);
		Sleep(1000);
		GAME->SetMouseClick(1820, 93);
		Sleep(5000);
		GAME->SetMouseClick(1362, 933);

		return;
	}

	GAME->SetMouseClick(940, 500);
	Sleep(500);
	GAME->SetMouseClick(940, 563);

}
