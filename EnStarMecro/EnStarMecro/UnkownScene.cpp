#include "UnkownScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"


UnkownScene::UnkownScene() {
	m_name = "UnkownScene";


	RESMGR->RegisterImage("img/UnkownScene/bt_back.jpg", "unkown_back");

}


UnkownScene::~UnkownScene() {}


bool UnkownScene::CheckFirst() {

	return RESMGR->CheckRGB(nullptr, 31, 65, 255, 197, 0, 5);

}


bool UnkownScene::CheckScene() {

	auto points = RESMGR->FindImages(nullptr, "unkown_back", 0.99, 1, true, cvRect(0, 0, 192, 128));
	if (points.empty()) return false;

	return true;
}


void UnkownScene::ActionDecision() {

	GAME->SetMouseClick(100, 75);

}
