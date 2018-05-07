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

	isScene = isScene && RESMGR->CheckRGB(nullptr, 460, 800, 43, 162, 226, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1528, 1037, 255, 255, 255, 5);

	return isScene;
}


bool TalkProduceScene::CheckScene() {

	auto points = RESMGR->FindImages(nullptr, "talk_produce_star", 0.98, 1, true, cvRect(1420, 945, 95, 95));
	if (points.empty()) return false;

	return true;

}



void TalkProduceScene::ActionDecision() {

	GAME->SetMouseClick(940, 500);
	Sleep(500);
	GAME->SetMouseClick(940, 563);

}
